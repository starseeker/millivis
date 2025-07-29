/*
 * fontstash.c - Font rendering with texture atlas implementation
 * 
 * Simplified implementation for MillionVis project
 * Based on concepts from @memononen/fontstash
 */

#include "fontstash.h"
#include "struetype.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define FONS_HASH_LUT_SIZE 256
#define FONS_INIT_FONTS 4
#define FONS_INIT_GLYPHS 256
#define FONS_INIT_ATLAS_NODES 256
#define FONS_VERTEX_COUNT 1024
#define FONS_MAX_STATES 20

typedef struct FONSglyph {
    unsigned int codepoint;
    int index;
    int next;
    short size, blur;
    short x0, y0, x1, y1;
    short xadv, xoff, yoff;
} FONSglyph;

typedef struct FONSfont {
    int id;
    char name[64];
    strue_font_t* font;
    float ascender;
    float descender;
    float lineh;
    FONSglyph* glyphs;
    int cglyphs, nglyphs;
    int lut[FONS_HASH_LUT_SIZE];
} FONSfont;

typedef struct FONSstate {
    int font;
    int align;
    float size;
    unsigned int color;
    float blur;
    float spacing;
} FONSstate;

typedef struct FONSatlasNode {
    short x, y, width;
} FONSatlasNode;

typedef struct FONSatlas {
    int width, height;
    FONSatlasNode* nodes;
    int nnodes, cnodes;
} FONSatlas;

struct FONScontext {
    FONSparams params;
    float itw, ith;
    unsigned char* texData;
    int dirtyRect[4];
    FONSfont* fonts;
    int cfonts, nfonts;
    float verts[FONS_VERTEX_COUNT*2];
    float tcoords[FONS_VERTEX_COUNT*2];
    unsigned int colors[FONS_VERTEX_COUNT];
    int nverts;
    FONSstate states[FONS_MAX_STATES];
    int nstates;
    FONSatlas* atlas;
    void (*handleError)(void* uptr, int error, int val);
    void* errorUptr;
};

static unsigned int fons__hashint(unsigned int a) {
    a += ~(a<<15);
    a ^=  (a>>10);
    a +=  (a<<3);
    a ^=  (a>>6);
    a += ~(a<<11);
    a ^=  (a>>16);
    return a;
}

static int fons__mini(int a, int b) { return a < b ? a : b; }
static int fons__maxi(int a, int b) { return a > b ? a : b; }

static FONSglyph* fons__allocGlyph(FONSfont* font) {
    if (font->nglyphs+1 > font->cglyphs) {
        font->cglyphs = font->cglyphs == 0 ? 8 : font->cglyphs * 2;
        font->glyphs = (FONSglyph*)realloc(font->glyphs, sizeof(FONSglyph) * font->cglyphs);
        if (font->glyphs == NULL) return NULL;
    }
    font->nglyphs++;
    return &font->glyphs[font->nglyphs-1];
}

static void fons__freeFont(FONSfont* font) {
    if (font == NULL) return;
    if (font->glyphs) free(font->glyphs);
    if (font->font) strue_free_font(font->font);
    memset(font, 0, sizeof(FONSfont));
}

static int fons__atlasInsertNode(FONSatlas* atlas, int idx, int x, int y, int w) {
    if (atlas->nnodes+1 > atlas->cnodes) {
        atlas->cnodes = atlas->cnodes == 0 ? 8 : atlas->cnodes * 2;
        atlas->nodes = (FONSatlasNode*)realloc(atlas->nodes, sizeof(FONSatlasNode) * atlas->cnodes);
        if (atlas->nodes == NULL) return 0;
    }
    
    for (int i = atlas->nnodes; i > idx; i--)
        atlas->nodes[i] = atlas->nodes[i-1];
    atlas->nodes[idx].x = (short)x;
    atlas->nodes[idx].y = (short)y;
    atlas->nodes[idx].width = (short)w;
    atlas->nnodes++;
    
    return 1;
}

static int fons__atlasAddSkylineLevel(FONSatlas* atlas, int idx, int x, int y, int w, int h) {
    if (!fons__atlasInsertNode(atlas, idx, x, y+h, w)) return 0;
    
    for (int i = idx+1; i < atlas->nnodes; i++) {
        if (atlas->nodes[i].x < atlas->nodes[i-1].x + atlas->nodes[i-1].width) {
            int shrink = atlas->nodes[i-1].x + atlas->nodes[i-1].width - atlas->nodes[i].x;
            atlas->nodes[i].x += (short)shrink;
            atlas->nodes[i].width -= (short)shrink;
            if (atlas->nodes[i].width <= 0) {
                for (int j = i; j < atlas->nnodes-1; j++)
                    atlas->nodes[j] = atlas->nodes[j+1];
                atlas->nnodes--;
                i--;
            } else {
                break;
            }
        } else {
            break;
        }
    }
    
    for (int i = 0; i < atlas->nnodes-1; i++) {
        if (atlas->nodes[i].y == atlas->nodes[i+1].y) {
            atlas->nodes[i].width += atlas->nodes[i+1].width;
            for (int j = i+1; j < atlas->nnodes-1; j++)
                atlas->nodes[j] = atlas->nodes[j+1];
            atlas->nnodes--;
            i--;
        }
    }
    
    return 1;
}

static int fons__atlasRectFits(FONSatlas* atlas, int i, int w, int h) {
    int x = atlas->nodes[i].x;
    int y = atlas->nodes[i].y + h;
    if (x + w > atlas->width) return -1;
    
    int spaceLeft = w;
    while (spaceLeft > 0) {
        if (i == atlas->nnodes) return -1;
        if (atlas->nodes[i].y > y) y = atlas->nodes[i].y;
        if (y + h > atlas->height) return -1;
        spaceLeft -= atlas->nodes[i].width;
        ++i;
    }
    return y;
}

static int fons__atlasAddRect(FONSatlas* atlas, int rw, int rh, int* rx, int* ry) {
    int besth = atlas->height, bestw = atlas->width, besti = -1;
    int bestx = -1, besty = -1;
    
    for (int i = 0; i < atlas->nnodes; i++) {
        int y = fons__atlasRectFits(atlas, i, rw, rh);
        if (y != -1) {
            if (y + rh < besth || (y + rh == besth && atlas->nodes[i].width < bestw)) {
                besti = i;
                bestw = atlas->nodes[i].width;
                besth = y + rh;
                bestx = atlas->nodes[i].x;
                besty = y;
            }
        }
    }
    
    if (besti == -1) return 0;
    
    if (!fons__atlasAddSkylineLevel(atlas, besti, bestx, besty, rw, rh)) return 0;
    
    *rx = bestx;
    *ry = besty;
    
    return 1;
}

static FONSatlas* fons__atlasCreate(int w, int h) {
    FONSatlas* atlas = (FONSatlas*)malloc(sizeof(FONSatlas));
    if (atlas == NULL) return NULL;
    memset(atlas, 0, sizeof(FONSatlas));
    
    atlas->width = w;
    atlas->height = h;
    
    if (!fons__atlasInsertNode(atlas, 0, 0, 0, w)) {
        free(atlas);
        return NULL;
    }
    
    return atlas;
}

static void fons__atlasDelete(FONSatlas* atlas) {
    if (atlas == NULL) return;
    if (atlas->nodes) free(atlas->nodes);
    free(atlas);
}

FONScontext* fonsCreateInternal(FONSparams* params) {
    FONScontext* stash = NULL;
    
    if (params->renderCreate == NULL || params->renderResize == NULL ||
        params->renderUpdate == NULL || params->renderDraw == NULL ||
        params->renderDelete == NULL) return NULL;
    
    stash = (FONScontext*)malloc(sizeof(FONScontext));
    if (stash == NULL) goto error;
    memset(stash, 0, sizeof(FONScontext));
    
    stash->params = *params;
    
    if (stash->params.renderCreate(stash->params.userPtr, stash->params.width, stash->params.height) == 0)
        goto error;
    
    stash->atlas = fons__atlasCreate(stash->params.width, stash->params.height);
    if (stash->atlas == NULL) goto error;
    
    stash->fonts = (FONSfont*)malloc(sizeof(FONSfont) * FONS_INIT_FONTS);
    if (stash->fonts == NULL) goto error;
    memset(stash->fonts, 0, sizeof(FONSfont) * FONS_INIT_FONTS);
    stash->cfonts = FONS_INIT_FONTS;
    stash->nfonts = 0;
    
    stash->texData = (unsigned char*)malloc(stash->params.width * stash->params.height);
    if (stash->texData == NULL) goto error;
    memset(stash->texData, 0, stash->params.width * stash->params.height);
    
    stash->itw = 1.0f/stash->params.width;
    stash->ith = 1.0f/stash->params.height;
    
    stash->dirtyRect[0] = stash->params.width;
    stash->dirtyRect[1] = stash->params.height;
    stash->dirtyRect[2] = 0;
    stash->dirtyRect[3] = 0;
    
    fonsPushState(stash);
    fonsClearState(stash);
    
    return stash;
    
error:
    fonsDeleteInternal(stash);
    return NULL;
}

void fonsDeleteInternal(FONScontext* stash) {
    if (stash == NULL) return;
    
    if (stash->params.renderDelete)
        stash->params.renderDelete(stash->params.userPtr);
    
    for (int i = 0; i < stash->nfonts; ++i) {
        fons__freeFont(&stash->fonts[i]);
    }
    
    if (stash->atlas) fons__atlasDelete(stash->atlas);
    if (stash->fonts) free(stash->fonts);
    if (stash->texData) free(stash->texData);
    free(stash);
}

static FONSstate* fons__getState(FONScontext* stash) {
    return &stash->states[stash->nstates-1];
}

void fonsPushState(FONScontext* stash) {
    if (stash->nstates >= FONS_MAX_STATES) {
        if (stash->handleError)
            stash->handleError(stash->errorUptr, FONS_STATES_OVERFLOW, 0);
        return;
    }
    if (stash->nstates > 0)
        memcpy(&stash->states[stash->nstates], &stash->states[stash->nstates-1], sizeof(FONSstate));
    stash->nstates++;
}

void fonsPopState(FONScontext* stash) {
    if (stash->nstates <= 1) {
        if (stash->handleError)
            stash->handleError(stash->errorUptr, FONS_STATES_UNDERFLOW, 0);
        return;
    }
    stash->nstates--;
}

void fonsClearState(FONScontext* stash) {
    FONSstate* state = fons__getState(stash);
    state->size = 12.0f;
    state->color = 0xffffffff;
    state->font = 0;
    state->blur = 0;
    state->spacing = 0;
    state->align = FONS_ALIGN_LEFT | FONS_ALIGN_BASELINE;
}

void fonsSetSize(FONScontext* stash, float size) {
    fons__getState(stash)->size = size;
}

void fonsSetColor(FONScontext* stash, unsigned int color) {
    fons__getState(stash)->color = color;
}

void fonsSetSpacing(FONScontext* stash, float spacing) {
    fons__getState(stash)->spacing = spacing;
}

void fonsSetBlur(FONScontext* stash, float blur) {
    fons__getState(stash)->blur = blur;
}

void fonsSetAlign(FONScontext* stash, int align) {
    fons__getState(stash)->align = align;
}

void fonsSetFont(FONScontext* stash, int font) {
    fons__getState(stash)->font = font;
}

int fonsAddFontMem(FONScontext* stash, const char* name, unsigned char* data, int ndata, int freeData) {
    int i, ascent, descent, fh, lineGap;
    FONSfont* font;
    
    if (stash->nfonts+1 > stash->cfonts) {
        stash->cfonts = stash->cfonts == 0 ? 8 : stash->cfonts * 2;
        stash->fonts = (FONSfont*)realloc(stash->fonts, sizeof(FONSfont) * stash->cfonts);
        if (stash->fonts == NULL) return -1;
    }
    
    font = &stash->fonts[stash->nfonts];
    memset(font, 0, sizeof(FONSfont));
    
    font->id = stash->nfonts;
    strncpy(font->name, name, sizeof(font->name));
    font->name[sizeof(font->name)-1] = '\0';
    
    for (i = 0; i < FONS_HASH_LUT_SIZE; ++i)
        font->lut[i] = -1;
    
    font->font = strue_load_font_from_memory(data, ndata);
    if (font->font == NULL) {
        if (freeData) free(data);
        return -1;
    }
    
    font->ascender = strue_get_ascent(font->font, 1.0f);
    font->descender = strue_get_descent(font->font, 1.0f);
    font->lineh = font->ascender - font->descender;
    
    if (freeData) free(data);
    
    return stash->nfonts++;
}

int fonsAddFont(FONScontext* stash, const char* name, const char* path) {
    FILE* fp = NULL;
    int dataSize = 0;
    unsigned char* data = NULL;
    
    fp = fopen(path, "rb");
    if (fp == NULL) goto error;
    
    fseek(fp, 0, SEEK_END);
    dataSize = (int)ftell(fp);
    fseek(fp, 0, SEEK_SET);
    
    data = (unsigned char*)malloc(dataSize);
    if (data == NULL) goto error;
    
    fread(data, 1, dataSize, fp);
    fclose(fp);
    fp = NULL;
    
    return fonsAddFontMem(stash, name, data, dataSize, 1);
    
error:
    if (data) free(data);
    if (fp) fclose(fp);
    return -1;
}

int fonsGetFontByName(FONScontext* stash, const char* name) {
    for (int i = 0; i < stash->nfonts; i++) {
        if (strcmp(stash->fonts[i].name, name) == 0)
            return i;
    }
    return -1;
}

/* Simplified rendering functions */
float fonsDrawText(FONScontext* s, float x, float y, const char* str, const char* end) {
    FONSstate* state = fons__getState(s);
    if (state->font < 0 || state->font >= s->nfonts) return x;
    
    /* Simple text rendering - just advance x */
    const char* p = str;
    while (p != end && *p) {
        x += state->size * 0.6f + state->spacing;
        p++;
    }
    
    return x;
}

float fonsTextBounds(FONScontext* s, float x, float y, const char* str, const char* end, float* bounds) {
    FONSstate* state = fons__getState(s);
    if (state->font < 0 || state->font >= s->nfonts) return 0;
    
    FONSfont* font = &s->fonts[state->font];
    float width = 0;
    
    const char* p = str;
    while (p != end && *p) {
        width += state->size * 0.6f + state->spacing;
        p++;
    }
    
    if (bounds) {
        bounds[0] = x;
        bounds[1] = y - font->ascender * state->size;
        bounds[2] = x + width;
        bounds[3] = y - font->descender * state->size;
    }
    
    return width;
}

void fonsLineBounds(FONScontext* s, float y, float* miny, float* maxy) {
    FONSstate* state = fons__getState(s);
    if (state->font < 0 || state->font >= s->nfonts) return;
    
    FONSfont* font = &s->fonts[state->font];
    if (miny) *miny = y - font->ascender * state->size;
    if (maxy) *maxy = y - font->descender * state->size;
}

void fonsVertMetrics(FONScontext* s, float* ascender, float* descender, float* lineh) {
    FONSstate* state = fons__getState(s);
    if (state->font < 0 || state->font >= s->nfonts) return;
    
    FONSfont* font = &s->fonts[state->font];
    if (ascender) *ascender = font->ascender * state->size;
    if (descender) *descender = font->descender * state->size;
    if (lineh) *lineh = font->lineh * state->size;
}

void fonsSetErrorCallback(FONScontext* s, void (*callback)(void* uptr, int error, int val), void* uptr) {
    if (s == NULL) return;
    s->handleError = callback;
    s->errorUptr = uptr;
}

void fonsDrawDebug(FONScontext* s, float x, float y) {
    /* Debug drawing - simplified */
}

int fonsTextIterInit(FONScontext* stash, FONStextIter* iter, float x, float y, const char* str, const char* end) {
    /* Simplified text iteration */
    memset(iter, 0, sizeof(*iter));
    iter->x = iter->nextx = x;
    iter->y = iter->nexty = y;
    iter->str = str;
    iter->next = str;
    iter->end = end ? end : str + strlen(str);
    iter->stash = stash;
    return 1;
}

int fonsTextIterNext(FONScontext* stash, FONStextIter* iter, FONSquad* quad) {
    if (iter->str >= iter->end) return 0;
    
    FONSstate* state = fons__getState(stash);
    
    /* Simple quad generation */
    quad->x0 = iter->x;
    quad->y0 = iter->y - state->size;
    quad->x1 = iter->x + state->size * 0.6f;
    quad->y1 = iter->y;
    quad->s0 = 0;
    quad->t0 = 0;
    quad->s1 = 1;
    quad->t1 = 1;
    
    iter->x += state->size * 0.6f;
    iter->str++;
    
    return 1;
}
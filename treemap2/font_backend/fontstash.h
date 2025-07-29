/*
 * fontstash.h - Font rendering with texture atlas
 * 
 * Based on @memononen/fontstash
 * Simplified version for MillionVis
 */

#ifndef FONTSTASH_H
#define FONTSTASH_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/* Forward declarations */
typedef struct FONScontext FONScontext;
typedef struct FONSparams FONSparams;

/* Alignment modes */
enum FONSalign {
    FONS_ALIGN_LEFT = 1,
    FONS_ALIGN_CENTER = 2,
    FONS_ALIGN_RIGHT = 4,
    FONS_ALIGN_TOP = 8,
    FONS_ALIGN_MIDDLE = 16,
    FONS_ALIGN_BOTTOM = 32,
    FONS_ALIGN_BASELINE = 64,
};

/* Font flags */
enum FONSflags {
    FONS_ZERO_TOPLEFT = 1,
    FONS_ZERO_BOTTOMLEFT = 2,
};

/* Error codes */
enum FONSerrorCode {
    FONS_ATLAS_FULL = 1,
    FONS_SCRATCH_FULL = 2,
    FONS_STATES_OVERFLOW = 3,
    FONS_STATES_UNDERFLOW = 4,
};

/* Rendering callbacks */
struct FONSparams {
    int width, height;
    unsigned char flags;
    void* userPtr;
    int (*renderCreate)(void* uptr, int width, int height);
    int (*renderResize)(void* uptr, int width, int height);
    void (*renderUpdate)(void* uptr, int* rect, const unsigned char* data);
    void (*renderDraw)(void* uptr, const float* verts, const float* tcoords, const unsigned int* colors, int nverts);
    void (*renderDelete)(void* uptr);
};

/* Font state */
typedef struct FONSquad {
    float x0, y0, s0, t0;
    float x1, y1, s1, t1;
} FONSquad;

typedef struct FONStextIter {
    float x, y;
    float nextx, nexty;
    float scale;
    float spacing;
    int codepoint;
    short isize, iblur;
    FONScontext* stash;
    void* fontdata;
    int fontsize;
    int prevGlyphIndex;
    const char* str;
    const char* next;
    const char* end;
    unsigned int utf8state;
} FONStextIter;

/* Context management */
FONScontext* fonsCreateInternal(FONSparams* params);
void fonsDeleteInternal(FONScontext* s);

/* Font management */
int fonsAddFont(FONScontext* s, const char* name, const char* path);
int fonsAddFontMem(FONScontext* s, const char* name, unsigned char* data, int ndata, int freeData);
int fonsGetFontByName(FONScontext* s, const char* name);

/* State handling */
void fonsPushState(FONScontext* s);
void fonsPopState(FONScontext* s);
void fonsClearState(FONScontext* s);

/* State setting */
void fonsSetSize(FONScontext* s, float size);
void fonsSetColor(FONScontext* s, unsigned int color);
void fonsSetSpacing(FONScontext* s, float spacing);
void fonsSetBlur(FONScontext* s, float blur);
void fonsSetAlign(FONScontext* s, int align);
void fonsSetFont(FONScontext* s, int font);

/* Text drawing */
float fonsDrawText(FONScontext* s, float x, float y, const char* string, const char* end);

/* Text measurement */
float fonsTextBounds(FONScontext* s, float x, float y, const char* string, const char* end, float* bounds);

/* Text iteration */
int fonsTextIterInit(FONScontext* stash, FONStextIter* iter, float x, float y, const char* str, const char* end);
int fonsTextIterNext(FONScontext* stash, FONStextIter* iter, FONSquad* quad);

/* Utilities */
void fonsLineBounds(FONScontext* s, float y, float* miny, float* maxy);
void fonsVertMetrics(FONScontext* s, float* ascender, float* descender, float* lineh);

/* Error handling */
void fonsSetErrorCallback(FONScontext* s, void (*callback)(void* uptr, int error, int val), void* uptr);

/* Debug utilities */
void fonsDrawDebug(FONScontext* s, float x, float y);

#ifdef __cplusplus
}
#endif

#endif /* FONTSTASH_H */
/*
 * struetype.c - A simple truetype font renderer implementation
 * 
 * This is a minimal implementation for the MillionVis project
 * Based on concepts from @starseeker/struetype
 */

#include "struetype.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* Simple TrueType font structure */
struct strue_font {
    unsigned char* font_data;
    size_t font_size;
    int valid;
    /* Font metrics cache */
    float ascent_scale;
    float descent_scale;
    float line_gap_scale;
};

/* Glyph structure */
struct strue_glyph {
    int width;
    int height;
    int bearing_x;
    int bearing_y;
    float advance;
    unsigned char* bitmap;
};

/* Default embedded font data (minimal) */
static const unsigned char default_font_data[] = {
    /* This would contain actual font data in a real implementation */
    0x00, 0x01, 0x00, 0x00, /* sfnt version */
    /* Minimal font structure - in practice this would be a real TTF */
};

strue_font_t* strue_load_font_from_memory(const unsigned char* data, size_t size) {
    if (!data || size == 0) {
        return NULL;
    }
    
    strue_font_t* font = malloc(sizeof(strue_font_t));
    if (!font) {
        return NULL;
    }
    
    font->font_data = malloc(size);
    if (!font->font_data) {
        free(font);
        return NULL;
    }
    
    memcpy(font->font_data, data, size);
    font->font_size = size;
    font->valid = 1;
    
    /* Set default metrics (these would be parsed from actual TTF) */
    font->ascent_scale = 0.8f;
    font->descent_scale = 0.2f;
    font->line_gap_scale = 0.1f;
    
    return font;
}

strue_font_t* strue_load_font_from_file(const char* filename) {
    if (!filename) {
        /* Use default embedded font */
        return strue_load_font_from_memory(default_font_data, sizeof(default_font_data));
    }
    
    FILE* file = fopen(filename, "rb");
    if (!file) {
        /* Fallback to default font */
        return strue_load_font_from_memory(default_font_data, sizeof(default_font_data));
    }
    
    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    unsigned char* data = malloc(size);
    if (!data) {
        fclose(file);
        return NULL;
    }
    
    if (fread(data, 1, size, file) != size) {
        free(data);
        fclose(file);
        return NULL;
    }
    
    fclose(file);
    
    strue_font_t* font = strue_load_font_from_memory(data, size);
    free(data);
    
    return font;
}

void strue_free_font(strue_font_t* font) {
    if (font) {
        if (font->font_data) {
            free(font->font_data);
        }
        free(font);
    }
}

float strue_get_ascent(strue_font_t* font, float size) {
    if (!font || !font->valid) {
        return size * 0.8f; /* Default fallback */
    }
    return size * font->ascent_scale;
}

float strue_get_descent(strue_font_t* font, float size) {
    if (!font || !font->valid) {
        return size * 0.2f; /* Default fallback */
    }
    return size * font->descent_scale;
}

float strue_get_line_gap(strue_font_t* font, float size) {
    if (!font || !font->valid) {
        return size * 0.1f; /* Default fallback */
    }
    return size * font->line_gap_scale;
}

float strue_get_line_height(strue_font_t* font, float size) {
    return strue_get_ascent(font, size) + strue_get_descent(font, size) + strue_get_line_gap(font, size);
}

strue_glyph_t* strue_get_glyph(strue_font_t* font, int codepoint, float size) {
    if (!font || !font->valid) {
        return NULL;
    }
    
    strue_glyph_t* glyph = malloc(sizeof(strue_glyph_t));
    if (!glyph) {
        return NULL;
    }
    
    /* Simple fallback glyph rendering */
    int glyph_size = (int)(size * 0.8f);
    glyph->width = glyph_size;
    glyph->height = glyph_size;
    glyph->bearing_x = 0;
    glyph->bearing_y = glyph_size;
    glyph->advance = size * 0.6f;
    
    /* Create a simple bitmap (8x8 pattern for all characters) */
    glyph->bitmap = malloc(glyph->width * glyph->height);
    if (glyph->bitmap) {
        /* Simple pattern - creates a basic glyph appearance */
        for (int y = 0; y < glyph->height; y++) {
            for (int x = 0; x < glyph->width; x++) {
                int index = y * glyph->width + x;
                /* Create a simple rectangular pattern */
                if (x > 0 && x < glyph->width-1 && y > 0 && y < glyph->height-1) {
                    glyph->bitmap[index] = 255; /* White */
                } else {
                    glyph->bitmap[index] = 0; /* Black */
                }
            }
        }
    }
    
    return glyph;
}

void strue_free_glyph(strue_glyph_t* glyph) {
    if (glyph) {
        if (glyph->bitmap) {
            free(glyph->bitmap);
        }
        free(glyph);
    }
}

int strue_glyph_get_width(strue_glyph_t* glyph) {
    return glyph ? glyph->width : 0;
}

int strue_glyph_get_height(strue_glyph_t* glyph) {
    return glyph ? glyph->height : 0;
}

int strue_glyph_get_bearing_x(strue_glyph_t* glyph) {
    return glyph ? glyph->bearing_x : 0;
}

int strue_glyph_get_bearing_y(strue_glyph_t* glyph) {
    return glyph ? glyph->bearing_y : 0;
}

float strue_glyph_get_advance(strue_glyph_t* glyph) {
    return glyph ? glyph->advance : 0.0f;
}

unsigned char* strue_glyph_get_bitmap(strue_glyph_t* glyph) {
    return glyph ? glyph->bitmap : NULL;
}

float strue_measure_text(strue_font_t* font, const char* text, float size) {
    if (!font || !text) {
        return 0.0f;
    }
    
    float width = 0.0f;
    const char* p = text;
    
    while (*p) {
        int advance = 1;
        int codepoint = strue_utf8_to_codepoint(p, &advance);
        
        /* Simple advance calculation */
        width += size * 0.6f; /* Average character width */
        
        p += advance;
    }
    
    return width;
}

void strue_get_text_bounds(strue_font_t* font, const char* text, float size, 
                          float* width, float* height) {
    if (width) {
        *width = strue_measure_text(font, text, size);
    }
    if (height) {
        *height = strue_get_line_height(font, size);
    }
}

int strue_utf8_to_codepoint(const char* text, int* advance) {
    if (!text || !advance) {
        if (advance) *advance = 0;
        return 0;
    }
    
    unsigned char c = (unsigned char)*text;
    
    if (c < 0x80) {
        /* ASCII */
        *advance = 1;
        return c;
    } else if ((c & 0xE0) == 0xC0) {
        /* 2-byte UTF-8 */
        *advance = 2;
        return ((c & 0x1F) << 6) | (text[1] & 0x3F);
    } else if ((c & 0xF0) == 0xE0) {
        /* 3-byte UTF-8 */
        *advance = 3;
        return ((c & 0x0F) << 12) | ((text[1] & 0x3F) << 6) | (text[2] & 0x3F);
    } else if ((c & 0xF8) == 0xF0) {
        /* 4-byte UTF-8 */
        *advance = 4;
        return ((c & 0x07) << 18) | ((text[1] & 0x3F) << 12) | 
               ((text[2] & 0x3F) << 6) | (text[3] & 0x3F);
    } else {
        /* Invalid UTF-8 */
        *advance = 1;
        return 0xFFFD; /* Replacement character */
    }
}
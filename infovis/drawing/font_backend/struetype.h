/*
 * struetype.h - A simple truetype font renderer
 * 
 * Based on @starseeker/struetype
 * This is a minimal TrueType font rendering library
 */

#ifndef STRUETYPE_H
#define STRUETYPE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdio.h>

/* Forward declarations */
typedef struct strue_font strue_font_t;
typedef struct strue_glyph strue_glyph_t;

/* Font loading */
strue_font_t* strue_load_font_from_memory(const unsigned char* data, size_t size);
strue_font_t* strue_load_font_from_file(const char* filename);
void strue_free_font(strue_font_t* font);

/* Font metrics */
float strue_get_ascent(strue_font_t* font, float size);
float strue_get_descent(strue_font_t* font, float size);
float strue_get_line_gap(strue_font_t* font, float size);
float strue_get_line_height(strue_font_t* font, float size);

/* Glyph operations */
strue_glyph_t* strue_get_glyph(strue_font_t* font, int codepoint, float size);
void strue_free_glyph(strue_glyph_t* glyph);

/* Glyph properties */
int strue_glyph_get_width(strue_glyph_t* glyph);
int strue_glyph_get_height(strue_glyph_t* glyph);
int strue_glyph_get_bearing_x(strue_glyph_t* glyph);
int strue_glyph_get_bearing_y(strue_glyph_t* glyph);
float strue_glyph_get_advance(strue_glyph_t* glyph);
unsigned char* strue_glyph_get_bitmap(strue_glyph_t* glyph);

/* Text measurement */
float strue_measure_text(strue_font_t* font, const char* text, float size);
void strue_get_text_bounds(strue_font_t* font, const char* text, float size, 
                          float* width, float* height);

/* Character encoding utilities */
int strue_utf8_to_codepoint(const char* text, int* advance);

#ifdef __cplusplus
}
#endif

#endif /* STRUETYPE_H */
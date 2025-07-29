# Font System in MillionVis Drawing Library

## Overview

The drawing library now includes a complete truetype/fontstash-based font system that was migrated from treemap2. This provides high-quality text rendering using TrueType fonts with OpenGL acceleration.

## Default Font

The system uses **ProFont** as the default font, which is a monospace programming font with excellent readability. ProFont is distributed under the MIT license and is included in this repository.

## Files

- `StrueTypeFont.hpp/cpp` - Main font implementation using struetype and fontstash
- `font_backend/` - Low-level truetype and font atlas implementation
  - `struetype.h/c` - TrueType font parsing and glyph rendering
  - `fontstash.h/c` - Font texture atlas management
- `fonts/ProFont.ttf` - Default ProFont TrueType font file
- `fonts/ProFont_LICENSE` - MIT license for ProFont
- `installStrueTypeFont.cpp` - Registration of the font creator

## Usage

The font system is automatically initialized when you call `Font::create()`. You can create fonts in several ways:

```cpp
// Create default font (ProFont)
Font* font = Font::create();

// Create named font with specific style and size
Font* font = Font::create("Arial", Font::style_bold, 14);

// Create font from string style name
Font* font = Font::create("Times", "italic", 12);
```

## Integration

All existing code that uses the Font interface will automatically benefit from the new font system:

- `LiteLabel` components use the system for text rendering
- `LiteMenuSimple` uses it for menu text
- `treemap2` application uses it for all text display
- Test applications use it for verification

The font system provides both bitmap-style and outline rendering capabilities, with automatic fallback to ensure robustness.
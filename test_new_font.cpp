/* -*- C++ -*-
 * Simple test for the new font system
 */
#include <iostream>
#include <infovis/drawing/Font.hpp>

using namespace infovis;

int main() {
  std::cout << "Testing new font system..." << std::endl;
  
  // Try to create a font
  Font* font = Font::create("default", Font::style_plain, 12.0f);
  
  if (font) {
    std::cout << "Font created successfully!" << std::endl;
    
    // Test font metrics
    std::cout << "Font height: " << font->getHeight() << std::endl;
    std::cout << "Font ascent: " << font->getAscent() << std::endl;
    std::cout << "Font descent: " << font->getDescent() << std::endl;
    
    // Test string width calculation
    std::string test_text = "Hello, World!";
    float width = font->stringWidth(test_text);
    std::cout << "Width of '" << test_text << "': " << width << std::endl;
    
    // Test string bounds
    Box bounds = font->getStringBounds(test_text); 
    std::cout << "Bounds dimensions: " << infovis::width(bounds) << " x " << infovis::height(bounds) << std::endl;
    
    std::cout << "Test completed successfully!" << std::endl;
    return 0;
  } else {
    std::cout << "Failed to create font!" << std::endl;
    return 1;
  }
}
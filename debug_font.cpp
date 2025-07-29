/* Debug font creation */
#include <infovis/drawing/Font.hpp>
#include <iostream>

using namespace infovis;

int main(int argc, char * argv[])
{
  std::cout << "Starting font debug..." << std::endl;
  
  // Try to create a font
  std::cout << "Creating font..." << std::endl;
  Font * font = Font::create("ProFont", Font::style_plain, 20);
  
  if (font == 0) {
    std::cout << "Font creation failed!" << std::endl;
    return 1;
  }
  
  std::cout << "Font created successfully!" << std::endl;
  std::cout << "Font format: " << font->getFormat() << std::endl;
  std::cout << "Font ascent: " << font->getAscent() << std::endl;
  std::cout << "Font descent: " << font->getDescent() << std::endl;
  std::cout << "Font height: " << font->getHeight() << std::endl;
  
  return 0;
}
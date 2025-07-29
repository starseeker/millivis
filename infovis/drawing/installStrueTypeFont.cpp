/* -*- C++ -*-
 *
 * Font installation for StrueTypeFont in drawing library
 */
#include "StrueTypeFont.hpp"

namespace infovis {

static StrueTypeFontCreator creator_instance;

void installStrueTypeFont() {
  // The constructor automatically registers the creator
  // Just ensure StrueTypeFont is initialized
  StrueTypeFont::initialize();
}

} // namespace infovis
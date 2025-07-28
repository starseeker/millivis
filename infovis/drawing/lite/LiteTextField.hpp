/* -*- C++ -*-
 *
 * Copyright (C) 2016 Jean-Daniel Fekete
 * 
 * This file is part of MillionVis.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef INFOVIS_DRAWING_LITE_LITETEXTFIELD_HPP
#define INFOVIS_DRAWING_LITE_LITETEXTFIELD_HPP

#include <infovis/drawing/lite/LiteLabel.hpp>
#include <infovis/drawing/inter/Interactor3States.hpp>
#include <infovis/drawing/inter/KeyCodeInteractor.hpp>

namespace infovis {

/**
 * Lite for displaying a text field.
 */
class LiteTextField : public LiteLabel,
		      public Interactor3States,
		      public KeyCodeInteractor
{
public:
  LiteTextField(const string& text,
		float width,
		justification j = just_left,
		Font * font = 0,
		const Color& fg = color_black,
		const Color& bg = color_white,
		const Color& gfg = color_none);

  virtual int getCaretPosition() const;
  virtual void setCaretPosition(unsigned pos);

  virtual void doRender(const RenderContext& c);
  virtual Lite * clone() const;
  virtual Interactor * interactor(const string& name, int toolid);
  
  // Interactor3States
  virtual bool doStart(const Event& ev);
  virtual void doDrag(const Event& ev);
  virtual void doFinish(const Event& ev);

  // KeyCodeInteractor
  virtual void doKeyDown(KeyCode code);
protected:
  int caret_position_;
};

} // namespace infovis


#endif // INFOVIS_DRAWING_LITE_LITETEXTFIELD_HPP

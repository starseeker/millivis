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
#include <LayoutVisuSD.hpp>

namespace infovis {

LayoutVisuSliceAndDice::LayoutVisuSliceAndDice(LiteTreemap * tm)
  : LayoutVisu(tm)
{ }

unsigned
LayoutVisuSliceAndDice::draw(float param)
{
  int displayed;

  glPushAttrib(GL_FOG_BIT);
  glFogi(GL_FOG_START, 0);
  //glFogi(GL_FOG_END, tm_->max_depth_.getBoundedRange()->value());
  glFogi(GL_FOG_END, 15);
  glShadeModel(GL_SMOOTH);
  glPushMatrix();
  glScalef(1, 1, -1);
  tm_->drawer_.start();
  if (param == 0) {
    treemap_slice_and_dice<
      Tree,
      Box,
      const FloatColumn&,
      //const FloatColumn&,
      Drawer&
      > treemap(tm_->tree_,
#ifdef VECTOR_AS_TREE
		tm_->tree_.get_prop_numeric(tm_->weight_prop_),
#else
		*FloatColumn::find(tm_->weight_prop_, tm_->tree_),
#endif
		tm_->drawer_);
    displayed = treemap.visit(((node_depth(tm_->current_root_,tm_->tree_)&1)
			       == 0) ? left_to_right : top_to_bottom,
			      tm_->getBounds(),
			      tm_->current_root_);
  }
  else {
#ifdef VECTOR_AS_TREE
    Interp weight2(tm_->tree_.get_prop_numeric(tm_->weight_prop_),
		   tm_->tree_.get_prop_numeric(tm_->weight2_prop_));
#else
    Interp weight2(*FloatColumn::find(tm_->weight_prop_, tm_->tree_),
		   *FloatColumn::find(tm_->weight2_prop_, tm_->tree_));
#endif
    weight2.set_std_balance();
    weight2.set_param(param);
    treemap_slice_and_dice<
      Tree,
      Box,
      Interp&,
      //Interp&,
      Drawer&
      > treemap(tm_->tree_,
		weight2,
		//total_weight2,
		tm_->drawer_);

    displayed = treemap.visit(((node_depth(tm_->current_root_, tm_->tree_)&1)
			       == 0) ? left_to_right : top_to_bottom,
			      tm_->getBounds(),
			      tm_->current_root_);
  }
  tm_->drawer_.finish();
  glShadeModel(GL_FLAT);
  glPopMatrix();
  glPopAttrib();
  return displayed;
}

unsigned
LayoutVisuSliceAndDice::pick(float param)
{
  int displayed;

  tm_->picker_.start();
  tm_->picker_.set_labels_clip(tm_->hilite_box_);

  if (param == 0.0f) {
    treemap_slice_and_dice<
      Tree,
      Box,
      const FloatColumn&,
      //const FloatColumn&,
      Picker&
      > treemap(tm_->tree_,
#ifdef VECTOR_AS_TREE
		tm_->tree_.get_prop_numeric(tm_->weight_prop_),
#else
		*FloatColumn::find(tm_->weight_prop_, tm_->tree_),
#endif
		tm_->picker_);
    displayed = treemap.visit(((node_depth(tm_->current_root_,tm_->tree_)&1)
			       == 0) ? left_to_right : top_to_bottom,
			      tm_->getBounds(),
			      tm_->current_root_);
  }
  else {
#ifdef VECTOR_AS_TREE
    Interp weight2(tm_->tree_.get_prop_numeric(tm_->weight_prop_),
		   tm_->tree_.get_prop_numeric(tm_->weight2_prop_));
#else
    Interp weight2(*FloatColumn::find(tm_->weight_prop_, tm_->tree_),
		   *FloatColumn::find(tm_->weight2_prop_, tm_->tree_));
#endif
    weight2.set_std_balance();
    //weight2.set_balance(total_weight2.get_balance());
    weight2.set_param(param);
    //total_weight2.set_param(param);
    treemap_slice_and_dice<
      Tree,
      Box,
      Interp&,
      //Interp&,
      Picker&
      > treemap(tm_->tree_,
		weight2,
		//total_weight2,
		tm_->picker_);

    displayed = treemap.visit(((node_depth(tm_->current_root_, tm_->tree_)&1)
			       == 0) ? left_to_right : top_to_bottom,
			      tm_->getBounds(),
			      tm_->current_root_);
  }

#ifdef VECTOR_AS_TREE
  Tree::string_prop& name = tm_->tree_.get_prop_string(tm_->name_prop_);
#else
  StringColumn& name = *StringColumn::find(tm_->name_prop_, tm_->tree_);
#endif
  float line_alpha = tm_->menu_.getLeftRight().getBoundedRange()->value();
  tm_->picker_.finish(tm_->getBounds(),
		      tm_->label_font_,
		      line_alpha,
		      name);
  return displayed;
}

void
LayoutVisuSliceAndDice::boxlist(float param,
				AnimateTree::BoxList& bl, int depth)
{
  int displayed;

  BoxDrawer drawer(tm_->drawer_, bl);
  drawer.set_depth(depth);
  drawer.start();

  if (param == 0.0f) {
    treemap_slice_and_dice<
      Tree,
      Box,
      const FloatColumn&,
      //const FloatColumn&,
      BoxDrawer&
      > treemap(tm_->tree_,
#ifdef VECTOR_AS_TREE
		tm_->tree_.get_prop_numeric(tm_->weight_prop_),
#else
		*FloatColumn::find(tm_->weight_prop_, tm_->tree_),
#endif
		drawer);
    displayed = treemap.visit(((node_depth(tm_->current_root_,tm_->tree_)&1)
			       == 0) ? left_to_right : top_to_bottom,
			      tm_->getBounds(),
			      tm_->current_root_);
  }
  else {
#ifdef VECTOR_AS_TREE
    Interp weight2(tm_->tree_.get_prop_numeric(tm_->weight_prop_),
		   tm_->tree_.get_prop_numeric(tm_->weight2_prop_));
#else
    Interp weight2(*FloatColumn::find(tm_->weight_prop_, tm_->tree_),
		   *FloatColumn::find(tm_->weight2_prop_, tm_->tree_));
#endif
    weight2.set_std_balance();
    weight2.set_param(param);
    treemap_slice_and_dice<
      Tree,
      Box,
      Interp&,
      //Interp&,
      BoxDrawer&
      > treemap(tm_->tree_,
		weight2,
		//total_weight2,
		drawer);

    displayed = treemap.visit(((node_depth(tm_->current_root_, tm_->tree_)&1)
			       == 0) ? left_to_right : top_to_bottom,
			      tm_->getBounds(),
			      tm_->current_root_);
  }
  drawer.finish();
}

} // namespace infovis

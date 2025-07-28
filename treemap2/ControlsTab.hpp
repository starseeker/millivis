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
#ifndef TREEMAP2_CONTROLSTAB_HPP
#define TREEMAP2_CONTROLSTAB_HPP

#include <infovis/drawing/notifiers/BoundedRange.hpp>
#include <infovis/drawing/notifiers/BeginEnd.hpp>
#include <infovis/drawing/lite/LiteBackground.hpp>
#include <infovis/drawing/inter/InteractorEnterLeave.hpp>
#include <vector>

#include <AnimateTree.hpp>
#include <ColorRamp.hpp>
#include <LiteTreemap.hpp>
#include <Recorder.hpp>

namespace infovis {

class BarGraph;
class Font;
class LiteBox;
class LiteComboBox;
class LiteMenuSimple;
class LiteRangeSlider;
class LiteRangeSliderGraph;
class LiteSliderExt;
class LiteTabbed;
class LiteToggleButton;

class ControlsTab : public LiteBackground,
		    public InteractorEnterLeave,
		    public BoundedRangeObserver,
		    public BeginEndObserver
{
public:
  ControlsTab(LiteTreemap * tm,
	      const Box& screen,
	      Font * label_font,
	      BoundedRangeObservable * param,
	      BoundedRangeObservable * transparency,
	      BoundedRangeObservable * plot_range);
  

  virtual Lite * clone() const;
  virtual Interactor * interactor(const string& name, int tool_id);

  void create_menus();
  void fill_weight_attributes_menu();
  void fill_x_attributes_menu();
  void fill_y_attributes_menu();
  void fill_color_attributes_menu();
  void fill_color_ramp_menu();
  void fill_color_range_slider();
  void fill_sort_by_menu();
  void fill_layout_menu();

  void setWeightAttribute(const string& prop);
  void setXAttribute(const string& prop);
  void setYAttribute(const string& prop);
  void setColorAttribute(const string& prop);
  void setRamp(Ramp r);
  void set_color_range(float min, float range);
  void sortBy(const string& order);
  void setLayout(LiteTreemap::Layout l);
  
  // InteractorEnterLeave
  virtual bool doEnter(const Event&);
  virtual void doLeave(const Event&);

  // BeginEndObserver
  virtual void begin(BeginEndObservable *);
  virtual void end(BeginEndObservable *);

  void disableControls_Except(BeginEndObservable * obs);
  void enableAllControls();

  // BoundedRangeObserver
  virtual void valueDragged(BoundedRangeObservable * obs);
  virtual void rangeDragged(BoundedRangeObservable * obs);
  
  virtual void updateBoundedRange(BoundedRangeObservable * obs);

  void record();
  void replayCurrent();
  

  //protected:
  virtual void doRender(const RenderContext&);
  void record(RecordItem& item);
  void replaySimple(const RecordItem& item);
  bool replayOneSquarified(const RecordItem& cur, const RecordItem& target);
  bool replayOneStrip(const RecordItem& cur, const RecordItem& target);
  bool replayOneSliceAndDice(const RecordItem& cur, const RecordItem& target);
  bool replayOneScatterPlot(const RecordItem& cur, const RecordItem& target);
  bool replayOne(const RecordItem& target);
  void replay(const RecordItem& item);

  
  void create_numeric(const FloatColumn * col);
  void create_string(const StringColumn * col);
  void filter(int index);
  void applyFilters(unsigned index);
protected:
  LiteTreemap * treemap_;
  Tree& tree_;
  Font * label_font_;
  FilterColumn * filter_;

  LiteBox * box_;
  LiteTabbed * tab_;

  LiteBox * queries_;
  std::vector<LiteRangeSlider*> slider_;
  std::vector<const column*> column_;

  LiteBox * controls_;
  DefaultBoundedRangeObservable animation_duration_;
  LiteSliderExt * animation_duration_slider_;

  LiteToggleButton * label_toggle_;
public:
  LiteSliderExt * border_slider_;
protected:
  BoundedRangeObservable * param_;
public:
  LiteSliderExt * blend_slider_;
protected:
  BoundedRangeObservable * transparency_;
  LiteSliderExt * transparency_slider_;
  BoundedRangeObservable * plot_range_;
  LiteRangeSlider * plot_range_slider_;

  LiteMenuSimple * weight_attributes_menu_;
  LiteComboBox * weight_attributes_combo_;

  LiteMenuSimple * x_attributes_menu_;
  LiteComboBox * x_attributes_combo_;

  LiteMenuSimple * y_attributes_menu_;
  LiteComboBox * y_attributes_combo_;

  DefaultBoundedRangeObservable color_range_;
  BarGraph * color_bargraph_;
  LiteRangeSliderGraph * color_range_slider_;

  LiteMenuSimple * sort_by_menu_;
  LiteComboBox * sort_by_combo_;
  string current_sort_by_;

  LiteMenuSimple * color_attributes_menu_;
  LiteComboBox * color_attributes_combo_;

  LiteMenuSimple * color_ramp_menu_;
  LiteComboBox * color_ramp_combo_;
  Ramp current_ramp_;

  LiteMenuSimple * layout_menu_;
  LiteComboBox * layout_combo_;
  LiteTreemap::Layout current_layout_;

public:
  Recorder recorder_;
protected:
  LiteSliderExt * replay_slider_;

  AnimateTree animate_;
};

} // namespace infovis


#endif // TREEMAP2_CONTROLSTAB_HPP


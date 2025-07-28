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
#include <infovis/drawing/Font.hpp>
#include <infovis/drawing/Image.hpp>
#include <infovis/drawing/gl_support.hpp>
#include <infovis/drawing/inter/Interactor3States.hpp>
#include <infovis/drawing/inter/KeyCodes.hpp>
#include <infovis/drawing/inter/KeyboardHandler.hpp>
#include <infovis/drawing/inter/MouseHandler.hpp>
#include <infovis/drawing/inter/MouseCodes.hpp>
#include <infovis/drawing/lite/LiteWindow.hpp>
#include <infovis/drawing/lite/LiteComboBox.hpp>
#include <infovis/drawing/lite/LiteSliderExt.hpp>
#include <infovis/table/metadata.hpp>
#include <infovis/drawing/notifiers/BoundedRange.hpp>
#include <ControlsTab.hpp>
#include <infovis/tree/dir_tree.hpp>
#include <infovis/tree/xmltree_tree.hpp>
#include <infovis/tree/xml_tree.hpp>
#include <infovis/tree/algorithm.hpp>
#include <infovis/tree/sum_weight_visitor.hpp>

#include <types.hpp>
#include <ColorRamp.hpp>
#include <FastDrawer.hpp>
#include <FileType.hpp>
#include <LiteTreemap.hpp>
#include <LiteSpeed.hpp>
#include <Properties.hpp>
#include <LiteRangeSliderGraph.hpp>
#include <LayoutVisu.hpp>
#include <DynaQueries.hpp>

#include <functional>
#include <cmath>
#include <cfloat>
#include <cstdlib>
#include <iostream>
#ifdef WIN32
#define isnan _isnan
#endif
#define VERTEXZ
#define USE_ARRAYS
//#define BUTTON_RIGHT_INTERPOLATE
#define USE_FOG
#define USE_SAVE_UNDER

using namespace infovis;

#undef DBG
#ifndef NDEBUG
#define DBG {						\
  int err = glGetError();				\
  if (err)						\
    std::cerr << __FILE__ << "(" << __LINE__ << ") : "	\
	      << "gl error: " << gl::glErrorString(err)	\
	      << std::endl;				\
}
#else
#define DBG
#endif

static Properties * props;
static string type_prop("type");


class TreemapWindow : public LiteWindow,
		      public Interactor3States,
		      public MouseHandler,
		      public KeyboardHandler
{
public:
  TreemapWindow(const string& name,
		const Box& b,
		Tree& t,
		const string& w_prop,
		const string& w_prop2,
		const string& color_prop)
    : LiteWindow(name, b, cap_double | cap_rgba | cap_stencil),
      tree_(t),
      tree_depth_(depth(tree_)),
      save_under_(LiteWindow::screenWidth(), LiteWindow::screenHeight(),
		  true),
      weight_prop_(w_prop),
      weight_prop2_(w_prop2),
      color_prop_(color_prop),
      drawer_(tree_, FloatColumn::find(color_prop_, tree_)),
      treemap_(0),
      inter_(0),
      inter_lite_(0),
      label_font_(props->get_font("label.font", 
				  Font::create("Arial", "plain", 12))),
      monitor_font_(props->get_font("monitor.label.font",
				    Font::create("Arial", "plain", 12))),
      controls_(0),
      param_(0.0f, 1.0f, 0.0f),
      transparency_(0, 100, 0),
      plot_range_(1, 30, 1, 14),
      color_range_(0.0f, 7.0f, 0.0f, 7.0f),
      speed_(0,
	     Box(xmin(bounds), ymax(bounds)-monitor_font_->getHeight(),
		 xmax(bounds), ymax(bounds)),
	     monitor_font_),
      dryrun_(false)
  {
    treemap_ = new LiteTreemap(Box(xmin(bounds),
				   ymin(bounds),
				   xmax(bounds),
				   ymax(bounds)-monitor_font_->getHeight()),
			       save_under_,
			       drawer_,
			       label_font_,
			       tree_,
			       "name",
			       weight_prop_,
			       weight_prop2_,
			       color_prop_,
			       param_,
			       transparency_,
			       plot_range_
			       );

    addChild(treemap_);

    controls_ = new ControlsTab(treemap_,
				b,
				label_font_,
				&param_,
				&transparency_,
				&plot_range_);
    controls_->sortBy("<"+weight_prop_);
    controls_->setWeightAttribute(weight_prop_);
    controls_->setXAttribute(weight_prop_);
    controls_->setYAttribute(weight_prop2_);
    controls_->setColorAttribute(color_prop_);
    controls_->setRamp(ramp_categorical2);
    controls_->setLayout(LiteTreemap::layout_squarified);
    
    addChild(controls_);
    controls_->setVisible(false);

    //speed_.setPosition(Point(0, monitor_font_->getDescent()));
    speed_.setTreemap(treemap_);
    addChild(&speed_);
  }

  void setDryRun(bool d) {
    dryrun_ = d;
    drawer_.set_dryrun(d);
  }

  void doKeyboard(int key, bool down, int x, int y) {
    static bool is_saving = false;

    switch(key) {
    case 27:			// escape
      exit(0);
    case keycode_return:
      if (! down)
	controls_->record();
      break;
    case keycode_left:
      if (controls_->recorder_.size() != 0) {
	if (down) {
	  controls_->recorder_.advance(-1);
	  controls_->recorder_.notifyBoundedRange();
	}
	else 
	  treemap_->endAnimation();
      }
      break;
    case keycode_right:
      if (controls_->recorder_.size() != 0) {
	if (down) {
	  controls_->recorder_.advance(1);
	  controls_->recorder_.notifyBoundedRange();
	}
	else 
	  treemap_->endAnimation();
      }
      break;
    case keycode_space:
      if (down && inter_ == 0) {
	controls_->setVisible(! controls_->isVisible());
      }
      break;
    case 's':
      if (down){
	if (is_saving) {
	  endSave();
	  setTimeDivisor();
	}
	else {
	  setTimeDivisor(5);
	  beginSave("save");
	}
	is_saving = ! is_saving;
      }
      break;
    default:
      treemap_->doKeyboard(key, down, x, y);
      return;
    }
    repaint();
  }
  void doReshape(int w, int h) {
    Box bounds(getBounds());
    treemap_->enableSaveUnder();
    treemap_->setBounds(Box(xmin(bounds),
			    ymin(bounds),
			    xmax(bounds),
			    ymax(bounds)-monitor_font_->getHeight()));
    speed_.setBounds(Box(xmin(bounds),
			 ymax(bounds)-monitor_font_->getHeight(),
			 xmax(bounds),
			 ymax(bounds)));
    controls_->border_slider_->setMaxBounds(treemap_->getBounds());
    controls_->blend_slider_->setMaxBounds(treemap_->getBounds());
  }

  virtual void doMouse(int button, bool down, int x, int y) {
    if (button == button_right && down) {
      controls_->setVisible(! controls_->isVisible());
      controls_->setPosition(Point(x,y));
      repaint();
    }
    LiteWindow::doMouse(button,down,x, y);
  }

  Lite * clone() const {
    return new TreemapWindow(getName(),
			     bounds,
			     tree_,
			     weight_prop_,
			     weight_prop2_,
			     color_prop_);
  }
  
  //protected:

  Tree& tree_;
  unsigned tree_depth_;
  SaveUnder save_under_;
  string weight_prop_;
  string weight_prop2_;
  string color_prop_;
  FastDrawer drawer_;
  LiteTreemap * treemap_;
  Interactor3States * inter_;
  Lite * inter_lite_;
  Font * label_font_;
  Font * monitor_font_;
  DefaultBoundedRangeObservable param_;
  DefaultBoundedRangeObservable transparency_;
  DefaultBoundedRangeObservable plot_range_;
  DefaultBoundedRangeObservable color_range_;
  ControlsTab * controls_;

  LiteSpeed speed_;

  bool dryrun_;
};

static WeightMap&
create_sum_weight(Tree& t, const string& prop)
{
  FloatColumn * swm = FloatColumn::find(prop, t);
  sum_weights(t, *swm);
  return *swm;
}

static bool
compute_file_types(Tree& t)
{
  FileType ft;
  ft.load("file_types.txt");
  column * c = t.find_column("name");
  if (c == 0)
    return false;

  StringColumn& name = *StringColumn::cast(c);
  FloatColumn& type = *FloatColumn::find("type", t);
  type.put_metadata(metadata::type, metadata::type_categorical);

  for (int i = 0; i < name.size(); i++) {
    float code = ft.fileType(name[i]).getCode();
    if (code != 0)
      code--;
    type[i] = code;
  }
  return true;
}

struct log_fn {
  const WeightMap& wm;
  log_fn(const WeightMap& w) : wm(w) { }
  float operator()(int i) const {
    double d = log(wm[i]+1.0);
    if (isnan(d)) {
      std::cerr << "got a nan for wm[" << i << "] = " << wm[i] << std::endl;
      return 0;
    }
    return d;
  }
};

struct degree_fn {
  const Tree& tree;
  degree_fn(const Tree& t) : tree(t) { }
  float operator()(node_descriptor n) const {
    return is_leaf(n, tree) ? 1 : 0;
  }
};

struct sqrt_fn {
  const WeightMap& wm;
  sqrt_fn(const WeightMap& w) : wm(w) { }
  float operator()(node_descriptor n) const {
    double d = sqrt(wm[n]);
    if (isnan(d)) {
      std::cerr << "got a nan for wm[" << n << "] = " << wm[n] << std::endl;
      return 0;
    }
    return d;
  }
};

struct depth_visitor {
  int depth_;
  WeightMap& col_;

  depth_visitor(WeightMap& col)
    : depth_(0), col_(col)
  { }

  void preorder(node_descriptor n) {
    col_[n] = depth_++;
  }
  void inorder(node_descriptor n) {
  }
  void postorder(node_descriptor n) {
    --depth_;
  }
};

static void
add_metadata(Tree& t)
{
  for (int i = 0; t.column_count(); i++) {
    column * col = t.get_column(i);
    if (col->get_name()[0] == '$')
      continue;			// don't look at internal columns
    FloatColumn * flt = FloatColumn::cast(col);
    if (flt == 0)
      continue;			// don't worry about string columns for now
    
  }
}

struct loader {
  const char * toload;
  Tree& t;

  loader(const char * tl, Tree& tree)
    : toload(tl),
      t(tree)
  { }

  void operator()() {
    if (toload == 0) {
      toload = ".";
      std::cout << "Loading current directory\n";
      dir_tree(".", t);
    }
    else {
      std::cout << "Trying to load " << toload << " as an xml file\n";
      if (xmltree_tree(toload, t) == 1) {
	if (xml_tree(toload, t) == 1) {
	  std::cout << "Not XML, trying as a directory\n";
	  dir_tree(toload, t);
	}
      }
    }
    std::cout << "Loaded\n";
  }
};

static Tree t;

static float abs(float a) { return a < 0 ? -a : a; }

static bool is_sum_weights(node_descriptor n,
			   const Tree& t,
			   const FloatColumn& c)
{
  if (is_leaf(n, t))
    return true;
  float sum = 0;
  Tree::children_iterator i, e;
  boost::tie(i, e) = children(n, t);
  while (i != e) {
    if (! is_sum_weights(*i, t, c))
      return false;
    sum += c[*i++];
  }
  float diff = abs(sum - c[n]);
  if (diff <= (abs(sum) * 0.00001))
    return true;
  else
    return false;
}

static void hide_sums(const Tree& t, column * c)
{
  for (node_descriptor n = 0; n < c->size(); n++) {
    if (! is_leaf(n, t))
      c->undefine(n);
  }
}

static void hide_sums(const Tree& t)
{
  for (int i = 0; i < t.column_count(); i++) {
    column * c = t.get_column(i);
    if (c->get_name()[0] == '$')
      continue;
    hide_sums(t, c);
  }
}


int main(int argc, char * argv[])
{
  LiteWindow::init(argc, argv);
  Properties::load();
  props = Properties::instance();

  int i;
  boost::function_requires< ParentedTreeConcept<Tree> >();

  StringColumn * names = StringColumn::find("name", t);

  const char * toload = 0;
  bool fullscreen = true;
  int width = 720, height = 480;
  bool dryrun = false;
  bool soft_cursor = false;

  for (i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      switch(argv[i][1]) {
      case 'f':
	fullscreen = ! fullscreen;
	break;
      case 'w':
	i++;
	if (i < argc) {
	  width = atoi(argv[i]);
	}
	break;
      case 'h':
	i++;
	if (i < argc) {
	  height = atoi(argv[i]);
	}
	break;
      case 'd':
	dryrun = true;
	break;
      case 'c':
	soft_cursor = !soft_cursor;
	break;
      default:
	std::cerr << "syntax: " << argv[0]
		  << "[-f] [-w <width>] [-h height] [-d] [-c] dir-or-xml-file\n";
	exit(1);
      }
    }
    else {
      toload = argv[i];
    }
  }
  if (toload == 0) {
    std::cout << "Loading current directory\n";
    dir_tree(".", t);
    toload = ".";
  }
  else {
    std::cout << "Trying to load " << toload << " as an xml file\n";
    if (xml_tree(toload, t) == 1) {
      std::cout << "Not XML, trying as a directory\n";
      dir_tree(toload, t);
    }
  }
  std::cout << "Loaded\n";

  (*names)[root(t)] = toload;
  string prop;
  FloatColumn * weight = 0;

  for (Tree::names_iterator n = t.begin_names();
       n != t.end_names(); n++) {
    prop = *n;
    FloatColumn * c = FloatColumn::cast(t.find_column(prop));
    if (c != 0 &&
	c->get_metadata(metadata::aggregate) == metadata::aggregate_sum) {
      weight = c;
      break;
    }
  }
  if (weight == 0) {
    for (Tree::names_iterator n = t.begin_names();
	 n != t.end_names(); n++) {
      prop = *n;
      FloatColumn * c = FloatColumn::cast(t.find_column(prop));
      if (c != 0) {
	  weight = c;
	  break;
      }
    }
    if (weight == 0) {
      std::cerr << "Cannot find a weight column\n";
      return 1;
    }
    else {
      sum_weights(t, *weight);
      weight->put_metadata(metadata::aggregate, metadata::aggregate_sum);
    }
  }

  FloatColumn * log = FloatColumn::find(subprop("log", prop), t);
  fill_column(*log, log_fn(*FloatColumn::cast(weight)));
  sum_weights(t, *log);
  log->put_metadata(metadata::aggregate, metadata::aggregate_sum);
  WeightMap& sw = *log;

  FloatColumn * degree = FloatColumn::find("degree", t);
  fill_column(*degree, degree_fn(t));
  sum_weights(t, *degree);
  degree->put_metadata(metadata::aggregate, metadata::aggregate_sum);

  FloatColumn * sqrt = FloatColumn::find(subprop("sqrt", prop), t);
  fill_column(*sqrt, sqrt_fn(*FloatColumn::cast(weight)));
  sum_weights(t, *sqrt);
  sqrt->put_metadata(metadata::aggregate, metadata::aggregate_sum);

  FloatColumn * depth = FloatColumn::find("depth", t);
  
  depth_visitor visitor(*depth);
  traverse_tree(root(t), t, visitor);

  FilterColumn::find("$filter", t); // create a filter
  
  if (! compute_file_types(t))
    type_prop = prop;
  t.sort_columns();
  hide_sums(t);
  TreemapWindow win(argv[0],
		    Box(0, 0, width, height),
		    t,
		    subprop("log", prop),
		    prop,
		    type_prop);
#ifdef USE_FOG
  glEnable(GL_FOG);
  glFogi(GL_FOG_MODE, GL_LINEAR);
  glFogi(GL_FOG_START, 0);
  glFogi(GL_FOG_END, 15);
  GLfloat fogColor[4] = {0.0f, 0.0f, 0.0f, 1.0f};
  glFogfv(GL_FOG_COLOR, fogColor);
  glHint (GL_FOG_HINT, GL_DONT_CARE);
#endif
#ifndef NO_STENCIL
  win.setClear(LiteWindow::buffer_stencil);
  glDisable(GL_STENCIL_TEST);
  //win.setClear(LiteWindow::buffer_depth);
#endif
  if (soft_cursor) {
    Image * image = Image::load("cursor_l.png");
    win.setCursor(image);
  }
  //win.setColor(color_none);
  win.setColor(props->get_color("background.color",
				color_black));
  win.setDryRun(dryrun);

  if (fullscreen)
    win.setFullscreen();
  win.setNearPlane(-1);
  win.setFarPlane(3000);
  try {
    win.run();
  }
  catch(...) {
    return 1;
  }
  return 0;
}

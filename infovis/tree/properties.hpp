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
#ifndef BOOST_TREE_PROPERTIES_HPP
#define BOOST_TREE_PROPERTIES_HPP

#include <boost/alloc.hpp>
#include <boost/config.hpp>
#include <cassert>
#include <boost/graph/properties.hpp>
#include <boost/tree/tree_traits.hpp>

namespace boost {

  namespace detail {
    enum tree_property_tag_num
    {
      node_index_num = 1,
      node_name_num,
      tree_name_num,
      node_weight_num,
      node_weight2_num,
      node_distance_num,
      node_color_num,
      node_degree_num,
      node_all_num,
      tree_all_num,
      last_tree_property_num
    };
  } // namespace detail

  struct tree_property_tag { };
  struct node_property_tag { };

#define BOOST_DEF_PROPERTY(KIND, NAME) \
  enum KIND##_##NAME##_t { KIND##_##NAME = detail::KIND##_##NAME##_num  }; \
  BOOST_INSTALL_PROPERTY(KIND, NAME)

  BOOST_DEF_PROPERTY(node,index);
  BOOST_DEF_PROPERTY(node,name);
  BOOST_DEF_PROPERTY(tree,name);
  BOOST_DEF_PROPERTY(node,weight);
  BOOST_DEF_PROPERTY(node,weight2);
  BOOST_DEF_PROPERTY(node,distance);
  BOOST_DEF_PROPERTY(node,color);
  BOOST_DEF_PROPERTY(node,degree);
  BOOST_DEF_PROPERTY(node,all);
  BOOST_DEF_PROPERTY(tree,all);
#undef BOOST_DEF_PROPERTY

  namespace detail {

    struct dummy_node_property_selector {
      template <class Tree, class Property, class Tag>
      struct bind {
        typedef identity_property_map type;
        typedef identity_property_map const_type;
      };
    };

  } // namespace detail

  // Tree classes can either partially specialize property_map
  // or they can specialize these two selector classes.
  template <class TreeTag>
  struct node_property_selector {
    typedef detail::dummy_node_property_selector type;
  };

  template <class Tree, class Property>
  class tree_property {
  public:
    typedef typename property_value<typename Tree::tree_property_type, 
      Property>::type type;
  };

  template <class Tree>
  class node_property {
  public:
    typedef typename Tree::node_property_type type;
  };

  template <typename Tree>
  class tree_degree_property_map 
    : public put_get_helper<typename tree_traits<Tree>::degree_size_type,
                            tree_degree_property_map<Tree> >                  
  {
  public:
    typedef typename tree_traits<Tree>::node_descriptor key_type;
    typedef typename tree_traits<Tree>::degree_size_type value_type;
    typedef value_type reference;
    typedef readable_property_map_tag category;
    tree_degree_property_map(const Tree& t) : m_t(t) { }
    value_type operator[](const key_type& v) const {
      return degree(v, m_t);
    }
  private:
    const Tree& m_t;
  };

  template <typename Tree>
  inline tree_degree_property_map<Tree>
  make_tree_degree_map(const Tree& t) {
    return tree_degree_property_map<Tree>(t);
  }

} // namespace boost

#endif // BOOST_TREE_PROPERTIES_HPP

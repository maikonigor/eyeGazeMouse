#include "Configuration.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
#include <exception>
#include <iostream>

namespace pt = boost::property_tree;

void Configuration::load(const std::string &filename)
{
    // Create empty property tree object
    pt::ptree tree;

    // Parse the XML into the property tree.
    pt::read_xml(filename, tree);
    
    // Load the pre, gaze and post components
    BOOST_FOREACH(pt::ptree::value_type &v, tree.get_child("components.pre")) {
        std::cout << "Pre: " << v.second.data() << std::endl;
        pre_components.push_back(v.second.data());
        all_components.push_back(v.second.data());
    }
    
    BOOST_FOREACH(pt::ptree::value_type &v, tree.get_child("components.gaze")) {
        std::cout << "Gaze: " << v.second.data() << std::endl;
        gaze_components.push_back(v.second.data());
        all_components.push_back(v.second.data());
    }
    
    BOOST_FOREACH(pt::ptree::value_type &v, tree.get_child("components.post")) {
        std::cout << "Post: " << v.second.data() << std::endl;
        post_components.push_back(v.second.data());
        all_components.push_back(v.second.data());
    }
}

#include <cmath>

#include "extreme_value.hpp"

namespace prot {

ExtremeValue::ExtremeValue (double one_match_prob, double test_num, 
                            double adjust_factor) {
  one_match_prob_ = one_match_prob;
  test_num_ = test_num;
  adjust_factor_ = adjust_factor;
  init();
}

ExtremeValue::ExtremeValue (xercesc::DOMElement* element){
  one_match_prob_ = getDoubleChildValue(element, "one_match_probability", 0);
  test_num_ = getDoubleChildValue(element, "test_number", 0);
  adjust_factor_ = getDoubleChildValue(element, "adjust_factor", 0);
  init();
}
    
void ExtremeValue::init() {
  e_value_ = one_match_prob_ * test_num_ * adjust_factor_;
  if (one_match_prob_ > 1) {
    p_value_  = 1.0;
  }
  else {
    double n = test_num_ * adjust_factor_;
    // approximation of 1 - (1- one_match_prob)^test_num
    // pValue = 1 - Math.pow(1.0 - oneMatchProb, nTests * adjustMul);
    p_value_ =  n * one_match_prob_ 
        - (n * (n-1))/2 * one_match_prob_ * one_match_prob_
        + (n*(n-1) * (n-2))/6 * std::pow(one_match_prob_,3);
    if (p_value_ > 1.0) {
      p_value_ = 1.0;
    }
  }
}
    
void ExtremeValue::appendXml(XmlDOMDocument* xml_doc, xercesc::DOMElement* parent) {
  xercesc::DOMElement* element = xml_doc->createElement("extreme_value");
  std::string str = convertToString(one_match_prob_);
  xml_doc->addElement(element, "one_match_probability", str.c_str());
  str = convertToString(test_num_);
  xml_doc->addElement(element, "test_number", str.c_str());
  str = convertToString(adjust_factor_);
  xml_doc->addElement(element, "adjust_factor", str.c_str());
  str = convertToString(p_value_);
  xml_doc->addElement(element, "p_value", str.c_str());
  str = convertToString(e_value_);
  xml_doc->addElement(element, "e_value", str.c_str());
    parent->appendChild(element);
}

}

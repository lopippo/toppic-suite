/*
 * Activation.cpp
 *
 *  Created on: Nov 25, 2013
 *      Author: xunlikun
 */

#include "base/activation.hpp"
#include "base/ion_type.hpp"
#include "base/xml_dom_document.hpp"

namespace prot {

Activation::Activation(std::string name, IonTypePtr n_ion_type_ptr, IonTypePtr c_ion_type_ptr) {
  name_ = name;
  n_ion_type_ = n_ion_type_ptr;
  c_ion_type_ = c_ion_type_ptr;
}

Activation::Activation(IonTypePtrVec ion_type_list,
                       xercesc::DOMElement * element) {
  name_ = getChildValue(element, "name", 0);
  n_ion_type_ = getIonTypePtrByName(ion_type_list,getChildValue(element, "n_ion_type", 0));
  c_ion_type_ = getIonTypePtrByName(ion_type_list,getChildValue(element, "c_ion_type", 0));

}

ActivationPtrVec getActivationPtrVecInstance(IonTypePtrVec ion_type_list, std::string file_name){
  ActivationPtrVec activationPtrVec;
  XmlDOMParser* parser = XmlDOMParserFactory::getXmlDOMInstance();
  if (parser) {
    XmlDOMDocument doc(parser, file_name.c_str());
    xercesc::DOMElement* parent = doc.getDocumentElement();
    int activation_num = getChildCount(parent, "activation");
    for (int i = 0; i < activation_num; i++) {
      xercesc::DOMElement* element = getChildElement(parent, "activation", i);
      activationPtrVec.push_back(ActivationPtr(new Activation(ion_type_list,element)));
    }
  }
  return activationPtrVec;
}

ActivationPtr getActivationPtrByName(ActivationPtrVec activation_list,std::string name){
  for (unsigned int i = 0; i < activation_list.size(); i++) {
    std::string n = activation_list[i]->getName();
    if (n == name) {
      return activation_list[i];
    }
  }
  return ActivationPtr(nullptr);
}

} /* namespace prot */

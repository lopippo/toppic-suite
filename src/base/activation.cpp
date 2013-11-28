/*
 * Activation.cpp
 *
 *  Created on: Nov 25, 2013
 *      Author: xunlikun
 */

#include "activation.hpp"
#include "ion_type.hpp"
#include "xml_dom_document.hpp"

namespace prot {
Activation::Activation(std::string name, IonTypePtr n_ion_type_ptr, IonTypePtr c_ion_type_ptr) {
	name_ = name;
	n_ion_type_ = n_ion_type_ptr;
	c_ion_type_ = c_ion_type_ptr;
}
Activation::Activation(IonTypePtrVec ion_type_list,
		xercesc::DOMElement * element) {
	name_ = getChildValue(element, "name");
	n_ion_type_ = getIonTypePtrByName(ion_type_list,getChildValue(element, "n_ion_type"));
	c_ion_type_ = getIonTypePtrByName(ion_type_list,getChildValue(element, "c_ion_type"));

}

ActivationPtrVec getActivationPtrVectInst(IonTypePtrVec ion_type_list,const char* file_name){
	ActivationPtrVec activationPtrVec;
		prot::XmlDOMParser* parser = prot::getXmlDOMInstance();
		if (parser) {
			prot::XmlDOMDocument* doc = new prot::XmlDOMDocument(parser, file_name);
			int acid_num = doc->getChildCount("activation_list", 0, "activation");
			for (int i = 0; i < acid_num; i++) {
				xercesc::DOMElement* element = doc->getElement("activation", i);
				activationPtrVec.push_back(ActivationPtr(new Activation(ion_type_list,element)));
			}
			delete doc;
		}
		delete parser;
		return activationPtrVec;
}

ActivationPtr getActivationPtrByName(ActivationPtrVec activation_list,std::string name){
  for (unsigned int i = 0; i < activation_list.size(); i++) {
    std::string n = activation_list[i]->getName();
    if (n.compare(name) == 0) {
      return activation_list[i];
    }
  }
  return ActivationPtr(nullptr);
}

} /* namespace prot */
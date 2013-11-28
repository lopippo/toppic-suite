/*
 * NeutralLoss.cpp
 *
 *  Created on: Nov 25, 2013
 *      Author: xunlikun
 */

#include "neutral_loss.hpp"
#include "xml_dom_document.hpp"

namespace prot {

NeutralLoss::NeutralLoss(std::string name,double mass){
	name_ = name;
	mass_ = mass;
}
NeutralLoss::NeutralLoss(xercesc::DOMElement * element){
	name_ = getChildValue(element,"name");
	mass_ = getDoubleChildValue(element,"mass");
}

NeutralLossPtrVec getNeutralLossPtrVecInstance(const char* file_name){
	NeutralLossPtrVec neutralLossPtrVec;
	prot::XmlDOMParser* parser = prot::getXmlDOMInstance();
	if (parser) {
		prot::XmlDOMDocument* doc = new prot::XmlDOMDocument(parser, file_name);
		int acid_num = doc->getChildCount("neutral_loss_list", 0, "neutral_loss");
		for (int i = 0; i < acid_num; i++) {
			xercesc::DOMElement* element = doc->getElement("neutral_loss", i);
			neutralLossPtrVec.push_back(NeutralLossPtr(new NeutralLoss(element)));
		}
		delete doc;
	}
	delete parser;
	return neutralLossPtrVec;
}
NeutralLossPtr getNeutralLossPtrByName(NeutralLossPtrVec &neutralLoss_ptr_vec, const std::string &name){
	for (unsigned int i = 0; i < neutralLoss_ptr_vec.size(); i++) {
	    std::string n = neutralLoss_ptr_vec[i]->getName();
	    if (n.compare(name) == 0) {
	      return neutralLoss_ptr_vec[i];
	    }
	  }
	  return NeutralLossPtr(nullptr);
}
} /* namespace prot */
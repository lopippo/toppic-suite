// Copyright (c) 2014 - 2017, The Trustees of Indiana University.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimer in the documentation and/or
// other materials provided with the distribution.
//
// Indiana University provides no reassurances that the source code provided does
// not infringe the patent or any other intellectual property rights of any other
// entity. Indiana University disclaims any liability to any recipient for claims
// brought by any other entity based on infringement of intellectual property
// rights or otherwise.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
// ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
// ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


#include "base/string_util.hpp"
#include "prsmview/anno_residue.hpp"

namespace prot {

AnnoResidue::AnnoResidue(ResiduePtr residue_ptr, int pos):
    Residue(residue_ptr->getAcidPtr(),residue_ptr->getPtmPtr()) {
      pos_= pos;
      type_= ANNO_RESIDUE_TYPE_NORMAL;
      is_unexpected_change_ = false;
      unexpected_change_color_ = 0;
    }

void AnnoResidue::appendViewXml(XmlDOMDocument* xml_doc,
                                xercesc::DOMElement* parent){
  xercesc::DOMElement* element = xml_doc->createElement("residue");
  std::string str = StringUtil::convertToString(pos_);
  xml_doc->addElement(element, "position", str.c_str());
  str = getAcidPtr()->getOneLetter();
  xml_doc->addElement(element, "acid", str.c_str());
  str = type_;
  xml_doc->addElement(element, "residue_type", str.c_str());
  str = StringUtil::convertToString(is_unexpected_change_);
  xml_doc->addElement(element, "is_unexpected_change", str.c_str());
  str = StringUtil::convertToString(unexpected_change_color_);
  xml_doc->addElement(element, "unexpected_change_color", str.c_str());
  str = StringUtil::convertToString(possible_pos_color_);
  xml_doc->addElement(element, "possible_pos_color", str.c_str());
  xml_doc->addElement(element, "anno", anno_.c_str());
  parent->appendChild(element);
}

}


//Copyright (c) 2014 - 2019, The Trustees of Indiana University.
//
//Licensed under the Apache License, Version 2.0 (the "License");
//you may not use this file except in compliance with the License.
//You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
//Unless required by applicable law or agreed to in writing, software
//distributed under the License is distributed on an "AS IS" BASIS,
//WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//See the License for the specific language governing permissions and
//limitations under the License.

#ifndef TOPPIC_SEQ_MASS_SHIFT_TYPE_HPP_
#define TOPPIC_SEQ_MASS_SHIFT_TYPE_HPP_

#include <memory>
#include <vector>
#include <string>

#include "common/xml/xml_dom_element.hpp"

namespace toppic {

class XmlDOMDocument;

class MassShiftType;
typedef std::shared_ptr<MassShiftType> MassShiftTypePtr;

class MassShiftType {
 public:
  static const MassShiftTypePtr INPUT;

  static const MassShiftTypePtr FIXED;

  static const MassShiftTypePtr PROTEIN_VARIABLE;

  static const MassShiftTypePtr VARIABLE;

  static const MassShiftTypePtr UNEXPECTED;

  MassShiftType(int id, std::string name): id_(id), name_(name) {}

  int getId() {return id_;}

  std::string getName() {return name_;}

  void appendXml(XmlDOMDocument* xml_doc, XmlDOMElement* parent);

  static MassShiftTypePtr getChangeTypePtrFromXml(XmlDOMElement * element);

  static std::string getXmlElementName() {return "mass_shift_type";}

 private:
  int id_;
  std::string name_;
};

typedef std::vector<MassShiftTypePtr> MassShiftTypePtrVec;

}  // namespace toppic

#endif


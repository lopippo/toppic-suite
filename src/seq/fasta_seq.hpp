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

#ifndef TOPPIC_SEQ_FASTA_SEQ_HPP_
#define TOPPIC_SEQ_FASTA_SEQ_HPP_

#include <memory>

#include "common/util/str_util.hpp"
#include "common/xml/xml_dom_element.hpp"

namespace toppic {

class XmlDOMDocument;

class FastaSeq {
 public:
  FastaSeq(const std::string &name_line, const std::string &ori_seq);

  FastaSeq(const std::string &name, const std::string &desc,
           const std::string &ori_seq);

  std::string getName() {return name_;}

  std::string getDesc() {return desc_;}

  std::string getRawSeq() {return seq_;}

  StringPairVec getAcidPtmPairVec() {return acid_ptm_pair_vec_;}

  int getAcidPtmPairLen() {return acid_ptm_pair_vec_.size();}

  static std::string getXmlElementName() {return "fasta_seq";}

  void appendNameDescToXml(XmlDOMDocument* xml_doc, XmlDOMElement* parent);

  static std::string getNameFromXml(XmlDOMElement* element);

  static std::string getDescFromXml(XmlDOMElement* element);

  static std::string getString(const std::pair<std::string, std::string> &str_pair);

  static std::string getString(const StringPairVec &str_pair_vec);

 private:
  std::string name_;

  std::string desc_;

  std::string seq_;

  StringPairVec acid_ptm_pair_vec_;

  void compAcidPtmPairVec();
};

typedef std::shared_ptr<FastaSeq> FastaSeqPtr;

}  // namespace toppic

#endif

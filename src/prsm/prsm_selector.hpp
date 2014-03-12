/*
 * prsm_selector.hpp
 *
 *  Created on: Feb 18, 2014
 *      Author: xunlikun
 */

#ifndef PRSM_SELECTOR_HPP_
#define PRSM_SELECTOR_HPP_

#include "base/string_util.hpp"
#include "base/proteoform.hpp"
#include "base/fasta_reader.hpp"
#include "prsm/prsm.hpp"
#include "prsm/prsm_writer.hpp"

namespace prot {

class PrSMSelector {
 public:
  PrSMSelector(std::string db_file,std::string spec_file,std::string in_file,std::string out_file,int n_top,std::string type);
  void process();
 private:
  std::string spec_file_;
  std::string db_file_;
  std::string input_file_;
  std::string output_file_;
  int n_top_;
  std::string type_;

  PrSMPtrVec getTopPrsms(PrSMPtrVec selected_prsm,int n_top);
  bool findPrsm(PrSMPtrVec result,PrSMPtr prsm);
};

} /* namespace prot */

#endif /* PRSM_SELECTOR_HPP_ */
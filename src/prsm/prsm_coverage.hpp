#ifndef PROT_PRSM_COVERAGE_HPP_
#define PROT_PRSM_COVERAGE_HPP_

#include "base/string_util.hpp"
#include "base/fasta_reader.hpp"
#include "prsm/prsm.hpp"
#include "prsm/prsm_para.hpp"

namespace prot {

class PrsmCoverage {
 public:
  PrsmCoverage(PrsmParaPtr prsm_para_ptr, std::string input_file_ext,
                std::string output_file_ext);
  void processSingleCoverage();
  void processCombineCoverage();

 private:
  PrsmParaPtr prsm_para_ptr_;
  std::string input_file_ext_;
  std::string output_file_ext_;

  void printTitle(std::ofstream &file);
  void compCoverage(std::ofstream &file, PrsmPtr prsm, 
                    PeakIonPairPtrVec &pairs, PrsmParaPtr prsm_para_ptr);
  void processOnePrsm(std::ofstream &file, PrsmPtr prsm, 
                      PrsmParaPtr prsm_para_ptr);
  void processTwoPrsms(std::ofstream &file, PrsmPtr prsm_1, 
                       PrsmPtr prsm_2, PrsmParaPtr prsm_para_ptr);

};

} /* namespace prot */

#endif /* PROT_PRSM_COVERAGE_HPP_ */

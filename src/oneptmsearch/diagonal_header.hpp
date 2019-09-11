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


#ifndef TOPPIC_ONE_PTM_SEARCH_DIAGONAL_HEADER_HPP_
#define TOPPIC_ONE_PTM_SEARCH_DIAGONAL_HEADER_HPP_

#include <vector>

#include "seq/proteoform.hpp"
#include "spec/prm_peak.hpp"

namespace toppic {

class DiagonalHeader;

typedef std::shared_ptr<DiagonalHeader>     DiagonalHeaderPtr;
typedef std::vector<DiagonalHeaderPtr>      DiagonalHeaderPtrVec;
typedef std::vector<DiagonalHeaderPtrVec>   DiagonalHeaderPtrVec2D;
typedef std::vector<DiagonalHeaderPtrVec2D> DiagonalHeaderPtrVec3D;

class DiagonalHeader {
 public:
  DiagonalHeader(double n_term_shift,
                 bool n_strict, bool c_strict,
                 bool prot_n_match, bool prot_c_match,
                 bool pep_n_match, bool pep_c_match):
      prot_N_term_shift_(n_term_shift),
      n_strict_(n_strict),
      c_strict_(c_strict),
      prot_N_term_match_(prot_n_match),
      prot_C_term_match_(prot_c_match),
      pep_N_term_match_(pep_n_match),
      pep_C_term_match_(pep_c_match) {}

  DiagonalHeaderPtr clone();

  void initHeader(double c_shift, ProteoformPtr proteoform, double align_pref_suff_shift_thresh);

  void changeOnlyNTermShift(double s) {
    prot_N_term_shift_ += s;
    pep_N_term_shift_  += s;
  }

  void changeOnlyCTermShift(double s) {
    prot_C_term_shift_ += s;
    pep_C_term_shift_  += s;
  }

  int getTruncFirstResPos() {return trunc_first_res_pos_;}

  int getMatchFirstBpPos() {return match_first_bp_pos_;}

  int getMatchLastBpPos() {return match_last_bp_pos_;}

  double getPepCTermShift() {return pep_C_term_shift_;}

  double getPepNTermShift() { return pep_N_term_shift_;}

  double getProtCTermShift() {return prot_C_term_shift_;}

  double getProtNTermShift() {return prot_N_term_shift_;}

  int getTruncLastResPos() {return trunc_last_res_pos_;}

  void setPepCTermShift(double pepCTermShift) {
    pep_C_term_shift_ = pepCTermShift;
  }

  void setPepNTermShift(double pepNTermShift) {
    pep_N_term_shift_ = pepNTermShift;
  }

  void setProtCTermShift(double protCTermShift) {
    prot_C_term_shift_ = protCTermShift;
  }

  void setProtNTermShift(double protNTermShift) {
    prot_N_term_shift_ = protNTermShift;
  }

  void setTruncFirstResPos(int truncFirstResPos) {
    trunc_first_res_pos_ = truncFirstResPos;
  }

  void setTruncLastResPos(int truncLastResPos) {
    trunc_last_res_pos_ = truncLastResPos;
  }

  void setMatchFirstBpPos(int match_first_bp_pos) {
    match_first_bp_pos_ = match_first_bp_pos;
  }

  void setMatchLastBpPos(int match_last_bp_pos) {
    match_last_bp_pos_ = match_last_bp_pos;
  }

  bool isNStrict() {return n_strict_;}
  bool isCStrict() {return c_strict_;}

  void setAlignPrefix(bool is_prefix) {is_align_prefix_ = is_prefix;}
  void setAlignSuffix(bool is_suffix) {is_align_suffix_ = is_suffix;}
  bool isAlignPrefix() {return is_align_prefix_;}
  bool isAlignSuffix() {return is_align_suffix_;}

  int getId() { return id_; }
  void setId(int id) { id_ = id; }

  bool isProtNTermMatch() {return prot_N_term_match_;}
  bool isProtCTermMatch() {return prot_C_term_match_;}
  bool isPepNTermMatch() {return pep_N_term_match_;}
  bool isPepCTermMatch() {return pep_C_term_match_;}

  void setAlignPrefixSuffix(double error_tolerance);

 private:
  int id_ = 0;

  double prot_N_term_shift_ = 0;

  // if it is generated by n-terminal truncation shift
  bool n_strict_ = false;

  // if it is generated by c-teriminal truncation shift
  bool c_strict_ = false;

  // with respect to the whole protein
  int trunc_first_res_pos_ = 0;
  int match_first_bp_pos_ = 0;

  bool prot_N_term_match_ = false;
  bool prot_C_term_match_ = false;

  double pep_N_term_shift_ = 0.0;
  bool pep_N_term_match_ = false;

  // if protNTermShift is not large
  bool is_align_prefix_ = false;

  int trunc_last_res_pos_ = 0;
  int match_last_bp_pos_ = 0;
  double prot_C_term_shift_ = 0.0;

  double pep_C_term_shift_ = 0.0;
  bool pep_C_term_match_ = false;

  // if protCTermShift is not large
  bool is_align_suffix_ = false;
};

// generate (clone) a new diagonal header with new bgn and end
DiagonalHeaderPtr geneDiagonalHeaderPtr(int bgn, int end, DiagonalHeaderPtr diag_ptr);

MassShiftPtrVec getDiagonalMassChanges(const DiagonalHeaderPtrVec &diag_ptrs,
                                       int first_res_pos, int last_res_pos,
                                       MassShiftTypePtr type_ptr);

MassShiftPtrVec getDiagonalMassChanges(const DiagonalHeaderPtrVec &header_ptrs,
                                       int first_res_pos, int last_res_pos,
                                       const MassShiftTypePtrVec & type_ptrs);

} /* namespace toppic */

#endif /* DIAGONAL_HEADER_HPP_ */

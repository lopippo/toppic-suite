#ifndef ZERO_PTM_SLOW_MATCH_HPP_
#define ZERO_PTM_SLOW_MATCH_HPP_

#include "spec/deconv_peak.hpp"
#include "zeroptmsearch/zero_ptm_mng.hpp"
#include "zeroptmsearch/zero_ptm_fast_match.hpp"

namespace prot {

class ZeroPtmSlowMatch {
 public:
  ZeroPtmSlowMatch(DeconvMsPtr deconv_ms_ptr, ZpFastMatchPtr fast_match_ptr,
                   int search_type, ZeroPtmMngPtr mng_ptr);
	double getScore() {return score;}

 private:
	ZeroPtmMngPtr mng_ptr_;
  ZpFastMatchPtr fast_match_ptr_;
  DeconvMsPtr deconv_ms_ptr_;
  ProteoformPtr proteoform_ptr_;

	double refine_prec_mass_;
	ExtendMsPtr refine_ms_ptr_;

	double score = 0;
	double recal = 0;

  void compScore (ExtendMsPtr refine_ms_ptr, TheoPeakPtrVec theo_peaks, double ppo);
  bool isValid (double recal, double ppo);
};

typedef std::shared_ptr<ZeroPtmSlowMatch> ZpSlowMatchPtr;
typedef std::vector<ZpSlowMatchPtr> ZpSlowMatchPtrVec;

inline bool compareZeroPtmSlowMatchDown(ZpSlowMatchPtr m1, ZpSlowMatchPtr m2) {
  if  (m1->getScore() > m2->getScore()) {
    return 1;
  }
  else {
    return 0;
  }
}

}

#endif

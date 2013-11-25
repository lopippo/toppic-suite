/*
 * author  Xiaowen Liu
 * date    2013-11-01
 */

#ifndef PROT_RESIDUE_H_
#define PROT_RESIDUE_H_

#include <string>
#include <memory>

#include "acid.hpp"
#include "ptm.hpp"

namespace prot {

class Residue {
 public:
	Residue(AcidPtr acid_ptr, PtmPtr ptm_ptr); 

  Residue(AcidPtrVec acid_ptr_vec, PtmPtrVec ptm_ptr_vec,
          std::string acid_name, std::string abbr_name);

	/** Get amino acid. */
	AcidPtr getAcidPtr() {return acid_ptr_; }
	
	/** Get residue mass. */
	double getMass() { return mass_; }

	/** Get post-translational modification. */
	PtmPtr getPtmPtr() { return ptm_ptr_; }

	/**
	 * Checks if the residue contains the same amino acid and ptm.
	 */
	bool isSame(AcidPtr acid_ptr, PtmPtr ptm_ptr) {
		return acid_ptr_.get() == acid_ptr.get() && ptm_ptr_.get() == ptm_ptr.get();
	}
	
	/** Get string representation */
  std::string toString(std::string delim_bgn, std::string delim_end);

 private:
	/** amino acid */
	AcidPtr acid_ptr_;
	/** post-translational modification */
	PtmPtr ptm_ptr_;
	/** residue mass */
	double mass_;
};

typedef std::shared_ptr<Residue> ResiduePtr;
typedef std::vector<ResiduePtr> ResiduePtrVec;

/**
 * Returns the first residue based on the acid and ptm. 
 */
ResiduePtr getResiduePtrByAcidPtm(ResiduePtrVec &residue_ptr_vec,
                                  AcidPtr acid_ptr, PtmPtr ptm_ptr);

ResiduePtrVec getResiduePtrVecInstance(AcidPtrVec &acid_ptr_vec, 
                                       PtmPtrVec &ptm_ptr_vec,
                                       const char* file_name); 

ResiduePtr addResidue(ResiduePtrVec &residue_ptr_vec, AcidPtr acid_ptr,
                      PtmPtr ptm_ptr);
}
#endif

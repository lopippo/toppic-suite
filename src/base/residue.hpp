/*
 * author  Xiaowen Liu
 * date    2013-11-01
 */

#ifndef PROTOMICS_RESIDUE_H_
#define PROTOMICS_RESIDUE_H_

#include <string>
#include <memory>

#include "acid.hpp"
#include "ptm.hpp"

namespace proteomics {

class Residue {
 public:
	Residue(AcidPtr acid_ptr, PtmPtr ptm_ptr); 

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
		return acid_ptr_ == acid_ptr && ptm_ptr_ == ptm_ptr;
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

}
#endif

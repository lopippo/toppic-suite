/*
 * ps_align.hpp
 *
 *  Created on: Jan 8, 2014
 *      Author: xunlikun
 */

#ifndef PS_ALIGN_HPP_
#define PS_ALIGN_HPP_

#include "dp_pair.hpp"
#include "ptmsearch/basic_diag_pair.hpp"

namespace prot {

class PSAlign {
public:
	PSAlign();
	PSAlign(std::vector<double> sp_masses,std::vector<double> seq_masses,BasicDiagPairDiagPtrVec diagonals,PtmMngPtr mng);
	void compute(int align_type);
	void initDPPair();
	void dp(int align_type);
	void backtrace(int align_type);
	DiagonalHeaderPtrVec backtrace(int s,int type);

	std::vector<double> getAlignScr(){return align_scores_;};
	DiagonalHeaderPtrVec2D getResult(){return backtrack_diagonals_;};

protected:
	PtmMngPtr mng_;
	std::vector<double> sp_masses_;
	std::vector<double> seq_masses_;
	BasicDiagPairDiagPtrVec diagonals_;

	BasicDiagPairPtrVec diag_pairs_;
	std::vector<DPPairPtrVec> dp_2d_pairs_;
	DPPairPtr first_pair_;
	DPPairPtr last_pair_;
	DPPairPtrVec segment_bgn_pairs_;
	DPPairPtrVec segment_end_pairs_;
	DPPairPtrVec dp_pairs_;
	DiagonalHeaderPtrVec2D backtrack_diagonals_;
	std::vector<double> align_scores_;

	void dpPrep();
	DPPairPtr getTruncPre(DPPairPtr cur_pair,int s,int type);
	DPPairPtr getShiftPre(DPPairPtr cur_pair,int p,int s,int type);
};

typedef std::shared_ptr<PSAlign> PSAlignPtr;
typedef std::vector<PSAlignPtr> PSAlignPtrVec;
} /* namespace prot */

#endif /* PS_ALIGN_HPP_ */
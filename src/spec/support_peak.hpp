/*
 * support_peak.hpp
 *
 *  Created on: Dec 4, 2013
 *      Author: xunlikun
 */

#ifndef PROT_SUPPORT_PEAK_HPP_
#define PROT_SUPPORT_PEAK_HPP_

#include "spec/deconv_peak.hpp"
#include "spec/support_peak_type.hpp"

namespace prot {

class SupportPeak {
public:
	SupportPeak(DeconvPeakPtr peak,double offset,double score,SupportPeakTypePtr peak_type);
	SupportPeakTypePtr getPeakType(){return peak_type_;}
	double getOffset(){return offset_;}
	double getScore(){return score_;}
	DeconvPeakPtr getPeak(){return peak_;}

private:
	DeconvPeakPtr peak_;
	double offset_;
	double score_;
	SupportPeakTypePtr peak_type_;
};
typedef std::shared_ptr<SupportPeak> SupportPeakPtr;
typedef std::vector<SupportPeakPtr> SupportPeakPtrVec;

} /* namespace prot */

#endif /* SUPPORT_PEAK_HPP_ */
/*
 * simple_prsm.hpp
 *
 *  Created on: Dec 1, 2013
 *      Author: xunlikun
 */

#ifndef PROT_SIMPLE_PRSM_HPP_
#define PROT_SIMPLE_PRSM_HPP_

#include <string>
#include <xercesc/dom/DOM.hpp>

#include "proteoform.hpp"
#include "ms_header.hpp"

namespace prot {
class SimplePrSM;
typedef std::shared_ptr<SimplePrSM> SimplePrSMPtr;
typedef std::vector<SimplePrSMPtr> SimplePrSMPtrVec;
typedef std::vector<SimplePrSMPtrVec> SimplePrSMPtrVec2D;
class SimplePrSM {
public:
	SimplePrSM(MsHeaderPtr header,ProteoformPtr seq,int score);
	SimplePrSM(xercesc::DOMElement* element);
	std::string getSeqName(){return seq_name_;}
	ProteoformPtr getSeq(){return seq_;}
	int getSeqId(){return seq_id_;}
	double getScore(){return score_;}
	int getSpectrumId(){return spectrum_id_;}
	std::string getSpectrumScan(){return spectrum_scan_;}
	double getPrecMass(){return prec_mass_;}
	void setPrecursorId(int precursorId){precursor_id_ = precursorId;}
	int getPrecursorId(){return precursor_id_;}
	int compareTo(SimplePrSMPtr simple_prsm_ptr);
	void findSeq(std::vector<ProteoformPtr> seqs);
	//todo:should the element be shared_prt?
	xercesc::DOMElement* toXml();
	bool isMatch(MsHeaderPtr header);

private:
	int spectrum_id_;
	std::string spectrum_scan_;
	int precursor_id_;
	double prec_mass_;
	//todo:should the follow be residue?
	ProteoformPtr seq_;
	int seq_id_;
	std::string seq_name_;
	double score_;
};

SimplePrSMPtrVec findSimplePrsms(SimplePrSMPtrVec simple_prsm,MsHeaderPtr header);

} /* namespace prot */

#endif /* SIMPLE_PRSM_HPP_ */
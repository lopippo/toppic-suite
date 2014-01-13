/*
 * diagonal.cpp
 *
 *  Created on: Jan 1, 2014
 *      Author: xunlikun
 */

#include <ptmsearch/diagonal.hpp>
#include "base/logger.hpp"
#include "float.h"
#include "limits.h"

namespace prot {
DiagonalHeaderPtrVec refineHeadersBgnEnd(int first_pos,ProteoformPtr seq,DeconvMsPtr deconv_ms,ExtendMsPtr ms_three,PtmMngPtr mng,DiagonalHeaderPtrVec headers){
	DiagonalHeaderPtrVec result_list;
	for(int i=0;i<headers.size();i++){
		TheoPeakPtrVec ions = getTheoPeak(seq,deconv_ms->getHeaderPtr()->getActivationPtr(),headers,i,mng->sp_para_->getMinMass());
		int bgn = headers[i]->getMatchFirstResPos()-first_pos;
		int end = headers[i]->getMatchLastResPos()+1-first_pos;
		PeakIonPairPtrVec pairs = prot::findPairs(ms_three,ions,bgn,end);

		if(pairs.size()<1){
			int pair_size = pairs.size();
			LOG_WARN("Empty Segment is found "+prot::convertToString(pair_size));
		}
		else{
			int new_bgn = first_pos + getNewBgn(pairs);
			int new_end = first_pos + getNewEnd(pairs);
			headers[i]->setMatchFirstResPos(new_bgn);
			headers[i]->setMatchLastResPos(new_end);
			result_list.push_back(headers[i]);
		}
	}
	return result_list;
}

TheoPeakPtrVec getTheoPeak(ProteoformPtr seq,ActivationPtr type,DiagonalHeaderPtrVec headers,int i,double min_mass){
	DiagonalHeaderPtr first_header = headers[0];
	DiagonalHeaderPtr last_header = headers[headers.size()-1];
	int first_res_pos = first_header->getTruncFirstResPos();
	int last_res_pos = last_header->getTruncLastResPos();
	//todo:: may be improved proteoform's sub;
	ResSeqPtr subseq = seq->getResSeqPtr()->getSubResidueSeq(first_res_pos,last_res_pos);
//	ProteoformPtr pep = ProteoformPtr(new Proteoform(seq->getDbResSeqPtr(),seq->getProtModPtr(),subseq,first_res_pos,last_res_pos,seq->getChangePtrVec()));
	BpSpecPtr pep = BpSpecPtr(new BpSpec(subseq));
	double pep_n_term_shift = headers[i]->getProtNTermShift()-first_header->getProtNTermShift()+first_header->getPepNTermShift();
	double pep_c_term_shift = headers[i]->getProtCTermShift()-last_header->getProtCTermShift()+last_header->getPepCTermShift();
	return prot::getTheoPeak(pep,type,pep_n_term_shift,pep_c_term_shift,headers[i]->getMatchFirstResPos()-first_res_pos,headers[i]->getMatchLastResPos()-first_res_pos+1,min_mass);
}

int getNewBgn(PeakIonPairPtrVec pairs){
	int newBgn = INT_MAX;
	for(int i=0;i<pairs.size();i++)
	{
		if(pairs[i]->getTheoPeakPtr()->getIonPtr()->getPos() < newBgn){
			newBgn = pairs[i]->getTheoPeakPtr()->getIonPtr()->getPos();
		}
	}
	return newBgn;
}
int getNewEnd(PeakIonPairPtrVec pairs){
	int newEnd = 0;
	for(int i=0;i<pairs.size();i++)
	{
		if(pairs[i]->getTheoPeakPtr()->getIonPtr()->getPos() > newEnd){
			newEnd = pairs[i]->getTheoPeakPtr()->getIonPtr()->getPos();
		}
	}
	return newEnd;
}
} /* namespace prot */
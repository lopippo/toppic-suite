/*
 * table_writer.cpp
 *
 *  Created on: Feb 19, 2014
 *      Author: xunlikun
 */

#include <console/table_writer.hpp>

namespace prot {

TableWriter::TableWriter(std::string spec_file,std::string db_file,std::string input_file,std::string output_file,double ppo){
    spec_file_ = spec_file;
    input_file_ = input_file;
    output_file_ = output_file;
    db_file_=db_file;
    ppo_=ppo;
}

void TableWriter::write(){
  std::string output_file_name = basename(spec_file_) + "." + output_file_;
  file_.open(output_file_name.c_str());
  //write titile
  file_ << "Data_file_name" << "\t"
      << "Prsm_ID" << "\t"
      << "Spectrum_ID"<< "\t"
      << "Activation_type" << "\t"
      << "Scan(s)" << "\t"
      << "#peaks"<< "\t"
      << "Charge" << "\t"
      << "Precursor_mass" << "\t"
      << "Adjusted_precursor_mass" << "\t"
      << "Protein_ID" << "\t"
      << "Species_ID" << "\t"
      << "Protein_name" << "\t"
      << "Protein_mass" << "\t"
      << "First_residue" << "\t"
      << "Last_residue" << "\t"
      << "Peptide" << "\t"
      << "#unexpected_modifications" << "\t"
      << "#matched_peaks" << "\t"
      << "#matched_fragment_ions" << "\t"
      << "P-Value" << "\t"
      << "E-Value" << "\t"
      << "One_Protein_probabilty"<< "\t"
      << "FDR" << "\t"
      << std::endl;
  //write spectrum
  std::string input_file_name = basename(spec_file_)+"."+input_file_;
  ProteoformPtrVec proteoforms_ = prot::readFastaToProteoform(db_file_,ResidueFactory::getBaseResiduePtrVec());
  PrSMPtrVec prsms = readPrsm(input_file_name,proteoforms_);
  sort(prsms.begin(),prsms.end(),prsm_spectrum);
  setSpeciesId(prsms,ppo_);
  for(unsigned int i=0;i<prsms.size();i++){
    file_ << spec_file_ << "\t"
        << prsms[i]->getId() << "\t"
        << prsms[i]->getSpectrumId()<< "\t"
        << prsms[i]->getDeconvMsPtr()->getHeaderPtr()->getActivationPtr()<< "\t"
        << prsms[i]->getSpectrumScan() << "\t"
        << prsms[i]->getDeconvMsPtr()->size()<< "\t"
        << prsms[i]->getDeconvMsPtr()->getHeaderPtr()->getPrecCharge() << "\t"
        << prsms[i]->getOriPrecMass()<< "\t"//"Precursor_mass"
        << prsms[i]->getAdjustedPrecMass() << "\t"
        << prsms[i]->getProteoformPtr()->getDbResSeqPtr()->getId() << "\t"
        << prsms[i]->getProteoformPtr()->getSpeciesId() << "\t"
        << prsms[i]->getProteoformPtr()->getDbResSeqPtr()->getName() << "\t"
        << prsms[i]->getProteoformPtr()->getDbResSeqPtr()->getSeqMass() << "\t"
        << prsms[i]->getProteoformPtr()->getStartPos() << "\t"
        << prsms[i]->getProteoformPtr()->getEndPos() << "\t"
        << prsms[i]->getProteoformPtr()->getProteinMatchSeq() << "\t"
        << prsms[i]->getProteoformPtr()->getUnexpectedChangeNum() << "\t"
        << prsms[i]->getMatchPeakNum() << "\t"
        << prsms[i]->getMatchFragNum() << "\t"
        << prsms[i]->getPValue() << "\t"
        << prsms[i]->getEValue() << "\t"
        << prsms[i]->getProbPtr()->getOneProtProb()<< "\t"
        << prsms[i]->getFdr() << "\t"
        << std::endl;
//    std::cout<<prsms[i]->getSpectrumId()<<std::endl;
//    std::cout<<prsms[i]->getProteoformPtr()->getProteinMatchSeq()<<std::endl;
  }
  //write end;
  file_.close();
}

} /* namespace prot */
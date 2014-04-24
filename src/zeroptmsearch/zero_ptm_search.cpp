#include "base/logger.hpp"
#include "base/proteoform.hpp"
#include "base/fasta_reader.hpp"
#include "spec/msalign_reader.hpp"
#include "prsm/prsm_writer.hpp"
#include "zeroptmsearch/zero_ptm_fast_match.hpp"
#include "zeroptmsearch/zero_ptm_slow_match.hpp"
#include "zeroptmsearch/zero_ptm_search.hpp"

namespace prot {

void zeroPtmSearch(SpectrumSetPtr spec_set_ptr, 
                   SemiAlignTypePtr type,
                   ProteoformPtrVec &proteoform_ptr_vec, 
                   ZeroPtmMngPtr mng_ptr, 
                   PrSMPtrVec &prsms) {
  ExtendMsPtr ms_three = spec_set_ptr->getSpThree();

  ZpFastMatchPtrVec fast_matches 
      = zeroPtmFastFilter(type, ms_three, proteoform_ptr_vec, 
                          mng_ptr->zero_ptm_filter_result_num_);

  LOG_DEBUG("fast_match ended size " << fast_matches.size());
  DeconvMsPtr deconv_ms = spec_set_ptr->getDeconvMs();
  ZpSlowMatchPtrVec slow_matches 
      = zeroPtmSlowFilter(deconv_ms, fast_matches, mng_ptr); 

  LOG_DEBUG("slow_match ended size " << slow_matches.size());
  for (unsigned int i = 0; i < slow_matches.size(); i++) {
      prsms.push_back(slow_matches[i]->geneResult());
  }
  LOG_DEBUG("prsm generation ended size " << prsms.size());

  std::sort(prsms.begin(), prsms.end(), prsmMatchFragmentDown);
  if (prsms.size() > 0) {
    prsms.erase(prsms.begin() + 1, prsms.end());
  }
}

void zeroPtmSearchProcess(ZeroPtmMngPtr mng_ptr) {

  ProteoformPtrVec raw_forms 
      = readFastaToProteoform(mng_ptr->search_db_file_name_,
                              mng_ptr->fix_mod_residue_list_);

  ProteoformPtrVec prot_mod_forms 
      = generateProtModProteoform(raw_forms, mng_ptr->allow_prot_mod_list_);

  int spectra_num = countSpNum (mng_ptr->spectrum_file_name_);
  LOG_DEBUG("spectra_number  " << spectra_num);

  MsAlignReader reader(mng_ptr->spectrum_file_name_);
  LOG_DEBUG("file name " << mng_ptr->spectrum_file_name_ 
            << " output " << mng_ptr->output_file_ext_);
  std::string output_file_name = basename(mng_ptr->spectrum_file_name_) 
                                          + "." + mng_ptr->output_file_ext_;
  PrSMWriter comp_writer(output_file_name + "_" 
                         + SemiAlignTypeFactory::getCompletePtr()->getName());
  PrSMWriter pref_writer(output_file_name + "_"
                         + SemiAlignTypeFactory::getPrefixPtr()->getName());
  PrSMWriter suff_writer(output_file_name + "_"
                         + SemiAlignTypeFactory::getSuffixPtr()->getName());
  PrSMWriter internal_writer(output_file_name + "_"
      + SemiAlignTypeFactory::getInternalPtr()->getName());
  PrSMWriter all_writer(output_file_name);

  LOG_DEBUG("start reading");
  int n = 0;
  DeconvMsPtr ms_ptr = reader.getNextMs();
  LOG_DEBUG("init ms_ptr");
  double delta = 0;
  while (ms_ptr.get() != nullptr) {
    n++;
    SpectrumSetPtr spec_set_ptr 
        = getSpectrumSet(ms_ptr, delta, mng_ptr->sp_para_ptr_);
    if (spec_set_ptr.get() != nullptr) {
      PrSMPtrVec comp_prsms;
      zeroPtmSearch(spec_set_ptr, SemiAlignTypeFactory::getCompletePtr(), 
                    prot_mod_forms, mng_ptr, comp_prsms);
      comp_writer.writeVector(comp_prsms);
      all_writer.writeVector(comp_prsms);
      PrSMPtrVec pref_prsms;
      zeroPtmSearch(spec_set_ptr, SemiAlignTypeFactory::getPrefixPtr(), 
                    prot_mod_forms, mng_ptr, pref_prsms);
      pref_writer.writeVector(pref_prsms);
      all_writer.writeVector(pref_prsms);
      PrSMPtrVec suff_prsms;
      zeroPtmSearch(spec_set_ptr, SemiAlignTypeFactory::getSuffixPtr(), 
                    raw_forms, mng_ptr, suff_prsms);
      suff_writer.writeVector(suff_prsms);
      all_writer.writeVector(suff_prsms);
      PrSMPtrVec internal_prsms;
      zeroPtmSearch(spec_set_ptr, SemiAlignTypeFactory::getInternalPtr(), 
                    raw_forms, mng_ptr, internal_prsms);
      internal_writer.writeVector(internal_prsms);
      all_writer.writeVector(internal_prsms);
      std::cout << std::flush << "Zero ptm search complete " << n << " of " << spectra_num << "\r";
    }
    ms_ptr = reader.getNextMs();
    LOG_DEBUG("spectrum " << n);
  }

  reader.close();

  //because the prsm_writer ~PrSMWriter changed and the fileclosing is an independant function
  comp_writer.close();
  pref_writer.close();
  suff_writer.close();
  internal_writer.close();
  all_writer.close();

  std::cout << "Zero ptm search finished." << std::endl;
}

} // end namespace

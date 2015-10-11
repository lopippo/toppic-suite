#include "prsm/prsm_writer.hpp"
#include "graph/graph_util.hpp"
#include "graph/proteo_graph_reader.hpp"
#include "graph/spec_graph_reader.hpp"
#include "graph_align/graph_align.hpp"
#include "graph_align/graph_align_processor.hpp"

namespace prot {

GraphAlignProcessor::GraphAlignProcessor(
    GraphAlignMngPtr mng_ptr) {
  mng_ptr_ = mng_ptr;
}

void GraphAlignProcessor::process() {
  PrsmParaPtr prsm_para_ptr = mng_ptr_->prsm_para_ptr_;
  SpParaPtr sp_para_ptr = prsm_para_ptr->getSpParaPtr();
  std::string db_file_name = prsm_para_ptr->getSearchDbFileName();
  LOG_DEBUG("Search db file name " << db_file_name);
  std::string sp_file_name = prsm_para_ptr->getSpectrumFileName();
  std::string residue_mod_file_name = mng_ptr_->residue_mod_file_name_;
  LOG_DEBUG("start reading " << residue_mod_file_name);
  ResiduePtrVec residue_mod_ptr_vec 
      = ResidueFactory::getResiduePtrVecInstance(residue_mod_file_name);
  LOG_DEBUG("end reading " << residue_mod_file_name);

  ProteoGraphReader reader(db_file_name, 
                           prsm_para_ptr->getFixModResiduePtrVec(),
                           prsm_para_ptr->getAllowProtModPtrVec(),
                           residue_mod_ptr_vec,
                           mng_ptr_->convert_ratio_,
                           mng_ptr_->getIntMaxPtmSumMass());
  LOG_DEBUG("init reader complete");
  ProteoGraphPtrVec proteo_ptrs;
  ProteoGraphPtr proteo_ptr;
  //writeToDot("proteo.dot", proteo_ptr->getMassGraphPtr());
  int count = 0;
  while ((proteo_ptr = reader.getNextProteoGraphPtr()) != nullptr) {
    count++;
    proteo_ptrs.push_back(proteo_ptr);
  }
  LOG_DEBUG("Prot graph number " << count);
  std::string output_file_name = basename(sp_file_name)+"."+mng_ptr_->output_file_ext_;
  PrsmWriter prsm_writer(output_file_name);

  SpecGraphReader spec_reader(sp_file_name, 
                              prsm_para_ptr->getGroupSpecNum(),
                              mng_ptr_->convert_ratio_,
                              sp_para_ptr);
  LOG_DEBUG("init spec reader complete");

  SpecGraphPtr spec_ptr = spec_reader.getNextSpecGraphPtr();
  LOG_DEBUG("spec ptr reading complete");
  //writeToDot("spec.dot", spec_ptr->getMassGraphPtr());
  int sp_count = 0;
  while (spec_ptr != nullptr) {
    sp_count++;
    LOG_DEBUG("spectrum id " << spec_ptr->getSpectrumSetPtr()->getSpecId());
    if (spec_ptr->getSpectrumSetPtr()->isValid()) {
      for (size_t i = 0; i < proteo_ptrs.size(); i++) {
        GraphAlign graph_align(mng_ptr_, proteo_ptrs[i], spec_ptr);
        graph_align.process();
        for (int s = 0; s <= mng_ptr_->n_unknown_shift_; s++) {
          PrsmPtr prsm_ptr = graph_align.geneResult(s);
          if (prsm_ptr != nullptr) {
            prsm_writer.write(prsm_ptr);
          }
        }
      }
    }
    spec_ptr = spec_reader.getNextSpecGraphPtr();
  }
  prsm_writer.close();
}

}

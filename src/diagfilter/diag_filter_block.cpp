#include <iostream>
#include "base/proteoform.hpp"
#include "diagfilter/diag_filter_block.hpp"

namespace prot {

DiagFilterBlock::DiagFilterBlock(const ProteoformPtrVec &proteo_ptrs,
                                       DiagFilterMngPtr mng_ptr){
  mng_ptr_ = mng_ptr;
  proteo_ptrs_ = proteo_ptrs;
  LOG_DEBUG("start init blocks.")
  proteo_blocks_ = getProteoBlocks(proteo_ptrs, mng_ptr->db_block_size_);
  LOG_DEBUG("init blocks is done.")
}

void DiagFilterBlock::initBlock(int i) {
  filter_ptr_ = DiagFilterPtr(new DiagFilter(proteo_blocks_[i],mng_ptr_));
}

SimplePrsmPtrVec DiagFilterBlock::getBestMathBatch(
    SpectrumSetPtr spectrum_set_ptr){
  PrmMsPtr ms_ptr = spectrum_set_ptr->getMsTwoPtr();
  return filter_ptr_->getBestMatch(ms_ptr);
}

} /* namespace prot */
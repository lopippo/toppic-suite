#include "base/logger.hpp"
#include "base/proteoform_reader.hpp"

namespace prot {

ProteoformReader::ProteoformReader(const std::string &file_name) {
  reader_ptr_ = FastaReaderPtr(new FastaReader(file_name));
}

/**
 * Read FASTA file and return next protein as an ResSeq.
 * residue_list determine fixed PTMs
 **/
ProteoformPtr ProteoformReader::getNextProteoformPtr(
    const ResiduePtrVec &residue_list) {
  FastaSeqPtr seq_ptr = reader_ptr_->getNextSeq();
  if (seq_ptr.get() == nullptr) {
    return ProteoformPtr(nullptr);
  }
  LOG_TRACE("name " << seq_ptr->getName() << " seq " << seq_ptr->getSeq());
  AcidPtrVec acid_seq = AcidFactory::convertSeqToAcidSeq(seq_ptr->getSeq());
  ResiduePtrVec residue_ptrs = convertAcidToResidueSeq(residue_list, acid_seq);
  DbResSeqPtr db_residue_seq_ptr(
      new DbResidueSeq(residue_ptrs, seq_id_, seq_ptr->getName(), seq_ptr->getDesc())); 
  seq_id_++;
  return getDbProteoformPtr(db_residue_seq_ptr);
}

ProteoformPtrVec readFastaToProteoform(const std::string &file_name,
                                       const ResiduePtrVec &residue_list) {
  return readFastaToProteoform(file_name, residue_list, 0);
}

ProteoformPtrVec readFastaToProteoform(const std::string &file_name, 
                                       const ResiduePtrVec &residue_list,
                                       int seq_bgn_id) {
  LOG_DEBUG( "start open file " << file_name);
  ProteoformReader reader(file_name);
  reader.setSeqId (seq_bgn_id);
  LOG_DEBUG( "open file done " << file_name);

  ProteoformPtrVec list;
  ProteoformPtr ptr = reader.getNextProteoformPtr(residue_list);
  int count = 0;
  while (ptr.get() != nullptr) {
    list.push_back(ptr);
    ptr = reader.getNextProteoformPtr(residue_list);
    count++;
  }
  return list;
}

ProteoformPtr readFastaToProteoform(faidx_t *fai,
                                    int id,
                                    const std::string &seq_name, 
                                    const std::string &seq_desc,
                                    const ResiduePtrVec &residue_list) {
  std::string seq_str = readFastaByIndex(fai, id, seq_name);
  //LOG_DEBUG("Seq name " << seq_name << " Seq: " << seq_str);
  AcidPtrVec acid_seq = AcidFactory::convertSeqToAcidSeq(seq_str); 
  ResiduePtrVec residue_ptrs = convertAcidToResidueSeq(residue_list, acid_seq);
  DbResSeqPtr db_residue_seq_ptr(
      new DbResidueSeq(residue_ptrs, id, seq_name, seq_desc)); 
  return getDbProteoformPtr(db_residue_seq_ptr);
}


}


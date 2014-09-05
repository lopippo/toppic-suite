
#include "base/anno_residue.hpp"
#include "spec/peak.hpp"
#include "prsmview/anno_view.hpp"

namespace prot{

xercesc::DOMElement* AnnoView::geneFileList(XmlDOMDocument* xml_doc){
  xercesc::DOMElement* element = xml_doc->createElement("file_list");
  for(size_t i=0;i<file_list_.size();i++){
    xercesc::DOMElement* file = xml_doc->createElement("file");
    xml_doc->addElement(file, "xml", file_list_[i][0].c_str());
    xml_doc->addElement(file, "xsl", file_list_[i][1].c_str());
    xml_doc->addElement(file, "html", file_list_[i][2].c_str());
    element->appendChild(file);
  }
  return element;
}

std::vector<std::vector<std::string>> readViewXmlFiles(const std::string &file_name){
  std::vector<std::vector<std::string>> file_list;
  XmlDOMParser* parser = XmlDOMParserFactory::getXmlDOMParserInstance();
  if(parser){
    XmlDOMDocument* doc = new XmlDOMDocument(parser, file_name.c_str());
    if (doc) {
      xercesc::DOMElement* root = doc->getDocumentElement();
      int file_num = getChildCount(root, "file");
      for (int i = 0; i < file_num; i++) {
        xercesc::DOMElement* file_element = getChildElement(root, "file", i);
        std::vector<std::string> file_info;
        file_info.push_back(prot::getChildValue(file_element,"xml",0));
        file_info.push_back(prot::getChildValue(file_element,"xsl",0));
        file_info.push_back(prot::getChildValue(file_element,"html",0));
        file_list.push_back(file_info);
      }
    }
    delete doc;
  }
  return file_list;
}

xercesc::DOMElement* genePrsmView(XmlDOMDocument* xml_doc,PrsmPtr prsm_ptr, PrsmViewMngPtr mng_ptr){
  xercesc::DOMElement* element = xml_doc->createElement("prsm");
  std::string str = convertToString(prsm_ptr->getId());
  xml_doc->addElement(element, "prsm_id", str.c_str());
  //  str = convertToString(prsm->getSpectrumId());
  //  xml_doc->addElement(element, "spectrum_id", str.c_str());
  //  xml_doc->addElement(element, "spectrum_scan", prsm->getSpectrumScan().c_str());
  if(prsm_ptr->getProbPtr().get()!=nullptr){
    str=convertToString(prsm_ptr->getProbPtr()->getPValue(), mng_ptr->decimal_point_num);
    xml_doc->addElement(element, "p_value", str.c_str());
  }
  else{
    xml_doc->addElement(element, "p_value", "N/A");
  }
  if(prsm_ptr->getProbPtr().get()!=nullptr){
    str=convertToString(prsm_ptr->getProbPtr()->getEValue(), mng_ptr->decimal_point_num);
    xml_doc->addElement(element, "e_value", str.c_str());
  }
  else{
    xml_doc->addElement(element, "e_value", "N/A");
  }
  double fdr = prsm_ptr->getFdr();
  if (fdr >= 0) {
    str=convertToString(prsm_ptr->getFdr(), mng_ptr->decimal_point_num);
    xml_doc->addElement(element, "fdr", str.c_str());
  }
  else {
    xml_doc->addElement(element, "fdr", "N/A");
  }
  str=convertToString((int)prsm_ptr->getMatchFragNum());
  xml_doc->addElement(element, "matched_fragment_number", str.c_str());
  str=convertToString((int)prsm_ptr->getMatchPeakNum());
  xml_doc->addElement(element, "matched_peak_number", str.c_str());
  //  str=convertToString(prsm->getOriPrecMass());
  //  xml_doc->addElement(element, "precursor_mass", str.c_str());
  str=convertToString(prsm_ptr->getAdjustedPrecMass(), mng_ptr->precise_point_num);
  xml_doc->addElement(element, "adjusted_precursor_mass", str.c_str());
  str=convertToString(prsm_ptr->getCalibration(), mng_ptr->precise_point_num);
  xml_doc->addElement(element, "calibration", str.c_str());

  //get ion_pair
  PeakIonPairPtrVec pair_ptrs =  getPeakIonPairs (prsm_ptr->getProteoformPtr(), 
                                                  prsm_ptr->getRefineMs(),
                                              mng_ptr->min_mass);
  //peaks to view
  xercesc::DOMElement* ms_element = xml_doc->createElement("ms");
  prsm_ptr->getDeconvMsPtr()->getHeaderPtr()->appendXml(xml_doc,ms_element);//attention
  xercesc::DOMElement* peaks = xml_doc->createElement("peaks");
  ms_element->appendChild(peaks);
  for(size_t i=0;i<prsm_ptr->getDeconvMsPtr()->size();i++){
    xercesc::DOMElement* peak_element = xml_doc->createElement("peak");
    peaks->appendChild(peak_element);
    DeconvPeakPtr peak_ptr = prsm_ptr->getDeconvMsPtr()->getPeakPtr(i);
    str=convertToString(peak_ptr->getId());
    xml_doc->addElement(peak_element, "id", str.c_str());
    double mass = peak_ptr->getPosition();
    int charge = peak_ptr->getCharge();
    str=convertToString(mass, mng_ptr->precise_point_num);
    xml_doc->addElement(peak_element, "monoisotopic_mass", str.c_str());
    double mz = compMonoMz(mass, charge);
    str=convertToString(mz, mng_ptr->precise_point_num);
    xml_doc->addElement(peak_element, "monoisotopic_mz", str.c_str());
    str=convertToString(peak_ptr->getIntensity(), mng_ptr->decimal_point_num);
    xml_doc->addElement(peak_element, "intensity", str.c_str());
    str=convertToString(charge);
    xml_doc->addElement(peak_element, "charge", str.c_str());
    PeakIonPairPtrVec selected_pair_ptrs = getMatchedPairs(pair_ptrs,peak_ptr->getId());
    if(selected_pair_ptrs.size()>0){
      int match_ions_number = selected_pair_ptrs.size();
      str=convertToString(match_ions_number);
      xml_doc->addElement(peak_element, "matched_ions_num", str.c_str());
      xercesc::DOMElement* mi_element = xml_doc->createElement("matched_ions");
      peak_element->appendChild(mi_element);
      for(size_t j=0;j< selected_pair_ptrs.size();j++){
        selected_pair_ptrs[j]->appendTheoPeakToXml(xml_doc,mi_element);
      }
    }
  }
  element->appendChild(ms_element);

  //proteoform to view
  xercesc::DOMElement* prot_element = geneProteinView(xml_doc,
                                                      prsm_ptr->getProteoformPtr(),
                                                      prsm_ptr->getRefineMs(),
                                                      mng_ptr);
  element->appendChild(prot_element);

  return element;
  }


xercesc::DOMElement* geneProteinView(XmlDOMDocument* xml_doc,
                                     ProteoformPtr proteoform_ptr,
                                     ExtendMsPtr ms_three_ptr,
                                     PrsmViewMngPtr mng_ptr) {
  xercesc::DOMElement* prot_element = xml_doc->createElement("annotated_protein");
  std::string str=convertToString(proteoform_ptr->getSeqId());
  xml_doc->addElement(prot_element, "sequence_id", str.c_str());
  str=convertToString(proteoform_ptr->getSpeciesId());
  xml_doc->addElement(prot_element, "species_id", str.c_str());
  str=proteoform_ptr->getSeqName();
  xml_doc->addElement(prot_element, "sequence_name", str.c_str());
  double mass = proteoform_ptr->getMass();
  str=convertToString(mass, mng_ptr->decimal_point_num);
  xml_doc->addElement(prot_element, "protein_mass", str.c_str());
  str=convertToString(proteoform_ptr->getStartPos());
  xml_doc->addElement(prot_element, "first_residue_position", str.c_str());
  str=convertToString(proteoform_ptr->getEndPos());
  xml_doc->addElement(prot_element, "last_residue_position", str.c_str());
  str=convertToString(proteoform_ptr->getProtModPtr()->getPtmPtr()->isAcetylation());
  xml_doc->addElement(prot_element, "n_acetylation", str.c_str());
  int unexpected_shift_number = proteoform_ptr->getChangePtrVec().size()-proteoform_ptr->getUnexpectedChangeNum();
  str=convertToString(unexpected_shift_number);
  xml_doc->addElement(prot_element, "unexpected_shift_number", str.c_str());
  str=convertToString(proteoform_ptr->getDbResSeqPtr()->getLen());
  xml_doc->addElement(prot_element, "db_acid_number", str.c_str());

  ChangePtrVec change_ptrs = proteoform_ptr->getChangePtrVec(); 
  std::sort(change_ptrs.begin(),change_ptrs.end(),compareChangeTypeUpPosUp);
  /*
  xercesc::DOMElement* change_list = xml_doc->createElement("known_change_list");
  for(size_t i=0;i<change_ptrs.size().size();i++){
    int change_type = change_ptrs[i]->getChangeType();
    if(charge_ptrs[i]->getChangeType()!=UNEXPECTED_CHANGE){
      xercesc::DOMElement* shift_element = xml_doc->createElement("known_change");
      shift_list->appendChild(shift_element);
      str=convertToString(change_type);
      xml_doc->addElement(shift_element, "change_type", str.c_str());
      xml_doc->addElement(shift_element, "ptm_name", abb_name.c_str());
    }
  }
  */

  xercesc::DOMElement* anno_element = xml_doc->createElement("annotation");
  prot_element->appendChild(anno_element);
  CleavagePtrVec cleavage_ptrs = getProteoCleavage(proteoform_ptr,ms_three_ptr,mng_ptr->min_mass);

  int prot_len = proteoform_ptr->getDbResSeqPtr()->getLen();
  // obtain residue_ptrs 
  AnnoResiduePtrVec res_ptrs;
  for(int i=0;i< prot_len;i++){
    res_ptrs.push_back(AnnoResiduePtr(new AnnoResidue(proteoform_ptr->getDbResSeqPtr()->getResiduePtr(i), 2*i + 1)));
  }

  // add information for N-terminal truncation
  int start_pos = proteoform_ptr->getStartPos();
  for (int i =0; i < start_pos; i++) { 
    cleavage_ptrs[i]->setType(CLEAVAGE_TYPE_N_TRUNCATION);
    res_ptrs[i]->setType(ANNO_RESIDUE_TYPE_N_TRUNCATION);
  }

  if (start_pos > 0) {
    cleavage_ptrs[start_pos]->setType(CLEAVAGE_TYPE_SEQ_START);
  }

  // add information for C-terminal truncation
  int end_pos = proteoform_ptr->getEndPos();
  if (end_pos < prot_len - 1) {
    cleavage_ptrs[end_pos + 1]->setType(CLEAVAGE_TYPE_SEQ_END);
  }

  for (int i = end_pos + 1; i < prot_len; i++) {
    cleavage_ptrs[i+1]->setType(CLEAVAGE_TYPE_C_TRUNCATION);
    res_ptrs[i]->setType(ANNO_RESIDUE_TYPE_C_TRUNCATION);
  }

  int unexpected_shift_color = 0;
  for (size_t i = 0; i < change_ptrs.size(); i++) {
    // add information for known changes 
    int left_bp = change_ptrs[i]->getLeftBpPos() + start_pos;
    int right_bp = change_ptrs[i]->getRightBpPos() + start_pos;
    if (change_ptrs[i]->getChangeType() != UNEXPECTED_CHANGE) { 
      res_ptrs[left_bp]->setType(ANNO_RESIDUE_TYPE_KNOWN_CHANGE);
    }
    else {
      if (left_bp == right_bp) {
        cleavage_ptrs[left_bp]->setUnexpectedChange(true);
        cleavage_ptrs[left_bp]->setUnexpectedChangeColor(unexpected_shift_color);;
      }
      else {
        for (int j = left_bp; j < right_bp; j++) {
          res_ptrs[j]->setUnexpectedChange(true);
          res_ptrs[j]->setUnexpectedChangeColor(unexpected_shift_color);;
          cleavage_ptrs[j+1]->setUnexpectedChange(true);
          cleavage_ptrs[j+1]->setUnexpectedChangeColor(unexpected_shift_color);;
        }
        res_ptrs[right_bp]->setUnexpectedChange(true);
        res_ptrs[right_bp]->setUnexpectedChangeColor(unexpected_shift_color);;
      }
      unexpected_shift_color = (unexpected_shift_color) + 1 % 2;
    }
  }

  for (int i = 0; i < prot_len; i++) {
    cleavage_ptrs[i]->appendXml(xml_doc, anno_element);
    res_ptrs[i]->appendViewXml(xml_doc, anno_element);
  }
  cleavage_ptrs[prot_len]->appendXml(xml_doc, anno_element);
  return prot_element;
}



xercesc::DOMElement* speciesToXml(XmlDOMDocument* xml_doc, const PrsmPtrVec &prsm_ptrs, 
                                  PrsmViewMngPtr mng_ptr){
  xercesc::DOMElement* species_element = xml_doc->createElement("species");
  std::string str=convertToString(prsm_ptrs[0]->getProteoformPtr()->getSeqId());
  xml_doc->addElement(species_element, "sequence_id", str.c_str());
  str=prsm_ptrs[0]->getProteoformPtr()->getSeqName();
  xml_doc->addElement(species_element, "sequence_name", str.c_str());
  str=convertToString(prsm_ptrs[0]->getProteoformPtr()->getSpeciesId());
  xml_doc->addElement(species_element, "species_id", str.c_str());
  int count = prsm_ptrs.size();
  str=convertToString(count);
  xml_doc->addElement(species_element, "prsm_number", str.c_str());
  for(size_t i=0;i<prsm_ptrs.size();i++){
    species_element->appendChild(genePrsmView(xml_doc,prsm_ptrs[i], mng_ptr));
  }
  return species_element;
}

xercesc::DOMElement* proteinToXml(XmlDOMDocument* xml_doc,
                                  const PrsmPtrVec &prsm_ptrs,
                                  ProteoformPtr proteo_ptr,
                                  const std::vector<int> &species_ids,
                                  PrsmViewMngPtr mng_ptr){
  xercesc::DOMElement* prot_element = xml_doc->createElement("protein");
  std::string str=convertToString(proteo_ptr->getSeqId());
  xml_doc->addElement(prot_element, "sequence_id", str.c_str());
  str=proteo_ptr->getSeqName();
  xml_doc->addElement(prot_element, "sequence_name", str.c_str());
  int count = species_ids.size();
  str=convertToString(count);
  xml_doc->addElement(prot_element, "species_number", str.c_str());
  for(size_t i=0;i<species_ids.size();i++){
    PrsmPtrVec select_prsm_ptrs = selectSpeciesPrsms(prsm_ptrs,species_ids[i]);
    std::sort(select_prsm_ptrs.begin(),select_prsm_ptrs.end(),prsmEValueUp);
    prot_element->appendChild(speciesToXml(xml_doc,select_prsm_ptrs, mng_ptr));
  }
  return prot_element;
}

PrsmPtr getBestEValuePrsmPtr (ProteoformPtr proteo_ptr, const PrsmPtrVec &prsm_ptrs) {
  PrsmPtr best_ptr(nullptr);
  double best_evalue = std::numeric_limits<double>::max();
  int seq_id = proteo_ptr->getDbResSeqPtr()->getId();
  for (size_t i = 0; i < prsm_ptrs.size(); i++) {
    if (prsm_ptrs[i]->getProteoformPtr()->getDbResSeqPtr()->getId() == seq_id && 
        prsm_ptrs[i]->getEValue() < best_evalue) {
      best_evalue = prsm_ptrs[i]->getEValue();
      best_ptr = prsm_ptrs[i];
    }
  }
  return best_ptr;
}

inline bool evalueCompare(const std::pair<ProteoformPtr, double> &a, const std::pair<ProteoformPtr, double> &b) {
    return a.second < b.second;
}


xercesc::DOMElement* allProteinToXml(XmlDOMDocument* xml_doc,
                                  const PrsmPtrVec &prsm_ptrs,
                                  const ProteoformPtrVec &proteo_ptrs,
                                  PrsmViewMngPtr mng_ptr){
  xercesc::DOMElement* prot_elements = xml_doc->createElement("proteins");
  // sort 
  std::vector<std::pair<ProteoformPtr, double>> proteo_evalues;
  for(size_t i=0;i<proteo_ptrs.size();i++){
    PrsmPtr best_ptr = getBestEValuePrsmPtr (proteo_ptrs[i], prsm_ptrs);
    if (best_ptr != nullptr) {
      std::pair<ProteoformPtr, double> cur_proteo_evalue(proteo_ptrs[i], best_ptr->getEValue());
      proteo_evalues.push_back(cur_proteo_evalue);
    }
  }
  std::sort(proteo_evalues.begin(), proteo_evalues.end(), evalueCompare);
  /*
  for (size_t i = 0; i < proteo_evalues.size(); i++) {
    LOG_DEBUG("rank " << i << " evalue " << proteo_evalues[i].second);
  }
  */

  
  for(size_t i=0;i<proteo_evalues.size();i++){
    std::vector<int> species_ids = getSpeciesIds(prsm_ptrs,proteo_evalues[i].first->getDbResSeqPtr()->getId());
    if(species_ids.size()>0){
      prot_elements->appendChild(proteinToXml(xml_doc,prsm_ptrs,proteo_evalues[i].first,species_ids, mng_ptr));
    }
  }
  return prot_elements;
}

}
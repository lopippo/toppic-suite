#ifndef PROT_PROT_MOD_HPP_
#define PROT_PROT_MOD_HPP_

#include "base/ptm.hpp"
#include "base/trunc.hpp"
#include "base/residue.hpp"
#include "base/xml_dom.hpp"
#include "base/xml_dom_document.hpp"

namespace prot {

#define PROT_MOD_NONE "NONE"
#define PROT_MOD_ACETYLATION "ACETYLATION"

class ProtMod {
 public:
  ProtMod(std::string name, TruncPtr trunc_ptr, PtmPtr ptm_ptr,
          AcidPtrVec valid_acid_ptrs);

  std::string getName() {return name_;};

  TruncPtr getTruncPtr() {return trunc_ptr_;}

  PtmPtr getPtmPtr() {return ptm_ptr_;}

  AcidPtrVec getAcidPtrVec(){return valid_acid_ptrs_;}

  double getProtShift() {return prot_shift_;}

  double getPepShift() {return pep_shift_;}

  void appendxml(XmlDOMDocument* xml_doc,xercesc::DOMElement* parent);

 private:
  std::string name_;
  TruncPtr trunc_ptr_;
  PtmPtr ptm_ptr_;
  AcidPtrVec valid_acid_ptrs_;
  double prot_shift_;
  double pep_shift_;
};

typedef std::shared_ptr<ProtMod> ProtModPtr;
typedef std::vector<ProtModPtr> ProtModPtrVec;


double getProtModAcetylationShift(ProtModPtrVec &prot_mod_list);

bool allowMod(ProtModPtr prot_mod_ptr, ResiduePtrVec residues);

/* prot mod factory */
class ProtModFactory {
 public:
  static void initFactory(const std::string &file_name);

  static ProtModPtrVec& getBaseProtModPtrVec() {return prot_mod_ptr_vec_;}
  
  static ProtModPtr getBaseProtModPtrByName (const std::string &name);

  static ProtModPtr getProtModPtr_NONE () {
    return getBaseProtModPtrByName(PROT_MOD_NONE);
  }

  static ProtModPtr getProtModPtr_ACETYLATION () {
    return getBaseProtModPtrByName(PROT_MOD_ACETYLATION);
  }
  
 private:
  static ProtModPtrVec prot_mod_ptr_vec_;
};


}
#endif

/*
 * prsm_writer.hpp
 *
 *  Created on: Dec 30, 2013
 *      Author: xunlikun
 */

#ifndef PRSM_WRITER_HPP_
#define PRSM_WRITER_HPP_

#include <iostream>
#include <fstream>

#include "base/xml_dom_document.hpp"
#include "prsm/prsm.hpp"

namespace prot {

class PrSMWriter {
public:
 PrSMWriter(std::string file_name);
 ~PrSMWriter();
 void close();
 void write(PrSMPtr prsm_ptr);
 void writeVector(PrSMPtrVec &prsms);
 void writeVector2D(PrSMPtrVec2D &prsms);
 void writeVector3D(PrSMPtrVec3D &prsms);
private:
  xercesc::DOMLSSerializer* serializer_;
  XmlDOMDocument* doc_;
  std::ofstream file_;
};

typedef std::shared_ptr<PrSMWriter> PrSMWriterPtr;
typedef std::vector<PrSMWriterPtr> PrSMWriterPtrVec;

} /* namespace prot */

#endif /* PRSM_WRITER_HPP_ */

/*
 * transformer.cpp
 *
 *  Created on: Feb 17, 2014
 *      Author: xunlikun
 */

#include <xpp/transformer.hpp>
#include <xpp/anno_view.hpp>

Transformer::Transformer() {
  // TODO Auto-generated constructor stub

}

Transformer::~Transformer() {
  // TODO Auto-generated destructor stub
}


void Transformer::trans(){
  xercesc::XMLPlatformUtils::Initialize();
  xalanc::XalanTransformer::initialize();
  xalanc::XalanTransformer theXanlanTransformer;

  std::vector<std::vector<std::string>> anno_view = prot::readFiles("xml/files.xml");
  for(unsigned int i=0;i<anno_view.size();i++){

    const char* xml_in = anno_view[i][0].c_str();
    const char* xsl_in = anno_view[i][1].c_str();
    const char* xml_out = anno_view[i][2].c_str();

    theXanlanTransformer.transform(xml_in,xsl_in,xml_out);
  }

  xalanc::XalanTransformer::terminate();
  xercesc::XMLPlatformUtils::Terminate();
  xalanc::XalanTransformer::ICUCleanUp();

}

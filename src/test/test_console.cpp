#include <iostream>

#include "base/base_data.hpp"

#include "prsm/prsm_combine.hpp"
#include "prsm/prsm_selector.hpp"
#include "prsm/output_selector.hpp"
#include "prsm/table_writer.hpp"

#include "zeroptmsearch/zero_ptm_mng.hpp"
#include "zeroptmsearch/zero_ptm_search.hpp"

#include "filterdiagonal/ptm_fast_filter_mng.hpp"
#include "filterdiagonal/ptm_fast_filter_processor.hpp"

#include "ptmsearch/ptm_mng.hpp"
#include "ptmsearch/ptm_processor.hpp"

#include "tdgf/evalue_processor.hpp"
#include "tdgf/tdgf_mng.hpp"

#include "xpp/xml_generator.hpp"


int main(int argc, char* argv[]) {
  try {
    std::cout << "Test msalign+ 0.1 " << std::endl;

    std::cout << "Zero ptm search 0.1 " << std::endl;
    prot::ZeroPtmMngPtr zero_mng_ptr
        = prot::ZeroPtmMngPtr(new prot::ZeroPtmMng (argv[1], argv[2], argv[3], "ZERO"));
    prot::zeroPtmSearchProcess(zero_mng_ptr);

    std::cout << "Fast filter 0.1 " << std::endl;
    prot::PtmFastFilterMngPtr filter_mng_ptr 
        = prot::PtmFastFilterMngPtr(new prot::PtmFastFilterMng(argv[1], argv[2], argv[3], "FILTER"));
    prot::PtmFastFilterProcessor filter_processor(filter_mng_ptr);
    filter_processor.process();


    std::cout << "Ptm alignment 0.1 " << std::endl;
		prot::PtmMngPtr ptm_mng_ptr 
        = prot::PtmMngPtr(new prot::PtmMng(argv[1], argv[2], argv[3], "FILTER_COMBINED", "PTM"));
    prot::PtmProcessor ptm_processor(ptm_mng_ptr);
		ptm_processor.process();

    std::cout << "Combine prsms " << std::endl;
		std::vector<std::string> input_exts ;
		input_exts.push_back("ZERO");
		input_exts.push_back("PTM");
    prot::PrSMCombine combine_processor(argv[2], argv[3], input_exts, "RAW_RESULT");
		combine_processor.process();

    std::cout << "EValueConsole 0.1 " << std::endl;
    prot::TdgfMngPtr tdgf_mng_ptr = 
        prot::TdgfMngPtr(new prot::TdgfMng (argv[1], argv[2], argv[3], "RAW_RESULT", "EVALUE"));
    prot::EValueProcessor processor(tdgf_mng_ptr);
    processor.init();
    // compute E-value for a set of prsms each run 
    processor.process(false);


    std::cout << "Top selector 0.1 " << std::endl;
    prot::PrSMSelector selector(argv[2], argv[3], "EVALUE", "TOP", 1, "e");
		selector.process();

    std::cout << "Cutoff selector 0.1 " << std::endl;
    prot::OutputSelector output_selector(argv[2], argv[3], "TOP", "OUTPUT_RESULT", 
                                   "EVALUE", 0.01, 0.01, zero_mng_ptr->ppo_);
		output_selector.process();

    std::cout << "Table output 0.1" << std::endl;
    prot::TableWriter table_out(argv[2], argv[3], "OUTPUT_RESULT",
                          "OUTPUT_TABLE", zero_mng_ptr->ppo_);
		table_out.write();

    /*
    prot::XmlGenerator xml_gene = prot::XmlGenerator(argv[2],argv[3],"OUTPUT_RESULT");
		xml_gene.process();
    */

    /*
		TransformerPtr trans = TransformerPtr(new Transformer());
		trans->trans();
    */

  } catch (const char* e) {
    std::cout << "Exception " << e << std::endl;
  }
  return 0;
}

#include "spec/ms.hpp"
#include "prsm/prsm.hpp"
#include "prsm/peak_ion_pair.hpp"

namespace prot {

PrSM::PrSM(ProteoformPtr proteoform_ptr, DeconvMsPtr deconv_ms_ptr, 
           double adjusted_prec_mass, double calibration, SpParaPtr sp_para_ptr) {
  proteoform_ptr_ = proteoform_ptr;
  deconv_ms_ptr_ = deconv_ms_ptr;
  MsHeaderPtr header_ptr = deconv_ms_ptr->getHeaderPtr();
  spectrum_id_ = header_ptr->getId();
  spectrum_scan_ = header_ptr->getScansString();
  precursor_id_ = header_ptr->getPrecId();
  ori_prec_mass_ = header_ptr->getPrecMonoMass();
  adjusted_prec_mass_ = adjusted_prec_mass;
  calibration_ = calibration;
  init(sp_para_ptr);
}

void PrSM::init(SpParaPtr sp_para_ptr) {
  double delta = adjusted_prec_mass_ = ori_prec_mass_;
  refine_ms_three_ = getMsThree(deconv_ms_ptr_, delta, sp_para_ptr);
  refine_ms_three_->recalibrate(calibration_);
  initScores(sp_para_ptr);
}

void PrSM::initScores(SpParaPtr sp_para_ptr) {
  // refined one 
  PeakIonPairPtrVec pairs;
  getPeakIonPairs (proteoform_ptr_, refine_ms_three_, 
                   sp_para_ptr->getMinMass(), pairs);
  match_peak_num_ = 0;
  match_fragment_num_ = 0;
  TheoPeakPtr prev_ion(nullptr);;
  for (unsigned int i = 0; i < pairs.size(); i++) {
    match_peak_num_ += pairs[i]->getRealPeakPtr()->getScore();
    if (pairs[i]->getTheoPeakPtr() != prev_ion) {
      prev_ion = pairs[i]->getTheoPeakPtr();
      match_fragment_num_ += pairs[i]->getRealPeakPtr()->getScore();
    }
  }
}

}

/*
	public PrSM(Element element) throws Exception {
		prsmId = Integer.parseInt(element.getChildText("prsm_id"));
		spectrumId = Integer.parseInt(element.getChildText("spectrum_id"));
		spectrumScan = element.getChildText("spectrum_scan");
		precursorId = Integer.parseInt(element.getChildText("precursor_id"));
		OriPrecMass = Double.parseDouble(element
				.getChildText("original_precursor_mass"));
		adjustedPrecMass = Double.parseDouble(element
				.getChildText("adjusted_precursor_mass"));
		calibration = Double.parseDouble(element.getChildText("calibration"));
		Element probElement = element.getChild("probability");
		prob = new ExtremeValueProb(probElement);
		fdr = Double.parseDouble(element.getChildText("fdr"));
		Element protElement = element.getChild("annotated_protein");
		annoProtein = new AnnoProtein(protElement);
		Element spParaElement = element.getChild("sp_para");
		spPara = new SpPara(spParaElement);
		
	}

	public void process(Ms<DeconvPeak> ms, BpSpec seqs[])
			throws Exception {
		annoProtein.process(seqs);
		this.deconvMs = ms;
		if (!ms.getHeader().getScansString().equals(spectrumScan)
				|| ms.getHeader().getPrecMonoMass() != OriPrecMass) {
			logger.error("Incorrect spectrum.");
			System.exit(1);
		}
		init();
	}

	public PeakIonPair[] getMatchedPairs() throws Exception {
		return annoProtein.getMatchPeak(refineMsThree, spPara.getMinMass());
	}


	public void outputMatchFragmentIon(PrintWriter writer) throws Exception {
		PeakIonPair pairs[] = annoProtein.getMatchPeak(refineMsThree,
				spPara.getMinMass());
		TheoPeak prevIon = null;
		int bp[] = new int[annoProtein.getSeq().getResSeq().getLen() - 1];
		for (int j = 0; j < pairs.length; j++) {
			nMatchPeak += pairs[j].getRealPeak().getScore();
			if (pairs[j].getTheoPeak() != prevIon) {
				prevIon = pairs[j].getTheoPeak();
				int pos = prevIon.getIon().getPos();
				if (prevIon.getIon().getIonType().isNTerm()) {
					if (bp[pos] == 0) {
						bp[pos] = 1;
					} else if (bp[pos] == 2) {
						bp[pos] = 3;
					}
				} else {
					if (bp[pos] == 0) {
						bp[pos] = 2;
					} else if (bp[pos] == 1) {
						bp[pos] = 3;
					}
				}
			}

		}
		for (int i = 0; i < bp.length; i++) {
			writer.print(bp[i]);
		}
	}

*/
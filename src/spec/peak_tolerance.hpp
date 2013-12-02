#ifndef PROT_PEAK_TOLERANCE_HPP_
#define PROT_PEAK_TOLERANCE_HPP_

#include "xml_dom_document.hpp"

namespace prot {

class PeakTolerance {
 public:
	PeakTolerance(double ppo, bool use_min_tolerance,
			double min_tolerance);

	double compStrictErrorTole(double mass);

	// consider zero ptm relaxed error
	double compRelaxErrorTole(double m1, double m2) {
		return compStrictErrorTole(m1 + m2);
	}

	double getPpo() {return ppo_;}

	bool isUseMinTolerance() {return use_min_tolerance_;}

	double getMinTolerance() {return min_tolerance_;}

	void setPpo(double ppo) {ppo_ = ppo;}
	
	void setUseMinTolerance(bool use_min_tolerance) {
		use_min_tolerance_ = use_min_tolerance;
	}
	
	void setMinTolerance(double min_tolerance) {
		min_tolerance_ = min_tolerance;
	}

  xercesc::DOMElement* toXml(XmlDOMDocument* xml_doc);

 private:
  double ppo_;
    /* whether or not use minimum tolerance */
	bool use_min_tolerance_;
	double min_tolerance_;
};

} /* namespace prot */

#endif	
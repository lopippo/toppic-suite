#include <sstream>

#include "base/logger.hpp"
#include "base/string_util.hpp"
#include "spec/msalign_reader.hpp"

namespace prot {

MsAlignReader::MsAlignReader (const char *spectrum_file) {
  file_name_ = std::string(spectrum_file);
  input_.open(spectrum_file, std::ios::in);
}

std::vector<std::string> MsAlignReader::readOneSpectrum() {
  std::string line;
  std::vector<std::string> line_list;
  while (std::getline(input_, line)) {
    line = trim(line);
    if (line ==  "BEGIN IONS") {
      line_list.push_back(line);
    }
    else if (line == "END IONS") {
      if (line_list.size() != 0) {
        line_list.push_back(line);
      }
      return line_list;
    }
    else if (line == "") {
      continue;
    }
    else {
      if (line_list.size() > 0) {
        line_list.push_back(line);
      }
    }
  }
  return line_list;
}

void MsAlignReader::readNext() {
  deconv_ms_ptr_ = DeconvMsPtr(nullptr);
  spectrum_str_ = readOneSpectrum();
  if (spectrum_str_.size() == 0) {
    input_.close();
    return;
  }
  int id = -1;
  int prec_id = 0;
  std::string scans;
  std::string activation;
  std::string title;
  int level = 2;
  double prec_mass = -1;
  int prec_charge = -1;
  std::vector<std::string> strs;
  for (unsigned int i = 1; i < spectrum_str_.size() - 1; i++) {
    std::string letter = spectrum_str_[i].substr(0,1);
    if (letter >= "A" && letter <= "Z") {
      strs = split(spectrum_str_[i], '=');
      if (strs[0] == "ID") {
        id = atoi(strs[1].c_str());
      }
      if (strs[0] == "PRECURSOR_ID") {
        prec_id = atoi(strs[1].c_str());
      }
      else if (strs[0] == "SCANS") {
        scans = strs[1];
      }
      else if (strs[0] == "ACTIVATION") {
        activation = strs[1];
      }
      else if (strs[0] == "TITLE") {
        title = strs[1];
      }
      else if (strs[0] == "LEVEL") {
        title = atoi(strs[1].c_str());
      }
      else if (strs[0] == "PRECURSOR_MASS") {
        prec_mass = atof(strs[1].c_str());
      }
      else if (strs[0] == "PRECURSOR_CHARGE") {
        prec_charge = atoi(strs[1].c_str());
      }
    }
  }
  if (id < 0 || prec_charge < 0 || prec_mass < 0) {
    LOG_ERROR("Input file format error: sp id " << id << " prec_chrg "
              << prec_charge << " prec mass " << prec_mass);
    std::exit(1);
  }

  MsHeaderPtr header_ptr(new MsHeader());
  header_ptr->setFileName(file_name_);
  header_ptr->setId(id);
  header_ptr->setPrecId(prec_id);
  if (scans != "") {
    header_ptr->setScans(scans);
  }
  else {
    header_ptr->setScans("");
  }
  if (title != "") {
    std::stringstream ss;
    ss << "sp_" << id;
    header_ptr->setTitle(ss.str());
  } else {
    header_ptr->setTitle(title);
  }
  if (activation != "") {
    ActivationPtr activation_ptr = 
        ActivationFactory::getBaseActivationPtrByName(activation);
    header_ptr->setActivationPtr(activation_ptr);
  }
  header_ptr->setMsLevel(level);

  header_ptr->setPrecMonoMz(prec_mass /prec_charge
                       + MassConstant::getProtonMass());
  header_ptr->setPrecCharge(prec_charge);

  std::vector<DeconvPeakPtr> peak_ptr_list;
  int idx = 0;
  for (unsigned int i = 1; i < spectrum_str_.size() - 1; i++) {
    std::string letter = spectrum_str_[i].substr(0,1);
    if (letter >= "0" && letter <= "9") {
      strs = split(spectrum_str_[i], '\t');
      double mass = atof(strs[0].c_str());
      double inte = atof(strs[1].c_str());
      int charge = atoi(strs[2].c_str());
      DeconvPeakPtr peak_ptr(new DeconvPeak(idx, mass, inte, charge));
      peak_ptr_list.push_back(peak_ptr);
      idx++;
    }
  }
  deconv_ms_ptr_ = DeconvMsPtr(new Ms<DeconvPeakPtr>(header_ptr, peak_ptr_list));

  current_++;
}

DeconvMsPtr MsAlignReader::getNextMs() {
  readNext();
  return deconv_ms_ptr_;
}
    
void MsAlignReader::close() {
  input_.close();
}

int countSpNum(const char *spectrum_file) {
  MsAlignReader reader(spectrum_file);
  int cnt = 0;
  DeconvMsPtr deconv_ms_ptr;
  while ((deconv_ms_ptr = reader.getNextMs()).get() != nullptr) {
    cnt++;
  }
  reader.close();
  return cnt;
}

}

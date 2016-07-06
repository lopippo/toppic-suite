#include "base/logger.hpp"
#include "feature/match_env.hpp" 
#include "feature/prec_env.hpp" 

namespace prot {

struct PeakIntv {
  int bgn;
  int end;
};

FeatureMngPtr initMngPtr(double prec_win_size) {
  FeatureMngPtr mng_ptr (new FeatureMng());
  mng_ptr->min_refer_inte_ = 0;
  mng_ptr->min_inte_ = 0;
  mng_ptr->max_miss_peak_num_ = 3;
  std::vector<int> num = {1,1,1};
  mng_ptr->min_match_peak_num_ = num;
  mng_ptr->min_consecutive_peak_num_ = num;
  mng_ptr->prec_deconv_interval_ = prec_win_size;
  return mng_ptr;
}


PeakIntv initPeakIntv(FeatureMngPtr mng_ptr, PeakPtrVec &peak_list, 
                      double prec_mz) {
  double base_mz  = prec_mz;
  double bgn_mz = base_mz - mng_ptr->prec_deconv_interval_;
  double end_mz = base_mz + mng_ptr->prec_deconv_interval_;
  PeakIntv peak_intv;
  peak_intv.bgn = peak_list.size();
  peak_intv.end = -1;
  for (size_t i = 0; i < peak_list.size(); i++) {
    if (peak_list[i]->getPosition() >= bgn_mz && 
        peak_list[i]->getPosition() <= end_mz) {
      if ((int)i < peak_intv.bgn) {
        peak_intv.bgn = i;
      }
      if ((int)i > peak_intv.end) {
        peak_intv.end = i;
      }
    }
  }
  return peak_intv;
}

int initPeakNum(PeakIntv peak_intv) {
  int peak_num;
  if (peak_intv.end < peak_intv.bgn) {
    peak_num = 0;
  } else {
    peak_num = peak_intv.end - peak_intv.bgn + 1;
  }
  return peak_num;
}

int initMaxChrg(PeakPtrVec &peak_list, PeakIntv peak_intv) {
  double min_dist = 1;
  for (int i = peak_intv.bgn - 1; i <= peak_intv.end; i++) {
    if (i < 0) {
      continue;
    }
    double cur_mz = peak_list[i]->getPosition();
    if (i + 1 >= (int)peak_list.size()) {
      continue;
    }
    double next_mz = peak_list[i+1]->getPosition();
    double dist = next_mz - cur_mz;
    if (dist < min_dist) {
      min_dist = dist; 
    }
  }
  int max_charge = (int) std::round(1.0 / min_dist);
  LOG_DEBUG("maximum charge: " << max_charge);
  return max_charge;
}

MatchEnvPtr2D initMatchEnv(FeatureMngPtr mng_ptr, PeakPtrVec &peak_list, 
                           PeakIntv peak_intv, int peak_num, int max_charge) {
  MatchEnvPtr2D result;
  for (int idx = peak_intv.bgn; idx <= peak_intv.end; idx++) {
    MatchEnvPtrVec env_ptrs;
    for (int charge = 1; charge <= max_charge; charge++) {
      double mass = peak_list[idx]->getPosition() * charge + 1;
      if (mass > mng_ptr->max_mass_) {
        mass = mng_ptr->max_mass_;
      }
      MatchEnvPtr env_ptr;
      /*
      MatchEnvPtr env_ptr  = EnvDetect.detectEnv(sp, idx, charge, mass, mng);
      if (env_ptr != nullptr) {
        if (!EnvFilter.testRealEnvValid(env, mng)) {
          env = nullptr;
        } else {
          env.compScr(mng_ptr);
        }
      }
      */
      env_ptrs.push_back(env_ptr);
    }
    result.push_back(env_ptrs);
  }
  return result;
}

MatchEnvPtr findBest(MatchEnvPtr2D &env_ptrs) {
  MatchEnvPtr best_env = nullptr;
  double best_score = -1;
  for (size_t i = 0; i < env_ptrs.size(); i++) {
    for (size_t j = 0; j < env_ptrs[i].size(); j++)  {
      if (env_ptrs[i][j] != nullptr && env_ptrs[i][j]->getScore() > best_score) {
        best_score = env_ptrs[i][j]->getScore();
        best_env = env_ptrs[i][j];
      }
    }
  }
  return best_env;
}

RealEnvPtr PrecEnv::deconv(double prec_win_size, PeakPtrVec &peak_list, 
                         double prec_mz, double prec_charge) {
  LOG_DEBUG("Prec: " << prec_mz << " charge: " << prec_charge);
  if (prec_mz <= 0) {
    return nullptr;
  }
  FeatureMngPtr mng_ptr = initMngPtr(prec_win_size);
  PeakIntv peak_intv = initPeakIntv(mng_ptr, peak_list, prec_mz);
  int peak_num = initPeakNum(peak_intv);
  if (peak_num  == 0) {
    return nullptr;
  }
  int max_charge = initMaxChrg(peak_list, peak_intv);
  LOG_DEBUG("Calcate match envelopes...");
  MatchEnvPtr2D match_envs = initMatchEnv(mng_ptr, peak_list, peak_intv, 
                                          peak_num, max_charge);
  LOG_DEBUG("Do filtering...");
  MatchEnvPtr env_ptr = findBest(match_envs);
  if (env_ptr != nullptr) {
    return env_ptr->getRealEnvPtr(); 
  }
  else {
    return nullptr;
  }
}

}
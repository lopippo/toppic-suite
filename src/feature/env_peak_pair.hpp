// Copyright (c) 2014 - 2016, The Trustees of Indiana University.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimer in the documentation and/or
// other materials provided with the distribution.
//
// Indiana University provides no reassurances that the source code provided does
// not infringe the patent or any other intellectual property rights of any other
// entity. Indiana University disclaims any liability to any recipient for claims
// brought by any other entity based on infringement of intellectual property
// rights or otherwise.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
// ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
// ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


#ifndef PROT_FEATURE_ENV_PEAK_PAIR_HPP_
#define PROT_FEATURE_ENV_PEAK_PAIR_HPP_

#include <memory>
#include <vector>

#include "feature/match_env.hpp"

namespace prot {

class EnvPeakPair;
typedef std::shared_ptr<EnvPeakPair> EnvPeakPairPtr;

class EnvPeakPair {
 public:
  EnvPeakPair(MatchEnvPtr env_ptr, int pos_idx);

  EnvPeakPair(EnvPeakPairPtr pair_ptr);

double getTheoIntensity();

double getPeakScore(double intensity_sum, double tolerance);

  MatchEnvPtr getMatchEnvPtr() {return env_ptr_;}
  int getPosIdx() { return pos_idx_;}

 private:
  MatchEnvPtr env_ptr_;
  int pos_idx_;
};

typedef std::vector<EnvPeakPairPtr> EnvPeakPairPtrVec;
typedef std::vector<EnvPeakPairPtrVec> EnvPeakPairPtr2D;

}
#endif
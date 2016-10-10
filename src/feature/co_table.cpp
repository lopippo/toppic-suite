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


#include "feature/co_table.hpp"

namespace prot {

int CoTable::cntTableSize(MatchEnvPtr2D &win_envs, int win, int pnt) {
  int size = 0;
  if (win - 2 >= 0) {
    size += win_envs[win - 2].size();
  }
  if (win - 1 >= 0) {
    size += win_envs[win - 1].size();
  }
  size += pnt;
  return size;
}

bool CoTable::checkCoexist(MatchEnvPtr env_a, MatchEnvPtr env_b, int pos_a,
                           int pos_b, double tolerance) {
  double inte_a = env_a->getTheoEnvPtr()->getIntensity(pos_a);
  double inte_b = env_b->getTheoEnvPtr()->getIntensity(pos_b);
  double inte_sum = inte_a + inte_b;
  double score_a = env_a->calcPeakScr(pos_a, inte_a, tolerance);
  double new_score_a = env_a->calcPeakScr(pos_a, inte_sum, tolerance);
  if (new_score_a < score_a) {
    return false;
  }
  double score_b = env_b->calcPeakScr(pos_b, inte_b, tolerance);
  double new_score_b = env_b->calcPeakScr(pos_b, inte_sum, tolerance);
  if (new_score_b < score_b) {
    return false;
  }
  return true;
}

bool CoTable::checkCoexist(MatchEnvPtr env_a, MatchEnvPtr env_b,
                           double tolerance) {
  std::vector<int> list_a = env_a->getRealEnvPtr()->getPeakIdxList();
  std::vector<int> list_b = env_b->getRealEnvPtr()->getPeakIdxList();
  int cnt_share = 0;
  int cnt_coexist = 0;
  for (size_t i = 0; i < list_a.size(); i++) {
    int peak_a = list_a[i];
    for (size_t j = 0; j < list_b.size(); j++) {
      int peak_b = list_b[j];
      if (env_a->getRealEnvPtr()->isExist(i) && peak_a == peak_b) {
        cnt_share++;
        if (checkCoexist(env_a, env_b, i, j, tolerance)) {
          cnt_coexist++;
        }
      }
    }
  }
  if (cnt_share <= 1 || cnt_coexist == cnt_share) {
    return true;
  } else {
    return false;
  }
}

void CoTable::compTableEntry(MatchEnvPtrVec &env_list, MatchEnvPtr2D &win_envs, 
                             std::vector<bool> &rows, int id, int win,
                             double tolerance) {
  if (win < 0) {
    return;
  }
  for (size_t k = 0; k < win_envs[win].size(); k++) {
    int prev_id = win_envs[win][k]->getId();
    if (prev_id >= id) {
      return;
    }
    rows[id - prev_id - 1] = checkCoexist(env_list[prev_id], env_list[id], tolerance);
  }
}

// initialize coexist table 
std::vector<std::vector<bool>> CoTable::initCoexistTable(MatchEnvPtr2D &win_envs, double tolerance) {
  MatchEnvPtrVec env_list;
  for (size_t i = 0; i < win_envs.size(); i++) {
    for (size_t j = 0; j < win_envs[i].size(); j++) {
      env_list.push_back(win_envs[i][j]);
    }
  }
  for (size_t i = 0; i < env_list.size(); i++) {
    env_list[i]->setId(i);
  }
  std::vector<std::vector<bool>> co_table(env_list.size());
  for (size_t i = 0; i < win_envs.size(); i++) {
    for (size_t j = 0; j < win_envs[i].size(); j++) {
      int id = win_envs[i][j]->getId();
      int size = cntTableSize(win_envs, i, j);
      co_table[id].resize(size);
      compTableEntry(env_list, win_envs, co_table[id], id, i - 2, tolerance);
      compTableEntry(env_list, win_envs, co_table[id], id, i - 1, tolerance);
      compTableEntry(env_list, win_envs, co_table[id], id, i, tolerance);
    }
  }
  return co_table;
}

}
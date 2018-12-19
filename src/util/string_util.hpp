//Copyright (c) 2014 - 2018, The Trustees of Indiana University.
//
//Licensed under the Apache License, Version 2.0 (the "License");
//you may not use this file except in compliance with the License.
//You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
//Unless required by applicable law or agreed to in writing, software
//distributed under the License is distributed on an "AS IS" BASIS,
//WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//See the License for the specific language governing permissions and
//limitations under the License.


#ifndef TOPPIC_UTIL_STRING_UTIL_HPP_
#define TOPPIC_UTIL_STRING_UTIL_HPP_

#include <string>
#include <vector>

namespace toppic {

typedef std::vector<std::pair<std::string, std::string> > StringPairVec;

namespace string_util {

std::string trim(const std::string & ori_s);

std::vector<std::string> split(const std::string & ori_s, char delim);

std::string convertToString(bool value);

std::string convertToString(int value);

std::string convertToString(size_t value);

std::string convertToString(double value);

std::string convertToString(double value, int precision);

std::string convertToScientificStr(double value, int precision);

std::string rmComment(const std::string &ori_s, const std::string & comment = "#");

double convertScientificToDouble(std::string str);

bool endsWith(const std::string &str, const std::string &suffix);

}  // namespace string_util

}  // namespace toppic

#endif

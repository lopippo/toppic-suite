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

#ifndef PROT_FEATURE_MSALIGN_WRITER_HPP_
#define PROT_FEATURE_MSALIGN_WRITER_HPP_

#include <fstream>

#include "spec/deconv_ms.hpp"

namespace toppic {

namespace msalign_writer {

void write(std::ofstream &file, DeconvMsPtr ms_ptr);

}  // namespace msalign_writer

}  // namespace toppic
#endif

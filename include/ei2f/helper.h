/*
    Copyright (c) 2018-2019 Fraunhofer FKIE

    This file is part of EI2F.

    EI2F is free software; you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    As a special exception, the Contributors give you permission to link
    this library with independent modules to produce an executable,
    regardless of the license terms of these independent modules, and to
    copy and distribute the resulting executable under terms of your choice,
    provided that you also meet, for each linked independent module, the
    terms and conditions of the license of that module. An independent
    module is a module which is not derived from or based on this library.
    If you modify this library, you must extend this exception to your
    version of the library.

    EI2F is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
    License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with EI2F.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef DE_FRAUNHOFER_FKIE_EI2F_HELPER_H
#define DE_FRAUNHOFER_FKIE_EI2F_HELPER_H

#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>

namespace de {
namespace fraunhofer {
namespace fkie {
namespace ei2f {

void read_directory(const std::string& name, std::vector<std::string>& v);
bool starts_with(const std::string &mainStr, const std::string &toMatch);
bool ends_with(const std::string &mainStr, const std::string &toMatch);
void left_split(const std::string& str, const std::string& delim, std::string& left, std::string& right);
void replace_all(std::string& str, const std::string& oldStr, const std::string& newStr);
void replace_all(std::string& str, const std::string& oldStr, const std::string& newStr, const std::string& start_delim, const std::string& end_delim);
void replace_all_by_dict(std::string& str, const std::unordered_map<std::string, std::string>& mapping, const std::string& start_delim, const std::string& end_delim);

/*
struct OMemoryBuffer: std::streambuf
{
    OMemoryBuffer(char* base, size_t size)
    {
        this->setp(base, base + size);
    }
};

struct OMemoryStream: virtual OMemoryBuffer, std::ostream
{
    OMemoryStream(char* base, size_t size) :
        OMemoryBuffer(base, size),
        std::ostream(static_cast<std::streambuf*>(this))
    {
    }
};
*/

}}}}

#endif // DE_FRAUNHOFER_FKIE_EI2F_HELPER_H

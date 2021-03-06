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

#include <set>
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <unordered_map>

#include <dlfcn.h>
#include <Poco/Glob.h>

#include <sys/types.h>
#include <dirent.h>

#include <ei2f/helper.h>
#include <ei2f/ObjectConfig.h>
#include <ei2f/transport/Transport.h>

namespace de {
namespace fraunhofer {
namespace fkie {
namespace ei2f {

using namespace std;

bool starts_with(const std::string &mainStr, const std::string &toMatch)
{
    if(mainStr.size() >= toMatch.size() &&
            mainStr.compare(0, toMatch.size(), toMatch) == 0)
            return true;
        else
            return false;
}

bool ends_with(const std::string &mainStr, const std::string &toMatch)
{
    if(mainStr.size() >= toMatch.size() &&
            mainStr.compare(mainStr.size() - toMatch.size(), toMatch.size(), toMatch) == 0)
            return true;
        else
            return false;
}

void read_directory(const std::string& name, std::vector<string>& v)
{
    DIR* dirp = opendir(name.c_str());
    struct dirent * dp;
    if (dirp == NULL) {
        throw std::runtime_error(string("Could not open directory ") + name);
    }
    while ((dp = readdir(dirp)) != NULL) {
        v.push_back(dp->d_name);
    }
    closedir(dirp);
}

void left_split(const std::string& str, const std::string& delim, string& left, string& right)
{
    right = str;

    auto pos = right.find(delim);
    if (pos == std::string::npos) {
        throw std::runtime_error(string("Could not left_split given string '") + right + "' with delimiter '" + delim + "'");
    }

    left = right.substr(0, pos);
    right.erase(0, pos + delim.length());
}

void
replace_all(std::string& str, const std::string& old_str, const std::string& new_str)
{
    std::string::size_type pos = str.find(old_str, 0);
    while(pos != std::string::npos) {
        str.replace(pos, old_str.length(), new_str);
        pos += new_str.length();
        pos = str.find(old_str, pos);
    }
}

void
replace_all(std::string& str, const std::string& old_str, const std::string& new_str, const string& start_delim, const string& end_delim)
{
    string decorated_old_str;

    decorated_old_str.reserve(start_delim.size() + old_str.size() + end_delim.size());

    decorated_old_str += start_delim;
    decorated_old_str += old_str;
    decorated_old_str += end_delim;

    replace_all(str, decorated_old_str, new_str);
}

void replace_all_by_dict(std::string& str, const std::unordered_map<string, string>& mapping, const string& start_delim, const string& end_delim)
{
    for (auto mapping_step: mapping) {
        replace_all(str, mapping_step.first, mapping_step.second, start_delim, end_delim);
    }
}

}}}}

//template class std::_Hashtable<std::string, std::string, std::allocator<std::string>, std::__detail::_Identity, std::equal_to<std::string>, std::hash<std::string>, std::__detail::_Mod_range_hashing, std::__detail::_Default_ranged_hash, std::__detail::_Prime_rehash_policy, std::__detail::_Hashtable_traits<true, true, true> >;
//template class std::_Hashtable<std::string, std::pair<std::string const, std::string>, std::allocator<std::pair<std::string const, std::string> >, std::__detail::_Select1st, std::equal_to<std::string>, std::hash<std::string>, std::__detail::_Mod_range_hashing, std::__detail::_Default_ranged_hash, std::__detail::_Prime_rehash_policy, std::__detail::_Hashtable_traits<true, false, true> >;

//template void std::_Sp_counted_ptr<de::fraunhofer::fkie::ei2f::transport::Transport*, (__gnu_cxx::_Lock_policy)2>::_M_destroy();
//template void std::_Sp_counted_ptr<de::fraunhofer::fkie::ei2f::transport::Transport*, (__gnu_cxx::_Lock_policy)2>::_M_dispose();

//template void std::_Sp_counted_base<(__gnu_cxx::_Lock_policy)2>::_M_destroy();
//template void std::_Sp_counted_base<(__gnu_cxx::_Lock_policy)2>::_M_release();

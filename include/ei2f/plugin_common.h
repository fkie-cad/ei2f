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

#ifndef DE_FRAUNHOFER_FKIE_EI2F_PLUGIN_COMMON_H
#define DE_FRAUNHOFER_FKIE_EI2F_PLUGIN_COMMON_H

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <memory>
#include <locale>

#include <ei2f/transport/Transport.h>

// TODO MAYBE TO BROAD (INCREASES LIB SIZE)
//extern template class std::unordered_map<std::string,std::string>;
//extern template class std::unordered_set<std::string>;

//extern template class std::_Hashtable<std::string, std::pair<std::string const, std::string>, std::allocator<std::pair<std::string const, std::string> >, std::__detail::_Select1st, std::equal_to<std::string>, std::hash<std::string>, std::__detail::_Mod_range_hashing, std::__detail::_Default_ranged_hash, std::__detail::_Prime_rehash_policy, std::__detail::_Hashtable_traits<true, false, true> >;
//extern template class std::_Hashtable<std::string, std::string, std::allocator<std::string>, std::__detail::_Identity, std::equal_to<std::string>, std::hash<std::string>, std::__detail::_Mod_range_hashing, std::__detail::_Default_ranged_hash, std::__detail::_Prime_rehash_policy, std::__detail::_Hashtable_traits<true, true, true> >;

//extern template void std::_Sp_counted_ptr<de::fraunhofer::fkie::ei2f::transport::Transport*, (__gnu_cxx::_Lock_policy)2>::_M_destroy();
//extern template void std::_Sp_counted_ptr<de::fraunhofer::fkie::ei2f::transport::Transport*, (__gnu_cxx::_Lock_policy)2>::_M_dispose();
//extern template void std::_Sp_counted_base<(__gnu_cxx::_Lock_policy)2>::_M_destroy();
//extern template void std::_Sp_counted_base<(__gnu_cxx::_Lock_policy)2>::_M_release();

//extern template std::unordered_map<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::hash<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::equal_to<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > > > >::unordered_map(std::unordered_map<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::hash<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::equal_to<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > > > > const&);
//extern template std::ctype<char>::char_type std::ctype<char>::do_widen(char) const;

#endif // DE_FRAUNHOFER_FKIE_EI2F_PLUGIN_COMMON_H

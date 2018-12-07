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

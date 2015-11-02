
#ifndef PROXY_HEXDATA_H
#define PROXY_HEXDATA_H

#include <memory>
#include <plugin/RSlice.h>

class HexData {

public:
    HexData(const std::string& hex);

    proxy::RSlice GetSlice() const;

private:

    static std::string RemoveSpaces(const std::string& hex);

    static void RemoveSpacesInPlace(std::string& s);

    static size_t Validate(const std::string& hex);

    static bool IsHexChar(char i);
    static bool IsDigit(char i);
    static bool IsUpperHexAlpha(char i);
    static bool IsLowerHexAlpha(char i);

    std::string m_input;
    size_t m_size;
    std::unique_ptr<uint8_t[]> m_buffer;

};


#endif //PROXY_HEXDATA_H

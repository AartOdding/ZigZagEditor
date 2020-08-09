#pragma once

#include <array>
#include <string>

#include <imgui.h>


namespace HexColor
{

    inline std::string write(ImVec4 color)
    {
        constexpr static std::array<char, 16> characters{ '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

        std::array<unsigned char, 4> bytes;
        bytes[0] = color.x * 255.0f;
        bytes[1] = color.y * 255.0f;
        bytes[2] = color.z * 255.0f;
        bytes[3] = color.w * 255.0f;

        std::string result = "#";

        for (int i = 0; i < 4; ++i)
        {
            unsigned char msbits = bytes[i] >> 4;
            unsigned char lsbits = bytes[i] & 0b00001111;
            result.push_back(characters[msbits]);
            result.push_back(characters[lsbits]);
        }
        return result;
    }

    // string doesnt need to be null terminated, but atleast 9 characters long.
    inline ImVec4 read(const std::string& string)
    {
        ImVec4 result{ 0, 0, 0, 1 };

        auto charValue = [](char character) -> int
        { 
            if      (character >= '0' && character <= '9') return  character - '0';
            else if (character >= 'a' && character <= 'f') return (character - 'a') + 10;
            else if (character >= 'A' && character <= 'F') return (character - 'A') + 10;
            return 0;
        };

        if (string.size() >= 9 && string[0] == '#')
        {
            int r = charValue(string[1]) * 16 + charValue(string[2]);
            int g = charValue(string[3]) * 16 + charValue(string[4]);
            int b = charValue(string[5]) * 16 + charValue(string[6]);
            int a = charValue(string[7]) * 16 + charValue(string[8]);
            result = { r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f };
        }
        return result;
    }

}



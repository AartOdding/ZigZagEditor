#pragma once

#include <algorithm>
#include <string>
#include <string_view>
#include <vector>

#include <iostream>

namespace StringUtils
{
    size_t countCharacter(const char* string, char character) 
    {
        size_t count = 0;

        for (const char* it = string; it != '\0'; ++it)
        {
            count += (*it == character);
        }
        return count;
    }

    std::vector<std::string_view> split(const char* string, char delimiter)
    {
        std::vector<std::string_view> parts;

        if (string)
        {
            auto numParts = countCharacter(string, delimiter);
            parts.reserve(numParts);
            
            bool searchingPartBegin = true;
            const char* partBegin;
            
            for (const char* it = string; it != '\0'; ++it)
            {
                auto character = *it;

                if (searchingPartBegin && character != delimiter)
                {
                    partBegin = it;
                    searchingPartBegin = false;
                }
                if (character == delimiter)
                {
                    if (!searchingPartBegin)
                    {
                        size_t sz = it - partBegin;
                        std::string_view strv(partBegin, sz);
                        parts.push_back(strv);
                    }
                    searchingPartBegin = true;
                }
            }
        }
        return parts;
    }

}

#pragma once

#include <algorithm>
#include <string>
#include <string_view>
#include <vector>

#include <iostream>

namespace StringUtils
{

    std::vector<std::string_view> split(std::string_view string, char delimiter)
    {
        std::vector<std::string_view> parts;

        if (!string.empty())
        {
            auto numParts = std::count(string.begin(), string.end(), delimiter);
            parts.reserve(numParts);
            
            bool searchingPartBegin = true;
            std::string_view::const_iterator partBegin;
            
            for (auto it = string.begin(); it != string.end(); ++it)
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
                        parts.emplace_back(partBegin, it);
                    }
                    searchingPartBegin = true;
                }
            }
        }
        return parts;
    }

}

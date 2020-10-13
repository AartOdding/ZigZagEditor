#pragma once

#include <cstdint>

#include <util/Platform.hpp>


ZIGZAG_API void onProjectCreated(const char* name, std::uint64_t projectID);

ZIGZAG_API void onProjectCleared();

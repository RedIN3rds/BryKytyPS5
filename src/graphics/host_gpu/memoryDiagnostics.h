#pragma once

#include "common/common.h"

#if KYTY_PLATFORM == KYTY_PLATFORM_WINDOWS

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <windows.h>
#include <psapi.h>
#include <cstdio>

#pragma comment(lib, "Psapi.lib")

namespace Libs::Graphics {

inline void LogHostMemoryUsage()
{
    PROCESS_MEMORY_COUNTERS_EX pmc{};
    pmc.cb = sizeof(pmc);

    MEMORYSTATUSEX mem{};
    mem.dwLength = sizeof(mem);

    if (!GetProcessMemoryInfo(
            GetCurrentProcess(),
            reinterpret_cast<PROCESS_MEMORY_COUNTERS*>(&pmc),
            sizeof(pmc)))
        return;

    if (!GlobalMemoryStatusEx(&mem))
        return;

    constexpr double MB = 1024.0 * 1024.0;

    std::printf(
        "[MEMORY] Private: %.2f MB | Working Set: %.2f MB | "
        "Available Physical: %.2f MB | Available Pagefile: %.2f MB\n",
        pmc.PrivateUsage / MB,
        pmc.WorkingSetSize / MB,
        mem.ullAvailPhys / MB,
        mem.ullAvailPageFile / MB
    );
}

inline void LogSystemCommit()
{
    PERFORMANCE_INFORMATION info{};
    info.cb = sizeof(info);

    if (!GetPerformanceInfo(&info, sizeof(info)))
        return;

    constexpr double MB = 1024.0 * 1024.0;

    const double used =
        static_cast<double>(info.CommitTotal) * info.PageSize / MB;

    const double limit =
        static_cast<double>(info.CommitLimit) * info.PageSize / MB;

    std::printf(
        "[COMMIT] Used: %.2f MB | Limit: %.2f MB | Free: %.2f MB\n",
        used,
        limit,
        limit - used
    );
}

} // namespace Libs::Graphics

#endif
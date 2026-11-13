#pragma once

#include <ctime>
#include <exception>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>

namespace rematch_cli {

inline std::optional<double> resident_memory_mb()
{
    std::ifstream status("/proc/self/status");
    std::string line;
    while (std::getline(status, line)) {
        std::istringstream fields(line);
        std::string name, unit;
        double kilobytes;
        if (fields >> name >> kilobytes >> unit &&
            name == "VmRSS:" && unit == "kB") {
            return kilobytes / 1024.0;
        }
    }
    return std::nullopt;
}

inline double milliseconds(clock_t ticks)
{
    return 1000.0 * static_cast<double>(ticks) / CLOCKS_PER_SEC;
}

inline void print_metric(const std::optional<double>& value)
{
    if (value) {
        std::cout << *value;
    } else {
        std::cout << "NA";
    }
}

inline int run(int argc, char* argv[],
               bool (*matcher)(std::string, std::string),
               const clock_t* build_ticks = nullptr,
               const bool* phase_timing_available = nullptr)
{
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " 'pattern' 'string'\n";
        return 2;
    }

    try {
        const auto memory_before = resident_memory_mb();
        const clock_t begin = clock();
        const bool matched = matcher(argv[1], argv[2]);
        const clock_t end = clock();
        const auto memory_after = resident_memory_mb();

        std::optional<double> build_ms, match_ms, memory_delta_mb;
        if (build_ticks && phase_timing_available && *phase_timing_available) {
            build_ms = milliseconds(*build_ticks);
            match_ms = milliseconds(end - begin - *build_ticks);
        }
        if (memory_before && memory_after) {
            memory_delta_mb = *memory_after - *memory_before;
        }

        std::cout << std::fixed << std::setprecision(6)
                  << (matched ? 1 : 0) << ' ';
        print_metric(build_ms);
        std::cout << ' ';
        print_metric(match_ms);
        std::cout << ' ' << milliseconds(end - begin) << ' ';
        print_metric(memory_delta_mb);
        std::cout << '\n';
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }
}

} // namespace rematch_cli

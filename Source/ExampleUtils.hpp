#pragma once

#include <array>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_set>

namespace {
void askBipolar(bool* bipolarMode) {
    // Instead of using menu manually, user can pass argument to the executable
    char input;
    std::cout << "Choose session mode\n"
              << "b - bipolar\n"
              << "m - monopolar" << std::endl;
    while (std::cin >> input) {
        if (input == 'b' || input == 'm') {
            *bipolarMode = input == 'b' ? true : false;
            break;
        } else {
            std::cerr << "Wrong input. Try again." << std::endl;
        }
    }
}

void askCsv(bool* writeCsv) {
    char input;
    std::cout << "Do you want to write raw data into csv?\n"
              << "y - yes\n"
              << "n - no" << std::endl;
    while (std::cin >> input) {
        if (input == 'y' || input == 'n') {
            *writeCsv = input == 'y' ? true : false;
            break;
        } else {
            std::cerr << "Wrong input. Try again." << std::endl;
        }
    }
}

void askLicense(std::string* licenseKey) {
    std::cout << "Enter your license key:" << std::endl;
    std::cin >> *licenseKey;
}
} // namespace

void parseArgs(int argc, char* argv[], std::string* licenseKey, bool* bipolarMode, bool* writeCsv) {
    using namespace std::string_view_literals;
    constexpr std::array flagsToFind{
        "--key"sv,
        "--bipolar"sv,
        "--csv"sv};
    const std::unordered_set<std::string_view> trueArgs{"true"sv, "on"sv, "1"sv};
    const std::unordered_set<std::string_view> falseArgs{"false"sv, "off"sv, "0"sv};

    std::optional<std::string> licenseKeyArg = std::nullopt;
    std::optional<bool> bipolarModeArg = std::nullopt;
    std::optional<bool> writeCsvArg = std::nullopt;

    for (int i = 1; i < argc; ++i) {
        const std::string_view arg(argv[i]);
        for (auto flag : flagsToFind) {
            if (auto flagPos = arg.find(flag); flagPos != std::string::npos) {
                const auto pos = arg.find("="sv);
                const auto res = arg.substr(pos + 1);
                if (licenseKey != nullptr && flag == flagsToFind[0]) {
                    if (pos != arg.npos) {
                        licenseKeyArg.emplace(res);
                    }
                    break;
                } else if (bipolarMode != nullptr && flag == flagsToFind[1]) {
                    if (trueArgs.contains(res)) {
                        bipolarModeArg.emplace(true);
                    } else if (falseArgs.contains(res)) {
                        bipolarModeArg.emplace(false);
                    }
                    break;
                } else if (writeCsv != nullptr && flag == flagsToFind[2]) {
                    if (trueArgs.contains(res)) {
                        writeCsvArg.emplace(true);
                    } else if (falseArgs.contains(res)) {
                        writeCsvArg.emplace(false);
                    }
                    break;
                }
            }
        }
    }

    if (licenseKey != nullptr) {
        if (licenseKeyArg.has_value()) {
            *licenseKey = *licenseKeyArg;
        } else {
            askLicense(licenseKey);
        }
    }
    if (bipolarMode != nullptr && !bipolarModeArg.has_value()) {
        if (bipolarModeArg.has_value()) {
            *bipolarMode = *bipolarModeArg;
        } else {
            askBipolar(bipolarMode);
        }
    }
    if (writeCsv != nullptr && !writeCsvArg.has_value()) {
        if (writeCsvArg.has_value()) {
            *writeCsv = *writeCsvArg;
        } else {
            askCsv(writeCsv);
        }
    }
}

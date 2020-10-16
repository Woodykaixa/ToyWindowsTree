//
// Created by RTkaixa on 2020/10/10.
//

#include "ArgParser.h"
#include <string_view>
#include <fmt/core.h>
#include <iostream>

using std::wcerr;

WindowsTree::ArgParser::ArgParser(int argc, wchar_t **argv) : _argc(argc), _argv(argv) {
    _config = {
            true,
            false,
            false,
            -1,
            L".",
            std::wregex(L".*")
    };
}

inline std::wstring_view secureGetNextArg(int argc, wchar_t **argv, int index) {
    if (index < argc && argv[index][0] != '-') {
        return argv[index];
    }
    wcerr << fmt::format(L"Missing parameter for option [{}]", argv[index - 1]);
    exit(1);
}

WindowsTree::WTConfig &WindowsTree::ArgParser::parse() {
    for (int i = 1; i < _argc; i++) {
        std::wstring_view currentArg = _argv[i];
        if (currentArg == L"-a") {
            _config.FolderOnly = false;
        } else if (currentArg == L"-f") {
            _config.FullPath = true;
        } else if (currentArg == L"-h") {
            _config.PrintHelp = true;
        } else if (currentArg.substr(0, 2) == L"-l") {
            try {
                _config.Level = std::stoi(currentArg.substr(3).data());

            } catch (std::exception &e) {
                std::cerr << fmt::format("An error occurred when parsing argument -l: {}\n", e.what());
                exit(1);
            }
        } else if (currentArg.substr(0, 2) == L"-p") {
            _config.MatchRegex = std::wregex(currentArg.substr(3).data());
        } else {
            _config.ScanPath = currentArg;
        }
    }
    return _config;
}

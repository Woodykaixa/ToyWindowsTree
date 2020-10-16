//
// Created by RTkaixa on 2020/10/10.
//

#pragma once

#include <string_view>
#include <regex>

namespace WindowsTree {

    struct WTConfig {
        bool FolderOnly;
        bool FullPath;
        bool PrintHelp;
        int Level;
        std::wstring ScanPath;
        std::wregex MatchRegex;
    };

    class ArgParser {
    public:
        ArgParser(int argc, wchar_t **argv);

        WTConfig &parse();


    private:
        int _argc;
        wchar_t **_argv;
        WTConfig _config;
    };
}


#include <iostream>
#include <cstring>
#include <fmt/core.h>
#include "FileScanner.h"
#include "ArgParser.h"

void help() {
    wprintf(LR"(
Visualize specific folder.
    Usage: WindowsTree [Options|Directory]

    Options:
        -a           Print all files instead of folders
        -f           Print full path of files
        -h           Print help text
        -l={level}   Print only LEVEL directories deep.
        -p={pattern} List only those files that match regex PATTERN.

    Directory:
        The directory to scan.

)");
}

int wmain(int argc, wchar_t **argv) {
    std::locale::global(std::locale("zh_CN.UTF-8"));
    auto parser = new WindowsTree::ArgParser(argc, argv);
    auto config = parser->parse();
    if (config.PrintHelp) {
        help();
        return 0;
    }
    auto fs = new WindowsTree::FileScanner(config.ScanPath, config.Level, config.MatchRegex);
    fs->recursiveScanFile(config.FolderOnly, config.FullPath);
    return 0;
}

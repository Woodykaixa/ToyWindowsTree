//
// Created by RTkaixa on 2020/10/10.
//

#include "FileScanner.h"
#include <fmt/core.h>
#include <regex>
#include <fmt/format.h>
#include <iostream>
#include "StupidEncodingUtil.h"

using namespace WindowsTree;
using std::wcout;
using std::wcerr;

bool FileScanner::isDirectory(DWORD attr) {
    return attr & FILE_ATTRIBUTE_DIRECTORY;
}


FileScanner::FileScanner(std::wstring &path, int level, std::wregex &matchRegex) {
    _basePath = path;
    _findHandle = nullptr;
    _fileData = new WIN32_FIND_DATA();
    _level = level;
    _matchRegex = matchRegex;
}

inline bool isCurrent(const wchar_t *path) {
    return path[0] == '.' && path[1] == '\0';
}

inline bool isParent(const wchar_t *path) {
    return path[0] == '.' && path[1] == '.' && path[2] == '\0';
}


void FileScanner::forEachFile(FileScanner::ForeachFunctionNoReturn function) {
    _findHandle = FindFirstFile(_basePath.c_str(), _fileData);
    if (_findHandle == INVALID_HANDLE_VALUE) {
        wcerr << fmt::format(L"Cannot open path: {}", _basePath);
        exit(1);
    }
    function(_fileData);
    while (FindNextFile(_findHandle, _fileData)) {
        function(_fileData);
    }
    FindClose(_findHandle);
    _findHandle = nullptr;
}

File *FileScanner::readFindData(WIN32_FIND_DATA *findData, std::wstring &path, int level, bool folderOnly) {
    if (level == 0) {
        return nullptr;
    }
    if (isCurrent(findData->cFileName) || isParent(findData->cFileName)) {
        return nullptr;
    }
    bool isDir = FileScanner::isDirectory(findData->dwFileAttributes);
    if (folderOnly && !isDir) {
        return nullptr;
    }
    if (!std::regex_match(findData->cFileName, _matchRegex) && !isDir) {
        return nullptr;
    }
    File *file = new File();
    file->filename = findData->cFileName;
    if (isDir) {
        auto np = fmt::format(L"{}/{}", path, findData->cFileName);
        auto fs = new FileScanner(np, level - 1, _matchRegex);
        file->files = fs->scanFile(np, folderOnly);
    }
    file->isFolder = isDir;
    return file;
}

std::vector<File> FileScanner::scanFile(std::wstring &basePath, bool FolderOnly) {
    std::vector<File> files;
    auto p = _basePath + L"/*";
    _findHandle = FindFirstFile(p.c_str(), _fileData);
    if (_findHandle == INVALID_HANDLE_VALUE) {
        wcerr << fmt::format(L"Cannot open path: {}", _basePath);
        exit(1);
    }
    auto f = readFindData(_fileData, _basePath, _level, FolderOnly);
    if (f) {
        files.emplace_back(*f);
    }
    while (FindNextFile(_findHandle, _fileData)) {
        f = readFindData(_fileData, _basePath, _level, FolderOnly);
        if (f) {
            files.emplace_back(*f);
        }
    }
    FindClose(_findHandle);
    _findHandle = nullptr;
    return files;
}


void FileScanner::printFile(std::wstring &basePath, bool fullPath, File &file, int currentLevel, bool isLast,
                            const std::vector<bool> &prefixLines) {
    auto path = fullPath ? fmt::format(L"{}/{}", basePath, file.filename) : file.filename;

    for (auto hasPrefix: prefixLines) {
        fmt::print("{}   ", hasPrefix ? '|' : ' ');
    }
    auto wStr = isLast ? L"└─" : L"├─";
    std::cout << WideCharToHumanChar(wStr);
    wcout << fmt::format(L" {}\n", path);

    if (file.isFolder) {
        for (auto f:file.files) {
            auto isLastFile = f.filename == file.files.rbegin()->filename;
            auto copyPrefixLines = prefixLines;
            copyPrefixLines.emplace_back(!isLast);
            printFile(path, fullPath, f, currentLevel + 1, isLastFile, copyPrefixLines);
        }
    }
}


void FileScanner::recursiveScanFile(bool folderOnly, bool fullPath) {
    auto files = this->scanFile(_basePath, folderOnly);
    wcout << fmt::format(L"   {}\n", _basePath);
    for (auto &file:files) {
        auto isLastFile = file.filename == files.rbegin()->filename;
        std::vector<bool> pl = {false};
        printFile(_basePath, fullPath, file, 0, isLastFile, pl);
    }
}


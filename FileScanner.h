//
// Created by RTkaixa on 2020/10/10.
//

#pragma once

#include <string_view>
#include <vector>
#include <Windows.h>
#include <minwindef.h>
#include <regex>

namespace WindowsTree {

    struct File {
        std::wstring filename;
        bool isFolder;
        std::vector<File> files;
    };

    class FileScanner {
    public:
        FileScanner(std::wstring &path, int level, std::wregex &matchRegex);

        using ForeachFunctionNoReturn = void (*)(WIN32_FIND_DATA *findData);

        void forEachFile(ForeachFunctionNoReturn function);

        void recursiveScanFile(bool folderOnly, bool fullPath);

        void printFile(std::wstring &basePath, bool fullPath, File &file, int currentLevel, bool isLast,
                       const std::vector<bool> &prefixLines);

        static bool isDirectory(DWORD);

    private:
        std::wstring _basePath;
        HANDLE _findHandle;
        WIN32_FIND_DATA *_fileData;
        std::wregex _matchRegex;
        int _level;

        std::vector<File> scanFile(std::wstring &basePath, bool FolderOnly);

        File *readFindData(WIN32_FIND_DATA *findData, std::wstring &path, int level, bool folderOnly);


    };
}


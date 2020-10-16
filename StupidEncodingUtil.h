//
// Created by RTkaixa on 2020/10/11.
//

#pragma once
#include <Windows.h>
#include <string>

std::string WideCharToHumanChar(const wchar_t * wStr){
    auto len = WideCharToMultiByte(GetACP(), 0, wStr, -1, nullptr, 0, nullptr, nullptr);
    auto cStr = new char[len + 1];
    WideCharToMultiByte(GetACP(), 0, wStr, -1, cStr, len, nullptr, nullptr);
    return std::string(cStr);
}

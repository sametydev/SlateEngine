#pragma once
#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <string_view>
#include <algorithm>

#pragma warning( disable : 4018 )
#pragma warning( disable : 4267 )
inline static wchar_t* charToWChar(const char* text)
{
    const size_t size = strlen(text) + 1;
    wchar_t* wText = new wchar_t[size];
    size_t outsize;
    mbstowcs_s(&outsize, wText, size, text, size - 1);
    return wText;
}

inline void convertWStringToCharPtr(_In_ std::wstring input, _Out_ char* outputString)
{
    size_t outputSize = input.length() + 1;
    outputString = new char[outputSize];
    size_t charsConverted = 0;
    wcstombs_s(&charsConverted, outputString, outputSize, input.c_str(), input.length());
}
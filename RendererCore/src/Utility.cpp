//
// Copyright (c) Microsoft. All rights reserved.
// This code is licensed under the MIT License (MIT).
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
// Developed by Minigraph
//
// Author:  James Stanard 
//

#include "Utility.h"
#include <locale>

// A faster version of memcopy that uses SSE instructions. 
void SIMDMemCopy(void* __restrict dest, const void* __restrict source, size_t numQuadwords)
{
	ASSERT(Math::IsAligned(dest, 16));
	ASSERT(Math::IsAligned(source, 16));

	__m128i* __restrict _dest = (__m128i* __restrict)dest;
	const __m128i* __restrict _source = (const __m128i* __restrict)source;
	// Cache line size is 64 bytes
	size_t initialQuadwordCount = (4 - ((size_t)_source >> 4) & 3) & 3;
	if (initialQuadwordCount > numQuadwords)
		initialQuadwordCount = numQuadwords;

	switch (initialQuadwordCount)
	{
	case 3: _mm_stream_si128(_dest + 2, _mm_load_si128(_source + 2));
	case 2: _mm_stream_si128(_dest + 1, _mm_load_si128(_source + 1));
	case 1: _mm_stream_si128(_dest,		_mm_load_si128(_source));
	default:
		break;
	}

	if (numQuadwords == initialQuadwordCount)
		return;

	_dest += initialQuadwordCount;
	_source += initialQuadwordCount;
	numQuadwords -= initialQuadwordCount;

	size_t cacheLines = numQuadwords >> 2;

	switch (cacheLines)
	{
	default:
	case 10: _mm_prefetch((char*)(_source + 36), _MM_HINT_NTA);
	case 9:  _mm_prefetch((char*)(_source + 32), _MM_HINT_NTA);
	case 8:  _mm_prefetch((char*)(_source + 28), _MM_HINT_NTA);
	case 7:  _mm_prefetch((char*)(_source + 24), _MM_HINT_NTA);
	case 6:  _mm_prefetch((char*)(_source + 20), _MM_HINT_NTA);
	case 5:  _mm_prefetch((char*)(_source + 16), _MM_HINT_NTA);
	case 4:  _mm_prefetch((char*)(_source + 12), _MM_HINT_NTA);
	case 3:  _mm_prefetch((char*)(_source + 8 ), _MM_HINT_NTA);
	case 2:  _mm_prefetch((char*)(_source + 4 ), _MM_HINT_NTA);
	case 1:  _mm_prefetch((char*)(_source),		 _MM_HINT_NTA);

		for (size_t i = cacheLines; i > 0; --i)
		{
			if (i >= 10)
				_mm_prefetch((char*)(_source + 40), _MM_HINT_NTA);

			_mm_stream_si128(_dest, _mm_load_si128(_source));
			_mm_stream_si128(_dest + 1, _mm_load_si128(_source + 1));
			_mm_stream_si128(_dest + 2, _mm_load_si128(_source + 2));
			_mm_stream_si128(_dest + 3, _mm_load_si128(_source + 3));

			_dest += 4;
			_source += 4;
		}
	case 0:
		break;
	}

	switch (numQuadwords & 3)
	{
	case 3: _mm_stream_si128(_dest + 2, _mm_load_si128(_source + 2));
	case 2: _mm_stream_si128(_dest + 1, _mm_load_si128(_source + 1));
	case 1: _mm_stream_si128(_dest,		_mm_load_si128(_source));
	default:
		break;
	}

	_mm_sfence();
}

void SIMDMemFill(void* __restrict dest, __m128 fillVector, size_t numQuadwords)
{
	ASSERT(Math::IsAligned(dest, 16));

	register const __m128i source = _mm_castps_si128(fillVector);
	__m128i* __restrict _dest = (__m128i* __restrict)dest;

	switch (((size_t)_dest >> 4) & 3)
	{
	case 1: _mm_stream_si128(_dest++, source); --numQuadwords;
	case 2: _mm_stream_si128(_dest++, source); --numQuadwords;
	case 3: _mm_stream_si128(_dest++, source); --numQuadwords;
	default:
		break;
	}

	size_t wholeCacheLines = numQuadwords >> 2;
	// Do four quadwords per loop to minimize stalls.
	while (wholeCacheLines--)
	{
		_mm_stream_si128(_dest++, source);
		_mm_stream_si128(_dest++, source);
		_mm_stream_si128(_dest++, source);
		_mm_stream_si128(_dest++, source);
	}

	// Copy the remaining quadwords
	switch (numQuadwords & 3)
	{
	case 3: _mm_stream_si128(_dest++, source);
	case 2: _mm_stream_si128(_dest++, source);
	case 1: _mm_stream_si128(_dest++, source);
	default:
		break;
	}

	_mm_sfence();
}

std::wstring Utility::AnsiToWideString(const std::string& str)
{
	wchar_t wstr[MAX_PATH];
	if (!MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str.c_str(), -1, wstr, MAX_PATH))
		wstr[0] = L'\0';
	return wstr;
}

std::string Utility::WideStringToAnsi(const std::wstring& wstr)
{
	char str[MAX_PATH];
	if (!WideCharToMultiByte(CP_ACP, MB_PRECOMPOSED, wstr.c_str(), -1, str, MAX_PATH, nullptr, nullptr))
		str[0] = '\0';
	return str;
}

std::string Utility::ToLower(const std::string& str)
{
	std::string lower_case = str;
	std::locale loc;
	for (char& s : lower_case)
		s = std::tolower(s, loc);
	return lower_case;
}

std::wstring Utility::ToLower(const std::wstring& wstr)
{
	std::wstring lower_case = wstr;
	std::locale loc;
	for (wchar_t& s : lower_case)
		s = std::tolower(s, loc);
	return lower_case;
}

std::string Utility::GetBasePath(const std::string& filePath)
{
	size_t lastSlash;
	if ((lastSlash = filePath.rfind('/')) != std::string::npos)
		return filePath.substr(0, lastSlash + 1);
	else if ((lastSlash = filePath.rfind('\\')) != std::string::npos)
		return filePath.substr(0, lastSlash + 1);
	else
		return "";
}

std::wstring Utility::GetBasePath(const std::wstring& filePath)
{
	size_t lastSlash;
	if ((lastSlash = filePath.rfind(L'/')) != std::wstring::npos)
		return filePath.substr(0, lastSlash + 1);
	else if ((lastSlash = filePath.rfind(L'\\')) != std::wstring::npos)
		return filePath.substr(0, lastSlash + 1);
	else
		return L"";
}

std::string Utility::RemoveBasePath(const std::string& filePath)
{
	size_t lastSlash;
	if ((lastSlash = filePath.rfind('/')) != std::string::npos)
		return filePath.substr(lastSlash + 1);
	else if ((lastSlash = filePath.rfind('\\')) != std::string::npos)
		return filePath.substr(lastSlash + 1);
	else
		return filePath;
}

std::wstring Utility::RemoveBasePath(const std::wstring& filePath)
{
	size_t lastSlash;
	if ((lastSlash = filePath.rfind(L'/')) != std::wstring::npos)
		return filePath.substr(lastSlash + 1);
	else if ((lastSlash = filePath.rfind(L'\\')) != std::wstring::npos)
		return filePath.substr(lastSlash + 1);
	else
		return filePath;
}

std::string Utility::GetFileExtension(const std::string& filePath)
{
	std::string fileName = RemoveBasePath(filePath);
	size_t extOffset = fileName.rfind('.');
	if (extOffset == std::string::npos)
		return "";

	return fileName.substr(extOffset + 1);
}

std::wstring Utility::GetFileExtension(const std::wstring& filePath)
{
	std::wstring fileName = RemoveBasePath(filePath);
	size_t extOffset = fileName.rfind(L'.');
	if (extOffset == std::wstring::npos)
		return L"";

	return fileName.substr(extOffset + 1);
}

std::string Utility::RemoveExtension(const std::string& filePath)
{
	return filePath.substr(0, filePath.rfind('.'));
}

std::wstring Utility::RemoveExtension(const std::wstring& filePath)
{
	return filePath.substr(0, filePath.rfind(L'.'));
}
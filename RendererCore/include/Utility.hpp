/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "SlimWindows.h"
#include <locale>

namespace LCH
{
	class Utility
	{
	private:
		Utility();

	public:
		static std::wstring AnsiToWideString(const std::string& str);
		static std::string WideStringToAnsi(const std::wstring& wstr);
		static std::string ToLower(const std::string& str);
		static std::wstring ToLower(const std::wstring& wstr);
		static std::string GetBasePath(const std::string& str);
		static std::wstring GetBasePath(const std::wstring& wstr);
		static std::string RemoveBasePath(const std::string& str);
		static std::wstring RemoveBasePath(const std::wstring& wstr);
		static std::string GetFileExtension(const std::string& str);
		static std::wstring GetFileExtension(const std::wstring& wstr);
		static std::string RemoveExtension(const std::string& str);
		static std::wstring RemoveExtension(const std::wstring& wstr);
	};
}
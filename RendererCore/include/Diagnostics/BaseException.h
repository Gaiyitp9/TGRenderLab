/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "../SlimWindows.h"
#include <exception>
#include <string>
#include <vector>

namespace LCH
{

	class BaseException : public std::exception
	{
		struct StackFrame
		{
			unsigned int index;
			std::wstring file;
			std::wstring function;
			unsigned int line;
		};

	public:
		BaseException(const std::wstring& description = L"No Description");
		~BaseException();

		virtual char const* what() const override;
		virtual wchar_t const* GetType() const noexcept;
		const std::wstring& GetExceptionInfo() const noexcept;

	protected:
		std::wstring wWhatBuffer;						// �쳣��Ϣ(���ַ���Unicode)
		std::string whatBuffer;							// �쳣��Ϣ(խ�ַ���GB2312)
		std::vector<StackFrame> stackFrameInfo;			// ջ֡��Ϣ

	private:
		void StackTrace();								// ׷��ջ֡��Ϣ

		HANDLE hProcess;								// ��ǰ���̾��
		constexpr static USHORT FRAMESTOCAPTURE = 100;	// ���׷��ջ֡��
		constexpr static UINT MAXNAMELEN = 255;			// ����������󳤶�
		const wchar_t* SEPARATOR = L"\n-----------------------------------------------\n";	// �ָ���
	};
}
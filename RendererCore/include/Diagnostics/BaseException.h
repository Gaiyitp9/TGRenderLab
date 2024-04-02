/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "../SlimWindows.h"
#include <exception>
#include <string>

namespace LCH
{
	class BaseException : public std::exception
	{
	public:
		BaseException(int lineNum, std::wstring filePath);
		~BaseException();

		virtual char const* what() const override;
		virtual wchar_t const* GetType() const noexcept;

		int GetLineNum() const noexcept;
		const std::wstring& GetFilePath() const noexcept;

	private:
		void StackTrace();		// ׷��ջ֡��Ϣ

	protected:
		int lineNum;
		std::wstring filePath;
		std::string whatBuffer;

	private:
		HANDLE hProcess;		// ��ǰ���̾��
		HANDLE hThread;			// ��ǰ�߳̾��
		constexpr static USHORT FramesToCapture = 100;	// ���׷��ջ֡��
		constexpr static UINT MaxNameLen = 255;			// ����������󳤶�
	};
}
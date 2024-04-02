/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Diagnostics/BaseException.h"
#include <format>
#include <DbgHelp.h>

namespace LCH
{
	BaseException::BaseException(std::string description)
	{
		// ��ʼ�����Ŵ����������ڶ�ջ��Ϣ׷��
		hProcess = GetCurrentProcess();
		SymInitialize(hProcess, nullptr, true);
		// ��¼ջ֡��Ϣ
		StackTrace();
		// ��¼�쳣��Ϣ
		whatBuffer = std::format("{}: {}\n", GetType(), description);
		whatBuffer += "-----------------------------------------------\n";
		for(const auto& info : stackFrameInfo)
		{
			whatBuffer += std::format("Frame: {}\nFile: {}\nFunction: {}\nLine: {}\n", 
				info.index, info.file, info.function, info.line);
			whatBuffer += "-----------------------------------------------\n";
		}
	}
	
	BaseException::~BaseException()
	{
		// �ͷŷ��Ŵ�������Դ
		SymCleanup(hProcess);
	}

	char const* BaseException::what() const
	{
		return whatBuffer.c_str();
	}

	char const* BaseException::GetType() const noexcept
	{
		return "Base Exception";
	}

	void BaseException::StackTrace()
	{
		void* stackFrames[FramesToCapture];
		USHORT frameCount = CaptureStackBackTrace(0, FramesToCapture, stackFrames, nullptr);

		SYMBOL_INFO* symbol = reinterpret_cast<SYMBOL_INFO*>(malloc(sizeof(SYMBOL_INFO) + MaxNameLen * sizeof(CHAR)));
		symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
		symbol->MaxNameLen = MaxNameLen;

		IMAGEHLP_LINE imageLine = { 0 };
		imageLine.SizeOfStruct = sizeof(IMAGEHLP_LINE);

		DWORD displacement;
		for (USHORT i = 0; i < frameCount; ++i)
		{
			SymFromAddr(hProcess, (DWORD64)stackFrames[i], nullptr, symbol);
			SymGetLineFromAddr(hProcess, (DWORD64)stackFrames[i], &displacement, &imageLine);
			StackFrame frame{ static_cast<unsigned int>(frameCount - i - 1), imageLine.FileName,
				symbol->Name, static_cast<unsigned int>(imageLine.LineNumber) };
			stackFrameInfo.push_back(frame);
		}

		free(symbol);
	}
}
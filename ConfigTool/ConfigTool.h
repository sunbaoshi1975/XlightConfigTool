
// ConfigTool.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CConfigToolApp: 
// �йش����ʵ�֣������ ConfigTool.cpp
//

class CConfigToolApp : public CWinApp
{
public:
	CConfigToolApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CConfigToolApp theApp;
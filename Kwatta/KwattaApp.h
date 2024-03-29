﻿///////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 
// ██╗░░██╗░██╗░░░░░░░██╗░█████╗░████████╗████████╗░█████╗░
// ██║░██╔╝░██║░░██╗░░██║██╔══██╗╚══██╔══╝╚══██╔══╝██╔══██╗
// █████═╝░░╚██╗████╗██╔╝███████║░░░██║░░░░░░██║░░░███████║
// ██╔═██╗░░░████╔═████║░██╔══██║░░░██║░░░░░░██║░░░██╔══██║
// ██║░╚██╗░░╚██╔╝░╚██╔╝░██║░░██║░░░██║░░░░░░██║░░░██║░░██║
// ╚═╝░░╚═╝░░░╚═╝░░░╚═╝░░╚═╝░░╚═╝░░░╚═╝░░░░░░╚═╝░░░╚═╝░░╚═╝
// 
// 
// This product: KWATTA (KWAliTy Test API) Test suite for Command-line SOAP/JSON/HTTP internet API's
// This program: TestSuite
// This File   : KwattaApp.h
// What it does: main header file for the AX/TestSuite application
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#ifndef __AFXWIN_H__
	#error _T("include 'stdafx.h' before including this file for PCH")
#endif

#include "TestSet.h"
#include "TestSuite.h"
#include "resource.h"       // main symbols
#include <afxwinappex.h>
#include <afxtooltipctrl.h>
#include <afxtooltipmanager.h>
#include <afxcontextmenumanager.h>

class KwattaApp : public CWinAppEx
{
public:
	KwattaApp() noexcept;

// Overrides
public:
	virtual BOOL InitInstance();

  void    SetBaseDirectory(CString p_directory) { m_baseDirectory = p_directory; };
  CString GetBaseDirectory()                    { return m_baseDirectory; };
  CString GetBaseDirectoryClean();

  int     StartTestEditor(Test& p_test,HWND p_hwnd,int p_row);
  int     StartTestRunner(Test& p_test,HWND p_hwnd,int p_row);

// Implementation
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

private:
  CString StandardParameters(HWND p_wnd,int p_row);
  CString GetParameterFilename(CString p_testname);
  bool    ParseCommandLine();
  void    Usage();

  CString m_baseDirectory;
  CString m_suiteFilename;
};

extern KwattaApp theApp;

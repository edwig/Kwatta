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
// This program: ResultViewer
// This File   : ResultViewer.h
// What it does: Shows the result of a test step.
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __AFXWIN_H__
	#error _T("include 'stdafx.h' before including this file for PCH")
#endif

#include "resource.h"		// main symbols

#define PRODUCT_NAME _T("ResultViewr")

enum class ResultType
{
  ResultType_unknown
 ,ResultType_Command
 ,ResultType_Internet
 ,ResultType_Database
 ,ResultType_Windows
};

// CResultViewerApp:
// See ResultViewer.cpp for the implementation of this class
//

class ResultViewerApp : public CWinAppEx
{
public:
	ResultViewerApp();

  CString GetResultFilename()     { return m_resultFilename; };
  CString GetParametersFilename() { return m_paramFilename;  };
  CString GetBaseDirectory()      { return m_baseDirectory;  };
  CString GetTestDirectory()      { return m_testDirectory;  };

  void    StartValidateEditor(CString p_validation,bool p_global,HWND p_wnd,int p_row);

private:
  void    Usage();
  bool    ParseStartParameters();
  CString StandardParameters(HWND p_wnd,int p_row,bool p_global);
  CString GetBaseDirectoryClean();
  CString GetTestDirectoryClean();

  CString m_resultFilename;
  CString m_paramFilename;
  CString m_baseDirectory;
  CString m_testDirectory;

  UINT64  m_callingHWND { NULL };
  int     m_callingROW  { 0    };
// Overrides
public:
	virtual BOOL InitInstance();
  ResultType DeduceValidateType();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern ResultViewerApp theApp;

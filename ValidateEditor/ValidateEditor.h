///////////////////////////////////////////////////////////////////////////////////////////////////////
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
// This program: ValidateEditor
// This File   : ValidateEditor.h
// What it does: Main program for the validation of a test step
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __AFXWIN_H__
	#error _T("include 'stdafx.h' before including this file for PCH")
#endif

#include "resource.h"		// main symbols
#include "Validate.h"

#define PRODUCT_NAME _T("Validate Editor")

// CValidateEditorApp:
// See ValidateEditor.cpp for the implementation of this class
//

class ValidateEditorApp : public CWinAppEx
{
public:
	ValidateEditorApp();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual BOOL ExitInstance();

  void    PromoteValidation(CWnd* p_wnd);
  XString GetEffectiveValidation();
  XString GetValidateFilename()  { return m_validateFilename; }
  XString GetParameterFilename() { return m_paramFilename;    }
  XString GetBaseDirectory()     { return m_baseDirectory;    }
  XString GetTestDirectory()     { return m_testDirectory;    }
  bool    GetGlobal()            { return m_global;           }

// Implementation

	DECLARE_MESSAGE_MAP()
private:
  bool ParseStartParameters();
  void Usage();
  ValidateType DeduceValidateType();

  XString m_validateFilename;
  XString m_paramFilename;
  XString m_baseDirectory;
  XString m_testDirectory;

  UINT64  m_callingHWND { NULL  };
  int     m_callingROW  { 0     };
  bool    m_global      { false };
};

extern ValidateEditorApp theApp;

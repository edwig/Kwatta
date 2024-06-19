// TestEditor.h : main header file for the PROJECT_NAME application
//
#pragma once
#include "TestSet.h"
#include <afxwinappex.h>
#include <afxtooltipctrl.h>
#include <afxtooltipmanager.h>

#ifndef __AFXWIN_H__
	#error _T("include 'stdafx.h' before including this file for PCH")
#endif

#include "resource.h"		// main symbols

#define PRODUCT_NAME _T("TestEditor")

// TestEditorApp:
// See TestEditor.cpp for the implementation of this class
//
class TestEditorApp : public CWinAppEx
{
public:
	TestEditorApp();

  CString GetTestFilename()  { return m_testFilename;  }
  CString GetTestDirectory() { return m_testDirectory; }
  CString GetParamFilename() { return m_paramFilename; }
  CString GetBaseDirectory() { return m_baseDirectory; }
  CString GetBaseDirectoryClean();
  CString GetTestDirectoryClean();
  CString GetParamFilenameClean();
  HWND    GetCallingHWND()   { return (HWND)m_callingHWND; }
  int     GetCallingRow()    { return m_callingROW;        }

  void    StartStepEditor    (CString p_stepname,  bool p_global,TSValSet* p_validations,HWND p_wnd,int p_row);
  int     StartTestRunner    (CString p_stepname,  bool p_global,TSValSet* p_validations,HWND p_wnd,int p_row,bool p_wait = true);
  void    StartValidateEditor(CString p_validation,bool p_global,HWND p_wnd,int p_row);
  void    StartResultViewer  (CString p_stepname,  HWND p_wnd,int p_row);

  bool    PromoteTestStep  (CWnd* p_wnd,CString p_filename);
  bool    DemoteTestStep   (CWnd* p_wnd,CString p_filename);
  bool    PromoteValidation(CWnd* p_wnd,CString p_filename);
  bool    DemoteValidation (CWnd* p_wnd,CString p_filename);

// Overrides
public:
	virtual BOOL InitInstance() override;
	virtual int  ExitInstance() override;

// Implementation

	DECLARE_MESSAGE_MAP()
private:
  CString StandardParameters(HWND p_wnd,int p_row,bool p_global);

  CString m_testDirectory;
  CString m_testFilename;
  CString m_paramFilename;
  CString m_baseDirectory;
  int     m_returnValue { 0 };
  UINT64  m_callingHWND { 0 };
  int     m_callingROW  { 0 };

  void Usage();
  bool ParseStartParameters();
};

extern TestEditorApp theApp;

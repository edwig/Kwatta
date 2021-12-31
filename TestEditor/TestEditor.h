// TestEditor.h : main header file for the PROJECT_NAME application
//
#pragma once
#include "TestSet.h"

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

#define PRODUCT_NAME "TestEditor"

// TestEditorApp:
// See TestEditor.cpp for the implementation of this class
//
class TestEditorApp : public CWinAppEx
{
public:
	TestEditorApp();

  CString GetTestFilename()  { return m_testFilename;  };
  CString GetTestDirectory() { return m_testDirectory; };
  CString GetParamFilename() { return m_paramFilename; };
  CString GetBaseDirectory() { return m_baseDirectory; };
  CString GetBaseDirectoryClean();
  CString GetTestDirectoryClean();
  CString GetParamFilenameClean();

  void    StartStepEditor    (CString p_stepname,TSValSet* p_validations,HWND p_wnd,int p_row);
  int     StartTestRunner    (CString p_stepname,TSValSet* p_validations,HWND p_wnd,int p_row);
  void    StartValidateEditor(CString p_validation,HWND p_wnd,int p_row);
  void    StartResultViewer  (CString p_stepname,  HWND p_wnd,int p_row);

// Overrides
public:
	virtual BOOL InitInstance() override;
	virtual int  ExitInstance() override;

// Implementation

	DECLARE_MESSAGE_MAP()
private:
  CString StandardParameters(HWND p_wnd,int p_row);

  CString m_testDirectory;
  CString m_testFilename;
  CString m_paramFilename;
  CString m_baseDirectory;
  int     m_returnValue;

  void Usage();
  bool ParseStartParameters();
};

extern TestEditorApp theApp;

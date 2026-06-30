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
// This program: ResultViewer
// This File   : ResultViewer.cpp
// What it does: Shows the result of a test step.
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ResultViewer.h"
#include "ResultViewerDlg.h"
#include "ResultInternetDlg.h"
#include "ResultDatabaseDlg.h"
#include "ResultWindowsDlg.h"
#include "ExtraExtensions.h"
#include <GetExePath.h>
#include <StartProgram.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// CResultViewerApp

BEGIN_MESSAGE_MAP(ResultViewerApp,CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinAppEx::OnHelp)
END_MESSAGE_MAP()

// CResultViewerApp construction

ResultViewerApp::ResultViewerApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CResultViewerApp object

ResultViewerApp theApp;


// CResultViewerApp initialization

BOOL ResultViewerApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

  AfxInitRichEdit2();
  AfxEnableControlContainer();
  
  CWinAppEx::InitInstance();

  EnableTaskbarInteraction(FALSE);

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
  SetRegistryKey(_T("EDO\\Kwatta"));

  InitContextMenuManager();
  InitKeyboardManager();
  InitTooltipManager();

  CMFCToolTipInfo ttParams;
  ttParams.m_bVislManagerTheme = TRUE;
  theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL, RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

  // Reading our command line for the XML files
  if(!ParseStartParameters())
  {
    return FALSE;
  }
  
  ResultType type = DeduceValidateType();
  if(type == ResultType::ResultType_Command)
  {
    ResultViewerDlg dlg;
    dlg.DoModal();
  }
  else if(type == ResultType::ResultType_Internet)
  {
    ResultInternetDlg dlg;
    dlg.DoModal();
  }
  else if(type == ResultType::ResultType_Database)
  {
    ResultDatabaseDlg dlg;
    dlg.DoModal();
  }
  else if(type == ResultType::ResultType_Windows)
  {
    ResultWindowsDlg dlg;
    dlg.DoModal();
  }
	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

ResultType
ResultViewerApp::DeduceValidateType()
{
  int pos = m_resultFilename.ReverseFind('.');
  if(pos > 0)
  {
    XString extension = m_resultFilename.Mid(pos);
    if(extension.CompareNoCase(EXTENSION_RESULT_CMD) == 0)
    {
      return ResultType::ResultType_Command;
    }
    else if(extension.CompareNoCase(EXTENSION_RESULT_NET) == 0)
    {
      return ResultType::ResultType_Internet;
    }
    else if(extension.CompareNoCase(EXTENSION_RESULT_SQL) == 0)
    {
      return ResultType::ResultType_Database;
    }
    else if(extension.CompareNoCase(EXTENSION_RESULT_WIN) == 0)
    {
      return ResultType::ResultType_Windows;
    }
  }
  // DEFAULT !!
  return ResultType::ResultType_unknown;
}

// /STEP:filename.xrun
// /PARAM:param.xpar
bool
ResultViewerApp::ParseStartParameters()
{
  for (int i = 1; i < __argc; i++)
  {
    LPCTSTR lpszParam = __targv[i];

    if (lpszParam[0] == _T('-') || lpszParam[0] == '/')
    {
                if (_tcsnicmp(&lpszParam[1], _T("RES:"),  4) == 0) m_resultFilename = &lpszParam[5];
           else if (_tcsnicmp(&lpszParam[1], _T("BASE:"), 5) == 0) m_baseDirectory  = &lpszParam[6];
           else if (_tcsnicmp(&lpszParam[1], _T("TDIR:"), 5) == 0) m_testDirectory  = &lpszParam[6];
           else if (_tcsnicmp(&lpszParam[1], _T("PARAM:"),6) == 0) m_paramFilename  = &lpszParam[7];
					 else if (_tcsnicmp(&lpszParam[1], _T("HWND:"), 5) == 0) m_callingHWND    = _ttoll(&lpszParam[6]);
					 else if (_tcsnicmp(&lpszParam[1], _T("ROW:"),  4) == 0) m_callingROW     = _ttol(&lpszParam[5]);
					 else
      {
        Usage();
        return false;
      }
    }
  }
  if(m_resultFilename.IsEmpty())
  {
    StyleMessageBox(NULL, _T("Cannot start without a <stepname>.xres file"), PRODUCT_NAME, MB_OK | MB_ICONERROR);
    Usage();
    return false;
  }
  if(m_baseDirectory.IsEmpty())
  {
    StyleMessageBox(NULL,_T("Cannot start without a BASE directory"),PRODUCT_NAME,MB_OK|MB_ICONERROR);
    Usage();
    return false;
  }
  if(m_testDirectory.IsEmpty())
  {
    StyleMessageBox(NULL,_T("Cannot start without a TESTdirectory"),PRODUCT_NAME,MB_OK|MB_ICONERROR);
    Usage();
    return false;
  }
  // Check formatting of the directories
  if(m_baseDirectory.Right(1) != _T("\\"))
  {
    m_baseDirectory += _T('\\');
  }
  if(m_testDirectory.Right(1) != _T("\\"))
  {
    m_testDirectory += _T('\\');
  }
  return true;
}

void
ResultViewerApp::Usage()
{
  XString usage = _T("USAGE of this program:\n\nResultViewer.exe /BASE:<base-directory> /RES:<filename>.xres");
  StyleMessageBox(NULL, usage.GetString(), PRODUCT_NAME, MB_OK | MB_ICONEXCLAMATION);
}

XString
ResultViewerApp::GetBaseDirectoryClean()
{
  XString clean(m_baseDirectory);
  clean = clean.TrimRight('\\');
  return clean;
}

XString
ResultViewerApp::GetTestDirectoryClean()
{
  XString clean(m_testDirectory);
  clean = clean.TrimRight('\\');
  return clean;
}

XString
ResultViewerApp::StandardParameters(HWND p_wnd, int p_row,bool p_global)
{
  XString param;
  param.Format(_T("/HWND:%u /ROW:%d "),p_wnd,p_row);
  if(p_global)
  {
    param += _T(" /GLOBAL");
  }
  return param;
}

void
ResultViewerApp::StartValidateEditor(XString p_validation,bool p_global,HWND p_wnd,int p_row)
{
  // Construct program name
  XString program = GetExePath() + _T("ValidateEditor.exe");
  // Construct parameters
  XString parameters = StandardParameters(p_wnd,p_row,p_global);
  parameters.AppendFormat(_T(" /BASE:\"%s\""), GetBaseDirectoryClean().GetString());
  parameters.AppendFormat(_T(" /TDIR:\"%s\""), GetTestDirectoryClean().GetString());
  parameters.AppendFormat(_T(" /PARAM:\"%s\""),m_paramFilename.GetString());
  parameters.AppendFormat(_T(" /VALI:\"%s\""), p_validation.GetString());

  // Call program
  TRACE(_T("Start validate editor: %s %s\n"),program,parameters);
  int result = StartProgram(program,parameters,false,false,false);
}

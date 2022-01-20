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
  SetRegistryKey("EDO\\Kwatta");

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
  
  if(DeduceValidateType() == ResultType::ResultType_Command)
  {
    ResultViewerDlg dlg;
    dlg.DoModal();
  }
  else
  {
    ResultInternetDlg dlg;
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
    CString extension = m_resultFilename.Mid(pos);
    if(extension.CompareNoCase(EXTENSION_RESULT_CL) == 0)
    {
      return ResultType::ResultType_Command;
    }
    else if(extension.CompareNoCase(EXTENSION_RESULT_IN) == 0)
    {
      return ResultType::ResultType_Internet;
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
    LPCSTR lpszParam = __argv[i];

    if (lpszParam[0] == '-' || lpszParam[0] == '/')
    {
                if (_strnicmp(&lpszParam[1], "RES:",  4) == 0) m_resultFilename = &lpszParam[5];
           else if (_strnicmp(&lpszParam[1], "BASE:", 5) == 0) m_baseDirectory  = &lpszParam[6];
           else if (_strnicmp(&lpszParam[1], "TDIR:", 5) == 0) m_testDirectory  = &lpszParam[6];
           else if (_strnicmp(&lpszParam[1], "PARAM:",6) == 0) m_paramFilename  = &lpszParam[7];
					 else if (_strnicmp(&lpszParam[1], "HWND:", 5) == 0) m_callingHWND    = atoll(&lpszParam[6]);
					 else if (_strnicmp(&lpszParam[1], "ROW:",  4) == 0) m_callingROW     = atol(&lpszParam[5]);
					 else
      {
        Usage();
        return false;
      }
    }
  }
  if(m_resultFilename.IsEmpty())
  {
    StyleMessageBox(NULL, "Cannot start without a <stepname>.xres file", PRODUCT_NAME, MB_OK | MB_ICONERROR);
    Usage();
    return false;
  }
  if(m_baseDirectory.IsEmpty())
  {
    StyleMessageBox(NULL,"Cannot start without a BASE directory",PRODUCT_NAME,MB_OK|MB_ICONERROR);
    Usage();
    return false;
  }
  if(m_testDirectory.IsEmpty())
  {
    StyleMessageBox(NULL,"Cannot start without a TESTdirectory",PRODUCT_NAME,MB_OK|MB_ICONERROR);
    Usage();
    return false;
  }
  // Check formatting of the directories
  if(m_baseDirectory.Right(1) != '\\')
  {
    m_baseDirectory += '\\';
  }
  if(m_testDirectory.Right(1) != '\\')
  {
    m_testDirectory += '\\';
  }
  return true;
}

void
ResultViewerApp::Usage()
{
  CString usage = "USAGE of this program:\n\nResultViewer.exe /BASE:<base-directory> /RES:<filename>.xres";
  StyleMessageBox(NULL, usage.GetString(), PRODUCT_NAME, MB_OK | MB_ICONEXCLAMATION);
}

CString
ResultViewerApp::GetBaseDirectoryClean()
{
  CString clean(m_baseDirectory);
  clean = clean.TrimRight('\\');
  return clean;
}

CString
ResultViewerApp::GetTestDirectoryClean()
{
  CString clean(m_testDirectory);
  clean = clean.TrimRight('\\');
  return clean;
}

CString
ResultViewerApp::StandardParameters(HWND p_wnd, int p_row,bool p_global)
{
  CString param;
  param.Format("/HWND:%u /ROW:%d ",p_wnd,p_row);
  if(p_global)
  {
    param += " /GLOBAL";
  }
  return param;
}

void
ResultViewerApp::StartValidateEditor(CString p_validation,bool p_global,HWND p_wnd,int p_row)
{
  // Construct program name
  CString program = GetExePath() + "ValidateEditor.exe";
  // Construct parameters
  CString parameters = StandardParameters(p_wnd,p_row,p_global);
  parameters.AppendFormat(" /BASE:\"%s\"", GetBaseDirectoryClean().GetString());
  parameters.AppendFormat(" /TDIR:\"%s\"", GetTestDirectoryClean().GetString());
  parameters.AppendFormat(" /PARAM:\"%s\"",m_paramFilename.GetString());
  parameters.AppendFormat(" /VALI:\"%s\"", p_validation.GetString());

  // Call program
  TRACE("Start validate editor: %s %s\n",program,parameters);
  int result = StartProgram(program,parameters,false,false,false);
}

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
// This File   : ValidateEditor.cpp
// What it does: Main program for the validation of a test step
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ValidateEditor.h"
#include "ValidateEditorDlg.h"
#include "ValidateInternetDlg.h"
#include <ExtraMessages.h>
#include "ExtraExtensions.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// CValidateEditorApp

BEGIN_MESSAGE_MAP(ValidateEditorApp,CWinAppEx)
	ON_COMMAND(ID_HELP,&CWinAppEx::OnHelp)
END_MESSAGE_MAP()

// CValidateEditorApp construction

ValidateEditorApp::ValidateEditorApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

// The one and only CValidateEditorApp object

ValidateEditorApp theApp;


// CValidateEditorApp initialization

BOOL ValidateEditorApp::InitInstance()
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
  if (!ParseStartParameters())
  {
    return FALSE;
  }

  ValidateType type = DeduceValidateType();
  if(type == ValidateType::Validate_command)
  {
    ValidateEditorDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();
  }
  else if(type == ValidateType::Validate_inet)
  {
    ValidateInternetDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();
  }
  else
  {
    CString error;
    error.Format("Cannot deduce the validation type of file: %s",m_validateFilename.GetString());
    StyleMessageBox(nullptr,error,"Kwatta",MB_OK|MB_ICONERROR);
  }
	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

int
ValidateEditorApp::ExitInstance()
{
  if(m_callingHWND && m_callingROW)
  {
    ::PostMessage((HWND)m_callingHWND,WM_READYVALI,(WPARAM)m_callingROW,0);
  }
  return CWinAppEx::ExitInstance();
}

ValidateType
ValidateEditorApp::DeduceValidateType()
{
  int pos = m_validateFilename.ReverseFind('.');
  if(pos > 0)
  {
    CString extension = m_validateFilename.Mid(pos);
    if(extension.CompareNoCase(EXTENSION_VALIDATE_CL) == 0)
    {
      return ValidateType::Validate_command;
    }
    else
    {
      return ValidateType::Validate_inet;
    }
  }
  // DEFAULT !!
  return ValidateType::Validate_unknown;
}

// /VALI:filename.xval
// /PARAM:param.xpar
bool
ValidateEditorApp::ParseStartParameters()
{
  for (int i = 1; i < __argc; i++)
  {
    LPCSTR lpszParam = __argv[i];

    if (lpszParam[0] == '-' || lpszParam[0] == '/')
    {
           if (_strnicmp(&lpszParam[1],"VALI:", 5) == 0) m_validateFilename = &lpszParam[6];
      else if (_strnicmp(&lpszParam[1],"BASE:", 5) == 0) m_baseDirectory    = &lpszParam[6];
      else if (_strnicmp(&lpszParam[1],"TDIR:", 5) == 0) m_testDirectory    = &lpszParam[6];
      else if (_strnicmp(&lpszParam[1],"PARAM:",6) == 0) m_paramFilename    = &lpszParam[7];
      else if (_strnicmp(&lpszParam[1],"HWND:", 5) == 0) m_callingHWND      = atoll(&lpszParam[6]);
      else if (_strnicmp(&lpszParam[1],"ROW:",  4) == 0) m_callingROW       = atol(&lpszParam[5]);
      else
      {
        Usage();
        return false;
      }
    }
  }
  if(m_validateFilename.IsEmpty())
  {
    StyleMessageBox(NULL,"Cannot start without a <validate>.xval file",PRODUCT_NAME,MB_OK|MB_ICONERROR);
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
    StyleMessageBox(NULL,"Cannot start without a TEST directory",PRODUCT_NAME,MB_OK|MB_ICONERROR);
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
ValidateEditorApp::Usage()
{
  CString usage = "USAGE of this program:\n\nValidateEditor.exe /BASE:<base-directory> /TDIR:<test-directory> /VALI:<filename>.xval [/PARAM:<filename>.xpar]";
  StyleMessageBox(NULL, usage.GetString(), PRODUCT_NAME, MB_OK | MB_ICONEXCLAMATION);
}

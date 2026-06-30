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
#include "ValidateDatabaseDlg.h"
#include "ValidateWindowsDlg.h"
#include <ExtraMessages.h>
#include "ExtraExtensions.h"
#include <GetLastErrorAsString.h>
#include <io.h>

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
  SetRegistryKey(_T("EDO\\Kwatta"));

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
  else if(type == ValidateType::Validate_sql)
  {
    ValidateDatabaseDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();
  }
  else if(type == ValidateType::Validate_win)
  {
    ValidateWindowsDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();
  }
  else
  {
    XString error;
    error.Format(_T("Cannot deduce the validation type of file: %s"),m_validateFilename.GetString());
    StyleMessageBox(nullptr,error,_T("Kwatta"),MB_OK|MB_ICONERROR);
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

XString
ValidateEditorApp::GetEffectiveValidation()
{
  XString filename(m_baseDirectory);
  filename += m_global ? XString(_T("Validations\\")) : m_testDirectory;
  filename += m_validateFilename;

  return filename;
}

void
ValidateEditorApp::PromoteValidation(CWnd* p_wnd)
{
  XString valiDirectory = m_baseDirectory + _T("Validations\\");
  XString globalVali    = valiDirectory + m_validateFilename;

  if(_taccess(globalVali, 0) == 0)
  {
    if(StyleMessageBox(p_wnd,_T("This validation already exists as a global validation. Do you wish to overwrite it?"),PRODUCT_NAME,MB_YESNO|MB_DEFBUTTON2|MB_ICONQUESTION) == IDNO)
    {
      return;
    }
  }

  CWaitCursor sigh;
  XString testfile = m_baseDirectory + m_testDirectory + m_validateFilename;
  if (CopyFile(testfile,globalVali,FALSE) == FALSE)
  {
    XString error = GetLastErrorAsString(GetLastError());
    StyleMessageBox(p_wnd,_T("Could not create a global validation: ") + error,PRODUCT_NAME,MB_OK|MB_ICONERROR);
  }
}

ValidateType
ValidateEditorApp::DeduceValidateType()
{
  int pos = m_validateFilename.ReverseFind('.');
  if(pos > 0)
  {
    XString extension = m_validateFilename.Mid(pos);
    if(extension.CompareNoCase(EXTENSION_VALIDATE_CMD) == 0)
    {
      return ValidateType::Validate_command;
    }
    else if(extension.CompareNoCase(EXTENSION_VALIDATE_NET) == 0)
    {
      return ValidateType::Validate_inet;
    }
    else if(extension.CompareNoCase(EXTENSION_VALIDATE_SQL) == 0)
    {
      return ValidateType::Validate_sql;
    }
    else if(extension.CompareNoCase(EXTENSION_VALIDATE_WIN) == 0)
    {
      return ValidateType::Validate_win;
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
    LPCTSTR lpszParam = __targv[i];

    if (lpszParam[0] == _T('-') || lpszParam[0] == '/')
    {
           if (_tcsncicmp(&lpszParam[1],_T("VALI:"), 5) == 0) m_validateFilename = &lpszParam[6];
      else if (_tcsncicmp(&lpszParam[1],_T("BASE:"), 5) == 0) m_baseDirectory    = &lpszParam[6];
      else if (_tcsncicmp(&lpszParam[1],_T("TDIR:"), 5) == 0) m_testDirectory    = &lpszParam[6];
      else if (_tcsncicmp(&lpszParam[1],_T("PARAM:"),6) == 0) m_paramFilename    = &lpszParam[7];
      else if (_tcsncicmp(&lpszParam[1],_T("HWND:"), 5) == 0) m_callingHWND      = _ttoll(&lpszParam[6]);
      else if (_tcsncicmp(&lpszParam[1],_T("ROW:"),  4) == 0) m_callingROW       = _ttol(&lpszParam[5]);
      else if (_tcsncicmp(&lpszParam[1],_T("GLOBAL"),6) == 0) m_global           = true;
      else
      {
        Usage();
        return false;
      }
    }
  }
  if(m_validateFilename.IsEmpty())
  {
    StyleMessageBox(NULL,_T("Cannot start without a <validate>.xval file"),PRODUCT_NAME,MB_OK|MB_ICONERROR);
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
    StyleMessageBox(NULL,_T("Cannot start without a TEST directory"),PRODUCT_NAME,MB_OK|MB_ICONERROR);
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
ValidateEditorApp::Usage()
{
  XString usage = _T("USAGE of this program:\n")
                  _T("\n")
                  _T("ValidateEditor.exe /BASE:<base-directory> /TDIR:<test-directory> /VALI:<filename>.xval [/PARAM:<filename>.xpar]");
  StyleMessageBox(NULL, usage.GetString(), PRODUCT_NAME, MB_OK | MB_ICONEXCLAMATION);
}

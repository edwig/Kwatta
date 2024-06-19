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
// This program: TestEditor
// This File   : TestEditor.cpp
// What it does: Defining a new test step in the set
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TestEditor.h"
#include "TestEditorDlg.h"
#include <ExtraExtensions.h>
#include <GetExePath.h>
#include <StartProgram.h>
#include <GetLastErrorAsString.h>
#include <io.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// CTestEditorApp

BEGIN_MESSAGE_MAP(TestEditorApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinAppEx::OnHelp)
END_MESSAGE_MAP()

// CTestEditorApp construction

TestEditorApp::TestEditorApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

// The one and only CTestEditorApp object

TestEditorApp theApp;

// CTestEditorApp initialization

BOOL TestEditorApp::InitInstance()
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
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

  if(!ParseStartParameters())
  {
    return FALSE;
  }

	TestEditorDlg dlg;
  m_pMainWnd = &dlg;
	m_returnValue = (int) dlg.DoModal();

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

int
TestEditorApp::ExitInstance()
{
  CWinAppEx::ExitInstance();

  return m_returnValue;
}

// /BASE:Base-directory
// /TEST:filename.xtest
// /PARAM:param.xpar
bool
TestEditorApp::ParseStartParameters()
{
  CString commandline(m_lpCmdLine);

  for(int index = 1;index < __argc;++index)
  {
    LPCTSTR lpszParam = __targv[index];

    if (lpszParam[0] == _T('-') || lpszParam[0] == '/')
    {
           if(_tcsnicmp(&lpszParam[1], _T("TEST:"), 5) == 0) m_testFilename  = &lpszParam[6];
      else if(_tcsnicmp(&lpszParam[1], _T("BASE:"), 5) == 0) m_baseDirectory = &lpszParam[6];
      else if(_tcsnicmp(&lpszParam[1], _T("TDIR:"), 5) == 0) m_testDirectory = &lpszParam[6];
      else if(_tcsnicmp(&lpszParam[1], _T("PARAM:"),6) == 0) m_paramFilename = &lpszParam[7];
      else if(_tcsnicmp(&lpszParam[1], _T("HWND:"), 5) == 0) m_callingHWND   = _ttoll(&lpszParam[6]);
      else if(_tcsnicmp(&lpszParam[1], _T("ROW:"),  4) == 0) m_callingROW    = _ttol (&lpszParam[5]);
      else
      {
        Usage();
        return false;
      }
    }
  }
  if(m_testFilename.IsEmpty())
  {
    StyleMessageBox(NULL,_T("Cannot start without a <test>.xset file"),PRODUCT_NAME,MB_OK|MB_ICONERROR);
    Usage();
    return false;
  }
  if(m_baseDirectory.IsEmpty())
  {
    StyleMessageBox(NULL,_T("Cannot start without a BASE directory"),PRODUCT_NAME,MB_OK|MB_ICONERROR);
    Usage();
    return false;
  }
  // Check formatting of the base directory
  if(m_baseDirectory.Right(1) != '\\')
  {
    m_baseDirectory += _T('\\');
  }
  // Check formatting of the test directory
  if(m_testDirectory.Right(1) != '\\')
  {
    m_testDirectory += _T('\\');
  }
  // Calculate the test filename
  m_testFilename  = m_baseDirectory + m_testDirectory + m_testFilename;
  // Calculate the parameters filename
  m_paramFilename = m_baseDirectory + m_testDirectory + m_paramFilename;

  return true;
}

void
TestEditorApp::Usage()
{
  CString usage = _T("USAGE of this program:\n\nStepEditor.exe /BASE:<base-directory> /TEST:<filename>.xset [/PARAM:<filename>.xpar]");
  StyleMessageBox(NULL, usage.GetString(),PRODUCT_NAME,MB_OK|MB_ICONEXCLAMATION);
}

bool
TestEditorApp::PromoteTestStep(CWnd* p_wnd,CString p_filename)
{
  CString stepsDirectory = m_baseDirectory + _T("Steps\\");
  CString globalStep     = stepsDirectory  + p_filename;

  if(_taccess(globalStep, 0) == 0)
  {
    INT_PTR result = StyleMessageBox(p_wnd,_T("This step exists as a global step. Do you wish to use it?"),PRODUCT_NAME,MB_YESNO|MB_DEFBUTTON1|MB_ICONQUESTION);
    return result == IDYES ? true : false;
  }

  StyleMessageBox(p_wnd,_T("This teststep does *NOT* exist as a global step. Make it global first!"),PRODUCT_NAME,MB_OK|MB_ICONWARNING);
  return false;
}

bool
TestEditorApp::DemoteTestStep(CWnd* p_wnd,CString p_filename)
{
  CString stepsDirectory = m_baseDirectory + _T("Steps\\");
  CString globalStep     = stepsDirectory  + p_filename;
  CString testfile       = m_baseDirectory + m_testDirectory + p_filename;

  if(_taccess(testfile, 0) == -1)
  {
    if(StyleMessageBox(p_wnd,_T("This teststep does *NOT* exists as a local step. Do you wish to create it?"),PRODUCT_NAME,MB_YESNO|MB_DEFBUTTON2|MB_ICONQUESTION) == IDNO)
    {
      return false;
    }
    CWaitCursor sigh;
    if(CopyFile(globalStep,testfile,FALSE) == FALSE)
    {
      CString error = GetLastErrorAsString(GetLastError());
      StyleMessageBox(p_wnd,_T("Could not create a local step: ") + error,PRODUCT_NAME,MB_OK|MB_ICONERROR);
      return false;
    }
  }
  return true;
}

bool
TestEditorApp::PromoteValidation(CWnd* p_wnd, CString p_filename)
{
  CString valiDirectory = m_baseDirectory + _T("Validations\\");
  CString globalVali    = valiDirectory   + p_filename;

  if(_taccess(globalVali, 0) == 0)
  {
    INT_PTR result = StyleMessageBox(p_wnd,_T("This validation exists as a global validation. Do you wish to use it?"),PRODUCT_NAME,MB_YESNO|MB_DEFBUTTON1|MB_ICONQUESTION);
    return result == IDYES ? true : false;
  }

  StyleMessageBox(p_wnd,_T("This validation does *NOT* exist as a global validation. Make it global first!"),PRODUCT_NAME,MB_OK|MB_ICONWARNING);
  return false;
  return true;
}

bool
TestEditorApp::DemoteValidation(CWnd* p_wnd, CString p_filename)
{
  CString valiDirectory = m_baseDirectory + _T("Validations\\");
  CString globalVali    = valiDirectory  + p_filename;
  CString valifile      = m_baseDirectory + m_testDirectory + p_filename;

  if(_taccess(valifile, 0) == -1)
  {
    if(StyleMessageBox(p_wnd,_T("This validation does *NOT* exists as a local validation. Do you wish to create it?"),PRODUCT_NAME,MB_YESNO|MB_DEFBUTTON2|MB_ICONQUESTION) == IDNO)
    {
      return false;
    }
    CWaitCursor sigh;
    if(CopyFile(globalVali,valifile,FALSE) == FALSE)
    {
      CString error = GetLastErrorAsString(GetLastError());
      StyleMessageBox(p_wnd,_T("Could not create a local validation: ") + error,PRODUCT_NAME,MB_OK|MB_ICONERROR);
      return false;
    }
  }
  return true;
}

CString 
TestEditorApp::GetBaseDirectoryClean()
{
  CString clean(m_baseDirectory);
  clean = clean.TrimRight('\\');
  return clean;
}

CString 
TestEditorApp::GetTestDirectoryClean()
{
  CString clean(m_testFilename);
  int pos = clean.ReverseFind('\\');
  if(pos > 0)
  {
    clean = clean.Left(pos);
    pos = clean.ReverseFind('\\');
    if(pos > 0)
    {
      clean = clean.Mid(pos + 1);
    }
  }
  return clean;
}

CString 
TestEditorApp::GetParamFilenameClean()
{
  CString clean(m_paramFilename);
  int pos = clean.ReverseFind('\\');
  if(pos > 0)
  {
    clean = clean.Mid(pos + 1);
  }
  return clean;
}

CString
TestEditorApp::StandardParameters(HWND p_wnd,int p_row,bool p_global)
{
  CString param;
  param.Format(_T("/HWND:%I64d /ROW:%d"),p_wnd,p_row);
  if(p_global)
  {
    param += _T(" /GLOBAL");
  }
  return param;
}

void
TestEditorApp::StartStepEditor(CString p_stepname,bool p_global,TSValSet* p_validations,HWND p_wnd,int p_row)
{
  // Construct program name
  CString program = GetExePath() + _T("StepEditor.exe");
  // Construct parameters
  CString parameters = StandardParameters(p_wnd,p_row,p_global);
  parameters.AppendFormat(_T(" /BASE:\"%s\""), GetBaseDirectoryClean().GetString());
  parameters.AppendFormat(_T(" /TDIR:\"%s\""), GetTestDirectoryClean().GetString());
  parameters.AppendFormat(_T(" /PARAM:\"%s\""),GetParamFilenameClean().GetString());
  parameters.Replace(EXTENSION_SET, EXTENSION_PARAMETERS);

  WinFile file(m_testFilename);
  CString path = file.GetFilenamePartDirectory();
  parameters.AppendFormat(_T(" /STEP:\"%s\""),p_stepname.GetString());

  if(p_validations)
  {
    for(auto& val : *p_validations)
    {
      parameters.AppendFormat(_T(" /%s:\"%s\""),val.m_global ? _T("GLVAL") : _T("VALI"),val.m_filename.GetString());
    }
  }

  // Call program
  TRACE(_T("Start step editor: %s %s\n"),program,parameters);
  int result = StartProgram(program,parameters,false,false,false);
}

void
TestEditorApp::StartValidateEditor(CString p_validation,bool p_global,HWND p_wnd,int p_row)
{
  // Construct program name
  CString program = GetExePath() + _T("ValidateEditor.exe");
  // Construct parameters
  CString parameters = StandardParameters(p_wnd,p_row,p_global);
  parameters.AppendFormat(_T(" /BASE:\"%s\""), GetBaseDirectoryClean().GetString());
  parameters.AppendFormat(_T(" /TDIR:\"%s\""), GetTestDirectoryClean().GetString());
  parameters.AppendFormat(_T(" /PARAM:\"%s\""),GetTestFilename().GetString());
  parameters.Replace(EXTENSION_SET, EXTENSION_PARAMETERS);

  parameters.AppendFormat(_T(" /VALI:\"%s\""),p_validation.GetString());

  // Call program
  TRACE(_T("Start validate editor: %s %s\n"),program,parameters);
  int result = StartProgram(program, parameters,false,false,false);
}

void
TestEditorApp::StartResultViewer(CString p_stepname,HWND p_wnd,int p_row)
{
  // Construct program name
  CString program = GetExePath() + _T("ResultViewer.exe");
  // Construct parameters
  CString parameters = StandardParameters(p_wnd,p_row,false);
  parameters.AppendFormat(_T(" /BASE:\"%s\""), GetBaseDirectoryClean().GetString());
  parameters.AppendFormat(_T(" /TDIR:\"%s\""), GetTestDirectoryClean().GetString());
  parameters.AppendFormat(_T(" /PARAM:\"%s\""),GetTestFilename().GetString());
  parameters.Replace(EXTENSION_SET,EXTENSION_PARAMETERS);

  p_stepname.Replace(EXTENSION_TESTSTEP_CMD,EXTENSION_RESULT_CMD);
  p_stepname.Replace(EXTENSION_TESTSTEP_NET,EXTENSION_RESULT_NET);
  p_stepname.Replace(EXTENSION_TESTSTEP_SQL,EXTENSION_RESULT_SQL);
  p_stepname.Replace(EXTENSION_TESTSTEP_WIN,EXTENSION_RESULT_WIN);
  parameters.AppendFormat(_T(" /RES:\"%s\""),p_stepname.GetString());

  // Call program
  TRACE(_T("Start result viewer: %s %s\n"),program,parameters);
  int result = StartProgram(program,parameters,false,false,false);
}

int
TestEditorApp::StartTestRunner(CString p_stepname,bool p_global,TSValSet* p_validations,HWND p_wnd,int p_row,bool p_wait /*=true*/)
{
  // Construct program name
  CString program = GetExePath() + _T("TestRunner.exe");
  // Construct parameters
  CString parameters = StandardParameters(p_wnd,p_row,p_global);
  parameters.AppendFormat(_T(" /BASE:\"%s\""), GetBaseDirectoryClean().GetString());
  parameters.AppendFormat(_T(" /TDIR:\"%s\""), GetTestDirectoryClean().GetString());
  parameters.AppendFormat(_T(" /PARAM:\"%s\""),GetParamFilenameClean().GetString());
  parameters.AppendFormat(_T(" /STEP:\"%s\""), p_stepname.GetString());
  // Eventually loadtesting
  if(!p_wait)
  {
    parameters += _T(" /LOAD");
  }

  if(p_validations)
  {
    for(auto& val : *p_validations)
    {
      parameters.AppendFormat(_T(" /%s:\"%s\""),val.m_global ? _T("GLVAL")  : _T("VALI"), val.m_filename.GetString());
    }
  }

  // Call program and possibly WAIT FOR EXIT!
  TRACE(_T("Start test runner: %s %s\n"),program,parameters);
  return StartProgram(program,parameters,false,false,p_wait);
}

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
#include <GetExePath.h>
#include <EnsureFile.h>
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
	SetRegistryKey("EDO\\Kwatta");

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
    LPCSTR lpszParam = __argv[index];

    if (lpszParam[0] == '-' || lpszParam[0] == '/')
    {
           if(_strnicmp(&lpszParam[1], "TEST:", 5) == 0) m_testFilename  = &lpszParam[6];
      else if(_strnicmp(&lpszParam[1], "BASE:", 5) == 0) m_baseDirectory = &lpszParam[6];
      else if(_strnicmp(&lpszParam[1], "TDIR:", 5) == 0) m_testDirectory = &lpszParam[6];
      else if(_strnicmp(&lpszParam[1], "PARAM:",6) == 0) m_paramFilename = &lpszParam[7];
      else if(_strnicmp(&lpszParam[1], "HWND:", 5) == 0) m_callingHWND   = atoll(&lpszParam[6]);
      else if(_strnicmp(&lpszParam[1], "ROW:",  4) == 0) m_callingROW    = atol (&lpszParam[5]);
      else
      {
        Usage();
        return false;
      }
    }
  }
  if(m_testFilename.IsEmpty())
  {
    StyleMessageBox(NULL,"Cannot start without a <test>.xset file",PRODUCT_NAME,MB_OK|MB_ICONERROR);
    Usage();
    return false;
  }
  if(m_baseDirectory.IsEmpty())
  {
    StyleMessageBox(NULL,"Cannot start without a BASE directory",PRODUCT_NAME,MB_OK|MB_ICONERROR);
    Usage();
    return false;
  }
  // Check formatting of the base directory
  if(m_baseDirectory.Right(1) != '\\')
  {
    m_baseDirectory += '\\';
  }
  // Check formatting of the test directory
  if(m_testDirectory.Right(1) != '\\')
  {
    m_testDirectory += '\\';
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
  CString usage = "USAGE of this program:\n\nStepEditor.exe /BASE:<base-directory> /TEST:<filename>.xset [/PARAM:<filename>.xpar]";
  StyleMessageBox(NULL, usage.GetString(),PRODUCT_NAME,MB_OK|MB_ICONEXCLAMATION);
}

bool
TestEditorApp::PromoteTestStep(CWnd* p_wnd,CString p_filename)
{
  CString stepsDirectory = m_baseDirectory + "Steps\\";
  CString globalStep     = stepsDirectory  + p_filename;

  if(_access(globalStep, 0) == 0)
  {
    INT_PTR result = StyleMessageBox(p_wnd,"This step exists as a global step. Do you wish to use it?",PRODUCT_NAME,MB_YESNO|MB_DEFBUTTON1|MB_ICONQUESTION);
    return result == IDYES ? true : false;
  }

  StyleMessageBox(p_wnd,"This teststep does *NOT* exist as a global step. Make it global first!",PRODUCT_NAME,MB_OK|MB_ICONWARNING);
  return false;
}

bool
TestEditorApp::DemoteTestStep(CWnd* p_wnd,CString p_filename)
{
  CString stepsDirectory = m_baseDirectory + "Steps\\";
  CString globalStep     = stepsDirectory  + p_filename;
  CString testfile       = m_baseDirectory + m_testDirectory + p_filename;

  if(_access(testfile, 0) == -1)
  {
    if(StyleMessageBox(p_wnd,"This teststep does *NOT* exists as a local step. Do you wish to create it?",PRODUCT_NAME,MB_YESNO|MB_DEFBUTTON2|MB_ICONQUESTION) == IDNO)
    {
      return false;
    }
    CWaitCursor sigh;
    if(CopyFile(globalStep,testfile,FALSE) == FALSE)
    {
      CString error = GetLastErrorAsString(GetLastError());
      StyleMessageBox(p_wnd,"Could not create a local step: " + error,PRODUCT_NAME,MB_OK|MB_ICONERROR);
      return false;
    }
  }
  return true;
}

bool
TestEditorApp::PromoteValidation(CWnd* p_wnd, CString p_filename)
{
  CString valiDirectory = m_baseDirectory + "Validations\\";
  CString globalVali    = valiDirectory   + p_filename;

  if(_access(globalVali, 0) == 0)
  {
    INT_PTR result = StyleMessageBox(p_wnd,"This validation exists as a global validation. Do you wish to use it?",PRODUCT_NAME,MB_YESNO|MB_DEFBUTTON1|MB_ICONQUESTION);
    return result == IDYES ? true : false;
  }

  StyleMessageBox(p_wnd,"This validation does *NOT* exist as a global validation. Make it global first!",PRODUCT_NAME,MB_OK|MB_ICONWARNING);
  return false;
  return true;
}

bool
TestEditorApp::DemoteValidation(CWnd* p_wnd, CString p_filename)
{
  CString valiDirectory = m_baseDirectory + "Validations\\";
  CString globalVali    = valiDirectory  + p_filename;
  CString valifile      = m_baseDirectory + m_testDirectory + p_filename;

  if(_access(valifile, 0) == -1)
  {
    if(StyleMessageBox(p_wnd,"This validation does *NOT* exists as a local validation. Do you wish to create it?",PRODUCT_NAME,MB_YESNO|MB_DEFBUTTON2|MB_ICONQUESTION) == IDNO)
    {
      return false;
    }
    CWaitCursor sigh;
    if(CopyFile(globalVali,valifile,FALSE) == FALSE)
    {
      CString error = GetLastErrorAsString(GetLastError());
      StyleMessageBox(p_wnd,"Could not create a local validation: " + error,PRODUCT_NAME,MB_OK|MB_ICONERROR);
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
  param.Format("/HWND:%I64d /ROW:%d",p_wnd,p_row);
  if(p_global)
  {
    param += " /GLOBAL";
  }
  return param;
}

void
TestEditorApp::StartStepEditor(CString p_stepname,bool p_global,TSValSet* p_validations,HWND p_wnd,int p_row)
{
  // Construct program name
  CString program = GetExePath() + "StepEditor.exe";
  // Construct parameters
  CString parameters = StandardParameters(p_wnd,p_row,p_global);
  parameters.AppendFormat(" /BASE:\"%s\"", GetBaseDirectoryClean().GetString());
  parameters.AppendFormat(" /TDIR:\"%s\"", GetTestDirectoryClean().GetString());
  parameters.AppendFormat(" /PARAM:\"%s\"",GetParamFilenameClean().GetString());
  parameters.Replace(".xset",".xpar");

  EnsureFile ensure;
  CString path = ensure.DirectoryPart(m_testFilename);
  parameters.AppendFormat(" /STEP:\"%s\"",p_stepname.GetString());

  if(p_validations)
  {
    for(auto& val : *p_validations)
    {
      parameters.AppendFormat(" /VALI:\"%s\"",val.m_filename.GetString());
    }
  }

  // Call program
  TRACE("Start step editor: %s %s\n",program,parameters);
  int result = StartProgram(program,parameters,false,false,false);
}

void
TestEditorApp::StartValidateEditor(CString p_validation,bool p_global,HWND p_wnd,int p_row)
{
  // Construct program name
  CString program = GetExePath() + "ValidateEditor.exe";
  // Construct parameters
  CString parameters = StandardParameters(p_wnd,p_row,p_global);
  parameters.AppendFormat(" /BASE:\"%s\"", GetBaseDirectoryClean().GetString());
  parameters.AppendFormat(" /TDIR:\"%s\"", GetTestDirectoryClean().GetString());
  parameters.AppendFormat(" /PARAM:\"%s\"",GetTestFilename().GetString());
  parameters.Replace(".xset",".xpar");

  parameters.AppendFormat(" /VALI:\"%s\"",p_validation.GetString());

  // Call program
  TRACE("Start validate editor: %s %s\n",program,parameters);
  int result = StartProgram(program, parameters,false,false,false);
}

void
TestEditorApp::StartResultViewer(CString p_stepname,HWND p_wnd,int p_row)
{
  // Construct program name
  CString program = GetExePath() + "ResultViewer.exe";
  // Construct parameters
  CString parameters = StandardParameters(p_wnd,p_row,false);
  parameters.AppendFormat(" /BASE:\"%s\"", GetBaseDirectoryClean().GetString());
  parameters.AppendFormat(" /TDIR:\"%s\"", GetTestDirectoryClean().GetString());
  parameters.AppendFormat(" /PARAM:\"%s\"",GetTestFilename().GetString());
  parameters.Replace(".xset",".xpar");

  p_stepname.Replace(".xrun",".xres");
  p_stepname.Replace(".irun",".ires");
  parameters.AppendFormat(" /RES:\"%s\"",p_stepname.GetString());

  // Call program
  TRACE("Start result viewer: %s %s\n",program,parameters);
  int result = StartProgram(program,parameters,false,false,false);
}

int
TestEditorApp::StartTestRunner(CString p_stepname,bool p_global,TSValSet* p_validations,HWND p_wnd,int p_row)
{
  // Construct program name
  CString program = GetExePath() + "TestRunner.exe";
  // Construct parameters
  CString parameters = StandardParameters(p_wnd,p_row,p_global);
  parameters.AppendFormat(" /BASE:\"%s\"", GetBaseDirectoryClean().GetString());
  parameters.AppendFormat(" /TDIR:\"%s\"", GetTestDirectoryClean().GetString());
  parameters.AppendFormat(" /PARAM:\"%s\"",GetParamFilenameClean().GetString());
  parameters.AppendFormat(" /STEP:\"%s\"", p_stepname.GetString());

  if(p_validations)
  {
    for(auto& val : *p_validations)
    {
      parameters.AppendFormat(" /%s:\"%s\"",val.m_global ? "GLVAL"  : "VALI", val.m_filename.GetString());
    }
  }

  // Call program and WAIT FOR EXIT!
  TRACE("Start test runner: %s %s\n",program,parameters);
  return StartProgram(program,parameters,false,false,true);
}

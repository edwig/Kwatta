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
// This program: TestRunner
// This File   : TestRunnerDlg.cpp
// What it does: Showing the result of the test on the screen in the lower right corner
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TestRunner.h"
#include "TestRunnerDlg.h"
#include "TestMessages.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// TestRunnerDlg dialog

TestRunnerDlg::TestRunnerDlg(CWnd* pParent /*=nullptr*/)
              :StyleDialog(IDD_TESTRUNNER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void 
TestRunnerDlg::DoDataExchange(CDataExchange* pDX)
{
	StyleDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_TEST,    m_editTest,m_test);
  DDX_Control(pDX, IDC_STEP,    m_editStep,m_step);
  DDX_Control(pDX, IDC_TOTAL,   m_total);
  DDX_Control(pDX, IDC_PROGRESS,m_progress);
}

BEGIN_MESSAGE_MAP(TestRunnerDlg, StyleDialog)
  ON_WM_SYSCOMMAND()
  ON_WM_PAINT()
  ON_WM_QUERYDRAGICON()
  ON_MESSAGE(WM_TEST_TESTNAME, OnTestName)
  ON_MESSAGE(WM_TEST_STEPNAME, OnStepName)
  ON_MESSAGE(WM_TEST_TOTAL,    OnSetTotal)
  ON_MESSAGE(WM_TEST_PROGRESS, OnProgress)
  ON_MESSAGE(WM_TEST_UPDATE,   OnUpdate)
  ON_MESSAGE(WM_TEST_ENDING,   OnEndTesting)
END_MESSAGE_MAP()

// CTestRunnerDlg message handlers

BOOL 
TestRunnerDlg::OnInitDialog()
{
	StyleDialog::OnInitDialog();
  SetWindowText(_T("Kwatta testing..."));
  SetAboutBoxAndIcon(IDM_ABOUTBOX,IDS_ABOUTBOX);

  // extra initialization here
  PlaceInRightBottomCorner();

  m_total   .SetRange(0,100);
  m_progress.SetRange(0,100);

  theApp.StartRunner();

  SetWindowPos(&CWnd::wndTopMost,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE|SWP_SHOWWINDOW);
  
	return TRUE;  // return TRUE  unless you set the focus to a control
}

int 
TestRunnerDlg::GetTaskBarHeight()
{
  RECT rect;
  HWND taskBar = ::FindWindow(_T("Shell_traywnd"), NULL);
  if (taskBar && ::GetWindowRect(taskBar, &rect)) 
  {
    return rect.bottom - rect.top;
  }
  return 0;
}

void
TestRunnerDlg::PlaceInRightBottomCorner()
{
  // Get size of the desktop
  int X = GetSystemMetrics(SM_CXSCREEN);
  int Y = GetSystemMetrics(SM_CYSCREEN) - GetTaskBarHeight();

  // Get size of the current window
  CRect rect;
  GetWindowRect(&rect);

  int x = rect.right;
  int y = rect.bottom;
  int bar = 7;  // Standard MS-Windows task border

  rect.left   = X - x + bar;
  rect.top    = Y - y + bar;
  rect.right  = X + bar;
  rect.bottom = Y + bar;

  MoveWindow(&rect);
  SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW | SWP_NOCOPYBITS);
}

void
TestRunnerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	StyleDialog::OnSysCommand(nID, lParam);
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void 
TestRunnerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width()  - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		StyleDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR 
TestRunnerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// INTERFACE FUNCTIONS

void
TestRunnerDlg::SetTest(XString p_test)
{
  m_test = p_test;
  UpdateData(FALSE);
  PumpMessages();
}

void
TestRunnerDlg::SetStep(XString p_step)
{
  m_step = p_step;
  UpdateData(FALSE);
  PumpMessages();
}

void 
TestRunnerDlg::SetTotal(int p_percent)
{
  m_total.SetPos(p_percent);
  UpdateData(FALSE);
  PumpMessages();
}

void 
TestRunnerDlg::SetProgress(int p_percent)
{
  m_progress.SetPos(p_percent);
  UpdateData(FALSE);
  PumpMessages();
}

LRESULT 
TestRunnerDlg::OnTestName(WPARAM wParam,LPARAM lParam)
{
  XString test = XString((TCHAR*)wParam);
  SetTest(test);
  return 0;
}

LRESULT 
TestRunnerDlg::OnStepName(WPARAM wParam,LPARAM lParam)
{
  XString step = XString((TCHAR*)wParam);
  SetStep(step);
  return 0;
}

LRESULT 
TestRunnerDlg::OnSetTotal(WPARAM wParam,LPARAM lParam)
{
  SetTotal((int)wParam);
  return 0;
}

LRESULT 
TestRunnerDlg::OnProgress(WPARAM wParam,LPARAM lParam)
{
  SetProgress((int)wParam);
  return 0;
}

LRESULT
TestRunnerDlg::OnUpdate(WPARAM wParam,LPARAM lParam)
{
  UpdateData(FALSE);
  PumpMessages();
  return 0;
}

LRESULT
TestRunnerDlg::OnEndTesting(WPARAM wParam,LPARAM lParam)
{
  // Safe for ExitInstance
  int endStatus = (int)wParam;
  theApp.EndApplication(endStatus);

  if(theApp.GetLastTest())
  {
    PumpMessages();
    OnOK();
  }
  return 0;
}

void
TestRunnerDlg::PumpMessages()
{
  MSG msg;
  UINT ticks = GetTickCount();
  while(GetTickCount() - ticks < 100 && (PeekMessage(&msg,NULL,WM_MOVE,WM_USER,PM_REMOVE)))
  {
    try
    {
      ::TranslateMessage(&msg);
      ::DispatchMessage(&msg);
    }
    catch (...)
    {
      // Wat nu
    }
  }
}

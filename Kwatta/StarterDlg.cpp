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
// This program: Kwatta
// This File   : StarterDlg.cpp
// What it does: Choose a test suite to work on, or create a new one
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "afxdialogex.h"
#include "StarterDlg.h"
#include "NewSuiteDlg.h"
#include "resource.h"
#include <RegistryManager.h>
#include <FileDialog.h>
#include <filesystem>

using namespace ThemeColor;

// StarterDlg dialog

IMPLEMENT_DYNAMIC(StarterDlg, StyleDialog)

StarterDlg::StarterDlg(CWnd* p_parent /*=nullptr*/)
	         :StyleDialog(IDD_STARTER,p_parent)
{
}

StarterDlg::~StarterDlg()
{
}

void StarterDlg::DoDataExchange(CDataExchange* pDX)
{
	StyleDialog::DoDataExchange(pDX);
  DDX_Control(pDX,IDC_TITLE,    m_editTitle,m_title);
  DDX_Control(pDX,IDC_SUITE1,   m_buttonSuite1);
  DDX_Control(pDX,IDC_SUITE2,   m_buttonSuite2);
  DDX_Control(pDX,IDC_SUITE3,   m_buttonSuite3);
  DDX_Control(pDX,IDC_SUITE4,   m_buttonSuite4);
  DDX_Control(pDX,IDC_SUITE5,   m_buttonSuite5);
  DDX_Control(pDX,IDC_SEARCH,   m_buttonSearch);
  DDX_Control(pDX,IDC_NEW,      m_buttonNewSuite);
  DDX_Control(pDX,IDCANCEL,     m_buttonCancel);
  DDX_Control(pDX,IDC_STOP,     m_buttonStop);
}

BEGIN_MESSAGE_MAP(StarterDlg, StyleDialog)
  ON_WM_SYSCOMMAND()
  ON_BN_CLICKED(IDC_SEARCH,   &StarterDlg::OnBnClickedSearch)
  ON_BN_CLICKED(IDC_NEW,      &StarterDlg::OnBnClickedNew)
  ON_BN_CLICKED(IDCANCEL,     &StarterDlg::OnBnClickedCancel)
  ON_BN_CLICKED(IDC_STOP,     &StarterDlg::OnBnClickedStop)
  ON_BN_CLICKED(IDC_SUITE1,   &StarterDlg::OnBnClickedSuite1)
  ON_BN_CLICKED(IDC_SUITE2,   &StarterDlg::OnBnClickedSuite2)
  ON_BN_CLICKED(IDC_SUITE3,   &StarterDlg::OnBnClickedSuite3)
  ON_BN_CLICKED(IDC_SUITE4,   &StarterDlg::OnBnClickedSuite4)
  ON_BN_CLICKED(IDC_SUITE5,   &StarterDlg::OnBnClickedSuite5)
END_MESSAGE_MAP()

BOOL
StarterDlg::OnInitDialog()
{
  StyleDialog::OnInitDialog();
  SetWindowText(_T("Starter"));
  ShowCloseButton();

  InitTitle();
  InitTestSuites();
  InitButtons();

  UpdateData(FALSE);
  m_buttonSuite1.SetFocus();
  return FALSE;
}

void
StarterDlg::InitTitle()
{
  m_title = _T("Kwatta Test Suite ") KWATTA_YEAR;
  m_editTitle.SetBkColor(ThemeColor::GetColor(Colors::AccentColor1));
  m_editTitle.SetTextColor(RGB(255, 255, 255));
  m_editTitle.SetFontSize(240);
  m_editTitle.SetFontStyle(true);
  m_editTitle.SetMutable(false);
}

void
StarterDlg::InitTestSuites()
{
  RegistryManager reg;
  reg.SetRegistryKey(_T("EDO"),_T("Kwatta"));

  for(int index = 0; index < MAX_SUITES; ++index)
  {
    CString key;
    key.Format(_T("TestSuite_%d"),index + 1);

    CString suite = reg.GetRegistryString(_T("Suites"),key,_T(""));
    if(!suite.IsEmpty())
    {
      m_suites.push_back(suite);
    }
  }
}

bool
StarterDlg::SaveTestSuites()
{
  RegistryManager reg;
  reg.SetRegistryKey(_T("EDO"),_T("Kwatta"));

  // Put chosen suite in front
  // So it comes back at the top of the list
  if(!m_chosenSuite.IsEmpty())
  {
    AllSuites::iterator it = m_suites.begin();
    while(it != m_suites.end())
    {
      if((*it).CompareNoCase(m_chosenSuite) == 0)
      {
        m_suites.erase(it);
        break;
      }
      ++it;
    }
  }
  m_suites.push_front(m_chosenSuite);

  for(int index = 0; index < MAX_SUITES; ++index)
  {
    CString key;
    key.Format(_T("TestSuite_%d"), index + 1);

    if(index < (int)m_suites.size())
    {
      if(!m_suites[index].IsEmpty())
      {
        if(!reg.SetRegistryString(_T("Suites"),key,m_suites[index]))
        {
          return false;
        }
      }
    }
    else
    {
      reg.SetRegistryString(_T("Suites"),key,_T(""));
    }
  }
  return true;
}

void
StarterDlg::InitButtons()
{
  WinFile file;
  EnableToolTips();

  // Do the test suite buttons
  for(int index = 0;index < MAX_SUITES;++index)
  {
    CString suite;
    CString shorter;
    bool active(false);
    bool bold(false);

    if (index < (int)m_suites.size())
    {
      suite   = m_suites[index];
      file.SetFilename(suite);
      shorter = file.GetFilenamePartFilename();
      active  = true;
      bold    = true;
    }
    else
    {
      shorter = _T("<No test suite yet>");
    }

    switch (index)
    {
      case 0: m_buttonSuite1.SetWindowText(shorter);
              m_buttonSuite1.EnableWindow(active);
              m_buttonSuite1.SetBold(bold);
              m_buttonSuite1.SetStyle(_T("ok"));
              RegisterTooltip(m_buttonSuite1,suite);
              break;
      case 1: m_buttonSuite2.SetWindowText(shorter);
              m_buttonSuite2.EnableWindow(active);
              m_buttonSuite2.SetBold(bold);
              m_buttonSuite2.SetStyle(_T("ok"));
              RegisterTooltip(m_buttonSuite2,suite);
              break;
      case 2: m_buttonSuite3.SetWindowText(shorter);
              m_buttonSuite3.EnableWindow(active);
              m_buttonSuite3.SetBold(bold);
              m_buttonSuite3.SetStyle(_T("ok"));
              RegisterTooltip(m_buttonSuite3,suite);
              break;
      case 3: m_buttonSuite4.SetWindowText(shorter);
              m_buttonSuite4.EnableWindow(active);
              m_buttonSuite4.SetBold(bold);
              m_buttonSuite4.SetStyle(_T("ok"));
              RegisterTooltip(m_buttonSuite4,suite);
              break;
      case 4: m_buttonSuite5.SetWindowText(shorter);
              m_buttonSuite5.EnableWindow(active);
              m_buttonSuite5.SetBold(bold);
              m_buttonSuite5.SetStyle(_T("ok"));
              RegisterTooltip(m_buttonSuite5,suite);
              break;
    }
  }

  // Do the starter buttons
  m_buttonSearch.SetBold(true);
  RegisterTooltip(m_buttonSearch,_T("Search for an existing test suite in the filesystem."));
  m_buttonNewSuite.SetBold(true);
  RegisterTooltip(m_buttonNewSuite,_T("Start a new test suite in a new empty directory from scratch."));
  RegisterTooltip(m_buttonCancel,_T("Create a test suite totally from scratch."));
  m_buttonStop.SetBold(true);
  m_buttonStop.SetStyle(_T("can"));
  RegisterTooltip(m_buttonStop,_T("Close-down the Kwatta application"));
}

// Service for our callers
CString
StarterDlg::GetChosenSuite()
{
  return m_chosenSuite;
}

bool
StarterDlg::CheckExists(int p_num)
{
  CString path = m_suites[p_num];
  if(!std::filesystem::exists(path.GetString()))
  {
    if(StyleMessageBox(this,_T("This test suite does no longer exists. Remove from this list?"),_T(KWATTA),MB_YESNO|MB_DEFBUTTON1|MB_ICONQUESTION) == IDYES)
    {
      m_suites.erase(m_suites.begin() + p_num);
      InitButtons();
      return false;
    }
  }
  return true;
}

// StarterDlg message handlers

void
StarterDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
  if(nID == SC_CLOSE)
  {
    EndDialog(IDNO);
    return;
  }
  StyleDialog::OnSysCommand(nID, lParam);
}

void 
StarterDlg::OnBnClickedSearch()
{
  AutoFocus focus;
  DocFileDialog dlg(GetSafeHwnd()
                   ,true
                   ,_T("Choose a Kwatta test suite")
                   ,_T("xtest")
                   ,_T("")
                   ,0
                   ,_T("Kwatta test suite *.xtest|*.xtest|All files|*.*"));
  if(dlg.DoModal() == IDOK)
  {
    m_chosenSuite = dlg.GetChosenFile();
    if(!m_chosenSuite.IsEmpty() && SaveTestSuites())
    {
      OnOK();
    }
  }
}

void 
StarterDlg::OnBnClickedNew()
{
  NewSuiteDlg suite(this);

  AutoFocus focus;
  if(suite.DoModal() == IDOK)
  {
    m_chosenSuite = suite.GetNewTestsuiteFile();
    if(!m_chosenSuite.IsEmpty() && SaveTestSuites())
    {
      OnOK();
    }
  }
}

void 
StarterDlg::OnBnClickedSuite1()
{
  m_chosenSuite = m_suites[0];
  if(!CheckExists(0))
  {
    return;
  }
  if(SaveTestSuites())
  {
    OnOK();
  }
}

void 
StarterDlg::OnBnClickedSuite2()
{
  m_chosenSuite = m_suites[1];
  if(!CheckExists(1))
  {
    return;
  }
  if(SaveTestSuites())
  {
    OnOK();
  }
}

void 
StarterDlg::OnBnClickedSuite3()
{
  m_chosenSuite = m_suites[2];
  if(!CheckExists(2))
  {
    return;
  }
  if(SaveTestSuites())
  {
    OnOK();
  }
}

void 
StarterDlg::OnBnClickedSuite4()
{
  m_chosenSuite = m_suites[3];
  if(!CheckExists(3))
  {
    return;
  }
  if(SaveTestSuites())
  {
    OnOK();
  }
}

void 
StarterDlg::OnBnClickedSuite5()
{
  m_chosenSuite = m_suites[4];
  if(!CheckExists(4))
  {
    return;
  }
  if(SaveTestSuites())
  {
    OnOK();
  }
}

void
StarterDlg::OnBnClickedCancel()
{
  m_chosenSuite.Empty();
  StyleDialog::OnCancel();
}

void 
StarterDlg::OnBnClickedStop()
{
  EndDialog(IDNO);
}

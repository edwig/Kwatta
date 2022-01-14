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
// What it does: Choose a project to work on, or create a new one
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
#include <EnsureFile.h>
#include <FileDialog.h>
#include <filesystem>

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
  DDX_Control(pDX,IDC_PROJECT1, m_buttonProject1);
  DDX_Control(pDX,IDC_PROJECT2, m_buttonProject2);
  DDX_Control(pDX,IDC_PROJECT3, m_buttonProject3);
  DDX_Control(pDX,IDC_PROJECT4, m_buttonProject4);
  DDX_Control(pDX,IDC_PROJECT5, m_buttonProject5);
  DDX_Control(pDX,IDC_SEARCH,   m_buttonSearch);
  DDX_Control(pDX,IDC_NEW,      m_buttonNewProject);
  DDX_Control(pDX,IDCANCEL,     m_buttonCancel);
  DDX_Control(pDX,IDC_STOP,     m_buttonStop);
}

BEGIN_MESSAGE_MAP(StarterDlg, StyleDialog)
  ON_WM_SYSCOMMAND()
  ON_BN_CLICKED(IDC_SEARCH,   &StarterDlg::OnBnClickedSearch)
  ON_BN_CLICKED(IDC_NEW,      &StarterDlg::OnBnClickedNew)
  ON_BN_CLICKED(IDCANCEL,     &StarterDlg::OnBnClickedCancel)
  ON_BN_CLICKED(IDC_STOP,     &StarterDlg::OnBnClickedStop)
  ON_BN_CLICKED(IDC_PROJECT1, &StarterDlg::OnBnClickedProject1)
  ON_BN_CLICKED(IDC_PROJECT2, &StarterDlg::OnBnClickedProject2)
  ON_BN_CLICKED(IDC_PROJECT3, &StarterDlg::OnBnClickedProject3)
  ON_BN_CLICKED(IDC_PROJECT4, &StarterDlg::OnBnClickedProject4)
  ON_BN_CLICKED(IDC_PROJECT5, &StarterDlg::OnBnClickedProject5)
END_MESSAGE_MAP()

BOOL
StarterDlg::OnInitDialog()
{
  StyleDialog::OnInitDialog();
  SetWindowText("Starter");
  ShowCloseButton();

  InitTitle();
  InitProjects();
  InitButtons();

  UpdateData(FALSE);
  m_buttonProject1.SetFocus();
  return FALSE;
}

void
StarterDlg::InitTitle()
{
  m_title = "Kwatta Test Suite " KWATTA_YEAR;
  m_editTitle.SetBkColor(ThemeColor::_Color1);
  m_editTitle.SetTextColor(RGB(255, 255, 255));
  m_editTitle.SetFontSize(240);
  m_editTitle.SetFontStyle(true);
  m_editTitle.SetMutable(false);
}

void
StarterDlg::InitProjects()
{
  RegistryManager reg;
  reg.SetRegistryKey("EDO","Kwatta");

  for(int index = 0; index < MAX_PROJECTS; ++index)
  {
    CString key;
    key.Format("Project_%d",index + 1);

    CString project = reg.GetRegistryString("Projects",key,"");
    if(!project.IsEmpty())
    {
      m_projects.push_back(project);
    }
  }
}

bool
StarterDlg::SaveProjects()
{
  RegistryManager reg;
  reg.SetRegistryKey("EDO","Kwatta");

  // Put chosen project in front
  // So it comes back at the top of the list
  if(!m_chosenProject.IsEmpty())
  {
    AllProjects::iterator it = m_projects.begin();
    while(it != m_projects.end())
    {
      if((*it).CompareNoCase(m_chosenProject) == 0)
      {
        m_projects.erase(it);
        break;
      }
      ++it;
    }
  }
  m_projects.push_front(m_chosenProject);

  for(int index = 0; index < MAX_PROJECTS; ++index)
  {
    CString key;
    key.Format("Project_%d", index + 1);

    if(index < (int)m_projects.size())
    {
      if(!m_projects[index].IsEmpty())
      {
        if(!reg.SetRegistryString("Projects",key,m_projects[index]))
        {
          return false;
        }
      }
    }
    else
    {
      reg.SetRegistryString("Projects",key,"");
    }
  }
  return true;
}

void
StarterDlg::InitButtons()
{
  EnsureFile ensure;
  EnableToolTips();

  // Do the project buttons
  for(int index = 0;index < MAX_PROJECTS;++index)
  {
    CString project;
    CString shorter;
    bool active(false);
    bool bold(false);

    if (index < (int)m_projects.size())
    {
      project = m_projects[index];
      shorter = ensure.FilenamePart(project);
      active  = true;
      bold    = true;
    }
    else
    {
      shorter = "<No project yet>";
    }

    switch (index)
    {
      case 0: m_buttonProject1.SetWindowText(shorter);
              m_buttonProject1.EnableWindow(active);
              m_buttonProject1.SetBold(bold);
              m_buttonProject1.SetStyle("ok");
              RegisterTooltip(m_buttonProject1,project);
              break;
      case 1: m_buttonProject2.SetWindowText(shorter);
              m_buttonProject2.EnableWindow(active);
              m_buttonProject2.SetBold(bold);
              m_buttonProject2.SetStyle("ok");
              RegisterTooltip(m_buttonProject2,project);
              break;
      case 2: m_buttonProject3.SetWindowText(shorter);
              m_buttonProject3.EnableWindow(active);
              m_buttonProject3.SetBold(bold);
              m_buttonProject3.SetStyle("ok");
              RegisterTooltip(m_buttonProject3,project);
              break;
      case 3: m_buttonProject4.SetWindowText(shorter);
              m_buttonProject4.EnableWindow(active);
              m_buttonProject4.SetBold(bold);
              m_buttonProject4.SetStyle("ok");
              RegisterTooltip(m_buttonProject4,project);
              break;
      case 4: m_buttonProject5.SetWindowText(shorter);
              m_buttonProject5.EnableWindow(active);
              m_buttonProject5.SetBold(bold);
              m_buttonProject5.SetStyle("ok");
              RegisterTooltip(m_buttonProject5,project);
              break;
    }
  }

  // Do the starter buttons
  m_buttonSearch.SetBold(true);
  RegisterTooltip(m_buttonSearch,"Search for an existing project in the filesystem.");
  m_buttonNewProject.SetBold(true);
  RegisterTooltip(m_buttonNewProject,"Start a new project in a new empty directory from scratch.");
  RegisterTooltip(m_buttonCancel,"Create a project totally from scratch.");
  m_buttonStop.SetBold(true);
  m_buttonStop.SetStyle("can");
  RegisterTooltip(m_buttonStop,"Close-down the Kwatta application");
}

// Service for our callers
CString
StarterDlg::GetChosenProject()
{
  return m_chosenProject;
}

bool
StarterDlg::CheckExists(int p_num)
{
  CString path = m_projects[p_num];
  if(!std::filesystem::exists(path.GetString()))
  {
    if(StyleMessageBox(this,"This project does no longer exists. Remove from this list?",KWATTA,MB_YESNO|MB_DEFBUTTON1|MB_ICONQUESTION) == IDYES)
    {
      m_projects.erase(m_projects.begin() + p_num);
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
  DocFileDialog dlg(true
                   ,"Choose a Kwatta test suite"
                   ,"xtest"
                   ,""
                   ,0
                   ,"Kwatta test suite *.xtest|*.xtest|All files|*.*");
  if(dlg.DoModal() == IDOK)
  {
    m_chosenProject = dlg.GetChosenFile();
    if(!m_chosenProject.IsEmpty() && SaveProjects())
    {
      OnOK();
    }
  }
}

void 
StarterDlg::OnBnClickedNew()
{
  NewSuiteDlg suite(this);

  if(suite.DoModal() == IDOK)
  {
    m_chosenProject = suite.GetNewProjectFile();
    if(!m_chosenProject.IsEmpty() && SaveProjects())
    {
      OnOK();
    }
  }
}

void 
StarterDlg::OnBnClickedProject1()
{
  m_chosenProject = m_projects[0];
  if(!CheckExists(0))
  {
    return;
  }
  if(SaveProjects())
  {
    OnOK();
  }
}

void 
StarterDlg::OnBnClickedProject2()
{
  m_chosenProject = m_projects[1];
  if(!CheckExists(1))
  {
    return;
  }
  if(SaveProjects())
  {
    OnOK();
  }
}

void 
StarterDlg::OnBnClickedProject3()
{
  m_chosenProject = m_projects[2];
  if(!CheckExists(2))
  {
    return;
  }
  if(SaveProjects())
  {
    OnOK();
  }
}

void 
StarterDlg::OnBnClickedProject4()
{
  m_chosenProject = m_projects[3];
  if(!CheckExists(3))
  {
    return;
  }
  if(SaveProjects())
  {
    OnOK();
  }
}

void 
StarterDlg::OnBnClickedProject5()
{
  m_chosenProject = m_projects[4];
  if(!CheckExists(4))
  {
    return;
  }
  if(SaveProjects())
  {
    OnOK();
  }
}

void
StarterDlg::OnBnClickedCancel()
{
  m_chosenProject.Empty();
  StyleDialog::OnCancel();
}

void 
StarterDlg::OnBnClickedStop()
{
  EndDialog(IDNO);
}

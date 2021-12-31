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
// This File   : StarterDlg.h
// What it does: Choose a project to work on, or create a new one
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "afxdialogex.h"
#include <deque>

constexpr int MAX_PROJECTS = 5;

// StarterDlg dialog

class StarterDlg : public StyleDialog
{
	DECLARE_DYNAMIC(StarterDlg)

public:
	StarterDlg(CWnd* p_parent = nullptr);   // standard constructor
	virtual ~StarterDlg();

  CString  GetChosenProject();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_STARTER };
#endif

protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual BOOL OnInitDialog() override;
          void InitTitle();
          void InitProjects();
          void InitButtons();
          bool SaveProjects();
          bool CheckExists(int p_num);

  typedef std::deque<CString> AllProjects;

  CString      m_chosenProject;
  AllProjects  m_projects;

  CString      m_title;
  StyleEdit    m_editTitle;
  StyleButton  m_buttonSearch;
  StyleButton  m_buttonNewProject;
  StyleButton  m_buttonCancel;
  StyleButton  m_buttonStop;
  StyleButton  m_buttonProject1;
  StyleButton  m_buttonProject2;
  StyleButton  m_buttonProject3;
  StyleButton  m_buttonProject4;
  StyleButton  m_buttonProject5;

	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnSysCommand(UINT nID,LPARAM lParam);
  afx_msg void OnBnClickedSearch();
  afx_msg void OnBnClickedNew();
  afx_msg void OnBnClickedCancel();
  afx_msg void OnBnClickedStop();
  afx_msg void OnBnClickedProject1();
  afx_msg void OnBnClickedProject2();
  afx_msg void OnBnClickedProject3();
  afx_msg void OnBnClickedProject4();
  afx_msg void OnBnClickedProject5();
};

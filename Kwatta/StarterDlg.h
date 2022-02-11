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
// What it does: Choose a test suite to work on, or create a new one
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "afxdialogex.h"
#include <deque>

constexpr int MAX_SUITES = 5;

// StarterDlg dialog

class StarterDlg : public StyleDialog
{
	DECLARE_DYNAMIC(StarterDlg)

public:
	StarterDlg(CWnd* p_parent = nullptr);   // standard constructor
	virtual ~StarterDlg();

  CString  GetChosenSuite();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_STARTER };
#endif

protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual BOOL OnInitDialog() override;
          void InitTitle();
          void InitTestSuites();
          void InitButtons();
          bool SaveTestSuites();
          bool CheckExists(int p_num);

  typedef std::deque<CString> AllSuites;

  CString      m_chosenSuite;
  AllSuites    m_suites;

  CString      m_title;
  StyleEdit    m_editTitle;
  StyleButton  m_buttonSearch;
  StyleButton  m_buttonNewSuite;
  StyleButton  m_buttonCancel;
  StyleButton  m_buttonStop;
  StyleButton  m_buttonSuite1;
  StyleButton  m_buttonSuite2;
  StyleButton  m_buttonSuite3;
  StyleButton  m_buttonSuite4;
  StyleButton  m_buttonSuite5;

	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnSysCommand(UINT nID,LPARAM lParam);
  afx_msg void OnBnClickedSearch();
  afx_msg void OnBnClickedNew();
  afx_msg void OnBnClickedCancel();
  afx_msg void OnBnClickedStop();
  afx_msg void OnBnClickedSuite1();
  afx_msg void OnBnClickedSuite2();
  afx_msg void OnBnClickedSuite3();
  afx_msg void OnBnClickedSuite4();
  afx_msg void OnBnClickedSuite5();
};

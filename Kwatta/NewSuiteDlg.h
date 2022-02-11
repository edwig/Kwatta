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
// This File   : NewSuiteDlg.h
// What it does: Creates a new test suite in a new and empty directory
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "afxdialogex.h"

// NewSuiteDlg dialog

class NewSuiteDlg : public StyleDialog
{
	DECLARE_DYNAMIC(NewSuiteDlg)

public:
	NewSuiteDlg(CWnd* p_parent);   // standard constructor
	virtual ~NewSuiteDlg();

  CString GetNewTestsuiteFile();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NEWSUITE };
#endif

protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual BOOL OnInitDialog() override;
          bool CheckDirectory();
          bool CheckSuiteName();
          bool CreateEmptySuite();

  bool    m_init;
  CString m_explain;
  CString m_directory;
  CString m_suiteName;

  StyleEdit   m_editExplain;
  StyleEdit   m_editDirectory;
  StyleEdit   m_editSuiteName;
  StyleButton m_buttonChooseDir;
  StyleButton m_buttonChooseFile;
  StyleButton m_buttonOK;
  StyleButton m_buttonCancel;

	DECLARE_MESSAGE_MAP()

  afx_msg void OnEnKillfocusExplain();
  afx_msg void OnEnKillfocusDirectory();
  afx_msg void OnEnKillfocusSuiteName();
  afx_msg void OnBnClickedChooseDir();
  afx_msg void OnBnClickedChooseFile();
  afx_msg void OnBnClickedOK();
  afx_msg void OnBnClickedCancel();
};

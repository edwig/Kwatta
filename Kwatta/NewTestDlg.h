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
// This program: TestSuite
// This File   : NewTestDlg.h
// What it does: Create a new test set for a test suite
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "resource.h"

// NewTestDlg dialog

class NewTestDlg : public StyleDialog
{
	DECLARE_DYNAMIC(NewTestDlg)

public:
	NewTestDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~NewTestDlg();

  XString GetDirectory() { return m_directory; }
  XString GetTestName()  { return m_testName;  }
  bool    GetActive()    { return m_active;    }

// Dialog Data
	enum { IDD = IDD_NEWTEST };

protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual BOOL OnInitDialog() override;
  bool CheckInput();

	DECLARE_MESSAGE_MAP()

  XString       m_directory;
  XString       m_testName;
  bool          m_active { true };

  StyleEdit     m_editDirectory;
  StyleEdit     m_editTestName;
  StyleCheckbox m_checkActive;
  StyleButton   m_buttonOK;
  StyleButton   m_buttonCancel;

  afx_msg void OnKillfocusDirectory();
  afx_msg void OnKillfocusTestname();
  afx_msg void OnBnClickedActive();
  afx_msg void OnBnClickedOK();
};

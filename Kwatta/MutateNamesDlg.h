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
// This File   : MutateNames.dlg
// What it does: Mutates the name of the directory or the filename of the testset in the suite.
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "afxdialogex.h"

// MutateNamesDlg dialog

class MutateNamesDlg : public StyleDialog
{
	DECLARE_DYNAMIC(MutateNamesDlg)

public:
	MutateNamesDlg(CWnd* p_parent,XString p_directory,XString p_filename);
	virtual ~MutateNamesDlg();

  XString GetDirectory() { return m_origDirectory; }
  XString GetFilename()  { return m_origFilename;  }
  XString GetTestname()  { return m_testName;      }

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MUTATENAMES };
#endif

protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual BOOL OnInitDialog() override;
          bool CheckDirectory();
          bool CheckFilename();
          void SetTestName();

  XString     m_origDirectory;
  XString     m_newDirectory;
  XString     m_origFilename;
  XString     m_newFilename;
  XString     m_testName;

  StyleEdit   m_editOrigDirectory;
  StyleEdit   m_editNewDirectory;
  StyleEdit   m_editOrigFilename;
  StyleEdit   m_editNewFilename;
  StyleButton m_buttonDoDirectory;
  StyleButton m_buttonDoFilename;
  StyleButton m_buttonOK;

  DECLARE_MESSAGE_MAP()

  afx_msg void OnEnKillFocusOrigDirectory();
  afx_msg void OnEnKillFocusNewDirectory();
  afx_msg void OnEnKillFocusOrigFilename();
  afx_msg void OnEnKillFocusNewFilename();
  afx_msg void OnBnClickedDoDirectory();
  afx_msg void OnBnClickedDoFilename();
  afx_msg void OnBnClickedOK();

};

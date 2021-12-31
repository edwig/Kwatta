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
	MutateNamesDlg(CWnd* p_parent,CString p_directory,CString p_filename);
	virtual ~MutateNamesDlg();

  CString GetDirectory() { return m_origDirectory; }
  CString GetFilename()  { return m_origFilename;  }

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MUTATENAMES };
#endif

protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual BOOL OnInitDialog() override;
          bool CheckDirectory();
          bool CheckFilename();

  CString     m_origDirectory;
  CString     m_newDirectory;
  CString     m_origFilename;
  CString     m_newFilename;

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

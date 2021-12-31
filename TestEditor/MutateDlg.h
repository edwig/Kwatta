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
// This File   : MutateDlg.h
// What it does: Change the underlying filename of a teststep or a validation
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "afxdialogex.h"

// MutateDlg dialog

class MutateDlg : public StyleDialog
{
	DECLARE_DYNAMIC(MutateDlg)

public:
	MutateDlg(CWnd* p_parent,CString p_object,CString p_filename);
	virtual ~MutateDlg();

  CString GetFilename();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MUTATE };
#endif

protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual BOOL OnInitDialog() override;
          bool CheckFilenames();

  CString     m_object;
  CString     m_orgFilename;
  CString     m_newFilename;

  StyleEdit   m_editOrgFilename;
  StyleEdit   m_editNewFilename;
  StyleButton m_buttonChange;
  StyleButton m_buttonOK;

	DECLARE_MESSAGE_MAP()

  afx_msg void OnEnKillfocusOrgFilename();
  afx_msg void OnEnKillfocusNewFilename();
  afx_msg void OnBnClickedChange();
  afx_msg void OnBnClickedOK();
};

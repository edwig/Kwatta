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
// This File   : SystemDlg.h
// What it does: System wide hidden parameters
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "afxdialogex.h"
#include <Parameters.h>

// SystemDlg dialog

class SystemDlg : public StyleDialog
{
	DECLARE_DYNAMIC(SystemDlg)

public:
	SystemDlg(CWnd* p_parent,Parameters* p_parameters);
	virtual ~SystemDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SYSTEM };
#endif

protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual BOOL OnInitDialog() override;
          void InitButtons();
          void InitParameters();
          void SaveParameters();
          bool Check();

  Parameters*   m_parameters { nullptr };
  bool          m_changed    { false   };

  int           m_loglevel;
  CString       m_logfile;
  bool          m_hidePassword;
  CString       m_password;

  StyleComboBox m_comboLoglevel;
  StyleEdit     m_editLogfile;
  StyleButton   m_buttonLogfile;
  StyleCheckbox m_checkHidePwd;
  StyleEdit     m_editPassword;
  StyleButton   m_buttonShow;
  StyleButton   m_buttonOK;
  StyleButton   m_buttonCancel;

  StyleHyperLink m_help;

	DECLARE_MESSAGE_MAP()

  afx_msg void OnTimer(UINT_PTR nIDEvent);
  afx_msg void OnCbnCloseupLoglevel();
  afx_msg void OnEnKillfocusLogfile();
  afx_msg void OnBnClickedLogfile();
  afx_msg void OnBnClickedHidePassword();
  afx_msg void OnBnClickedShow();
  afx_msg void OnBnClickedOK();
  afx_msg void OnBnClickedCancel();
};

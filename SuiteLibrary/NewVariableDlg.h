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
// This program: StepEditor
// This File   : NewVariableDlg.h
// What it does: Define a new local variable for a test step
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Parameters.h"

// NewVariableDlg dialog

class NewVariableDlg : public StyleDialog
{
	DECLARE_DYNAMIC(NewVariableDlg)

public:
	NewVariableDlg(ParType p_type,ParMap& p_map,CWnd* pParent = nullptr);   // standard constructor
	virtual ~NewVariableDlg();
  BOOL     OnInitDialog();
  XString  GetNewName() { return m_newName; };

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NEWVAR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
          void FillCombo();
          bool CheckName();
          void SaveName();

  XString       m_explain;
  XString       m_newName;
  ParType       m_type;
  ParMap&       m_map;
  StyleComboBox m_comboExisting;
  StyleEdit     m_editExplain;
  StyleEdit     m_editNewName;
  StyleButton   m_buttonOK;
  StyleButton   m_buttonCancel;

	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnEnChangeExplain();
  afx_msg void OnEnChangeName();
  afx_msg void OnBnClickedOk();
  afx_msg void OnBnClickedCancel();
};

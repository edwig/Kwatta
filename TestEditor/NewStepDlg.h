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
// This File   : NewStepDlg.h
// What it does: Defining a new test step in the set
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

// NewStepDlg dialog

class NewStepDlg : public StyleDialog
{
  DECLARE_DYNAMIC(NewStepDlg)

public:
  NewStepDlg(CWnd* pParent = nullptr,bool p_valiOnly = false);
  virtual ~NewStepDlg();

  StepType GetType()     { return m_type;      }
  CString  GetName()     { return m_name;      }
  CString  GetFilename() { return m_filename;  }

  int     GetValiType() { return m_valiType;  }
  CString GetValiName() { return m_valiName;  }
  CString GetValiFile() { return m_valiFile;  }

  // Dialog Data
  enum { IDD = IDD_NEW_STEP };

protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual BOOL OnInitDialog() override;

  void CheckFilename();
  void CheckValiFile();

  StyleComboBox  m_comboType;
  StyleEdit      m_editName;
  StyleEdit      m_editFilename;

  StyleComboBox  m_comboVali;
  StyleEdit      m_editVali;
  StyleEdit      m_editValiFile;

  StyleButton    m_buttonOK;
  StyleButton    m_buttonCancel;

  StepType       m_type;
  CString        m_name;
  CString        m_filename;

  bool           m_valiOnly;
  int            m_valiType;
  CString        m_valiName;
  CString        m_valiFile;

  DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnCbnSelchangeTestType();
  afx_msg void OnEnChangeName();
  afx_msg void OnEnChangeFilename();

  afx_msg void OnCbnSelchangeValiType();
  afx_msg void OnEnChangeValiName();
  afx_msg void OnEnChangeValiFile();

  afx_msg void OnBnClickedOK();
};

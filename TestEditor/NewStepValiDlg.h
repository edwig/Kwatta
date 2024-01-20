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
// This File   : NewStepNamesDlg.h
// What it does: Defining the third step of the new teststep wizard
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "afxdialogex.h"


// NewStepValiDlg dialog

class NewStepValiDlg : public StyleTab
{
	DECLARE_DYNAMIC(NewStepValiDlg)

public:
	NewStepValiDlg(CWnd* pParent = nullptr);
	virtual ~NewStepValiDlg();

  virtual bool InitStyleTab (void* p_data) override;
  virtual bool CheckStyleTab(void* p_data) override;
  virtual bool SaveStyleTab (void* p_data) override;

  bool         GetValiGlobal()  { return m_valiGlobal;  }
  CString      GetValiName()    { return m_valiName;    }
  CString      GetValiFile()    { return m_valiFile;    }

  // Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NEWSTEP_VALI };
#endif

protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual void SetupDynamicLayout() override;

  CString StripExtension(CString p_filename);
  void    ResetPage();
  void    CheckValiFile();
  void    PresetFilenames();

  StepType     m_stepType { StepType::Step_command };
  StyleButton  m_buttonValiGlobal;
  StyleEdit    m_editName;
  StyleEdit    m_editFile;
  
  CString      m_valiName;
  CString      m_valiFile;
  bool         m_valiGlobal { false };

  DECLARE_MESSAGE_MAP()

  afx_msg void OnBnClickedValiGlobal();
  afx_msg void OnEnChangeValiName();
  afx_msg void OnEnChangeValiFile();
};

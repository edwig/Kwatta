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
// This File   : NewStepTypeDlg.h
// What it does: Defining the first step of the new teststep wizard
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "afxdialogex.h"


// NewStepTypeDlg dialog

class NewStepTypeDlg : public StyleTab
{
	DECLARE_DYNAMIC(NewStepTypeDlg)

public:
	NewStepTypeDlg(CWnd* p_parent = nullptr);   // standard constructor
	virtual ~NewStepTypeDlg();

  virtual bool InitStyleTab (void* p_data) override;
  virtual bool CheckStyleTab(void* p_data) override;
  virtual bool SaveStyleTab (void* p_data) override;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NEWSTEP_TYPE };
#endif

protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;

  StyleComboBox  m_comboType;
  StepType       m_stepType;

	DECLARE_MESSAGE_MAP()

  afx_msg void OnCbnSelchangeTestType();


};

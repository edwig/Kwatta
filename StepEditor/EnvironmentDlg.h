﻿///////////////////////////////////////////////////////////////////////////////////////////////////////
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
// This File   : EnvironmentDlg.h
// What it does: Changing the environment before a command-line test step
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "StyleGridCtrl.h"

class TestStepCMD;

// EnvironmentDlg dialog

class EnvironmentDlg : public StyleTab
{
	DECLARE_DYNAMIC(EnvironmentDlg)

public:
	EnvironmentDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~EnvironmentDlg();
  void InitTab(TestStepCMD* p_step);
  void StoreVariables();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ENVIRON };
#endif

protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual void SetupDynamicLayout() override;
  virtual BOOL OnInitDialog() override;

  void InitLijst();

	DECLARE_MESSAGE_MAP()

  TestStepCMD*   m_step;
  StyleGridCtrl m_list;
  StyleButton   m_buttonNew;
  StyleButton   m_buttonDelete;
public:
  afx_msg void OnBnClickedNew();
  afx_msg void OnBnClickedDelete();
  afx_msg void OnLvnItemchangedGrid(NMHDR* pNMHDR,LRESULT* pResult);
};

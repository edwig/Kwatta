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
// This File   : UrlHeaderDlg.h
// What it does: Request headers for a HTTP internet test call
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "StyleGridCtrl.h"

class TestStepNET;

// UrlHeaderDlg- dialog

class UrlHeaderDlg : public StyleTab
{
	DECLARE_DYNAMIC(UrlHeaderDlg)

public:
	UrlHeaderDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~UrlHeaderDlg();
  void InitTab(TestStepNET* p_step,Parameters* p_parameters);
  void StoreVariables();
  bool IsFilled();

// Dialog Data
	enum { IDD = IDD_URL_HEADERS };

protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual void SetupDynamicLayout() override;
  virtual BOOL OnInitDialog() override;

  void InitList();
  void EffectiveParameters();

	DECLARE_MESSAGE_MAP()

  TestStepNET*   m_step       { nullptr };
  Parameters*   m_parameters { nullptr };
  StyleGridCtrl m_list;
  StyleButton   m_buttonNew;
  StyleButton   m_buttonDelete;
  StyleButton   m_buttonParms;
public:
  afx_msg void OnBnClickedNew();
  afx_msg void OnBnClickedDelete();
  afx_msg void OnBnClickedParms();
  afx_msg void OnLvnItemchangedGrid(NMHDR* pNMHDR,LRESULT* pResult);
};

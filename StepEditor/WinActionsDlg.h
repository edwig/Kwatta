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
// This File   : UrlParameterDlg.h
// What it does: Request URL parameters for a Windows UI test call
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "StyleGridCtrl.h"
#include "WinAction.h"

class TestStepWIN;

// EnvironmentDlg dialog

class WinActionsDlg : public StyleTab
{
	DECLARE_DYNAMIC(WinActionsDlg)

public:
	WinActionsDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~WinActionsDlg();
  void InitTab(TestStepWIN* p_step,Parameters* p_parameters);
  void StoreVariables();
  bool IsFilled();

// Dialog Data
	enum { IDD = IDD_WIN_ACTIONS};

protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual void SetupDynamicLayout() override;
  virtual BOOL OnInitDialog() override;

  void InitLijst();
  void EffectiveParameters();
  void AddActions(int p_row);
  void AddTitleTips(int p_row,WinUIAction p_action);

	DECLARE_MESSAGE_MAP()

  TestStepWIN*  m_step       { nullptr };
  Parameters*   m_parameters { nullptr };

  StyleGridCtrl m_list;
  StyleButton   m_buttonNew;
  StyleButton   m_buttonDelete;
  StyleButton   m_buttonParm;
  StyleButton   m_buttonDown;
  StyleButton   m_buttonUp;
public:
  afx_msg void OnBnClickedNew();
  afx_msg void OnBnClickedDelete();
  afx_msg void OnBnClickedParm();
  afx_msg void OnBnClickedDown();
  afx_msg void OnBnClickedUp();
  afx_msg void OnLvnItemchangedGrid(NMHDR* pNMHDR,LRESULT* pResult);
  afx_msg void OnEndInPlaceEdit    (NMHDR* pNMHDR,LRESULT* pResult);
};

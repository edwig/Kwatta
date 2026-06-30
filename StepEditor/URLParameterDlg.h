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
// This File   : UrlParameterDlg.h
// What it does: Request URL parameters for a HTTP internet test call
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "StyleGridCtrl.h"

class TestStepNET;

// EnvironmentDlg dialog

class URLParameterDlg : public StyleTab
{
	DECLARE_DYNAMIC(URLParameterDlg)

public:
	URLParameterDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~URLParameterDlg();
  void InitTab(TestStepNET* p_step,Parameters* p_parameters);
  void StoreVariables();
  bool IsFilled();

// Dialog Data
	enum { IDD = IDD_HTTP_PARAM};

protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual void SetupDynamicLayout() override;
  virtual BOOL OnInitDialog() override;

  void InitLijst();
  void EffectiveParameters();

	DECLARE_MESSAGE_MAP()

  TestStepNET*   m_step       { nullptr };
  Parameters*   m_parameters { nullptr };

  StyleGridCtrl m_list;
  XString       m_anchor;
  StyleButton   m_buttonNew;
  StyleButton   m_buttonDelete;
  StyleButton   m_buttonParmParm;
  StyleEdit     m_editAnchor;
  StyleButton   m_buttonAnchorParm;
public:
  afx_msg void OnBnClickedNew();
  afx_msg void OnBnClickedDelete();
  afx_msg void OnBnClickedParmParm();
  afx_msg void OnEnKillfocusAnchor();
  afx_msg void OnBnClickedAnchorParm();
  afx_msg void OnLvnItemchangedGrid(NMHDR* pNMHDR,LRESULT* pResult);
};

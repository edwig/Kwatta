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
// This File   : RequestDlg.h
// What it does: Tab in the test editor for the plain request of an internet HTTP call
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

class TestStepIn;
class Parameters;

// PayloadDlg dialog

class RequestDlg : public StyleDialog
{
	DECLARE_DYNAMIC(RequestDlg)

public:
	RequestDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~RequestDlg();

  void InitTab(TestStepIN* p_testStep,Parameters* p_parameters);
  void InitCombo();
  void InitPayload();
  void StoreVariables();
  void EffectiveParameters();
  void CheckXML();
  void CheckJSON();


// Dialog Data
	enum { IDD = IDD_REQUEST };

protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual BOOL OnInitDialog() override;
  virtual void SetupDynamicLayout() override;

  CString       m_contentType;
  CString       m_payload;

  StyleComboBox m_comboContent;
  StyleButton   m_buttonCheck;
  StyleButton   m_buttonParam;
  StyleEdit     m_editPayload;

  TestStepIN*   m_testStep   { nullptr };
  Parameters*   m_parameters { nullptr };

	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnCbnSelchangeContent();
  afx_msg void OnBnClickedCheck();
  afx_msg void OnBnClickedParam();
  afx_msg void OnEnKillfocusPayload();
};

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
// This File   : RawResponseDlg.h
// What it does: Resulting raw response of a HTTP internet test call
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

class TestStepIn;
class StepResultNET;

// RawResponseDlg dialog

class RawResponseDlg : public StyleTab
{
  DECLARE_DYNAMIC(RawResponseDlg)

public:
  RawResponseDlg(CWnd* pParent = nullptr);   // standard constructor
  virtual ~RawResponseDlg();

  void InitTab();
  void SetResult(StepResultNET* p_result);
  void InitPayload();
  void StoreVariables();

// Dialog Data
  enum { IDD = IDD_RAW_RESPONSE};

protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual BOOL OnInitDialog() override;
  virtual void SetupDynamicLayout() override;

  CString     m_payload;
  StyleEdit   m_editPayload;
  TestStepNET* m_testStep { nullptr };

  DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnEnKillfocusPayload();
};

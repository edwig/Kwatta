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
// This program: ResultViewer
// This File   : ResponseDlg.h
// What it does: Shows the response body of the HTTP internet message 
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

class StepResultNET;

// ResponseDlg dialog

class ResponseDlg : public StyleTab
{
  DECLARE_DYNAMIC(ResponseDlg)

public:
  ResponseDlg(CWnd* pParent = nullptr);   // standard constructor
  virtual ~ResponseDlg();

  void LoadVariables(StepResultNET* p_testStep);

// Dialog Data
  enum { IDD = IDD_RESPONSE };

protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual BOOL OnInitDialog() override;
  virtual void SetupDynamicLayout() override;
          void FormatPayload(StepResultNET* p_result);

  int         m_status { 0 };
  CString     m_statusText;
  CString     m_time;
  CString     m_payload;

  StyleEdit   m_editStatus;
  StyleEdit   m_editStatusText;
  StyleEdit   m_editTime;
  StyleEdit   m_editPayload;

  DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnEnChangeStatus();
  afx_msg void OnEnChangeStatusText();
  afx_msg void OnEnChangeTime();
  afx_msg void OnEnChangePayload();
};

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
// This File   : ResponseDlg.h
// What it does: Resulting response only of a HTTP internet test call
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

class TestStepNET;
class StepResultNET;

// ResponseDlg dialog

class ResponseDlg : public StyleDialog
{
	DECLARE_DYNAMIC(ResponseDlg)

public:
	ResponseDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~ResponseDlg();

  void InitTab();
  void SetResult(StepResultNET* p_result);

// Dialog Data
	enum { IDD = IDD_RESPONSE };

protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual BOOL OnInitDialog() override;
  virtual void SetupDynamicLayout() override;
          void InitPayload();
          void FormatPayload(StepResultNET* p_result);

  int         m_status { 0 };
  CString     m_statusText;
  CString     m_time;
  CString     m_payload;

  StyleEdit   m_editStatus;
  StyleEdit   m_editStatusText;
  StyleEdit   m_editTime;
  StyleEdit   m_editPayload;
  StyleButton m_buttonClear;

	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnEnChangeStatus();
  afx_msg void OnEnChangeStatusText();
  afx_msg void OnEnChangeTime();
  afx_msg void OnEnChangePayload();
  afx_msg void OnBnClickedClear();
};

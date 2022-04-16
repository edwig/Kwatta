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
// This File   : RawResponseDlg.h
// What it does: Shows raw response from the HTTP internet message 
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

class StepResultNET;

// RawResponseDlg dialog

class RawResponseDlg : public StyleDialog
{
	DECLARE_DYNAMIC(RawResponseDlg)

public:
	RawResponseDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~RawResponseDlg();

  void InitPayload();
  void LoadVariables(StepResultNET* p_result);
  void StoreVariables();

// Dialog Data
	enum { IDD = IDD_RAW_RESPONSE};

protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual BOOL OnInitDialog() override;
  virtual void SetupDynamicLayout() override;

  CString       m_payload;
  StyleEdit     m_editPayload;
  StepResultNET* m_result { nullptr };

	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnEnKillfocusPayload();
};

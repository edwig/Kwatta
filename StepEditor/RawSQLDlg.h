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
// This File   : RawSQLDlg.h
// What it does: Resulting raw SQL database test call
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

class TestStepSQL;

// RawSQLDlg dialog

class RawSQLDlg : public StyleTab
{
	DECLARE_DYNAMIC(RawSQLDlg)

public:
	RawSQLDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~RawSQLDlg();

  void InitTab(TestStepSQL* p_testStep = nullptr);
  void InitPayload();
  void StoreVariables();
  bool IsFilled();

// Dialog Data
	enum { IDD = IDD_RAW_SQL};

protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual BOOL OnInitDialog() override;
  virtual void SetupDynamicLayout() override;

  CString       m_payload;
  StyleEdit     m_editPayload;
  TestStepSQL*  m_testStep { nullptr };

	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnEnKillfocusPayload();
};

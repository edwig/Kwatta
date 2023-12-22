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
// This File   : WinResultDlg.h
// What it does: Resulting response of a Windows UIT test
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

class TestStepWIN;
class StepResultWIN;

// WinResultDlg dialog

class WinResultDlg : public StyleTab
{
	DECLARE_DYNAMIC(WinResultDlg)

public:
	WinResultDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~WinResultDlg();

  void InitTab();
  void SetResult(StepResultWIN* p_result);

// Dialog Data
	enum { IDD = IDD_RESPONSE };

protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual BOOL OnInitDialog() override;
  virtual void SetupDynamicLayout() override;
          void InitTextFields();

  int         m_status { 0 };
  CString     m_errors;
  CString     m_logging;

  StyleEdit   m_editStatus;
  StyleEdit   m_editErrors;
  StyleEdit   m_editLogging;
  StyleButton m_buttonClear;

	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnEnChangeStatus();
  afx_msg void OnEnChangeErrors();
  afx_msg void OnEnChangeLogging();
  afx_msg void OnBnClickedClear();
};

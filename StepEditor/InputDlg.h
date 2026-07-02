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
// This File   : InputDlg.h
// What it does: Standard input of a command-line test step
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

class TestStepCMD;


// InputDlg dialog

class InputDlg : public StyleTab
{
	DECLARE_DYNAMIC(InputDlg)

public:
	InputDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~InputDlg();
  void InitTab(TestStepCMD* p_step,Parameters* p_parameters);
  void StoreVariables();
  void Effectuate();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_INPUT };
#endif

protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual BOOL OnInitDialog() override;
  virtual void SetupDynamicLayout() override;

	DECLARE_MESSAGE_MAP()

  XString     m_input;
  XString     m_effective;
  StyleEdit   m_editInput;
  StyleEdit   m_editEffective;
  StyleButton m_buttonInputParm;
  TestStepCMD* m_step       { nullptr };
  Parameters* m_parameters { nullptr };
public:
  afx_msg void OnEnChangeInput();
  afx_msg void OnBnClickedInputParm();
};

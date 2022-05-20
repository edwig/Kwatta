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

  void InitTab(TestStepNET* p_testStep,Parameters* p_parameters);
  void InitCombo();
  void InitPayload();
  bool IsFilled();
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

  void UpdateStepHeader(CString p_contentType);

  CString       m_mimeType;
  CString       m_payload;
  CString       m_inputFile;
  CString       m_outputFile;
  bool          m_useFile;
  bool          m_saveFile;

  StyleComboBox m_comboMime;
  StyleButton   m_buttonCheck;
  StyleButton   m_buttonParam;
  StyleButton   m_buttonMulti;
  StyleEdit     m_editPayload;
  StyleCheckbox m_checkUseFile;
  StyleEdit     m_editInputFile;
  StyleButton   m_buttonChooseFile;
  StyleCheckbox m_checkSaveFile;
  StyleEdit     m_editOutputFile;
  StyleButton   m_buttonSaveFile;

  TestStepNET*  m_testStep   { nullptr };
  Parameters*   m_parameters { nullptr };

	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnCbnSelchangeMime();
  afx_msg void OnBnClickedCheck();
  afx_msg void OnBnClickedParam();
  afx_msg void OnBnClickedMulti();
  afx_msg void OnEnKillfocusPayload();
  afx_msg void OnBnClickedUseFile();
  afx_msg void OnEnKillfocusInputFile();
  afx_msg void OnBnClickedChooseFile();
  afx_msg void OnBnClickedSaveFile();
  afx_msg void OnEnKillfocusOutputFile();
  afx_msg void OnBnClickedOutputFile();
};

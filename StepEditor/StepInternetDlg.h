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
// This File   : StepInternetDlg.h
// What it does: Main editor for one internet HTTP test call
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <Credentials.h>
#include <Parameters.h>

class RequestDlg;
class URLParameterDlg;
class UrlHeaderDlg;
class RawRequestDlg;
class AuthenticateDlg;
class TimingHTTPDlg;
class ScriptDlg;
class ResponseDlg;
class RespHeadersDlg;
class RawResponseDlg;
class TestStepNET;
class StepResultNET;

// StepInternetDlg dialog

class StepInternetDlg : public StyleDialog
{
	DECLARE_DYNAMIC(StepInternetDlg)

public:
	StepInternetDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~StepInternetDlg();
  void EffectiveParameters();
  void ResetStepResult();
  void SetStepResult(StepResultNET* p_result);
  void RefreshHeaders();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_STEPEDITOR_INET };
#endif

protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual BOOL OnInitDialog() override;
  virtual void SetupDynamicLayout() override;

  void InitTabs();
  void InitVerbs();
  void InitStep();
  void InitGlobalParameters();
  void InitCredentials();
  void InitParameters();
  void InitButtons();
  void InitGlobals();
  void InitTooltips();
  void ReadParameters(CString p_file, bool p_global = true);
  bool SaveStep();
  void StoreCredentials();
  void StoreVariables();
  void LoadVariables();
  void LoadVariablesTabs();
  void SetFirstTab();
  void Redraw();

  CString           m_name;
  CString           m_comment;
  CString           m_bound;
  CString           m_verb;
  CString           m_url;

  StyleEdit         m_editName;
  StyleButton       m_buttonGlobal;
  StyleEdit         m_editComment;
  StyleEdit         m_editBound;
  StyleButton       m_buttonGO;
  StyleComboBox     m_comboVerb;
  StyleEdit         m_editUrl;
  StyleButton       m_buttonUrlParm;
  StyleTabCtrl      m_tabsRequest;
  StyleTabCtrl      m_tabsResponse;
  StyleButton       m_buttonErrors;
  StyleButton       m_buttonOK;
  StyleButton       m_buttonCancel;

  TestStepNET*      m_testStep { nullptr };
  Credentials       m_credentials;
  Parameters        m_parameters;
  RequestDlg*       m_page1;
  URLParameterDlg*  m_page2;
  UrlHeaderDlg*     m_page3;
  AuthenticateDlg*  m_page4;
  RawRequestDlg*    m_page5;
  TimingHTTPDlg*    m_page6;
  ScriptDlg*        m_page7;

  ResponseDlg*      m_page10;
  RespHeadersDlg*   m_page11;
  RawResponseDlg*   m_page12;

  bool              m_busy{ false };
  int               m_unbound{ 0 };

	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnEnChangeStepname();
  afx_msg void OnBnClickedGlobal();
  afx_msg void OnEnChangeComment();
  afx_msg void OnCbnSelchangeVerb();
  afx_msg void OnEnChangeUrl();
  afx_msg void OnBnClickedGO();
  afx_msg void OnBnClickedOk();
  afx_msg void OnBnClickedUrlParm();
  afx_msg void OnBnClickedErrors();
  afx_msg void OnAbout();
  afx_msg void OnExit();
};

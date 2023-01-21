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
// This File   : AuthenticateDlg.h
// What it does: Authentications for an HTTP internet call
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

class TestStepNET;
class StepResultNET;

#define AUTH_ANONYMOUS  1
#define AUTH_BASIC      2
#define AUTH_NTLM_SSO   3
#define AUTH_NTLM       4
#define AUTH_OAUTH      5

// AuthenticateDlg dialog

class AuthenticateDlg : public StyleTab
{
	DECLARE_DYNAMIC(AuthenticateDlg)

public:
	AuthenticateDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~AuthenticateDlg();
  virtual BOOL OnInitDialog() override;

  void InitTab(TestStepNET* p_step,Parameters* p_parameters);
  void SetResult(CString p_token);
  void StoreVariables();
  bool IsFilled();

// Dialog Data
	enum { IDD = IDD_AUTHENTICATE };

protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual void SetupDynamicLayout() override;

  void InitButtons();
  void InitCombos();
  int  CalcAuthenticationType();
  void EffectiveParameters();
  void ChooseVariable(StyleEdit& p_edit);
  void AdjustAuthentication();
  void PresetBasicAuthentication();

  TestStepNET* m_testStep   { nullptr };
  Parameters* m_parameters { nullptr };
  
  StyleComboBox   m_comboType;
  StyleEdit       m_editUsername;
  StyleEdit       m_editPassword;
  StyleComboBox   m_comboGrant;
  StyleEdit       m_editTokenServer;
  StyleEdit       m_editClientID;
  StyleEdit       m_editClientKey;
  StyleEdit       m_editClientScope;
  StyleEdit       m_editBearerToken;
  StyleButton     m_buttonRefresh;
  StyleButton     m_buttonUsernameParm;
  StyleButton     m_buttonPasswordParm;
  StyleButton     m_buttonTokenServerParm;
  StyleButton     m_buttonClientIDParm;
  StyleButton     m_buttonClientKeyParm;
  StyleButton     m_buttonClientScopeParm;

  CString         m_authType;
  CString         m_userName;
  CString         m_password;
  CString         m_oauthGrant;
  CString         m_tokenServer;
  CString         m_clientID;
  CString         m_clientKey;
  CString         m_clientScope;
  CString         m_bearerToken;

	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnTimer(UINT_PTR p_timer);
  afx_msg void OnCbnSelchangeAuthType();
  afx_msg void OnEnChangeUsername();
  afx_msg void OnEnChangePassword();
  afx_msg void OnCbnSelchangeGrant();
  afx_msg void OnEnChangeTokenServer();
  afx_msg void OnEnChangeClientid();
  afx_msg void OnEnChangeClientkey();
  afx_msg void OnEnChangeClientscope();
  afx_msg void OnEnChangeBearertoken();
  afx_msg void OnBnClickedRefresh();

  afx_msg void OnBnClickedUsernameParm();
  afx_msg void OnBnClickedPasswordParm();
  afx_msg void OnBnClickedTokenParm();
  afx_msg void OnBnClickedClientIDParm();
  afx_msg void OnBnClickedClientKeyParm();
  afx_msg void OnBnClickedClientScopeParm();
};

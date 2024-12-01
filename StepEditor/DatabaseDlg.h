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
// This File   : DatabaseDlg.h
// What it does: Authentications for an HTTP internet call
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "afxdialogex.h"

class TestStepSQL;
class Parameters;
class Credentials;

// DatabaseDlg dialog

class DatabaseDlg : public StyleTab
{
  DECLARE_DYNAMIC(DatabaseDlg)

public:
  DatabaseDlg(CWnd* pParent = nullptr);   // standard constructor
  virtual ~DatabaseDlg();

  void InitTab(TestStepSQL* p_testStep
              ,Parameters*  p_parameters
              ,Credentials* p_credentials);
  void StoreVariables();
  void EffectiveParameters();
  bool IsFilled();
  void SaveCredentials();
  void DeleteCredentials();
  void SetCredentials();

// Dialog Data
#ifdef AFX_DESIGN_TIME
  enum { IDD = IDD_DATABASE };
#endif

protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual BOOL OnInitDialog() override;
          void InitButtons();
          void InitCombo();

  TestStepSQL*  m_testStep   { nullptr };
  Parameters*   m_parameters { nullptr };
  Credentials*  m_credentials{ nullptr };

  CString       m_credential;
  CString       m_datasource;
  CString       m_user;
  CString       m_password;
  CString       m_effDatasource;
  CString       m_effUser;
  CString       m_effPassword;

  StyleComboBox m_comboCredential;
  StyleButton   m_buttonSave;
  StyleButton   m_buttonDelete;
  StyleEdit     m_editDatasource;
  StyleEdit     m_editUser;
  StyleEdit     m_editPassword;
  StyleButton   m_buttonDatasource;
  StyleButton   m_buttonUser;
  StyleButton   m_buttonPassword;
  StyleEdit     m_editEffDatasource;
  StyleEdit     m_editEffUser;
  StyleEdit     m_editEffPassword;

  DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnCbnSelChangeCredential();
  afx_msg void OnBnClickedSave();
  afx_msg void OnBnClickedDelete();
  afx_msg void OnEnChangeDatasource();
  afx_msg void OnBnClickedButDatasource();
  afx_msg void OnEnChangeEffDatasource();
  afx_msg void OnEnChangeUser();
  afx_msg void OnBnClickedButUser();
  afx_msg void OnEnChangeEffUser();
  afx_msg void OnEnChangePassword();
  afx_msg void OnBnClickedButPassword();
  afx_msg void OnEnChangeEffPassword();
};

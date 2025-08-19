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
// This File   : AuthenticateDlg.cpp
// What it does: Authentications for an HTTP internet call
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StepEditor.h"
#include "AuthenticateDlg.h"
#include "StepInternetDlg.h"
#include <TestStepNET.h>
#include <StepResultNET.h>
#include <SearchVarDlg.h>
#include <afxdialogex.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// AuthenticateDlg dialog

IMPLEMENT_DYNAMIC(AuthenticateDlg,StyleTab)

AuthenticateDlg::AuthenticateDlg(CWnd* pParent /*=nullptr*/)
	              :StyleTab(IDD_AUTHENTICATE, pParent)
{
}

AuthenticateDlg::~AuthenticateDlg()
{
}

void AuthenticateDlg::DoDataExchange(CDataExchange* pDX)
{
	StyleTab::DoDataExchange(pDX);

  DDX_CBString(pDX,IDC_IDENTIFIER,  m_comboIdentifier,m_identifier);
  DDX_CBString(pDX,IDC_AUTH_TYPE,   m_comboType,      m_authType);
  DDX_Control (pDX,IDC_AUTHSAVE,    m_buttonSave);
  DDX_Control (pDX,IDC_AUTHDELETE,  m_buttonDelete);
  DDX_Control (pDX,IDC_USERNAME,    m_editUsername,   m_userName);
  DDX_Control (pDX,IDC_PASSWORD,    m_editPassword,   m_password);
  DDX_CBString(pDX,IDC_GRANT,       m_comboGrant,     m_oauthGrant);
  DDX_Control (pDX,IDC_REFRESH,     m_buttonRefresh);

  DDX_Control (pDX,IDC_TOKENSERVER, m_editTokenServer,m_tokenServer);
  DDX_Control (pDX,IDC_CLIENTID,    m_editClientID,   m_clientID);
  DDX_Control (pDX,IDC_CLIENTKEY,   m_editClientKey,  m_clientKey);
  DDX_Control (pDX,IDC_CLIENTSCOPE, m_editClientScope,m_clientScope);
  DDX_Control (pDX,IDC_BEARERTOKEN, m_editBearerToken,m_bearerToken);

  DDX_Control (pDX,IDC_USER_PARM,   m_buttonUsernameParm);
  DDX_Control (pDX,IDC_PWD_PARM,    m_buttonPasswordParm);
  DDX_Control (pDX,IDC_TOKEN_PARM,  m_buttonTokenServerParm);
  DDX_Control (pDX,IDC_ID_PARM,     m_buttonClientIDParm);
  DDX_Control (pDX,IDC_KEY_PARM,    m_buttonClientKeyParm);
  DDX_Control (pDX,IDC_SCOPE_PARM,  m_buttonClientScopeParm);

  if(pDX->m_bSaveAndValidate == FALSE)
  {
    CredType auth = CalcAuthenticationType();

    m_editUsername   .EnableWindow(auth == CredType::BASIC || auth == CredType::NTLM);
    m_editPassword   .EnableWindow(auth == CredType::BASIC || auth == CredType::NTLM);
    m_comboGrant     .EnableWindow(auth == CredType::OAUTH2);
    m_editTokenServer.EnableWindow(auth == CredType::OAUTH2);
    m_editClientID   .EnableWindow(auth == CredType::OAUTH2);
    m_editClientKey  .EnableWindow(auth == CredType::OAUTH2);
    m_editClientScope.EnableWindow(auth == CredType::OAUTH2);
    m_editBearerToken.EnableWindow(auth == CredType::OAUTH2);
    m_buttonRefresh  .EnableWindow(auth == CredType::OAUTH2);

    m_buttonUsernameParm   .EnableWindow(auth == CredType::BASIC || auth == CredType::NTLM);
    m_buttonPasswordParm   .EnableWindow(auth == CredType::BASIC || auth == CredType::NTLM);
    m_buttonTokenServerParm.EnableWindow(auth == CredType::OAUTH2);
    m_buttonClientIDParm   .EnableWindow(auth == CredType::OAUTH2);
    m_buttonClientKeyParm  .EnableWindow(auth == CredType::OAUTH2);
    m_buttonClientScopeParm.EnableWindow(auth == CredType::OAUTH2);

    m_buttonSave  .EnableWindow(!m_identifier.IsEmpty());
    m_buttonDelete.EnableWindow(!m_identifier.IsEmpty());
  }
}

BEGIN_MESSAGE_MAP(AuthenticateDlg, StyleTab)
  ON_WM_TIMER()
  ON_CBN_KILLFOCUS(IDC_IDENTIFIER,  &AuthenticateDlg::OnCbnSelchangeIdentifier)
  ON_CBN_SELCHANGE(IDC_IDENTIFIER,  &AuthenticateDlg::OnCbnSelchangeIdentifier)
  ON_CBN_SELCHANGE(IDC_AUTH_TYPE,   &AuthenticateDlg::OnCbnSelchangeAuthType)
  ON_EN_KILLFOCUS (IDC_USERNAME,    &AuthenticateDlg::OnEnChangeUsername)
  ON_EN_KILLFOCUS (IDC_PASSWORD,    &AuthenticateDlg::OnEnChangePassword)
  ON_CBN_SELCHANGE(IDC_GRANT,       &AuthenticateDlg::OnCbnSelchangeGrant)
  ON_EN_KILLFOCUS (IDC_TOKENSERVER, &AuthenticateDlg::OnEnChangeTokenServer)
  ON_EN_KILLFOCUS (IDC_CLIENTID,    &AuthenticateDlg::OnEnChangeClientid)
  ON_EN_KILLFOCUS (IDC_CLIENTKEY,   &AuthenticateDlg::OnEnChangeClientkey)
  ON_EN_KILLFOCUS (IDC_CLIENTSCOPE, &AuthenticateDlg::OnEnChangeClientscope)
  ON_EN_KILLFOCUS (IDC_BEARERTOKEN, &AuthenticateDlg::OnEnChangeBearertoken)
  
  ON_BN_CLICKED   (IDC_AUTHSAVE,    &AuthenticateDlg::OnBnClickedSave)
  ON_BN_CLICKED   (IDC_AUTHDELETE,  &AuthenticateDlg::OnBnClickedDelete)
  ON_BN_CLICKED   (IDC_REFRESH,     &AuthenticateDlg::OnBnClickedRefresh)
  ON_BN_CLICKED   (IDC_USER_PARM,   &AuthenticateDlg::OnBnClickedUsernameParm)
  ON_BN_CLICKED   (IDC_PWD_PARM,    &AuthenticateDlg::OnBnClickedPasswordParm)
  ON_BN_CLICKED   (IDC_TOKEN_PARM,  &AuthenticateDlg::OnBnClickedTokenParm)
  ON_BN_CLICKED   (IDC_ID_PARM,     &AuthenticateDlg::OnBnClickedClientIDParm)
  ON_BN_CLICKED   (IDC_KEY_PARM,    &AuthenticateDlg::OnBnClickedClientKeyParm)
  ON_BN_CLICKED   (IDC_SCOPE_PARM,  &AuthenticateDlg::OnBnClickedClientScopeParm)
END_MESSAGE_MAP()

BOOL
AuthenticateDlg::OnInitDialog()
{
  StyleTab::OnInitDialog();

  m_editBearerToken.SetMutable(false);
  m_editPassword.SetPassword(true);

  InitButtons();
  InitCombos();
  SetCanResize();

  // Check bearer token every second
  SetTimer(1,1000,NULL);

  return FALSE;
}

void
AuthenticateDlg::SetupDynamicLayout()
{
  StyleTab::SetupDynamicLayout();

  CMFCDynamicLayout& manager = *GetDynamicLayout();
#ifdef _DEBUG
  manager.AssertValid();
#endif

  manager.AddItem(IDC_GRP_EFFECTIVE, CMFCDynamicLayout::MoveNone(),         CMFCDynamicLayout::SizeHorizontal(100));
  manager.AddItem(IDC_USERNAME,      CMFCDynamicLayout::MoveNone(),         CMFCDynamicLayout::SizeHorizontal(100));
  manager.AddItem(IDC_USER_PARM,     CMFCDynamicLayout::MoveHorizontal(100),CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_PASSWORD,      CMFCDynamicLayout::MoveNone(),         CMFCDynamicLayout::SizeHorizontal(100));
  manager.AddItem(IDC_PWD_PARM,      CMFCDynamicLayout::MoveHorizontal(100),CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_GRP_OAUTH,     CMFCDynamicLayout::MoveNone(),         CMFCDynamicLayout::SizeHorizontal(100));
  manager.AddItem(IDC_TOKENSERVER,   CMFCDynamicLayout::MoveNone(),         CMFCDynamicLayout::SizeHorizontal(100));
  manager.AddItem(IDC_TOKEN_PARM,    CMFCDynamicLayout::MoveHorizontal(100),CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_CLIENTID,      CMFCDynamicLayout::MoveNone(),         CMFCDynamicLayout::SizeHorizontal(100));
  manager.AddItem(IDC_ID_PARM,       CMFCDynamicLayout::MoveHorizontal(100),CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_CLIENTKEY,     CMFCDynamicLayout::MoveNone(),         CMFCDynamicLayout::SizeHorizontal(100));
  manager.AddItem(IDC_KEY_PARM,      CMFCDynamicLayout::MoveHorizontal(100),CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_CLIENTSCOPE,   CMFCDynamicLayout::MoveNone(),         CMFCDynamicLayout::SizeHorizontal(100));
  manager.AddItem(IDC_SCOPE_PARM,    CMFCDynamicLayout::MoveHorizontal(100),CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_BEARERTOKEN,   CMFCDynamicLayout::MoveNone(),         CMFCDynamicLayout::SizeHorizontal(100));
}

void
AuthenticateDlg::InitButtons()
{
  EnableToolTips();

  m_buttonUsernameParm   .SetIconImage(IDI_LIST);
  m_buttonPasswordParm   .SetIconImage(IDI_LIST);
  m_buttonTokenServerParm.SetIconImage(IDI_LIST);
  m_buttonClientIDParm   .SetIconImage(IDI_LIST);
  m_buttonClientKeyParm  .SetIconImage(IDI_LIST);
  m_buttonClientScopeParm.SetIconImage(IDI_LIST);

  RegisterTooltip(m_buttonUsernameParm,   _T("Choose global/test parameter for the username"));
  RegisterTooltip(m_buttonPasswordParm,   _T("Choose global/test parameter for the password"));
  RegisterTooltip(m_buttonTokenServerParm,_T("Choose global/test parameter for the OAuth2 token server"));
  RegisterTooltip(m_buttonClientIDParm,   _T("Choose global/test parameter for the client ID"));
  RegisterTooltip(m_buttonClientKeyParm,  _T("Choose global/test parameter for the client KEY"));
  RegisterTooltip(m_buttonClientScopeParm,_T("Choose global/test parameter for the client Scope"));
}

void
AuthenticateDlg::InitCombos()
{
  m_comboType.ResetContent();
  m_comboType.AddString(_T("Anonymous"));
  m_comboType.AddString(_T("Basic authentication"));
  m_comboType.AddString(_T("NTLM Single-signon"));
  m_comboType.AddString(_T("NTLM Logon"));
  m_comboType.AddString(_T("OAuth2"));

  m_comboGrant.ResetContent();
  m_comboGrant.AddString(_T("Client credentials grant"));
  m_comboGrant.AddString(_T("User credentials grant"));

  // Add all named credential sets
  if(m_credentials)
  {
    CredentialMap& map = m_credentials->GetAllCredentials();
    m_comboIdentifier.ResetContent();
    m_comboIdentifier.AddString(_T(""));
    for(auto& cred : map)
    {
      m_comboIdentifier.AddString(cred.first);
    }
  }
}

void 
AuthenticateDlg::InitTab(TestStepNET* p_step
                        ,Parameters*  p_parameters
                        ,Credentials* p_credentials)
{
  m_testStep    = p_step;
  m_parameters  = p_parameters;
  m_credentials = p_credentials;

  m_authType    = m_testStep->GetAuthType();
  m_userName    = m_testStep->GetAuthUser();
  m_password    = m_testStep->GetAuthPassword();
  m_oauthGrant  = m_testStep->GetAuthGrant();
  m_tokenServer = m_testStep->GetAuthTokenServer();
  m_clientID    = m_testStep->GetAuthClientID();
  m_clientKey   = m_testStep->GetAuthClientKey();
  m_clientScope = m_testStep->GetAuthClientScope();
  m_bearerToken = m_testStep->GetAuthBearerToken();

  InitCombos();

  int ind = m_comboType.FindStringExact(0,m_authType);
  if(ind >= 0) m_comboType.SetCurSel(ind);
  ind = m_comboGrant.FindStringExact(0,m_oauthGrant);
  if(ind >= 0) m_comboGrant.SetCurSel(ind);

  m_identifier = m_testStep->GetCredential();
  if(!m_identifier.IsEmpty())
  {
    SetCredentials();
  }
  UpdateData(FALSE);
}

bool
AuthenticateDlg::IsFilled()
{
  return !m_authType.IsEmpty();
}

void 
AuthenticateDlg::StoreVariables()
{
  m_testStep->SetAuthType(m_authType);
  m_testStep->SetAuthUser(m_userName);
  m_testStep->SetAuthPassword(m_password);
  m_testStep->SetAuthGrant(m_oauthGrant);
  m_testStep->SetAuthTokenServer(m_tokenServer);
  m_testStep->SetAuthClientID(m_clientID);
  m_testStep->SetAuthClientKey(m_clientKey);
  m_testStep->SetAuthClientScope(m_clientScope);
  m_testStep->SetAuthBearerToken(m_bearerToken);
}

void 
AuthenticateDlg::SetResult(CString p_token)
{
  if(!p_token.IsEmpty() && p_token.Compare(m_bearerToken))
  {
    m_bearerToken = p_token;
    m_editBearerToken.SetBkColor(RGB(128,255,128));
    UpdateData(FALSE);
  }
}

void 
AuthenticateDlg::OnTimer(UINT_PTR p_timer)
{
  if(p_timer == 1)
  {
    if(!m_bearerToken.IsEmpty())
    {
      if(theApp.GetBearerTokenIsExpired())
      {
        m_editBearerToken.SetBkColor(RGB(255,128,128));
      }
    }
  }
}

void
AuthenticateDlg::EffectiveParameters()
{
  if(m_testStep)
  {
    // Check parameters
    StepInternetDlg* step = reinterpret_cast<StepInternetDlg*>(GetParent()->GetParent());
    step->EffectiveParameters();
  }
}

CredType
AuthenticateDlg::CalcAuthenticationType()
{
  if(m_authType.CompareNoCase(_T("Anonymous"))            == 0) return CredType::ANONYMOUS;
  if(m_authType.CompareNoCase(_T("Basic authentication")) == 0) return CredType::BASIC;
  if(m_authType.CompareNoCase(_T("NTLM Single-signon"))   == 0) return CredType::NTLM_SSO;
  if(m_authType.CompareNoCase(_T("NTLM Logon"))           == 0) return CredType::NTLM;
  if(m_authType.CompareNoCase(_T("OAuth2"))               == 0) return CredType::OAUTH2;
 
  return CredType::ANONYMOUS;
}

void 
AuthenticateDlg::AdjustAuthentication()
{
  CredType type = CalcAuthenticationType();
  switch (type)
  {
    case CredType::NTLM_SSO:  // Fall through
    case CredType::OAUTH2:    // Fall through
    case CredType::ANONYMOUS: m_userName.Empty();
                              m_password.Empty();
                              break;
    case CredType::BASIC:     PresetBasicAuthentication();
                              break;
    case CredType::NTLM:      break;
  }
}

void 
AuthenticateDlg::PresetBasicAuthentication()
{
  if(m_userName.IsEmpty())
  {
    m_userName = _T("$user$");
  }
  if(m_password.IsEmpty())
  {
    m_password = _T("$password$");
  }
}

void
AuthenticateDlg::ChooseVariable(StyleEdit& p_edit)
{
  AutoFocus focus;
  SearchVarDlg dlg(this,m_parameters,true,true,true,true);
  if(dlg.DoModal() == IDOK || dlg.GetSaved())
  {
    CString variable = dlg.GetVariable();
    p_edit.InsertAtCurPos(variable,0);
    UpdateData();
  }
}

void
AuthenticateDlg::SaveCredentials()
{
  UpdateData();
  if(m_identifier.IsEmpty())
  {
    return;
  }
  if(StyleMessageBox(this,_T("Do you want to save the current credentials to: ") + m_identifier,PRODUCT_NAME,MB_YESNO|MB_DEFBUTTON2|MB_ICONQUESTION) == IDNO)
  {
    return;
  }
  Credential* cred = m_credentials->FindCredential(m_identifier);
  if(cred)
  {
    m_credentials->DeleteCredentials(m_identifier);
  }

  CredType type = CalcAuthenticationType();
  switch(type)
  {
    case CredType::BASIC:    m_credentials->SetBasicCredential(m_identifier,m_userName,m_password);
                             break;
    case CredType::NTLM:     [[fallthrough]];
    case CredType::NTLM_SSO: m_credentials->SetWNTLMCredential(m_identifier,m_userName,m_password);
                             break;
    case CredType::OAUTH2:   m_credentials->SetOAuthCredential(m_identifier,m_oauthGrant,m_tokenServer,m_clientID,m_clientKey,m_clientScope);
                             break;
  }
}

void
AuthenticateDlg::DeleteCredentials()
{
  if(m_identifier.IsEmpty())
  {
    return;
  }
  Credential* cred = m_credentials->FindCredential(m_identifier);
  if(cred)
  {
    if(StyleMessageBox(this,_T("Do you want to delete the credential set: ") + m_identifier,PRODUCT_NAME,MB_YESNO|MB_DEFBUTTON2|MB_ICONQUESTION) == IDYES)
    {
      m_credentials->DeleteCredentials(m_identifier);
      int ind = m_comboIdentifier.FindStringExact(0,m_identifier);
      if(ind >= 0)
      {
        m_comboIdentifier.DeleteString(ind);
      }
      // Reset the type
      m_authType.Empty();
      m_comboType.SetCurSel(0);
      // Reset all fields
      m_userName.Empty();
      m_password.Empty();
      m_oauthGrant.Empty();
      m_tokenServer.Empty();
      m_clientID.Empty();
      m_clientKey.Empty();
      m_clientScope.Empty();
      m_bearerToken.Empty();
      UpdateData(FALSE);
    }
  }
}

void
AuthenticateDlg::SetCredentials()
{
  Credential* cred = m_credentials->FindCredential(m_identifier);
  if(cred == nullptr)
  {
    return;
  }
  // Forget last bearer token
  m_bearerToken.Empty();

  // Replace these
  m_userName    = cred->m_username;
  m_password    = cred->m_password;
  m_oauthGrant  = cred->m_oauthGrant;
  m_tokenServer = cred->m_tokenServer;
  m_clientID    = cred->m_clientID;
  m_clientKey   = cred->m_clientKey;
  m_clientScope = cred->m_clientScope;

  int ind = m_comboType.FindString(0,cred->m_typeName);
  if(ind >= 0)
  {
    m_comboType.SetCurSel(ind);
    m_comboType.GetLBText(ind,m_authType);
  }
  ind = m_comboIdentifier.FindStringExact(0,m_identifier);
  if(ind >= 0)
  {
    m_comboIdentifier.SetCurSel(ind);
  }
  ind = m_comboGrant.FindStringExact(0,m_oauthGrant);
  if(ind >= 0)
  {
    m_comboGrant.SetCurSel(ind);
  }
}

//////////////////////////////////////////////////////////////////////////
//
// AuthenticateDlg message handlers
//
//////////////////////////////////////////////////////////////////////////

void
AuthenticateDlg::OnCbnSelchangeIdentifier()
{
  CString identifier(m_identifier);
  UpdateData();

  if(identifier.IsEmpty() && !m_identifier.IsEmpty() &&
    (!m_userName.IsEmpty() || !m_password.IsEmpty()  || !m_oauthGrant.IsEmpty() || !m_tokenServer.IsEmpty() ||
     !m_clientID.IsEmpty() || !m_clientKey.IsEmpty() || !m_clientScope.IsEmpty()))
  {
    if(StyleMessageBox(this,_T("Current credentials are not stored\n")
                            _T("Would you like to store them first, before selecting another set?")
                           ,PRODUCT_NAME,MB_YESNO|MB_DEFBUTTON1|MB_ICONQUESTION) == IDYES)
    {
      return;
    }
  }
  // Remember it in the test step
  m_testStep->SetCredential(m_identifier);

  // Show the new set
  SetCredentials();
  UpdateData(FALSE);
}

void 
AuthenticateDlg::OnCbnSelchangeAuthType()
{
  UpdateData();
  AdjustAuthentication();
  UpdateData(FALSE);
}

void
AuthenticateDlg::OnBnClickedSave()
{
  SaveCredentials();
  InitCombos();
  SetCredentials();
}

void 
AuthenticateDlg::OnBnClickedDelete()
{
  UpdateData();
  DeleteCredentials();
  InitCombos();
}

void
AuthenticateDlg::OnEnChangeUsername()
{
  UpdateData();
}

void
AuthenticateDlg::OnBnClickedUsernameParm()
{
  ChooseVariable(m_editUsername);
}

void
AuthenticateDlg::OnEnChangePassword()
{
  UpdateData();
}

void
AuthenticateDlg::OnBnClickedPasswordParm()
{
  ChooseVariable(m_editPassword);
}

void 
AuthenticateDlg::OnEnChangeTokenServer()
{
  UpdateData();
}

void
AuthenticateDlg::OnBnClickedTokenParm()
{
  ChooseVariable(m_editTokenServer);
}

void 
AuthenticateDlg::OnCbnSelchangeGrant()
{
  // TODO: Add your control notification handler code here
}

void 
AuthenticateDlg::OnEnChangeClientid()
{
  UpdateData();
}

void
AuthenticateDlg::OnBnClickedClientIDParm()
{
  ChooseVariable(m_editClientID);
}

void 
AuthenticateDlg::OnEnChangeClientkey()
{
  UpdateData();
}

void
AuthenticateDlg::OnBnClickedClientKeyParm()
{
  ChooseVariable(m_editClientKey);
}

void
AuthenticateDlg::OnEnChangeClientscope()
{
  UpdateData();
}

void
AuthenticateDlg::OnBnClickedClientScopeParm()
{
  ChooseVariable(m_editClientScope);
}


void 
AuthenticateDlg::OnEnChangeBearertoken()
{
  UpdateData();
}

void
AuthenticateDlg::OnBnClickedRefresh()
{
  SetResult(theApp.RefreshBearerToken());
}

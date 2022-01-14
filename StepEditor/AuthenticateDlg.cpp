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
#include <TestStepIN.h>
#include <StepResultIN.h>
#include <SearchVarDlg.h>
#include <afxdialogex.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// AuthenticateDlg dialog

IMPLEMENT_DYNAMIC(AuthenticateDlg, StyleDialog)

AuthenticateDlg::AuthenticateDlg(CWnd* pParent /*=nullptr*/)
	              :StyleDialog(IDD_AUTHENTICATE, pParent)
{
}

AuthenticateDlg::~AuthenticateDlg()
{
}

void AuthenticateDlg::DoDataExchange(CDataExchange* pDX)
{
	StyleDialog::DoDataExchange(pDX);

  DDX_CBString(pDX,IDC_AUTH_TYPE,   m_comboType,      m_authType);
  DDX_Control (pDX,IDC_USERNAME,    m_editUsername,   m_userName);
  DDX_Control (pDX,IDC_PASSWORD,    m_editPassword,   m_password);
  DDX_CBString(pDX,IDC_GRANT,       m_comboGrant,     m_oauthGrant);
  DDX_Control (pDX,IDC_TOKENSERVER, m_editTokenServer,m_tokenServer);
  DDX_Control (pDX,IDC_CLIENTID,    m_editClientID,   m_clientID);
  DDX_Control (pDX,IDC_CLIENTKEY,   m_editClientKey,  m_clientKey);
  DDX_Control (pDX,IDC_CLIENTSCOPE, m_editClientScope,m_clientScope);
  DDX_Control (pDX,IDC_BEARERTOKEN, m_editBearerToken,m_bearerToken);

  DDX_Control (pDX,IDC_REFRESH,     m_buttonRefresh);

  DDX_Control (pDX,IDC_USER_PARM,   m_buttonUsernameParm);
  DDX_Control (pDX,IDC_PWD_PARM,    m_buttonPasswordParm);
  DDX_Control (pDX,IDC_TOKEN_PARM,  m_buttonTokenServerParm);
  DDX_Control (pDX,IDC_ID_PARM,     m_buttonClientIDParm);
  DDX_Control (pDX,IDC_KEY_PARM,    m_buttonClientKeyParm);
  DDX_Control (pDX,IDC_SCOPE_PARM,  m_buttonClientScopeParm);

  if(pDX->m_bSaveAndValidate == FALSE)
  {
    int auth = CalcAuthenticationType();

    m_editUsername   .EnableWindow(auth == AUTH_BASIC || auth == AUTH_NTLM);
    m_editPassword   .EnableWindow(auth == AUTH_BASIC || auth == AUTH_NTLM);
    m_comboGrant     .EnableWindow(auth == AUTH_OAUTH);
    m_editTokenServer.EnableWindow(auth == AUTH_OAUTH);
    m_editClientID   .EnableWindow(auth == AUTH_OAUTH);
    m_editClientKey  .EnableWindow(auth == AUTH_OAUTH);
    m_editClientScope.EnableWindow(auth == AUTH_OAUTH);
    m_editBearerToken.EnableWindow(auth == AUTH_OAUTH);
    m_buttonRefresh  .EnableWindow(auth == AUTH_OAUTH);

    m_buttonUsernameParm   .EnableWindow(auth == AUTH_BASIC || auth == AUTH_NTLM);
    m_buttonPasswordParm   .EnableWindow(auth == AUTH_BASIC || auth == AUTH_NTLM);
    m_buttonTokenServerParm.EnableWindow(auth == AUTH_OAUTH);
    m_buttonClientIDParm   .EnableWindow(auth == AUTH_OAUTH);
    m_buttonClientKeyParm  .EnableWindow(auth == AUTH_OAUTH);
    m_buttonClientScopeParm.EnableWindow(auth == AUTH_OAUTH);
  }
}

BEGIN_MESSAGE_MAP(AuthenticateDlg, StyleDialog)
  ON_WM_TIMER()
  ON_CBN_SELCHANGE(IDC_AUTH_TYPE,   &AuthenticateDlg::OnCbnSelchangeAuthType)
  ON_EN_KILLFOCUS (IDC_USERNAME,    &AuthenticateDlg::OnEnChangeUsername)
  ON_EN_KILLFOCUS (IDC_PASSWORD,    &AuthenticateDlg::OnEnChangePassword)
  ON_CBN_SELCHANGE(IDC_GRANT,       &AuthenticateDlg::OnCbnSelchangeGrant)
  ON_EN_KILLFOCUS (IDC_TOKENSERVER, &AuthenticateDlg::OnEnChangeTokenServer)
  ON_EN_KILLFOCUS (IDC_CLIENTID,    &AuthenticateDlg::OnEnChangeClientid)
  ON_EN_KILLFOCUS (IDC_CLIENTKEY,   &AuthenticateDlg::OnEnChangeClientkey)
  ON_EN_KILLFOCUS (IDC_CLIENTSCOPE, &AuthenticateDlg::OnEnChangeClientscope)
  ON_EN_KILLFOCUS (IDC_BEARERTOKEN, &AuthenticateDlg::OnEnChangeBearertoken)
  
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
  StyleDialog::OnInitDialog();

  m_editBearerToken.SetMutable(false);

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
  StyleDialog::SetupDynamicLayout();

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

  RegisterTooltip(m_buttonUsernameParm,   "Choose global/test parameter for the username");
  RegisterTooltip(m_buttonPasswordParm,   "Choose global/test parameter for the password");
  RegisterTooltip(m_buttonTokenServerParm,"Choose global/test parameter for the OAuth2 token server");
  RegisterTooltip(m_buttonClientIDParm,   "Choose global/test parameter for the client ID");
  RegisterTooltip(m_buttonClientKeyParm,  "Choose global/test parameter for the client KEY");
  RegisterTooltip(m_buttonClientScopeParm,"Choose global/test parameter for the client Scope");
}

void
AuthenticateDlg::InitCombos()
{
  m_comboType.AddString("Anonymous");
  m_comboType.AddString("Basic authentication");
  m_comboType.AddString("NTLM Single-signon");
  m_comboType.AddString("NTLM");
  m_comboType.AddString("OAuth2");

  m_comboGrant.AddString("Client credentials grant");
  m_comboGrant.AddString("User credentials grant");
}

void 
AuthenticateDlg::InitTab(TestStepIN* p_step,Parameters* p_parameters)
{
  m_testStep   = p_step;
  m_parameters = p_parameters;

  m_authType    = m_testStep->GetAuthType();
  m_userName    = m_testStep->GetAuthUser();
  m_password    = m_testStep->GetAuthPassword();
  m_oauthGrant  = m_testStep->GetAuthGrant();
  m_tokenServer = m_testStep->GetAuthTokenServer();
  m_clientID    = m_testStep->GetAuthClientID();
  m_clientKey   = m_testStep->GetAuthClientKey();
  m_clientScope = m_testStep->GetAuthClientScope();
  m_bearerToken = m_testStep->GetAuthBearerToken();

  int ind = m_comboType.FindStringExact(0,m_authType);
  if(ind >= 0) m_comboType.SetCurSel(ind);
  ind = m_comboGrant.FindStringExact(0,m_oauthGrant);
  if(ind >= 0) m_comboGrant.SetCurSel(ind);

  UpdateData(FALSE);
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

int  
AuthenticateDlg::CalcAuthenticationType()
{
  if(m_authType.CompareNoCase("Anonymous")            == 0) return AUTH_ANONYMOUS;
  if(m_authType.CompareNoCase("Basic authentication") == 0) return AUTH_BASIC;
  if(m_authType.CompareNoCase("NTLM Single-signon")   == 0) return AUTH_NTLM_SSO;
  if(m_authType.CompareNoCase("NTLM")                 == 0) return AUTH_NTLM;
  if(m_authType.CompareNoCase("OAuth2")               == 0) return AUTH_OAUTH;
 
  return 0;
}

void 
AuthenticateDlg::AdjustAuthentication()
{
  int type = CalcAuthenticationType();
  switch (type)
  {
    case AUTH_NTLM_SSO:   // Fall through
    case AUTH_OAUTH:      // Fall through
    case AUTH_ANONYMOUS:  m_userName.Empty();
                          m_password.Empty();
                          break;
    case AUTH_BASIC:      break;
    case AUTH_NTLM:       break;
  }
}

void
AuthenticateDlg::ChooseVariable(StyleEdit& p_edit)
{
  SearchVarDlg dlg(m_parameters,true,true,true);
  if(dlg.DoModal() == IDOK || dlg.GetSaved())
  {
    CString var = dlg.GetChosenVariable();
    CString variable;
    switch (dlg.GetResultType())
    {
      case ParType::PAR_GLOBAL: variable.Format("$%s$",var.GetString()); break;
      case ParType::PAR_RETURN: variable.Format("[%s]",var.GetString()); break;
      case ParType::PAR_BUFFER: variable.Format("<%s>",var.GetString()); break;
    }
    p_edit.InsertAtCurPos(variable,0);
    UpdateData();
  }
}

// AuthenticateDlg message handlers

void 
AuthenticateDlg::OnCbnSelchangeAuthType()
{
  UpdateData();
  AdjustAuthentication();
  UpdateData(FALSE);
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

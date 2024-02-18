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
// This File   : DatabaseDlg.cpp
// What it does: Authentications for an HTTP internet call
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StepEditor.h"
#include "DatabaseDlg.h"
#include "StepDatabaseDlg.h"
#include "TestStepSQL.h"
#include <SearchVarDlg.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// DatabaseDlg dialog

IMPLEMENT_DYNAMIC(DatabaseDlg,StyleTab)

DatabaseDlg::DatabaseDlg(CWnd* p_parent)
	          :StyleTab(IDD_DATABASE,p_parent)
{
}

DatabaseDlg::~DatabaseDlg()
{
}

void DatabaseDlg::DoDataExchange(CDataExchange* pDX)
{
	StyleTab::DoDataExchange(pDX);

	DDX_Control(pDX,IDC_DATASOURCE,			m_editDatasource,   m_datasource);
	DDX_Control(pDX,IDC_BUT_DATASOURCE,	m_buttonDatasource);
	DDX_Control(pDX,IDC_EFF_DATASOURCE,	m_editEffDatasource,m_effDatasource);
	DDX_Control(pDX,IDC_USER,           m_editUser,					m_user);
	DDX_Control(pDX,IDC_BUT_USER,       m_buttonUser);
	DDX_Control(pDX,IDC_EFF_USER,       m_editEffUser,			m_effUser);
	DDX_Control(pDX,IDC_PASSWORD,       m_editPassword,     m_password);
	DDX_Control(pDX,IDC_BUT_PASSWORD,   m_buttonPassword);
	DDX_Control(pDX,IDC_EFF_PASSWORD,   m_editEffPassword,  m_effPassword);
}

BEGIN_MESSAGE_MAP(DatabaseDlg, StyleTab)
	ON_EN_KILLFOCUS(IDC_DATASOURCE,		  &DatabaseDlg::OnEnChangeDatasource)
	ON_BN_CLICKED  (IDC_BUT_DATASOURCE, &DatabaseDlg::OnBnClickedButDatasource)
	ON_EN_KILLFOCUS(IDC_EFF_DATASOURCE, &DatabaseDlg::OnEnChangeEffDatasource)
	ON_EN_KILLFOCUS(IDC_USER,						&DatabaseDlg::OnEnChangeUser)
	ON_BN_CLICKED  (IDC_BUT_USER,				&DatabaseDlg::OnBnClickedButUser)
	ON_EN_KILLFOCUS(IDC_EFF_USER,				&DatabaseDlg::OnEnChangeEffUser)
	ON_EN_KILLFOCUS(IDC_PASSWORD,				&DatabaseDlg::OnEnChangePassword)
	ON_BN_CLICKED  (IDC_BUT_PASSWORD,		&DatabaseDlg::OnBnClickedButPassword)
	ON_EN_KILLFOCUS(IDC_EFF_PASSWORD,		&DatabaseDlg::OnEnChangeEffPassword)
END_MESSAGE_MAP()

BOOL
DatabaseDlg::OnInitDialog()
{
	StyleTab::OnInitDialog();
	
	InitButtons();

	return TRUE;
}

void
DatabaseDlg::InitButtons()
{
  m_buttonDatasource.SetIconImage(IDI_LIST);
  m_buttonUser      .SetIconImage(IDI_LIST);
  m_buttonPassword  .SetIconImage(IDI_LIST);
}

void 
DatabaseDlg::InitTab(TestStepSQL* p_testStep, Parameters* p_parameters)
{
	// Register
	m_testStep   = p_testStep;
	m_parameters = p_parameters;

	// Set the parameters
	m_datasource		= p_testStep->GetDataSource();
	m_user					= p_testStep->GetUser();
	m_password			= p_testStep->GetPassword();

	UpdateData(FALSE);
	EffectiveParameters();
	UpdateData(FALSE);
}

bool
DatabaseDlg::IsFilled()
{
  return !m_datasource.IsEmpty();
}

void 
DatabaseDlg::StoreVariables()
{
	UpdateData();

	m_testStep->SetDatasource(m_datasource);
	m_testStep->SetUser(m_user);
	m_testStep->SetPassword(m_password);
}

void 
DatabaseDlg::EffectiveParameters()
{
  if(m_testStep)
  {
    // Check parameters
    StepDatabaseDlg* step = dynamic_cast<StepDatabaseDlg*>(GetParent()->GetParent());
		if(step)
		{
			step->EffectiveParameters();

			m_effDatasource = m_testStep->GetEffectiveDatasource();
			m_effUser       = m_testStep->GetEffectiveUser();
			m_effPassword   = m_testStep->GetEffectivePassword();
			UpdateData(FALSE);
		}
  }
}

// DatabaseDlg message handlers

void 
DatabaseDlg::OnEnChangeDatasource()
{
	UpdateData();
  EffectiveParameters();
}

void 
DatabaseDlg::OnBnClickedButDatasource()
{
  AutoFocus focus;
  SearchVarDlg dlg(this,m_parameters,true,true,true,true);
  if (dlg.DoModal() == IDOK || dlg.GetSaved())
  {
    CString variable = dlg.GetVariable();
    m_editDatasource.InsertAtCurPos(variable,0);
    UpdateData();
    EffectiveParameters();
  }
}


void 
DatabaseDlg::OnEnChangeEffDatasource()
{
  UpdateData();
}

void 
DatabaseDlg::OnEnChangeUser()
{
  UpdateData();
  EffectiveParameters();
}

void 
DatabaseDlg::OnBnClickedButUser()
{
  AutoFocus focus;
  SearchVarDlg dlg(this,m_parameters,true,true,true,true);
  if (dlg.DoModal() == IDOK || dlg.GetSaved())
  {
    CString variable = dlg.GetVariable();
    m_editUser.InsertAtCurPos(variable,0);
    UpdateData();
    EffectiveParameters();
  }
}

void 
DatabaseDlg::OnEnChangeEffUser()
{
	UpdateData();
}

void 
DatabaseDlg::OnEnChangePassword()
{
	UpdateData();
  EffectiveParameters();
}

void 
DatabaseDlg::OnBnClickedButPassword()
{
  AutoFocus focus;
  SearchVarDlg dlg(this,m_parameters,true,true,true,true);
  if (dlg.DoModal() == IDOK || dlg.GetSaved())
  {
    CString variable = dlg.GetVariable();
    m_editPassword.InsertAtCurPos(variable,0);
    UpdateData();
    EffectiveParameters();
   }
}

void 
DatabaseDlg::OnEnChangeEffPassword()
{
  UpdateData();
}

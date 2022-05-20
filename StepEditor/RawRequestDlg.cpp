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
// This File   : RawRequestDlg.cpp
// What it does: Resulting raw request of a HTTP internet test call
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StepEditor.h"
#include "TestStepNET.h"
#include "RawRequestDlg.h"
#include "StepInternetDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// RawRequestDlg dialog

IMPLEMENT_DYNAMIC(RawRequestDlg, StyleDialog)

RawRequestDlg::RawRequestDlg(CWnd* pParent /*=nullptr*/)
              :StyleDialog(IDD_RAW_REQUEST, pParent)
{
}

RawRequestDlg::~RawRequestDlg()
{
}

void 
RawRequestDlg::DoDataExchange(CDataExchange* pDX)
{
	StyleDialog::DoDataExchange(pDX);
  DDX_Control (pDX,IDC_RAW, m_editPayload,m_payload);
}

BEGIN_MESSAGE_MAP(RawRequestDlg, StyleDialog)
  ON_EN_KILLFOCUS (IDC_RAW, &RawRequestDlg::OnEnKillfocusPayload)
END_MESSAGE_MAP()

BOOL
RawRequestDlg::OnInitDialog()
{
  StyleDialog::OnInitDialog();

  InitPayload();

  SetCanResize();
  return TRUE;
}

void
RawRequestDlg::SetupDynamicLayout()
{
  StyleDialog::SetupDynamicLayout();

  CMFCDynamicLayout& manager = *GetDynamicLayout();
#ifdef _DEBUG
  manager.AssertValid();
#endif

  manager.AddItem(IDC_RAW,CMFCDynamicLayout::MoveNone(),CMFCDynamicLayout::SizeHorizontalAndVertical(100,100));
}

void
RawRequestDlg::InitPayload()
{
  m_editPayload.SetFontName("Courier new",100);
  m_editPayload.SetMutable(false);
}

void 
RawRequestDlg::InitTab(TestStepNET* p_testStep)
{
  if(p_testStep)
  {
    m_testStep = p_testStep;
  }
  m_payload = m_testStep->GetRawRequest();
  UpdateData(FALSE);
}

bool
RawRequestDlg::IsFilled()
{
  return !m_payload.IsEmpty();
}

void 
RawRequestDlg::StoreVariables()
{
  // Nothing to do
}

// RawRequestDlg message handlers

void 
RawRequestDlg::OnEnKillfocusPayload()
{
  // Getting the payload as body for the message
  UpdateData();

  if(m_testStep)
  {
    m_testStep->SetBody(m_payload);

    // Check parameters
    StepInternetDlg* step = reinterpret_cast<StepInternetDlg*>(GetParent()->GetParent());
    step->EffectiveParameters();
  }
}

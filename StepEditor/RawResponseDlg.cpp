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
// This File   : RawResponseDlg.cpp
// What it does: Resulting raw response of a HTTP internet test call
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StepEditor.h"
#include "TestStepNET.h"
#include "StepResultNET.h"
#include "RawResponseDlg.h"
#include "StepInternetDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// RawResponseDlg dialog

IMPLEMENT_DYNAMIC(RawResponseDlg,StyleTab)

RawResponseDlg::RawResponseDlg(CWnd* pParent /*=nullptr*/)
               :StyleTab(IDD_RAW_RESPONSE,pParent)
{
}

RawResponseDlg::~RawResponseDlg()
{
}

void
RawResponseDlg::DoDataExchange(CDataExchange* pDX)
{
  StyleTab::DoDataExchange(pDX);
  DDX_Control(pDX,IDC_RAW,m_editPayload,m_payload);
}

BEGIN_MESSAGE_MAP(RawResponseDlg,StyleTab)
  ON_EN_KILLFOCUS(IDC_RAW,&RawResponseDlg::OnEnKillfocusPayload)
END_MESSAGE_MAP()

BOOL
RawResponseDlg::OnInitDialog()
{
  StyleTab::OnInitDialog();

  InitPayload();

  SetCanResize();
  return TRUE;
}

void
RawResponseDlg::SetupDynamicLayout()
{
  StyleTab::SetupDynamicLayout();

  CMFCDynamicLayout& manager = *GetDynamicLayout();
#ifdef _DEBUG
  manager.AssertValid();
#endif

  manager.AddItem(IDC_RAW,CMFCDynamicLayout::MoveNone(),CMFCDynamicLayout::SizeHorizontalAndVertical(100,100));
}

void
RawResponseDlg::InitPayload()
{
  m_editPayload.SetFontName("Courier new",100);
  m_editPayload.SetMutable(false);
}

void
RawResponseDlg::InitTab()
{
  m_payload.Empty();
  UpdateData(FALSE);
}

void 
RawResponseDlg::SetResult(StepResultNET* p_result)
{
  m_payload = p_result->GetRawResponse();
  m_payload.Replace("\n","\r\n");
  UpdateData(FALSE);
}


void
RawResponseDlg::StoreVariables()
{
  // Nothing to do
}

// RawResponseDlg message handlers

void
RawResponseDlg::OnEnKillfocusPayload()
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

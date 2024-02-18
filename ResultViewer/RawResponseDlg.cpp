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
// This program: ResultViewer
// This File   : RawResponseDlg.cpp
// What it does: Shows raw response from the HTTP internet message 
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ResultViewer.h"
#include "TestStepNET.h"
#include "RawResponseDlg.h"
#include "ResultInternetDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// RawResponseDlg dialog

IMPLEMENT_DYNAMIC(RawResponseDlg,StyleTab)

RawResponseDlg::RawResponseDlg(CWnd* pParent /*=nullptr*/)
               :StyleTab(IDD_RAW_RESPONSE, pParent)
{
}

RawResponseDlg::~RawResponseDlg()
{
}

void 
RawResponseDlg::DoDataExchange(CDataExchange* pDX)
{
	StyleTab::DoDataExchange(pDX);
  DDX_Control (pDX,IDC_RAW, m_editPayload,m_payload);
}

BEGIN_MESSAGE_MAP(RawResponseDlg, StyleTab)
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
  m_editPayload.SetFontName(_T("Courier new"),90);
  m_editPayload.SetMutable(false);
}

void 
RawResponseDlg::LoadVariables(StepResultNET* p_result)
{
  if(p_result)
  {
    m_result = p_result;
  }
  m_payload = m_result->GetRawResponse();
  m_payload.Replace(_T("\n"),_T("\r\n"));
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

//   if(m_result)
//   {
//     m_result->SetBody(m_payload);
// 
//     // Check parameters
//     ResultInternetDlg* step = reinterpret_cast<ResultInternetDlg*>(GetParent()->GetParent());
//     step->EffectiveParameters();
//   }
}

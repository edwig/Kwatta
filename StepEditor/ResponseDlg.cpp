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
// This File   : ResponseDlg.cpp
// What it does: Resulting response only of a HTTP internet test call
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "StepEditor.h"
#include "ResponseDlg.h"
#include "StepInternetDlg.h"
#include <TestStepIN.h>
#include <StepResultIN.h>
#include <HTTPError.h>
#include <JSONMessage.h>
#include <XMLMessage.h>
#include <afxdialogex.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// ResponseDlg dialog

IMPLEMENT_DYNAMIC(ResponseDlg, StyleDialog)

ResponseDlg::ResponseDlg(CWnd* pParent /*=nullptr*/)
	          :StyleDialog(IDD_RESPONSE, pParent)
{
}

ResponseDlg::~ResponseDlg()
{
}

void 
ResponseDlg::DoDataExchange(CDataExchange* pDX)
{
	StyleDialog::DoDataExchange(pDX);
  DDX_Control(pDX,IDC_STATUS,     m_editStatus,     m_status);
  DDX_Control(pDX,IDC_STATUS_TEXT,m_editStatusText, m_statusText);
  DDX_Control(pDX,IDC_TIME,       m_editTime,       m_time);
  DDX_Control(pDX,IDC_PAYLOAD,    m_editPayload,    m_payload);
  DDX_Control(pDX,IDC_CLEAR,      m_buttonClear);
}

BEGIN_MESSAGE_MAP(ResponseDlg, StyleDialog)
  ON_EN_CHANGE (IDC_STATUS,      &ResponseDlg::OnEnChangeStatus)
  ON_EN_CHANGE (IDC_STATUS_TEXT, &ResponseDlg::OnEnChangeStatusText)
  ON_EN_CHANGE (IDC_TIME,        &ResponseDlg::OnEnChangeTime)
  ON_EN_CHANGE (IDC_PAYLOAD,     &ResponseDlg::OnEnChangePayload)
  ON_BN_CLICKED(IDC_CLEAR,       &ResponseDlg::OnBnClickedClear)
END_MESSAGE_MAP()

BOOL
ResponseDlg::OnInitDialog()
{
  StyleDialog::OnInitDialog();

  m_editStatus    .SetMutable(false);
  m_editStatusText.SetMutable(false);
  m_editTime      .SetMutable(false);

  EnableToolTips();
  RegisterTooltip(m_buttonClear, "Remove test results on response/header/raw-body pane!");
  m_buttonClear.SetIconImage(IDI_CLEAR);

  InitPayload();
  SetCanResize();
  return TRUE;
}

void
ResponseDlg::SetupDynamicLayout()
{
  StyleDialog::SetupDynamicLayout();

  CMFCDynamicLayout& manager = *GetDynamicLayout();
#ifdef _DEBUG
  manager.AssertValid();
#endif
  manager.AddItem(IDC_CLEAR,   CMFCDynamicLayout::MoveHorizontal(100), CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_PAYLOAD, CMFCDynamicLayout::MoveNone(),          CMFCDynamicLayout::SizeHorizontalAndVertical(100,100));
}

void 
ResponseDlg::InitTab()
{
  m_status = 0;
  m_statusText.Empty();
  m_time.Empty();
  m_payload.Empty();

  UpdateData(FALSE);
}

void
ResponseDlg::InitPayload()
{
  m_editPayload.SetFontName("Courier new",100);
  m_editPayload.SetMutable(false);
}

void 
ResponseDlg::SetResult(StepResultIN* p_result)
{
  m_status = p_result->GetStatus();
  m_statusText = GetHTTPStatusText(m_status);
  m_payload = p_result->GetBody();
  m_time.Format("%6.3f",p_result->GetTiming());

  // In case we had an HTTP OS error!
  if(m_payload.IsEmpty() && !p_result->GetOSErrorString().IsEmpty())
  {
    m_payload = p_result->GetOSErrorString();
  }
  else
  {
    FormatPayload(p_result);
  }

  m_payload.Replace("\n","\r\n");
  UpdateData(FALSE);
}

void
ResponseDlg::FormatPayload(StepResultIN* p_result)
{
  CString content = p_result->GetHeader("content-type");
  if (content.Find("xml") >= 0)
  {
    XMLMessage xml;
    xml.ParseMessage(m_payload);
    xml.SetCondensed(false);
    m_payload = xml.Print();
  }
  if (content.Find("json") >= 0)
  {
    JSONMessage json(m_payload);
    json.SetWhitespace(true);
    m_payload = json.GetJsonMessage();
  }
}

// ResponseDlg message handlers

void 
ResponseDlg::OnEnChangeStatus()
{
  UpdateData();
}

void 
ResponseDlg::OnEnChangeStatusText()
{
  UpdateData();
}

void 
ResponseDlg::OnEnChangeTime()
{
  UpdateData();
}

void 
ResponseDlg::OnEnChangePayload()
{
  UpdateData();
}

void 
ResponseDlg::OnBnClickedClear()
{
  StepInternetDlg* dlg = reinterpret_cast<StepInternetDlg*>(GetParent()->GetParent());
  if(dlg)
  {
    dlg->ResetStepResult();
  }
}

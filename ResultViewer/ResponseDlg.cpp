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
// This File   : ResponseDlg.cpp
// What it does: Shows the response body of the HTTP internet message 
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "ResultViewer.h"
#include "ResponseDlg.h"
#include <StepResultNET.h>
#include <HTTPError.h>
#include <JSONMessage.h>
#include <afxdialogex.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// ResponseDlg dialog

IMPLEMENT_DYNAMIC(ResponseDlg,StyleTab)

ResponseDlg::ResponseDlg(CWnd* pParent /*=nullptr*/)
	          :StyleTab(IDD_RESPONSE, pParent)
{
}

ResponseDlg::~ResponseDlg()
{
}

void 
ResponseDlg::DoDataExchange(CDataExchange* pDX)
{
	StyleTab::DoDataExchange(pDX);
  DDX_Control(pDX,IDC_STATUS,     m_editStatus,     m_status);
  DDX_Control(pDX,IDC_STATUS_TEXT,m_editStatusText, m_statusText);
  DDX_Control(pDX,IDC_TIME,       m_editTime,       m_time);
  DDX_Control(pDX,IDC_PAYLOAD,    m_editPayload,    m_payload);
}

BEGIN_MESSAGE_MAP(ResponseDlg, StyleTab)
  ON_EN_CHANGE(IDC_STATUS,      &ResponseDlg::OnEnChangeStatus)
  ON_EN_CHANGE(IDC_STATUS_TEXT, &ResponseDlg::OnEnChangeStatusText)
  ON_EN_CHANGE(IDC_TIME,        &ResponseDlg::OnEnChangeTime)
  ON_EN_CHANGE(IDC_PAYLOAD,     &ResponseDlg::OnEnChangePayload)
END_MESSAGE_MAP()

BOOL
ResponseDlg::OnInitDialog()
{
  StyleTab::OnInitDialog();

  SetCanResize();
  return TRUE;
}

void
ResponseDlg::SetupDynamicLayout()
{
  StyleTab::SetupDynamicLayout();

  CMFCDynamicLayout& manager = *GetDynamicLayout();
#ifdef _DEBUG
  manager.AssertValid();
#endif
  manager.AddItem(IDC_PAYLOAD,CMFCDynamicLayout::MoveNone(),CMFCDynamicLayout::SizeHorizontalAndVertical(100,100));
}

void 
ResponseDlg::LoadVariables(StepResultNET* p_result)
{
  m_status     = p_result->GetStatus();
  m_statusText = GetHTTPStatusText(m_status);
  m_payload    = p_result->GetBody();
  m_time.Format(_T("%G"),p_result->GetTiming());

  // In case we had an HTTP OS error!
  if(m_payload.IsEmpty() && !p_result->GetOSErrorString().IsEmpty())
  {
    m_payload = p_result->GetOSErrorString();
  }
  else
  {
    FormatPayload(p_result);
  }
  m_payload.Replace(_T("\n"),_T("\r\n"));
  UpdateData(FALSE);
}

void 
ResponseDlg::FormatPayload(StepResultNET* p_result)
{
  CString content = p_result->GetHeader(_T("content-type"));
  if(content.Find(_T("xml")) >= 0)
  {
    XMLMessage xml;
    xml.ParseMessage(m_payload);
    xml.SetCondensed(false);
    m_payload = xml.Print();
  }
  if(content.Find(_T("json")) >= 0)
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

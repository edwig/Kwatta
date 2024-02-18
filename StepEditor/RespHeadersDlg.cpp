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
// This File   : RespHeadersDlg.cpp
// What it does: Resulting headers of the response of a HTTP internet test call
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "StepEditor.h"
#include "RespHeadersDlg.h"
#include <StepResultNET.h>
#include "afxdialogex.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// RespHeadersDlg dialog

IMPLEMENT_DYNAMIC(RespHeadersDlg,StyleTab)

RespHeadersDlg::RespHeadersDlg(CWnd* pParent /*=nullptr*/)
	             :StyleTab(IDD_RESP_HEADERS, pParent)
{
}

RespHeadersDlg::~RespHeadersDlg()
{
}

void RespHeadersDlg::DoDataExchange(CDataExchange* pDX)
{
	StyleTab::DoDataExchange(pDX);
  DDX_Control(pDX,IDC_GRID,m_list);
}

BEGIN_MESSAGE_MAP(RespHeadersDlg, StyleTab)
END_MESSAGE_MAP()

BOOL
RespHeadersDlg::OnInitDialog()
{
  StyleTab::OnInitDialog();
  // InitLijst();
  SetCanResize();
  return TRUE;
}

void 
RespHeadersDlg::SetupDynamicLayout()
{
  StyleTab::SetupDynamicLayout();

  CMFCDynamicLayout& manager = *GetDynamicLayout();
#ifdef _DEBUG
  manager.AssertValid();
#endif
  manager.AddItem(IDC_GRID,CMFCDynamicLayout::MoveNone(),CMFCDynamicLayout::SizeHorizontalAndVertical(100,100));
}

void
RespHeadersDlg::InitLijst()
{
  m_list.SetColumnCount(2);
  m_list.SetRowCount(1);
  m_list.SetFixedRowCount(1);
  m_list.GetCell(0,0)->SetText(_T("Header"));
  m_list.GetCell(0,1)->SetText(_T("Value"));
  m_list.SetColumnWidth(0,150);
  m_list.SetColumnWidth(1,500);
  m_list.SetEditable(FALSE);
  m_list.SetSingleRowSelection();
  m_list.SetSortColumn(0);
}

void 
RespHeadersDlg::InitTab()
{
  m_list.DeleteAllItems();
  InitLijst();
}

void 
RespHeadersDlg::SetResult(StepResultNET* p_result)
{
  int count = 0;
  UrlHeaders& headers = p_result->GetHeaders();

  for(auto& header : headers)
  {
    int row = m_list.InsertRow(header.m_name);
    m_list.GetCell(row,1)->SetText(header.m_value);
    ++count;
  }
  if(count)
  {
    m_list.SetFocusCell(1,0);
  }
}


// RespHeadersDlg message handlers


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
// This File   : RespHeadersDlg.cpp
// What it does: Shows the response headers the HTTP internet message 
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "ResultViewer.h"
#include "RespHeadersDlg.h"
#include "afxdialogex.h"
#include <StepResultIN.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// RespHeadersDlg dialog

IMPLEMENT_DYNAMIC(RespHeadersDlg,StyleDialog)

RespHeadersDlg::RespHeadersDlg(CWnd* pParent /*=nullptr*/)
               :StyleDialog(IDD_RESP_HEADERS,pParent)
{
}

RespHeadersDlg::~RespHeadersDlg()
{
}

void RespHeadersDlg::DoDataExchange(CDataExchange* pDX)
{
  StyleDialog::DoDataExchange(pDX);
  DDX_Control(pDX,IDC_GRID,m_list);
}

BEGIN_MESSAGE_MAP(RespHeadersDlg,StyleDialog)
END_MESSAGE_MAP()

BOOL
RespHeadersDlg::OnInitDialog()
{
  StyleDialog::OnInitDialog();
  InitLijst();
  SetCanResize();
  return TRUE;
}

void
RespHeadersDlg::SetupDynamicLayout()
{
  StyleDialog::SetupDynamicLayout();

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
  m_list.GetCell(0,0)->SetText("Header");
  m_list.GetCell(0,1)->SetText("Value");
  m_list.SetColumnWidth(0,150);
  m_list.SetColumnWidth(1,500);
  m_list.SetEditable(FALSE);
  m_list.SetSingleRowSelection();
  m_list.SetSortColumn(0);
}

void
RespHeadersDlg::LoadVariables(StepResultIN* p_result)
{
  int count = 0;

  // Fill in all variables
  for(auto& header : p_result->GetHeaders())
  {
    int row = m_list.InsertRow(header.m_name);
    m_list.GetCell(row,1)->SetText(header.m_value);
    ++count;
  }

  // If something read-in
  if(count)
  {
    // Set focus to first cell
    m_list.SetFocusCell(0,0);
  }
}

// RespHeadersDlg message handlers


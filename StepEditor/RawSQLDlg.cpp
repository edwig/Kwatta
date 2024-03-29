﻿///////////////////////////////////////////////////////////////////////////////////////////////////////
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
// This File   : RawSQLDlg.cpp
// What it does: Resulting raw request of a HTTP internet test call
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StepEditor.h"
#include "TestStepSQL.h"
#include "RawSQLDlg.h"
#include "StepDatabaseDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// RawSQLDlg dialog

IMPLEMENT_DYNAMIC(RawSQLDlg,StyleTab)

RawSQLDlg::RawSQLDlg(CWnd* pParent /*=nullptr*/)
          :StyleTab(IDD_RAW_SQL, pParent)
{
}

RawSQLDlg::~RawSQLDlg()
{
}

void 
RawSQLDlg::DoDataExchange(CDataExchange* pDX)
{
	StyleTab::DoDataExchange(pDX);
  DDX_Control (pDX,IDC_RAW, m_editPayload,m_payload);
}

BEGIN_MESSAGE_MAP(RawSQLDlg, StyleTab)
  ON_EN_KILLFOCUS (IDC_RAW, &RawSQLDlg::OnEnKillfocusPayload)
END_MESSAGE_MAP()

BOOL
RawSQLDlg::OnInitDialog()
{
  StyleTab::OnInitDialog();

  InitPayload();

  SetCanResize();
  return TRUE;
}

void
RawSQLDlg::SetupDynamicLayout()
{
  StyleTab::SetupDynamicLayout();

  CMFCDynamicLayout& manager = *GetDynamicLayout();
#ifdef _DEBUG
  manager.AssertValid();
#endif

  manager.AddItem(IDC_RAW,CMFCDynamicLayout::MoveNone(),CMFCDynamicLayout::SizeHorizontalAndVertical(100,100));
}

void
RawSQLDlg::InitPayload()
{
  m_editPayload.SetFontName(_T("Courier new"),100);
  m_editPayload.SetMutable(false);
}

void 
RawSQLDlg::InitTab(TestStepSQL* p_testStep)
{
  if(p_testStep)
  {
    m_testStep = p_testStep;
    m_payload  = m_testStep->GetEffectiveSQL();
    m_payload.Replace(_T("\n"),_T("\r\n"));
  }
  UpdateData(FALSE);
}

bool
RawSQLDlg::IsFilled()
{
  return !m_payload.IsEmpty();
}

void 
RawSQLDlg::StoreVariables()
{
  // Nothing to do
}

// RawSQLDlg message handlers

void 
RawSQLDlg::OnEnKillfocusPayload()
{
  // Getting the payload as body for the message
  UpdateData();

  if(m_testStep)
  {
    // Check parameters
    StepDatabaseDlg* step = reinterpret_cast<StepDatabaseDlg*>(GetParent()->GetParent());
    step->EffectiveParameters();
  }
}

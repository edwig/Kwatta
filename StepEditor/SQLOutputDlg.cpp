// SQLOutputDlg.cpp : implementation file
//

#include "stdafx.h"
#include "StepEditor.h"
#include "afxdialogex.h"
#include "SQLOutputDlg.h"
#include <StepResultSQL.h>

// SQLOutputDlg dialog

IMPLEMENT_DYNAMIC(SQLOutputDlg, StyleDialog)

SQLOutputDlg::SQLOutputDlg(CWnd* p_parent)
	           :StyleDialog(IDD_SQLOUTPUT, p_parent)
{
}

SQLOutputDlg::~SQLOutputDlg()
{
}

void SQLOutputDlg::DoDataExchange(CDataExchange* pDX)
{
	StyleDialog::DoDataExchange(pDX);

	DDX_Control(pDX,IDC_SUCCEEDED,m_checkSucceeded);
	DDX_Control(pDX,IDC_ROWS,     m_editRows,		 m_rows);
	DDX_Control(pDX,IDC_COLUMNS,  m_editCols,		 m_cols);
	DDX_Control(pDX,IDC_SQLSTATE, m_editSqlState,m_SQLState);
	DDX_Control(pDX,IDC_NATIVE,   m_editNative,  m_NativeError);
}

BEGIN_MESSAGE_MAP(SQLOutputDlg, StyleDialog)
END_MESSAGE_MAP()

BOOL
SQLOutputDlg::OnInitDialog()
{
	StyleDialog::OnInitDialog();

	InitTab();
	SetCanResize();

	return TRUE;
}

void
SQLOutputDlg::SetupDynamicLayout()
{
  StyleDialog::SetupDynamicLayout();

  CMFCDynamicLayout& manager = *GetDynamicLayout();
#ifdef _DEBUG
  manager.AssertValid();
#endif
  manager.AddItem(IDC_GRP_OUTPUT,CMFCDynamicLayout::MoveNone(),CMFCDynamicLayout::SizeHorizontal(100));
  manager.AddItem(IDC_NATIVE,    CMFCDynamicLayout::MoveNone(),CMFCDynamicLayout::SizeHorizontal(100));
}

void
SQLOutputDlg::InitTab()
{
	m_checkSucceeded.EnableWindow(false);
	m_editRows			.SetMutable(false);
	m_editCols			.SetMutable(false);
	m_editSqlState	.SetMutable(false);
	m_editNative		.SetMutable(false);
}

void 
SQLOutputDlg::SetResult(StepResultSQL* p_result)
{
	m_result = p_result;

	m_checkSucceeded.SetCheck(p_result->GetSucceeded());
	m_rows				= p_result->GetResultRows();
	m_cols				= p_result->GetResultCols();
	m_SQLState		= p_result->GetSQLState();
	m_NativeError = p_result->GetNativeStatus();

	UpdateData(FALSE);
}

// SQLOutputDlg message handlers

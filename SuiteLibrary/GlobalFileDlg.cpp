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
// This program: SuiteLibrary
// This File   : GlobalFileDlg.cpp
// What it does: Displaying the contents of the global steps or global validation files
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SuiteLibrary.h"
#include "afxdialogex.h"
#include "GlobalFileDlg.h"
#include "resource.h"

// GlobalFileDlg dialog

IMPLEMENT_DYNAMIC(GlobalFileDlg, StyleDialog)

GlobalFileDlg::GlobalFileDlg(CWnd* p_parent,bool p_step,StepType p_type,CString p_directory)
	            :StyleDialog(IDD_GLOBALFILE,p_parent)
							,m_directory(p_directory)
              ,m_step(p_step)
              ,m_type(p_type)
{
}

GlobalFileDlg::~GlobalFileDlg()
{
}

void GlobalFileDlg::DoDataExchange(CDataExchange* pDX)
{
	StyleDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_FILEGRID,	m_grid);
	DDX_Control(pDX,IDOK,			    m_buttonOK);
	DDX_Control(pDX,IDCANCEL,     m_buttonCancel);
}

BEGIN_MESSAGE_MAP(GlobalFileDlg, StyleDialog)
  ON_NOTIFY(LVN_ITEMCHANGING,IDC_FILEGRID, &GlobalFileDlg::OnLvnItemchangingGrid)
  ON_NOTIFY(NM_DBLCLK,       IDC_FILEGRID, &GlobalFileDlg::OnLvnDoubleClickGrid)
	ON_BN_CLICKED(IDOK,			OnBnClickedOK)
	ON_BN_CLICKED(IDCANCEL,	OnBnClickedCancel)
END_MESSAGE_MAP()

BOOL 
GlobalFileDlg::OnInitDialog()
{
	StyleDialog::OnInitDialog();

  // Setting the title
  CString title(_T("Choose global: "));
  title += m_step ? _T("step") : _T("validation");
	SetWindowText(title);

	InitGrid();
	FillGrid();

  m_buttonOK.SetStyle(_T("ok"));
  m_buttonCancel.SetStyle(_T("can"));

	return FALSE;
}

void 
GlobalFileDlg::InitGrid()
{
  m_grid.SetColumnCount(2);
  m_grid.SetRowCount(1);
  m_grid.SetFixedRowCount(1);
  m_grid.GetCell(0, 0)->SetText(_T("Number"));
  m_grid.GetCell(0, 1)->SetText(m_step ? _T("Step filename") : _T("Validation filename"));
  m_grid.SetColumnWidth(0, 80);
  m_grid.SetColumnWidth(1, 300);
  m_grid.SetSingleRowSelection();
  m_grid.SetSortColumn(0);
}

void 
GlobalFileDlg::FillGrid()
{
	CString path(m_directory);
  path += m_step ? _T("Steps\\") : _T("Validations\\");
  CString pattern(path);
  int row = 0;
  CString number;

  if(m_step)
  {
    if(m_type == StepType::Step_command)
    {
      pattern += _T("*.xrun");
    }
    else
    {
      pattern += _T("*.irun");
    }
  }
  else
  {
    if(m_type == StepType::Step_command)
    {
      pattern += _T("*.xval");
    }
    else
    {
      pattern += _T("*.ival");
    }
  }

  // Read in all known files
  WIN32_FIND_DATA data;
  HANDLE hFind = nullptr;
  if((hFind = FindFirstFile(pattern, &data)) != INVALID_HANDLE_VALUE)
  {
    do
    {
      if((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
      {
        number.Format(_T("%d"),++row);
        CString filename(data.cFileName);
        m_grid.InsertRow(number);
        m_grid.GetCell(row,1)->SetText(filename);
      }
    } 
    while (FindNextFile(hFind, &data) != 0);
    FindClose(hFind);
  }
  m_grid.Invalidate();
}

// GlobalFileDlg message handlers

void
GlobalFileDlg::OnLvnItemchangingGrid(NMHDR* pNMHDR, LRESULT* pResult)
{
  LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

  if (pNMLV->uNewState)
  {
    int row = pNMLV->iItem;
    if(row > 0)
    {
      m_chosen = m_grid.GetCell(row,1)->GetText();
    }
    *pResult = 1;
    return;
  }
  *pResult = 0;
}

void 
GlobalFileDlg::OnLvnDoubleClickGrid(NMHDR* pNMHDR, LRESULT* pResult)
{
  LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

  if (pNMLV->uNewState)
  {
    int row = pNMLV->iItem;
    if (row > 0)
    {
      m_chosen = m_grid.GetCell(row, 1)->GetText();
      OnOK();
    }
    *pResult = 1;
    return;
  }
  *pResult = 0;
}

void
GlobalFileDlg::OnBnClickedOK()
{
	OnOK();
}

void
GlobalFileDlg::OnBnClickedCancel()
{
  m_chosen.Empty();
	OnCancel();
}

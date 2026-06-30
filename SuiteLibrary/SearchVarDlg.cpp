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
// This File   : SearchVarDlg.cpp
// What it does: Find and/or maintain global and local variables
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#define KWATTA_NOAUTOLINK
#include "SearchVarDlg.h"
#include "SuiteLibrary.h"
#include <RegistryManager.h>
#include <GridCellCombo.h>
#include <afxdialogex.h>
#include <Crypto.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// SearchVarDlg dialog

IMPLEMENT_DYNAMIC(SearchVarDlg, StyleDialog)

SearchVarDlg::SearchVarDlg(CWnd*        p_parent 
                          ,Parameters*  p_parameters
                          ,bool         p_doGlobals
                          ,bool         p_doLocals
                          ,bool         p_doReturns
                          ,bool         p_doBuffers
                          ,bool         p_useUSE /*=true*/)
             :StyleDialog(IDD_SEARCHVAR, p_parent)
             ,m_parameters(p_parameters)
             ,m_doGlobals(p_doGlobals)
             ,m_doLocals(p_doLocals)
             ,m_doReturns(p_doReturns)
             ,m_doBuffers(p_doBuffers)
             ,m_useUSEbutton(p_useUSE)
{
}

SearchVarDlg::~SearchVarDlg()
{
}

void SearchVarDlg::DoDataExchange(CDataExchange* pDX)
{
	StyleDialog::DoDataExchange(pDX);

  DDX_Control(pDX, IDC_CONTEXT, m_editContext,m_context);
  DDX_Control(pDX, IDC_VARGRID, m_grid);
  DDX_Control(pDX, IDC_ADD,     m_buttonAdd);
  DDX_Control(pDX, IDC_DELETE,  m_buttonDelete);
  DDX_Control(pDX, IDOK,        m_buttonOK);
  DDX_Control(pDX, IDCANCEL,    m_buttonCancel);
  DDX_Control(pDX, IDC_SAVE,    m_buttonSave);

  m_buttonSave.EnableWindow(m_changed);
}

BEGIN_MESSAGE_MAP(SearchVarDlg,StyleDialog)
  ON_NOTIFY(LVN_ITEMCHANGED, IDC_VARGRID, &SearchVarDlg::OnLvnItemchangedGrid)
  ON_NOTIFY(LVN_ENDLABELEDIT,IDC_VARGRID, &SearchVarDlg::OnLvnEndLabelEditGrid)
  ON_BN_CLICKED(IDC_ADD,                  &SearchVarDlg::OnBnClickedAdd)
  ON_BN_CLICKED(IDC_DELETE,               &SearchVarDlg::OnBnClickedDelete)
  ON_BN_CLICKED(IDOK,                     &SearchVarDlg::OnBnClickedOk)
  ON_BN_CLICKED(IDCANCEL,                 &SearchVarDlg::OnBnClickedCancel)
  ON_BN_CLICKED(IDC_SAVE,                 &SearchVarDlg::OnBnClickedSave)
END_MESSAGE_MAP()

BOOL
SearchVarDlg::OnInitDialog()
{
  StyleDialog::OnInitDialog();
  SetWindowText(_T("Search Variable"));
  ShowMinMaxButton(true,false);

  InitPasswordProtect();
  InitGrid();
  LoadGrid();
  LoadContext();

  m_buttonOK.SetStyle(_T("ok"));
  m_buttonSave.SetStyle(_T("save"));
  m_buttonCancel.SetStyle(_T("can"));
  if(!m_useUSEbutton)
  {
    m_buttonOK.SetWindowText(_T("OK"));
  }

  SetCanResize();
  UpdateData(FALSE);
  return TRUE;
}

void
SearchVarDlg::SetupDynamicLayout()
{
  StyleDialog::SetupDynamicLayout();

  CMFCDynamicLayout& manager = *GetDynamicLayout();
#ifdef _DEBUG
  manager.AssertValid();
#endif

  manager.AddItem(IDC_CONTEXT,CMFCDynamicLayout::MoveNone(),                        CMFCDynamicLayout::SizeHorizontal(100));
  manager.AddItem(IDC_VARGRID,CMFCDynamicLayout::MoveNone(),                        CMFCDynamicLayout::SizeHorizontalAndVertical(100,100));
  manager.AddItem(IDOK,       CMFCDynamicLayout::MoveHorizontalAndVertical(100,100),CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDCANCEL,   CMFCDynamicLayout::MoveHorizontalAndVertical(100,100),CMFCDynamicLayout::SizeNone());
}

void 
SearchVarDlg::InitPasswordProtect()
{
  int protect = _ttoi(m_parameters->FindSystemParameter(_T("HidePassword")));
  m_doPasswordProtect = protect > 0;
}

bool
SearchVarDlg::CheckReadPassword(XString& p_name,XString& p_value)
{
  if(m_doPasswordProtect)
  {
    XString name(p_name);
    name.MakeLower();
    if(name.Find(_T("password")) >= 0)
    {
      return true;
    }
  }
  if(p_value.IsEmpty() && p_name.CompareNoCase(_T("password")) == 0)
  {
    if(!p_value.GetEnvironmentVariable(_T(KWATTA_PASSWORD)))
    {
      p_value = GetGlobalEnvironmentVariable(_T(KWATTA_PASSWORD));
    }
    if(p_value.IsEmpty())
    {
      Crypto crypt;
      p_value = crypt.Decryption(p_value,_T(KWATTA_ENCRYPT));
    }
    else
    {
      p_value.Empty();
    }
  }
  return false;
}

void
SearchVarDlg::InitGrid()
{
  m_grid.SetColumnCount(3);
  m_grid.SetRowCount(1);
  m_grid.SetFixedRowCount(1);
  m_grid.GetCell(0, 0)->SetText(_T("Type"));
  m_grid.GetCell(0, 1)->SetText(_T("Variable"));
  m_grid.GetCell(0, 2)->SetText(_T("Value"));
  m_grid.SetColumnWidth(0, 100);
  m_grid.SetColumnWidth(1, 100);
  m_grid.SetColumnWidth(2, 300);
  m_grid.SetSingleRowSelection();
  m_grid.SetSortColumn(0);

  m_buttonAdd.SetIconImage(IDI_ADD);
  m_buttonDelete.SetStyle(_T("rem"));
}

void 
SearchVarDlg::InsertRow(ParType p_type,XString p_name,XString p_value)
{
  XString type;
  switch (p_type)
  {
    case ParType::PAR_GLOBAL: type = _T("Global"); break;
    case ParType::PAR_LOCAL:  type = _T("Local");  break;
    case ParType::PAR_RETURN: type = _T("Return"); break;
    case ParType::PAR_BUFFER: type = _T("Buffer"); break;
  }

  // Check for password. Can alter the value
  if(CheckReadPassword(p_name,p_value))
  {
    return;
  }

  int row = m_grid.InsertRow(type);
  m_grid.SetCellType(row,0,RUNTIME_CLASS(CGridCellCombo));
  CGridCellCombo* types = (CGridCellCombo*)m_grid.GetCell(row,0);

  CStringArray list;
  list.Add(_T("Global"));
  list.Add(_T("Local"));
  list.Add(_T("Return"));
  list.Add(_T("Buffer"));
  types->SetOptions(list);
  types->SetStyle(CBS_DROPDOWNLIST);

  m_grid.GetCell(row,1)->SetText(p_name);
  m_grid.GetCell(row,2)->SetText(p_value);
}

void
SearchVarDlg::LoadGrid()
{
  int count = 0;

  // Fill in all variables

  if(m_doGlobals)
  {
    for(auto& var : m_parameters->GetGlobals())
    {
      InsertRow(ParType::PAR_GLOBAL,var.first,var.second);
      ++count;
    }
  }

  if(m_doLocals)
  {
    for(auto& var : m_parameters->GetLocals())
    {
      InsertRow(ParType::PAR_LOCAL,var.first,var.second);
      ++count;
    }
  }

  if(m_doReturns)
  {
    for(auto& var : m_parameters->GetReturns())
    {
      InsertRow(ParType::PAR_RETURN,var.first,var.second);
      ++count;
    }
  }

  if(m_doBuffers)
  {
    for(auto& var : m_parameters->GetBuffers())
    {
      InsertRow(ParType::PAR_BUFFER,var.first,var.second);
      ++count;
    }
  }

  // If something read-in
  if(count)
  {
    // Set focus to first cell
    m_grid.SetFocusCell(1,1);
  }
}

void
SearchVarDlg::CheckSavePassword(XString& p_name,XString& p_value)
{
  if(m_doPasswordProtect)
  {
    XString name(p_name);
    name.MakeLower();
    if(name.Find(_T("password")) >= 0)
    {
      p_value.Empty();
    }
  }
}

void
SearchVarDlg::SaveParameters()
{
  int count = m_grid.GetRowCount();
  for(int index = 1;index < count; ++index)
  {
    XString type  = m_grid.GetCell(index,0)->GetText();
    XString name  = m_grid.GetCell(index,1)->GetText();
    XString value = m_grid.GetCell(index,2)->GetText();

    CheckSavePassword(name,value);

    if(type == _T("Global")) m_parameters->OverwriteGlobalParameter(name,value);
    if(type == _T("Local"))  m_parameters->OverwriteLocalParameter (name,value);
    if(type == _T("Return")) m_parameters->OverwriteReturnParameter(name,value);
    if(type == _T("Buffer")) m_parameters->OverwriteBufferParameter(name,value);
  }
}

void
SearchVarDlg::LoadContext()
{
  m_context = _T("Variables: ");

  if (m_doGlobals) m_context += _T("Globals ");
  if (m_doLocals)  m_context += _T("Locals ");
  if (m_doReturns) m_context += _T("Return-values ");
  if (m_doBuffers) m_context += _T("Buffer-streams");
}

// THIS IS THE RESULT OF THIS DIALOG!!
XString
SearchVarDlg::GetVariable()
{
  XString variable;
  switch(m_resultType)
  {
    case ParType::PAR_GLOBAL: variable.Format(_T("$%s$"), m_resultVariable.GetString()); break;
    case ParType::PAR_LOCAL:  variable.Format(_T("#%s#"), m_resultVariable.GetString()); break;
    case ParType::PAR_RETURN: variable.Format(_T("[%s]"), m_resultVariable.GetString()); break;
    case ParType::PAR_BUFFER: variable.Format(_T("<%s>"), m_resultVariable.GetString()); break;
    default:                  variable = m_resultVariable; 
                              break;
  }
  return variable;
}

// SearchVarDlg message handlers

void 
SearchVarDlg::OnLvnItemchangedGrid(NMHDR* pNMHDR, LRESULT* pResult)
{
  LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

  if(pNMLV->uNewState)
  {
    m_chosen = pNMLV->iItem;
    *pResult = 1;
    return;
  }
  *pResult = 0;
}

void 
SearchVarDlg::OnLvnEndLabelEditGrid(NMHDR* pNMHDR,LRESULT* pResult)
{
  m_changed = true;
  UpdateData(FALSE);

  // Check on double parameters (case insensitive)
  LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
  if(pNMLV->uNewState)
  {
    int count = 0;
    int item = pNMLV->iItem;
    XString var = m_grid.GetCell(item,1)->GetText();
    for(int index = 1; index < m_grid.GetRowCount(); ++index)
    {
      XString name = m_grid.GetCell(index,1)->GetText();
      if(name.CompareNoCase(var) == 0)
      {
        ++count;
      }
    }
    if(count > 1)
    {
      XString warning;
      warning.Format(_T("BEWARE: Double defined parameter in the list [%s] !!"),var.GetString());
      StyleMessageBox(this,warning,_T(KWATTA),MB_OK|MB_ICONERROR);
    }
  }
  *pResult  = 1;
}

void
SearchVarDlg::OnBnClickedAdd()
{
  InsertRow(ParType::PAR_LOCAL,_T("Name"),_T(""));
  m_grid.Refresh();
  m_changed = true;
}

void
SearchVarDlg::OnBnClickedDelete()
{
  CCellID id = m_grid.GetFocusCell();
  if(id.row > 0)
  {
    XString type     = m_grid.GetCell(id.row,0)->GetText();
    XString variable = m_grid.GetCell(id.row,1)->GetText();
    XString question;
    question.Format(_T("Do you wish to delete variable: %s"),variable.GetString());
    if(StyleMessageBox(this,question,_T("Kwatta"),MB_YESNO|MB_DEFBUTTON2|MB_ICONQUESTION) == IDYES)
    {
      m_grid.DeleteRow(id.row);
      m_grid.Refresh();

      if(type == _T("Global")) m_parameters->RemoveGlobalParameter(variable);
      if(type == _T("Local"))  m_parameters->RemoveLocalParameter (variable);
      if(type == _T("Return")) m_parameters->RemoveReturnParameter(variable);
      if(type == _T("Buffer")) m_parameters->RemoveBufferParameter(variable);
      
      m_changed = true;
    }
  }
}

void
SearchVarDlg::OnBnClickedSave()
{
  if(m_changed)
  {
    SaveParameters();
    if(m_doGlobals)
    {
      m_parameters->WriteToXML(false,true);
    }
    if(m_doLocals || m_doReturns || m_doBuffers)
    {
      m_parameters->SetChanged();
      m_parameters->WriteToXML(true,false);
    }
    m_changed = false;
    m_saved   = true;
    UpdateData(FALSE);
  }
}

void 
SearchVarDlg::OnBnClickedOk()
{
  if(m_changed)
  {
    OnBnClickedSave();
  }

  if(m_chosen > 0 && m_chosen < m_grid.GetRowCount())
  {
    XString type     = m_grid.GetCell(m_chosen, 0)->GetText();
    m_resultVariable = m_grid.GetCell(m_chosen, 1)->GetText();
    if (type == _T("Global")) m_resultType = ParType::PAR_GLOBAL;
    if (type == _T("Local"))  m_resultType = ParType::PAR_LOCAL;
    if (type == _T("Return")) m_resultType = ParType::PAR_RETURN;
    if (type == _T("Buffer")) m_resultType = ParType::PAR_BUFFER;
  }
  StyleDialog::OnOK();
}

void
SearchVarDlg::OnBnClickedCancel()
{
  if(m_changed)
  {
    if(StyleMessageBox(this,_T("Variables have been edited. Save your results?"),_T(KWATTA),MB_YESNO|MB_DEFBUTTON2|MB_ICONQUESTION) == IDYES)
    {
      SaveParameters();
      if(m_doGlobals)
      {
        m_parameters->WriteToXML(false, true);
      }
      if(m_doLocals || m_doReturns || m_doBuffers)
      {
        m_parameters->WriteToXML(true, false);
      }
    }
  }
  StyleDialog::OnCancel();
}

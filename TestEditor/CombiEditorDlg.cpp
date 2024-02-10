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
// This program: TestEditor
// This File   : CombiEditorDlg.cpp
// What it does: Multiple (combined) validations for a teststep in the test-set
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TestEditor.h"
#include "CombiEditorDlg.h"
#include "NewStepTypeDlg.h"
#include "NewStepValiDlg.h"
#include "TestEditorDlg.h"
#include "MutateDlg.h" 
#include <Validate.h>


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// CombeEditorDlg dialog

IMPLEMENT_DYNAMIC(CombiEditorDlg, StyleDialog)

CombiEditorDlg::CombiEditorDlg(CWnd* p_parent,TestSet& p_testSet,CString p_stepname,int p_row)
               :StyleDialog(IDD_COMBIEDITOR,p_parent)
               ,m_testSet(p_testSet)
               ,m_stepName(p_stepname)
               ,m_row(p_row)
               ,m_changed(false)
{
}

CombiEditorDlg::~CombiEditorDlg()
{
}

void CombiEditorDlg::DoDataExchange(CDataExchange* pDX)
{
  StyleDialog::DoDataExchange(pDX);
  DDX_Control(pDX,IDC_NAME,         m_editName,m_name);
  DDX_Control(pDX,IDC_DOCUMENTATION,m_editDocu,m_documentation);
  DDX_Control(pDX,IDC_COMMANDLINE,  m_editStep,m_stepName);
  DDX_Control(pDX,IDC_GRID,         m_grid);
  DDX_Control(pDX,IDC_ADDVAL,       m_buttonAddValidation);
  DDX_Control(pDX,IDC_DELVAL,       m_buttonDelValidation);
  DDX_Control(pDX,IDC_BUT_MUTATE,   m_buttonMutValidation);
  DDX_Control(pDX,IDOK,             m_buttonOK);
  DDX_Control(pDX,IDCANCEL,         m_buttonCancel);
}

BEGIN_MESSAGE_MAP(CombiEditorDlg, StyleDialog)
  ON_NOTIFY(NM_DBLCLK, IDC_GRID, OnGridDblClick)
  ON_BN_CLICKED(IDC_ADDVAL,      OnBnClickedAddValidation)
  ON_BN_CLICKED(IDC_DELVAL,      OnBnClickedDelValidation)
  ON_BN_CLICKED(IDC_BUT_MUTATE,  OnBnClickedMutValidation)
END_MESSAGE_MAP()

BOOL
CombiEditorDlg::OnInitDialog()
{
  StyleDialog::OnInitDialog();
  SetWindowText("Multiple Validations");

  m_name          = m_testSet.GetName();
  m_documentation = m_testSet.GetDocumentation();

  InitButtons();
  FillGrid();

  UpdateData(FALSE);
  return TRUE;
}

void
CombiEditorDlg::InitButtons()
{
  m_buttonOK.SetStyle("ok");
  m_buttonCancel.SetStyle("can");

  m_buttonAddValidation.SetIconImage(IDI_NEW);
  m_buttonDelValidation.SetIconImage(IDI_DELETE);
  m_buttonMutValidation.SetIconImage(IDI_MUTATE);

  EnableToolTips();
  RegisterTooltip(m_buttonAddValidation,"Add a new validation to the teststep.");
  RegisterTooltip(m_buttonDelValidation,"Delete a validation from the teststep.");
  RegisterTooltip(m_buttonMutValidation,"Change the underlying filename of the validation.");
}

void
CombiEditorDlg::FillGrid()
{
  m_grid.SetColumnCount(4);
  m_grid.SetRowCount(1);
  m_grid.SetFixedRowCount(1);
  m_grid.SetFixedColumnCount(1);
  m_grid.SetEditable(FALSE);
  m_grid.GetCell(0, 0)->SetText("Number");
  m_grid.GetCell(0, 1)->SetText("GT");
  m_grid.GetCell(0, 2)->SetText("Validation");
  m_grid.GetCell(0, 3)->SetText("Filename");
  m_grid.SetSingleRowSelection();
  m_grid.SetSortColumn(0);

  m_grid.SetColumnWidth(0, 100);
  m_grid.SetColumnWidth(1, 32);
  m_grid.SetColumnWidth(2, 200);
  m_grid.SetColumnWidth(3, 300);

  // Create image list from bitmap
  m_images.Create(MAKEINTRESOURCE(IDB_STATUS),16,1,RGB(256,256,256));
  m_grid.SetImageList(&m_images);

  TSValSet* vals = m_testSet.GetValidationsByName(m_stepName);

  if(vals)
  {
    int number = 1;
    CString num;
    for(auto& val : *vals)
    {
      num.Format("Number %d",number++);
      int row = m_grid.InsertRow(num);
      SetTextImage(row,1,"",val.m_global ? 2 : 3);
      m_grid.GetCell(row,2)->SetText(val.m_name);
      m_grid.GetCell(row,3)->SetText(val.m_filename);

      if(m_stepType == StepType::Step_unknown)
      {
        m_stepType = Validate::FindStepTypeFromFile(val.m_filename);
      }
    }
  }
}

void
CombiEditorDlg::SetTextImage(int p_row,int p_col,CString p_text,int p_image)
{
  GV_ITEM item;
  item.mask    = GVIF_IMAGE;
  item.row     = p_row;
  item.col     = p_col;
  item.iImage  = p_image;
  if(!p_text.IsEmpty())
  {
    item.mask   |= GVIF_TEXT;
    item.strText = p_text;
  }
  m_grid.SetItem(&item);
}

void
CombiEditorDlg::TryChangeValiGlobalLocal(int p_row)
{
  TSValSet& vals = *m_testSet.GetValidationsByName(m_stepName);
  if(vals.size() < p_row)
  {
    return;
  }
  TRValidation& vali = vals[p_row-1];

  CString ask;
  ask.Format("Do you want to change the validation to a [%s] validation?", vali.m_global ? "local" : "global");
  if(StyleMessageBox(this,ask,PRODUCT_NAME,MB_YESNO|MB_DEFBUTTON2|MB_ICONQUESTION) == IDNO)
  {
    return;
  }

  if(vali.m_global)
  {
    // It was global. Try to make it local
    if(theApp.DemoteValidation(this,vali.m_filename))
    {
      vali.m_global = false;
      m_changed = true;
    }
  }
  else
  {
    // It was local. Try to make it global
    if(theApp.PromoteValidation(this,vali.m_filename))
    {
      vali.m_global = true;
      m_changed = true;
    }
  }
  SetTextImage(p_row,1,"",vali.m_global ? COL_STATUS_GLOBAL : COL_STATUS_LOCAL);
  m_grid.Invalidate();
}

//////////////////////////////////////////////////////////////////////////
// 
// CombeEditorDlg message handlers
//
//////////////////////////////////////////////////////////////////////////

void 
CombiEditorDlg::OnGridDblClick(NMHDR* pNMHDR, LRESULT* pResult)
{
  LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
  CCellID id = m_grid.GetFocusCell();
  if(id.col == 1)
  {
    TryChangeValiGlobalLocal(id.row);
  } 
  else if(id.col >= 2)
  {
    TSValSet& vals = *m_testSet.GetValidationsByName(m_stepName);
    if (vals.size() < id.row)
    {
      return;
    }
    TRValidation& vali = vals[id.row - 1];
    theApp.StartValidateEditor(vali.m_filename,vali.m_global,GetSafeHwnd(),id.row);
  }
}

void 
CombiEditorDlg::OnBnClickedAddValidation()
{
  AutoFocus focus;

  StepType type = m_stepType;
  StyleStepper wizard(this,IDD_STEPPER,"New validation");
  wizard.SetStepperData(&type);

  // Optionally add first page
  NewStepTypeDlg page1;
  if(type == StepType::Step_unknown)
  {
    wizard.AddPage("Step type",&page1,IDD_NEWSTEP_TYPE);
  }
  // Then only add the last page (validation)
  NewStepValiDlg page3(&wizard);
  wizard.AddPage("Validation",&page3,IDD_NEWSTEP_VALI);

  if(wizard.DoModal() == IDOK)
  {
    TestEditorDlg* editor = reinterpret_cast<TestEditorDlg*>(GetParent());
    if(editor)
    {
      editor->MakeNewVali(type,page3.GetValiGlobal(),page3.GetValiName(),page3.GetValiFile(),m_row);

      CString num;
      int number = m_grid.GetRowCount();
      num.Format("Number %d",number);

      int row = m_grid.InsertRow(num);
      SetTextImage(row,1,"",page3.GetValiGlobal() ? 2 : 3);
      m_grid.GetCell(row,2)->SetText(page3.GetValiName());
      m_grid.GetCell(row,3)->SetText(page3.GetValiFile());
      m_grid.Refresh();
    }
  }
  if(m_stepType == StepType::Step_unknown)
  {
    // Now all validations are of this type!!!
    m_stepType = type;
  }
}

void 
CombiEditorDlg::OnBnClickedDelValidation()
{
  CCellID id = m_grid.GetFocusCell();
  if (id.row >= 1)
  {
    CString validation = m_grid.GetItemText(id.row,2 /*VALIDATION*/);
    TSValSet* vals = m_testSet.GetValidationsByName(m_stepName);

    TSValSet::iterator it = vals->begin();
    while(it != vals->end())
    {
      if(it->m_name.CompareNoCase(validation) == 0)
      {
        if(it->m_global == false)
        {
          CString ask;
          ask.Format("Delete validation [%s] definitely! Are you sure?",validation.GetString());
          if(StyleMessageBox(this,ask,KWATTA,MB_YESNO|MB_DEFBUTTON2|MB_ICONQUESTION) == IDNO)
          {
            return;
          }
          CString filename = theApp.GetBaseDirectory() + theApp.GetTestDirectory() + it->m_filename;
          DeleteFile(filename);
        }
        else
        {
          StyleMessageBox(this,"Global validation no longer connected to this teststep",KWATTA,MB_OK|MB_ICONINFORMATION);
        }
        vals->erase(it);
        m_grid.DeleteRow(id.row);
        m_grid.Refresh();

        m_testSet.WriteToXML();
        return;
      }
      ++it;
    }
  }
}

void 
CombiEditorDlg::OnBnClickedMutValidation()
{
  CCellID id = m_grid.GetFocusCell();
  if(id.row >= 1)
  {
    CString validation = m_grid.GetItemText(id.row,2);
    TSValSet* vals = m_testSet.GetValidationsByName(m_stepName);

    TSValSet::iterator it = vals->begin();
    while (it != vals->end())
    {
      if(it->m_name.CompareNoCase(validation) == 0)
      {
        if(it->m_global)
        {
          StyleMessageBox(this,"Global validation cannot be edited!",KWATTA,MB_OK|MB_ICONWARNING);
        }
        else
        {
          CString filename = it->m_filename;

          AutoFocus focus;
          MutateDlg dlg(this, "validation", filename);
          dlg.DoModal();

          CString newfile = dlg.GetFilename();
          if (newfile.CompareNoCase(filename))
          {
            it->m_filename = newfile;
            m_grid.SetItemText(id.row, 2, newfile);
            m_grid.Refresh();
            m_testSet.WriteToXML();
            return;
          }
        }
      }
      ++it;
    }
  }
}

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
// This File   : FormDataDlg.cpp
// What it does: Editing the request body as if a Multipart/Form-data buffer
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StepEditor.h"
#include "FormDataDlg.h"
#include "MultiPartDlg.h"
#include <ConvertWideString.h>

// FormDataDlg dialog

IMPLEMENT_DYNAMIC(FormDataDlg, StyleDialog)

FormDataDlg::FormDataDlg(CWnd* p_parent,CString p_body,CString p_content)
            :StyleDialog(IDD_FORMDATA, p_parent)
            ,m_orignalBody(p_body)
            ,m_contentType(p_content)
            ,m_buffer(FormDataType::FD_MULTIPART)
            ,m_useExtensions(false)
            ,m_useCharset(false)
{
}

FormDataDlg::~FormDataDlg()
{
}

void 
FormDataDlg::DoDataExchange(CDataExchange* pDX)
{
  StyleDialog::DoDataExchange(pDX);
  DDX_Control(pDX,IDC_BOUNDARY,	m_editBoundary,m_boundary);
  DDX_Control(pDX,IDC_CHARSET,	m_editCharset,m_inCharset);
  DDX_Control(pDX,IDC_GENERATE,	m_buttonGenerate);
  DDX_Control(pDX,IDC_USECHAR,	m_checkUseCharset);
  DDX_Control(pDX,IDC_EXTENS,	  m_checkUseExtensions);
  DDX_Control(pDX,IDC_GRID,     m_grid);
  DDX_Control(pDX,IDC_NEW,      m_buttonNew);
  DDX_Control(pDX,IDC_DELETE,   m_buttonDelete);
  DDX_Control(pDX,IDC_EDIT,     m_buttonEdit);
  DDX_Control(pDX,IDOK,					m_buttonOK);
  DDX_Control(pDX,IDCANCEL,			m_buttonCancel);
}

BEGIN_MESSAGE_MAP(FormDataDlg, StyleDialog)
  ON_EN_KILLFOCUS(IDC_BOUNDARY,  &FormDataDlg::OnEnChangeBoundary)
  ON_EN_CHANGE   (IDC_CHARSET,   &FormDataDlg::OnEnChangeCharset)
  ON_BN_CLICKED  (IDC_GENERATE,  &FormDataDlg::OnBnClickedGenerate)
  ON_BN_CLICKED  (IDC_USECHAR,   &FormDataDlg::OnBnClickedUsechar)
  ON_BN_CLICKED  (IDC_EXTENS,    &FormDataDlg::OnBnClickedExtens)
  ON_NOTIFY(NM_DBLCLK,IDC_GRID,  &FormDataDlg::OnGridDblClick)
  ON_BN_CLICKED  (IDC_NEW,       &FormDataDlg::OnBnClickedNew)
  ON_BN_CLICKED  (IDC_DELETE,    &FormDataDlg::OnBnClickedDelete)
  ON_BN_CLICKED  (IDC_EDIT,      &FormDataDlg::OnBnClickedEdit)
  ON_BN_CLICKED  (IDOK,          &FormDataDlg::OnBnClickedOk)
  ON_BN_CLICKED  (IDCANCEL,      &FormDataDlg::OnBnClickedCancel)
END_MESSAGE_MAP()

BOOL 
FormDataDlg::OnInitDialog()
{
  StyleDialog::OnInitDialog();
  SetWindowText("Multipart/Form-data buffer");
  ShowMinMaxButton(false);

  InitButtons();
  InitGrid();
  ParsePayload();
  FillGrid();

  SetCanResize();
  UpdateData(FALSE);
  return TRUE;
}

void
FormDataDlg::SetupDynamicLayout()
{
  StyleDialog::SetupDynamicLayout();

  CMFCDynamicLayout& manager = *GetDynamicLayout();
#ifdef _DEBUG
  manager.AssertValid();
#endif

  manager.AddItem(IDC_BOX_HEADER, CMFCDynamicLayout::MoveNone(),					CMFCDynamicLayout::SizeHorizontal(100));
  manager.AddItem(IDC_BOX_PARTS,  CMFCDynamicLayout::MoveNone(),					CMFCDynamicLayout::SizeHorizontalAndVertical(100,100));
  manager.AddItem(IDC_BOUNDARY,   CMFCDynamicLayout::MoveNone(),					CMFCDynamicLayout::SizeHorizontal(100));
  manager.AddItem(IDC_GENERATE,   CMFCDynamicLayout::MoveHorizontal(100),	CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_CHARSET,    CMFCDynamicLayout::MoveNone(),					CMFCDynamicLayout::SizeHorizontal(100));
  manager.AddItem(IDC_USECHAR,    CMFCDynamicLayout::MoveHorizontal(100), CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_GRID,       CMFCDynamicLayout::MoveNone(),					CMFCDynamicLayout::SizeHorizontalAndVertical(100,100));
  manager.AddItem(IDC_NEW,        CMFCDynamicLayout::MoveVertical(100),   CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_DELETE,     CMFCDynamicLayout::MoveVertical(100),   CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_EDIT,       CMFCDynamicLayout::MoveVertical(100),   CMFCDynamicLayout::SizeNone());

  manager.AddItem(IDOK,			CMFCDynamicLayout::MoveHorizontalAndVertical(100,100),CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDCANCEL, CMFCDynamicLayout::MoveHorizontalAndVertical(100,100),CMFCDynamicLayout::SizeNone());
}

void
FormDataDlg::InitButtons()
{
  m_buttonNew.   SetIconImage(IDI_NEW);
  m_buttonDelete.SetIconImage(IDI_DELETE);
  m_buttonEdit.  SetIconImage(IDI_LIST);

  m_buttonOK    .SetStyle("ok");
  m_buttonCancel.SetStyle("can");
}

void
FormDataDlg::InitGrid()
{
  m_grid.SetColumnCount(5);
  m_grid.SetRowCount(1);
  m_grid.SetFixedRowCount(1);
  m_grid.GetCell(0, 0)->SetText("Part");
  m_grid.GetCell(0, 1)->SetText("Name");
  m_grid.GetCell(0, 2)->SetText("ContentType");
  m_grid.GetCell(0, 3)->SetText("File");
  m_grid.GetCell(0, 4)->SetText("Size");
  m_grid.SetColumnWidth(0, 80);
  m_grid.SetColumnWidth(1, 150);
  m_grid.SetColumnWidth(2, 150);
  m_grid.SetColumnWidth(3, 350);
  m_grid.SetColumnWidth(4, 100);
  m_grid.SetEditable(FALSE);
  m_grid.SetSingleRowSelection();
}

void 
FormDataDlg::ParsePayload()
{
  // Check that we have something to do
  if(m_orignalBody.IsEmpty())
  {
    return;
  }

  // In order to parse, it must be exactly as we receive it from the HTTP internet endpoint
  m_orignalBody.Replace("\r\n","\n");
  m_orignalBody.Replace("\n","\r\n");

  FileBuffer file;
  file.SetBuffer((uchar*)m_orignalBody.GetString(), m_orignalBody.GetLength());
  m_buffer.SetFileExtensions(m_useExtensions);
  if(!m_buffer.ParseBuffer(m_contentType,&file))
  {
    StyleMessageBox(this,"ALARM: Cannot parse body into a MultiPart/Form-data buffer!",KWATTA,MB_OK|MB_ICONERROR);
    OnCancel();
  }
  m_boundary = m_buffer.GetBoundary();

  if(m_inCharset.IsEmpty())
  {
    m_inCharset = "utf-8";
  }
}

void 
FormDataDlg::FillGrid()
{
  int index = 0;
  MultiPart* part = m_buffer.GetPart(index++);
  while(part)
  {
    FillRow(part,index);
    // Get next part
    part = m_buffer.GetPart(index++);
  }
}

void
FormDataDlg::FillRow(MultiPart* p_part,int p_row,bool p_new /*=true*/)
{
  CString size;
  size.Format("%d",(int)p_part->GetSize());

  if(p_new)
  {
    CString heading;
    heading.Format("%d", p_row);
    int row = m_grid.InsertRow(heading);
  }
  CString content = FindMimeTypeInContentType(p_part->GetContentType());

  m_grid.GetCell(p_row,1)->SetText(p_part->GetName());
  m_grid.GetCell(p_row,2)->SetText(content);
  m_grid.GetCell(p_row,3)->SetText(p_part->GetShortFileName());
  m_grid.GetCell(p_row,4)->SetText(size);
  m_grid.Invalidate();
}

// FormDataDlg message handlers

void 
FormDataDlg::OnEnChangeBoundary()
{
  UpdateData();
  if(m_buffer.SetBoundary(m_boundary) == false)
  {
    StyleMessageBox(this,"This boundary already exists in one of the parts!\n"
                         "Choose a different boundary or generate one!",KWATTA,MB_OK|MB_ICONERROR);
  }
  if(m_boundary.FindOneOf("$[<%#") >= 0)
  {
    StyleMessageBox(this,"This boundary contains one of the Kwatta parameter substitution characters!\n"
                         "Remove all the characters '$[]<>%#' from the boundary!",KWATTA,MB_OK|MB_ICONERROR);
  }
}

void 
FormDataDlg::OnEnChangeCharset()
{
  UpdateData();
}

void 
FormDataDlg::OnBnClickedGenerate()
{
  m_buffer.CalculateBoundary("@@");
  m_boundary = m_buffer.GetBoundary();
  UpdateData(FALSE);
}

void 
FormDataDlg::OnBnClickedUsechar()
{
  m_useCharset = m_checkUseCharset.GetCheck() > 0;
}

void 
FormDataDlg::OnBnClickedExtens()
{
  m_useExtensions = m_checkUseExtensions.GetCheck() > 0;
}

void 
FormDataDlg::OnGridDblClick(NMHDR* pNMHDR,LRESULT* pResult)
{
  LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
  OnBnClickedEdit();
}

void 
FormDataDlg::OnBnClickedNew()
{
  MultiPart* part = m_buffer.AddPart("part","","");
  int row = m_grid.GetRowCount();
  FillRow(part,row);

  AutoFocus focus;
  MultiPartDlg dlg(this, part);
  if(dlg.DoModal() == IDOK)
  {
    FillRow(part,row,false);
  }
}

void 
FormDataDlg::OnBnClickedDelete()
{
  CCellID cell = m_grid.GetFocusCell();
  if(cell.row > 0 && cell.row < m_grid.GetRowCount())
  {
    MultiPart* part = m_buffer.GetPart(cell.row - 1);
    if(part)
    {
      CString question;
      question.Format("Do you want to delete the buffer part [%s]",part->GetName().GetString());
      if(StyleMessageBox(this,question,KWATTA,MB_YESNO|MB_DEFBUTTON2|MB_ICONQUESTION) == IDNO)
      {
        return;
      }

      m_buffer.DeletePart(part);
      m_grid.DeleteRow(cell.row);
      // Renumber the headings of the rows
      CString heading;
      for(int index = 1;index < m_grid.GetRowCount();++index)
      {
        heading.Format("%d",index);
        m_grid.GetCell(index,0)->SetText(heading);
      }
      m_grid.Invalidate();
    }
  }
}

void FormDataDlg::OnBnClickedEdit()
{
  CCellID cell = m_grid.GetFocusCell();
  if(cell.row >= 0 && cell.row < m_grid.GetRowCount())
  {
    MultiPart* part = m_buffer.GetPart(cell.row - 1);
    if(part)
    {
      AutoFocus focus;
      MultiPartDlg dlg(this,part);
      if(dlg.DoModal() == IDOK)
      {
        FillRow(part,cell.row,false);
      }
    }
  }
}

void 
FormDataDlg::OnBnClickedOk()
{
  OnOK();
}

void 
FormDataDlg::OnBnClickedCancel()
{
  OnCancel();
}


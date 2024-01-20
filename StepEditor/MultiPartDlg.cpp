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
// This File   : MutliPartDlg.cpp
// What it does: Editing one of the parts of a multipart/form-data buffer
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StepEditor.h"
#include "MultiPartDlg.h"
#include "FormDataDlg.h"
#include "HTTPMessage.h"
#include <ConvertWideString.h>
#include <MultiPartBuffer.h>
#include <FileDialog.h>

// MultiPartDlg dialog

IMPLEMENT_DYNAMIC(MultiPartDlg, StyleDialog)

MultiPartDlg::MultiPartDlg(CWnd* p_parent,MultiPart* p_part)
             :StyleDialog(IDD_MULTIPART,p_parent)
             ,m_part(p_part)
             ,m_isFile(false)
{
}

MultiPartDlg::~MultiPartDlg()
{
}

void MultiPartDlg::DoDataExchange(CDataExchange* pDX)
{
  StyleDialog::DoDataExchange(pDX);

  DDX_Control (pDX,IDC_NAME,		 m_editName,	 m_name);
  DDX_Control (pDX,IDC_CHARSET,	 m_editCharset,m_charset);
  DDX_Control (pDX,IDC_ID,       m_editContent,m_contentID);
  DDX_CBString(pDX,IDC_CONTENT,	 m_comboContent,m_contentType);
  DDX_Control (pDX,IDC_ISFILE,	 m_checkIsFile);
  DDX_Control (pDX,IDC_MPART,    m_buttonMulti);
  DDX_Control (pDX,IDC_BOUNDARY, m_editBoundary,m_boundary);
  DDX_Control (pDX,IDC_FULLFILE, m_editLongFilename, m_LongFilename);
  DDX_Control (pDX,IDC_SHORTFILE,m_editShortFilename,m_shortFilename);
  DDX_Control (pDX,IDC_BUTT_FILE,m_buttonFile);
  DDX_Control (pDX,IDC_DATA,		 m_editData,m_data);
  DDX_Control (pDX,IDOK,				 m_buttonOK);
  DDX_Control (pDX,IDCANCEL,		 m_buttonCancel);

  if(pDX->m_bSaveAndValidate == FALSE)
  {
    // Filename
    m_editLongFilename .EnableWindow(m_isFile);
    m_editShortFilename.EnableWindow(m_isFile);
    m_buttonFile       .EnableWindow(m_isFile);
    // Datapart
    m_editData.EnableWindow(!m_isFile);

    bool multi = false;
    if(m_contentType.Find("multipart") >= 0)
    {
      multi = true;
    }
    m_buttonMulti.EnableWindow(multi);
    m_editContent.EnableWindow(!multi);
    m_editData.SetMutable(!multi);
  }
}

BEGIN_MESSAGE_MAP(MultiPartDlg, StyleDialog)
  ON_EN_CHANGE    (IDC_NAME,			&MultiPartDlg::OnEnChangeName)
  ON_EN_CHANGE    (IDC_CHARSET,		&MultiPartDlg::OnEnChangeCharset)
  ON_EN_CHANGE    (IDC_ID,        &MultiPartDlg::OnEnChangeContentID)
  ON_CBN_SELCHANGE(IDC_CONTENT,		&MultiPartDlg::OnCbnSelchangeContent)
  ON_BN_CLICKED   (IDC_ISFILE,		&MultiPartDlg::OnBnClickedIsfile)
  ON_BN_CLICKED   (IDC_MPART,     &MultiPartDlg::OnBnClickedMulti)
  ON_EN_CHANGE    (IDC_FULLFILE,	&MultiPartDlg::OnEnChangeFullfile)
  ON_BN_CLICKED   (IDC_BUTT_FILE, &MultiPartDlg::OnBnClickedButtFile)
  ON_EN_CHANGE    (IDC_SHORTFILE, &MultiPartDlg::OnEnChangeShortfile)
  ON_EN_CHANGE    (IDC_DATA,			&MultiPartDlg::OnEnChangeData)
  ON_BN_CLICKED   (IDOK,					&MultiPartDlg::OnBnClickedOk)
  ON_BN_CLICKED   (IDCANCEL,			&MultiPartDlg::OnBnClickedCancel)
END_MESSAGE_MAP()

BOOL
MultiPartDlg::OnInitDialog()
{
  StyleDialog::OnInitDialog();
  SetWindowText("MultiPart");

  UseMultiPart();
  InitButtons();
  InitContent();

  UpdateData(FALSE);
  return TRUE;
}

void
MultiPartDlg::InitButtons()
{
  m_buttonOK    .SetStyle("ok");
  m_buttonCancel.SetStyle("can");
  m_buttonFile  .SetStyle("dir");
}

void
MultiPartDlg::InitContent()
{
  m_comboContent.AddString("application/soap+xml");
  m_comboContent.AddString("application/xml");
  m_comboContent.AddString("application/json");
  m_comboContent.AddString("application/http");
  m_comboContent.AddString("application/octet-stream");
  m_comboContent.AddString("text/plain");
  m_comboContent.AddString("text/html");
  m_comboContent.AddString("text/xml");
  m_comboContent.AddString("text/javascript");
  m_comboContent.AddString("multipart/form-data");
  m_comboContent.AddString("multipart/mixed");

  if(m_charset.IsEmpty())
  {
    m_charset = "utf-8";
  }
  m_checkIsFile.SetCheck(m_isFile);
  if(m_isFile && m_contentType.IsEmpty())
  {
    m_contentType = "application/octet-stream";
  }
}

void
MultiPartDlg::UseMultiPart()
{
  m_name          =  m_part->GetName();
  m_contentType   =  m_part->GetContentType();
  m_charset       =  m_part->GetCharset();
  m_boundary      =  m_part->GetBoundary();
  m_isFile        = !m_part->GetShortFileName().IsEmpty();
  m_shortFilename =  m_part->GetShortFileName();
  m_LongFilename  =  m_part->GetLongFileName();
  m_data          =  m_part->GetData();
  m_contentID     =  m_part->GetHeader("Content-ID");

  m_boundary = FindFieldInHTTPHeader(m_contentType,"boundary");
  int pos = m_contentType.Find(';');
  if (pos > 0)
  {
    m_contentType = m_contentType.Left(pos);
  }
}

void
MultiPartDlg::SaveMultiPart()
{
  CString content = m_contentType + "; boundary=" + m_boundary;

  m_part->SetName(m_name);
  m_part->SetContentType(content);
  m_part->SetCharset(m_charset);
  m_part->SetBoundary(m_boundary);
  if(m_isFile)
  {
    m_part->SetFileName(m_shortFilename);
  }
  else
  {
    m_part->SetData(m_data);
  }
  if(!m_contentID.IsEmpty())
  {
    m_part->AddHeader("Content-ID",m_contentID);
  }
}

// MultiPartDlg message handlers

void
MultiPartDlg::OnEnChangeName()
{
  UpdateData();
}

void 
MultiPartDlg::OnEnChangeCharset()
{
  UpdateData();
}

void 
MultiPartDlg::OnEnChangeContentID()
{
  UpdateData();
}

void 
MultiPartDlg::OnCbnSelchangeContent()
{
  int ind = m_comboContent.GetCurSel();
  if (ind >= 0)
  {
    m_comboContent.GetLBText(ind,m_contentType);
  }
  UpdateData(FALSE);
}

void 
MultiPartDlg::OnBnClickedIsfile()
{
  m_isFile = m_checkIsFile.GetCheck() > 0;
  UpdateData(FALSE);
}

void 
MultiPartDlg::OnBnClickedMulti()
{
  UpdateData();

  CString content = m_contentType;
  content += "; boundary=";
  content += m_boundary;

  AutoFocus focus;
  FormDataDlg dlg(this,m_data,content);
  dlg.DoModal();

  HTTPMessage msg;
  msg.SetMultiPartFormData(dlg.GetBuffer());
  m_data = msg.GetBody();
  m_contentType = msg.GetContentType();

  m_boundary = FindFieldInHTTPHeader(m_contentType,"boundary");
  int pos = m_contentType.Find(';');
  if (pos)
  {
    m_contentType = m_contentType.Left(pos);
  }
  UpdateData(FALSE);
}

void 
MultiPartDlg::OnEnChangeFullfile()
{
  UpdateData();
  if(!m_LongFilename.IsEmpty() && m_contentType.IsEmpty())
  {
    m_contentType = "application/octet-stream";
    UpdateData(FALSE);
  }
}

void MultiPartDlg::OnBnClickedButtFile()
{
  AutoFocus focus;
  DocFileDialog dlg(GetSafeHwnd(),true,"Select a file for a MultiPart","",m_LongFilename);
  if(dlg.DoModal())
  {
    m_LongFilename = dlg.GetChosenFile();
    m_part->SetFile(m_LongFilename);
    m_shortFilename = m_part->GetShortFileName();
    if(m_contentType.IsEmpty())
    {
      m_contentType = "application/octet-stream";
    }
    UpdateData(FALSE);
  }
}

void 
MultiPartDlg::OnEnChangeShortfile()
{
  UpdateData();
}

void 
MultiPartDlg::OnEnChangeData()
{
  UpdateData();
}

void 
MultiPartDlg::OnBnClickedOk()
{
  SaveMultiPart();
  StyleDialog::OnOK();
}

void 
MultiPartDlg::OnBnClickedCancel()
{
  StyleDialog::OnCancel();
}

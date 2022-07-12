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
// This File   : RequestDlg.cpp
// What it does: Tab in the test editor for the plain request of an internet HTTP call
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StepEditor.h"
#include "TestStepNET.h"
#include "RequestDlg.h"
#include "StepInternetDlg.h"
#include "FormDataDlg.h"
#include <FileDialog.h>
#include <SearchVarDlg.h>
#include <XMLMessage.h>
#include <JSONMessage.h>
#include <EnsureFile.h>
#include <ExecuteShell.h>
#include <WinFile.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// PayloadDlg dialog

IMPLEMENT_DYNAMIC(RequestDlg, StyleDialog)

RequestDlg::RequestDlg(CWnd* pParent /*=nullptr*/)
           :StyleDialog(IDD_REQUEST, pParent)
{
}

RequestDlg::~RequestDlg()
{
}

void 
RequestDlg::DoDataExchange(CDataExchange* pDX)
{
	StyleDialog::DoDataExchange(pDX);
  DDX_CBString(pDX,IDC_CONTENT,     m_comboMime,m_mimeType);
  DDX_Control (pDX,IDC_CHECK,       m_buttonCheck);
  DDX_Control (pDX,IDC_PARAM,       m_buttonParam);
  DDX_Control (pDX,IDC_MULTI,       m_buttonMulti);
  DDX_Control (pDX,IDC_PAYLOAD,     m_editPayload,m_payload);
  DDX_Control (pDX,IDC_USEFILE,     m_checkUseFile);
  DDX_Control (pDX,IDC_INPUTFILE,   m_editInputFile,m_inputFile);
  DDX_Control (pDX,IDC_BUT_INFILE,  m_buttonChooseFile);
  DDX_Control (pDX,IDC_SAVEFILE,    m_checkSaveFile);
  DDX_Control (pDX,IDC_OUTPUTFILE,  m_editOutputFile,m_outputFile);
  DDX_Control (pDX,IDC_BUT_OUTFILE, m_buttonSaveFile);
  DDX_Control(pDX, IDC_SHOW_INFILE, m_buttonShowInfile);
  DDX_Control(pDX, IDC_SHOW_OUTFILE,m_buttonShowOutfile);

  if(pDX->m_bSaveAndValidate == FALSE)
  {
    bool active = false;
    if(m_mimeType.Find("xml")  >= 0)       active = true;
    if(m_mimeType.Find("json") >= 0)       active = true;
    if(m_mimeType.Find("multipart") >= 0)  active = true;

    m_buttonCheck.EnableWindow(active);

    bool multi = false;
    if(m_mimeType.Find("multipart") >= 0)
    {
      multi = true;
    }
    m_editPayload.SetMutable(!multi && !m_useFile);
    m_buttonMulti      .EnableWindow(multi);
    m_editInputFile    .EnableWindow(m_useFile);
    m_buttonChooseFile .EnableWindow(m_useFile);
    m_buttonShowInfile .EnableWindow(m_useFile);
    m_editOutputFile   .EnableWindow(m_saveFile);
    m_buttonSaveFile   .EnableWindow(m_saveFile);
    m_buttonShowOutfile.EnableWindow(m_saveFile);
  }
}

BEGIN_MESSAGE_MAP(RequestDlg, StyleDialog)
  ON_CBN_SELCHANGE(IDC_CONTENT,     &RequestDlg::OnCbnSelchangeMime)
  ON_BN_CLICKED   (IDC_CHECK,       &RequestDlg::OnBnClickedCheck)
  ON_BN_CLICKED   (IDC_PARAM,       &RequestDlg::OnBnClickedParam)
  ON_BN_CLICKED   (IDC_MULTI,       &RequestDlg::OnBnClickedMulti)
  ON_EN_KILLFOCUS (IDC_PAYLOAD,     &RequestDlg::OnEnKillfocusPayload)
  ON_BN_CLICKED   (IDC_USEFILE,     &RequestDlg::OnBnClickedUseFile)
  ON_EN_KILLFOCUS (IDC_INPUTFILE,   &RequestDlg::OnEnKillfocusInputFile)
  ON_BN_CLICKED   (IDC_BUT_INFILE,  &RequestDlg::OnBnClickedChooseFile)
  ON_BN_CLICKED   (IDC_SAVEFILE,    &RequestDlg::OnBnClickedSaveFile)
  ON_EN_KILLFOCUS (IDC_OUTPUTFILE,  &RequestDlg::OnEnKillfocusOutputFile)
  ON_BN_CLICKED   (IDC_BUT_OUTFILE, &RequestDlg::OnBnClickedOutputFile)
  ON_BN_CLICKED   (IDC_SHOW_INFILE, &RequestDlg::OnBnClickedShowInfile)
  ON_BN_CLICKED   (IDC_SHOW_OUTFILE,&RequestDlg::OnBnClickedShowOutfile)
END_MESSAGE_MAP()

BOOL
RequestDlg::OnInitDialog()
{
  StyleDialog::OnInitDialog();

  m_buttonCheck.SetIconImage(IDI_CHECK);
  m_buttonParam.SetIconImage(IDI_LIST);
  m_buttonChooseFile.SetStyle("dir");
  m_buttonSaveFile  .SetStyle("dir");
  m_buttonShowInfile.SetIconImage(IDI_EXAMPLE);
  m_buttonShowOutfile.SetIconImage(IDI_EXAMPLE);
  EnableToolTips();
  RegisterTooltip(m_buttonCheck,"Check correctness of the XML/JSON structures.");
  RegisterTooltip(m_buttonParam,"Choose global/test parameter.");

  InitCombo();
  InitPayload();

  SetCanResize();
  return TRUE;
}

void
RequestDlg::SetupDynamicLayout()
{
  StyleDialog::SetupDynamicLayout();

  CMFCDynamicLayout& manager = *GetDynamicLayout();
#ifdef _DEBUG
  manager.AssertValid();
#endif

  manager.AddItem(IDC_PAYLOAD,      CMFCDynamicLayout::MoveNone(),                         CMFCDynamicLayout::SizeHorizontalAndVertical(100,100));
  manager.AddItem(IDC_MULTI,        CMFCDynamicLayout::MoveHorizontal(100),                CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_ST_USEFILE,   CMFCDynamicLayout::MoveVertical(100),                  CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_USEFILE,      CMFCDynamicLayout::MoveVertical(100),                  CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_INPUTFILE,    CMFCDynamicLayout::MoveVertical(100),                  CMFCDynamicLayout::SizeHorizontal(100));
  manager.AddItem(IDC_BUT_INFILE,   CMFCDynamicLayout::MoveHorizontalAndVertical(100,100), CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_ST_SAVEFILE,  CMFCDynamicLayout::MoveVertical(100),                  CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_SAVEFILE,     CMFCDynamicLayout::MoveVertical(100),                  CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_OUTPUTFILE,   CMFCDynamicLayout::MoveVertical(100),                  CMFCDynamicLayout::SizeHorizontal(100));
  manager.AddItem(IDC_BUT_OUTFILE,  CMFCDynamicLayout::MoveHorizontalAndVertical(100,100), CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_SHOW_INFILE,  CMFCDynamicLayout::MoveHorizontalAndVertical(100,100), CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_SHOW_OUTFILE, CMFCDynamicLayout::MoveHorizontalAndVertical(100,100), CMFCDynamicLayout::SizeNone());
}

void
RequestDlg::InitCombo()
{
  m_comboMime.AddString("application/soap+xml");
  m_comboMime.AddString("application/xml");
  m_comboMime.AddString("application/json");
  m_comboMime.AddString("application/octet-stream");
  m_comboMime.AddString("text/plain");
  m_comboMime.AddString("text/html");
  m_comboMime.AddString("text/xml");
  m_comboMime.AddString("text/javascript");
  m_comboMime.AddString("multipart/form-data");
  m_comboMime.AddString("multipart/mixed");
}

void
RequestDlg::InitPayload()
{
  m_editPayload.SetFontName("Courier new",100);
}

void 
RequestDlg::InitTab(TestStepNET* p_testStep,Parameters* p_parameters)
{
  // Register
  m_testStep   = p_testStep;
  m_parameters = p_parameters;
  // Setting the tab
  m_payload    = m_testStep->GetBody();
  m_mimeType   = m_testStep->GetMimeType();
  m_inputFile  = m_testStep->GetFilenameInput();
  m_useFile    = m_testStep->GetBodyInputIsFile();
  m_outputFile = m_testStep->GetFilenameOutput();
  m_saveFile   = m_testStep->GetBodyOutputIsFile();

  m_payload.Replace("\n","\r\n");

  int ind = m_comboMime.FindStringExact(0,m_mimeType);
  if(ind >= 0)
  {
    m_comboMime.SetCurSel(ind);
  }
  m_checkUseFile.SetCheck(m_useFile);
  m_checkSaveFile.SetCheck(m_saveFile);
  UpdateData(FALSE);
}

bool
RequestDlg::IsFilled()
{
  return !m_payload.IsEmpty();
}

void
RequestDlg::EffectiveParameters()
{
  if(m_testStep)
  {
    // Check parameters
    StepInternetDlg* step = reinterpret_cast<StepInternetDlg*>(GetParent()->GetParent());
    step->EffectiveParameters();
  }
}

void
RequestDlg::StoreVariables()
{
  UpdateData();

  CString payload(m_payload);
  payload.Remove('\r');

  m_testStep->SetBody(payload);
  m_testStep->SetMimeType(m_mimeType);
  m_testStep->SetBodyInputIsFile(m_useFile);
  m_testStep->SetFilenameInput(m_inputFile);
  m_testStep->SetBodyOutputIsFile(m_saveFile);
  m_testStep->SetFilenameOutput(m_outputFile);
}

void
RequestDlg::CheckXML()
{
  XMLMessage msg;
  msg.ParseMessage(m_payload);

  if(msg.GetInternalError() != XmlError::XE_NoError)
  {
    CString warning;
    warning.Format("BEWARE: Body payload has an XML error in it!\n%s",msg.GetInternalErrorString().GetString());
    StyleMessageBox(this,warning,"WARNING",MB_OK|MB_ICONERROR);
    return;
  }
  StyleMessageBox(this,"XML Message checks out OK","CHECK",MB_OK);
}

void
RequestDlg::CheckJSON()
{
  JSONMessage msg;
  msg.ParseMessage(m_payload);

  if(msg.GetErrorState())
  {
    CString warning;
    warning.Format("BEWARE: Body payload has an JSON error in it!\n%s",msg.GetLastError().GetString());
    StyleMessageBox(this,warning,"WARNING",MB_OK | MB_ICONERROR);
    return;
  }
  StyleMessageBox(this,"JSON Message checks out OK","CHECK",MB_OK);
}

void 
RequestDlg::UpdateStepHeader(CString p_contentType)
{
  for(auto& head : m_testStep->GetHeaders())
  {
    if(head.m_name.CompareNoCase("content-type") == 0)
    {
      head.m_value = p_contentType;
      break;
    }
  }

  m_testStep->SetHeader("Content-type",p_contentType);

  StepInternetDlg* parent = reinterpret_cast<StepInternetDlg*>(GetParent()->GetParent());
  if(parent)
  {
    parent->RefreshHeaders();
  }
}

// PayloadDlg message handlers

void 
RequestDlg::OnCbnSelchangeMime()
{
  UpdateData();
  UpdateData(FALSE);
}

void 
RequestDlg::OnBnClickedCheck()
{
  if(m_mimeType.Find("xml") >= 0)
  {
    CheckXML();
  }
  if(m_mimeType.Find("json") >= 0)
  {
    CheckJSON();
  }
}

void 
RequestDlg::OnBnClickedParam()
{
  SearchVarDlg dlg(this,m_parameters,true,true,true,true);
  if(dlg.DoModal() == IDOK || dlg.GetSaved())
  {
    CString variable = dlg.GetVariable();
    m_editPayload.InsertAtCurPos(variable,0);
    OnEnKillfocusPayload();
  }
}

void 
RequestDlg::OnBnClickedMulti()
{
  //  Get a content type
  CString content = m_testStep->GetHeader("Content-Type");
  if(content.IsEmpty())
  {
    content = m_mimeType;
  }

  FormDataDlg dlg(this,m_payload,content);
  dlg.DoModal();

  HTTPMessage msg;
  msg.SetMultiPartFormData(dlg.GetBuffer());
  m_payload  = msg.GetBody();
  m_mimeType = msg.GetContentType();

  UpdateStepHeader(m_mimeType);

  int pos = m_mimeType.Find(';');
  if (pos)
  {
    m_mimeType = m_mimeType.Left(pos);
  }
  UpdateData(FALSE);
}

void 
RequestDlg::OnEnKillfocusPayload()
{
  // Getting the payload as body for the message
  UpdateData();
  EffectiveParameters();
}

void 
RequestDlg::OnBnClickedUseFile()
{
  m_useFile = m_checkUseFile.GetCheck() > 0;
  UpdateData(FALSE);
}

void 
RequestDlg::OnEnKillfocusInputFile()
{
  UpdateData();
  EffectiveParameters();
}

void 
RequestDlg::OnBnClickedChooseFile()
{
  CString filter;
  filter  = "All files *.*|*.*";
  filter += "|PNG Image files *.png|*.png";
  filter += "|JPG Image files *.jpg|*.jpg";
  filter += "|GIF Image files *.gif|*.gif";
  filter += "|PDF portable documents *.pdf|*.pdf";
  filter += "|DOCX MS-Word documents *.docx|*.docx";
  filter += "|RTF Richt Text Format *.rtf|*.rtf";

  DocFileDialog doc(GetSafeHwnd(),true,"Choose file to input as body","",m_inputFile,0,filter);
  if(doc.DoModal() == IDOK)
  {
    CString file = doc.GetChosenFile();
    EnsureFile ens(file);

    CString relative;
    CString basedir = theApp.GetBaseDirectory();
    ens.MakeRelativePathname(basedir,file,relative);
    relative.Replace("/","\\");
    m_inputFile = relative;
    UpdateData(FALSE);
  }
}

void 
RequestDlg::OnBnClickedSaveFile()
{
  m_saveFile = m_checkSaveFile.GetCheck() > 0;
  UpdateData(FALSE);
}

void 
RequestDlg::OnEnKillfocusOutputFile()
{
  UpdateData();
  EffectiveParameters();
}

void 
RequestDlg::OnBnClickedOutputFile()
{
  CString filter;
  filter  = "All files *.*|*.*";
  filter += "|PNG Image files *.png|*.png";
  filter += "|JPG Image files *.jpg|*.jpg";
  filter += "|GIF Image files *.gif|*.gif";
  filter += "|PDF portable documents *.pdf|*.pdf";
  filter += "|DOCX MS-Word documents *.docx|*.docx";
  filter += "|RTF Richt Text Format *.rtf|*.rtf";

  DocFileDialog doc(GetSafeHwnd(),false,"Choose file to output the body to","",m_outputFile,0,filter);
  if(doc.DoModal() == IDOK)
  {
    CString file = doc.GetChosenFile();
    EnsureFile ens(file);

    CString relative;
    CString basedir = theApp.GetBaseDirectory();
    ens.MakeRelativePathname(basedir,file,relative);
    relative.Replace("/","\\");
    m_outputFile = relative;
    UpdateData(FALSE);
  }
}

void 
RequestDlg::OnBnClickedShowInfile()
{
  CString error;
  CString filename = theApp.GetBaseDirectory() + m_inputFile;
  WinFile file(filename);
  if(file.Exists())
  {
    ExecuteShell("open",filename,"",GetSafeHwnd(),true,&error);
  }
}

void 
RequestDlg::OnBnClickedShowOutfile()
{
  CString error;
  CString filename = theApp.GetBaseDirectory() + m_outputFile;
  WinFile file(filename);
  if(file.Exists())
  {
    ExecuteShell("open",filename,"",NULL,true,&error);
  }
}

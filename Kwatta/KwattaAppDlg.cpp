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
// This program: TestSuite
// This File   : KwattaAppDlg.cpp
// What it does: Displays the main application window
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "KwattaAppDlg.h"
#include "KwattaApp.h"
#include "EnsureFile.h"
#include "StdException.h"
#include "afxdialogex.h"
#include "GridCellCheck.h"
#include "NewTestDlg.h"
#include "StarterDlg.h"
#include "ProceduresDlg.h"
#include "MutateNamesDlg.h"
#include "SystemDlg.h"
#include <FileDialog.h>
#include <SearchVarDlg.h>
#include <AboutDlg.h>
#include <GetLastErrorAsString.h>
#include <ExtraMessages.h>
#include "resource.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

using namespace ThemeColor;

IMPLEMENT_DYNAMIC(KwattaAppDlg, StyleDialog)

KwattaAppDlg::KwattaAppDlg(CWnd* p_parent,CString p_suiteFilename)
             :StyleDialog(IDD_TESTSUITE, p_parent)
             ,m_testsuite(p_suiteFilename)
{
}

KwattaAppDlg::~KwattaAppDlg()
{
  Reset();
}

void
KwattaAppDlg::Reset()
{
  if(m_suite)
  {
    delete m_suite;
    m_suite = nullptr;
  }
  if(m_parameters)
  {
    delete m_parameters;
    m_parameters = nullptr;
  }
}

void 
KwattaAppDlg::DoDataExchange(CDataExchange* pDX)
{
  StyleDialog::DoDataExchange(pDX);

  DDX_Control(pDX,IDC_SUITE,			m_editTestsuite,	m_testsuite);
  DDX_Control(pDX,IDC_DESCRIPTION,m_editDescription,m_description);
  DDX_Control(pDX,IDC_LIST,       m_list);
  DDX_Control(pDX,IDC_BUT_CHOOSE, m_buttonChoose);
  DDX_Control(pDX,IDC_BUT_SUITE,  m_buttonSuite);
  DDX_Control(pDX,IDC_BUT_PARAM,  m_buttonParam);
  DDX_Control(pDX,IDC_BUT_PROC,   m_buttonProc);
  DDX_Control(pDX,IDC_BUT_RUN,    m_buttonRun);
  DDX_Control(pDX,IDC_BUT_ALL,    m_buttonRunAll);
  DDX_Control(pDX,IDC_BUT_EDIT,   m_buttonEdit);
  DDX_Control(pDX,IDC_BUT_NEW,    m_buttonNew);
  DDX_Control(pDX,IDC_BUT_REMOVE, m_buttonRemove);
  DDX_Control(pDX,IDC_BUT_UP,     m_buttonUP);
  DDX_Control(pDX,IDC_BUT_DOWN,   m_buttonDN);
  DDX_Control(pDX,IDC_BUT_MUTATE, m_buttonMutate);
  DDX_Control(pDX,IDOK,           m_buttonOK);
  DDX_Control(pDX,IDCANCEL,       m_buttonCancel);

  if(pDX->m_bSaveAndValidate == FALSE)
  {
    bool active = m_list.GetRowCount() > 1;
    m_buttonRun   .EnableWindow(active);
    m_buttonRunAll.EnableWindow(active);
    m_buttonEdit  .EnableWindow(active);
    m_buttonRemove.EnableWindow(active);
    m_buttonParam .EnableWindow(active);
    m_buttonUP    .EnableWindow(active);
    m_buttonDN    .EnableWindow(active);
    m_buttonMutate.EnableWindow(active);

  }
  m_editTestsuite.SetMutable(m_testsuite.IsEmpty());
  m_editTestsuite.SetBkColor(m_testsuite.IsEmpty() ? ClrEditBkgnd : ThemeColor::GetColor(Colors::AccentColor2));
}

BEGIN_MESSAGE_MAP(KwattaAppDlg, StyleDialog)
  ON_WM_SYSCOMMAND()

  ON_MESSAGE     (WM_READYTEST,       &KwattaAppDlg::OnReadyTest)
  ON_EN_KILLFOCUS(IDC_SUITE,          &KwattaAppDlg::OnEnChangeSuite)
  ON_EN_KILLFOCUS(IDC_DESCRIPTION,    &KwattaAppDlg::OnEnChangeDescription)
  ON_BN_CLICKED  (IDC_BUT_CHOOSE,     &KwattaAppDlg::OnBnClickedButChoose)
  ON_BN_CLICKED  (IDC_BUT_SUITE,      &KwattaAppDlg::OnBnClickedButSuite)
  ON_BN_CLICKED  (IDC_BUT_PARAM,      &KwattaAppDlg::OnBnClickedButParam)
  ON_BN_CLICKED  (IDC_BUT_PROC,       &KwattaAppDlg::OnBnClickedButProc)
  ON_NOTIFY(LVN_ITEMCHANGED,IDC_LIST, &KwattaAppDlg::OnGridItemChanged)
  ON_NOTIFY(NM_DBLCLK,      IDC_LIST, &KwattaAppDlg::OnGridDblClick)

  ON_BN_CLICKED(IDC_BUT_RUN,          &KwattaAppDlg::OnBnClickedButRun)
  ON_BN_CLICKED(IDC_BUT_ALL,          &KwattaAppDlg::OnBnClickedButAll)
  ON_BN_CLICKED(IDC_BUT_EDIT,         &KwattaAppDlg::OnBnClickedButEdit)
  ON_BN_CLICKED(IDC_BUT_NEW,          &KwattaAppDlg::OnBnClickedButNew)
  ON_BN_CLICKED(IDC_BUT_REMOVE,       &KwattaAppDlg::OnBnClickedButRemove)
  ON_BN_CLICKED(IDC_BUT_UP,           &KwattaAppDlg::OnBnClickedButUP)
  ON_BN_CLICKED(IDC_BUT_DOWN,         &KwattaAppDlg::OnBnClickedButDN)
  ON_BN_CLICKED(IDC_BUT_MUTATE,       &KwattaAppDlg::OnBnClickedButMutate)
  ON_BN_CLICKED(IDOK,                 &KwattaAppDlg::OnBnClickedOk)
  ON_BN_CLICKED(IDCANCEL,             &KwattaAppDlg::OnBnClickedCancel)

  ON_COMMAND(ID_MENU_THEMELIME,       OnStyleLime)
  ON_COMMAND(ID_MENU_THEMESKYBLUE,    OnStyleSkyblue)
  ON_COMMAND(ID_MENU_THEMEPURPLE,     OnStylePurple)
  ON_COMMAND(ID_MENU_THEMEMODERATE,   OnStyleModerateGray)
  ON_COMMAND(ID_MENU_THEMEPURE,       OnStylePureGray)
  ON_COMMAND(ID_MENU_THEMEBLACK,      OnStyleBlackWhite)
  ON_COMMAND(ID_MENU_ABOUT,           OnAbout)
  ON_COMMAND(ID_MENU_EXIT,            OnExit)

END_MESSAGE_MAP()

BOOL
KwattaAppDlg::OnInitDialog()
{
  StyleDialog::OnInitDialog();
  SetWindowText("Kwatta");
  ShowMinMaxButton();
  SetSysMenu(IDR_MENU);
  ShowSysMenu();
  AppendSysMenu();

  // Fill the dialog
  InitButtons();
  InitGrid();
  DoStarter();

  SetCanResize();

  if(m_testsuite.IsEmpty())
  {
    m_editTestsuite.SetFocus();
  }
  else if(m_list.GetRowCount() > 1)
  {
    m_list.SetFocus();
    m_list.SetFocusCell(1,0);
    SetUpDownButtons(1);
  }
  else
  {
    m_editDescription.SetFocus();
  }
  return FALSE;
}

void
KwattaAppDlg::SetupDynamicLayout()
{
  StyleDialog::SetupDynamicLayout();

  CMFCDynamicLayout& manager = *GetDynamicLayout();
#ifdef _DEBUG
  manager.AssertValid();
#endif

  manager.AddItem(IDC_GRP_SUITE,  CMFCDynamicLayout::MoveNone(),				CMFCDynamicLayout::SizeHorizontalAndVertical(100,20));
  manager.AddItem(IDC_GRP_TESTS,  CMFCDynamicLayout::MoveVertical(20),	CMFCDynamicLayout::SizeHorizontalAndVertical(100,80));

  manager.AddItem(IDC_SUITE,			CMFCDynamicLayout::MoveNone(),				 CMFCDynamicLayout::SizeHorizontal(100));
  manager.AddItem(IDC_BUT_CHOOSE, CMFCDynamicLayout::MoveHorizontal(100),CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_BUT_SUITE,  CMFCDynamicLayout::MoveHorizontal(100),CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_DESCRIPTION,CMFCDynamicLayout::MoveNone(),         CMFCDynamicLayout::SizeHorizontalAndVertical(100,20));
  manager.AddItem(IDC_BUT_PARAM,  CMFCDynamicLayout::MoveHorizontal(100),CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_BUT_PROC,   CMFCDynamicLayout::MoveHorizontal(100),CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_LIST,       CMFCDynamicLayout::MoveVertical(20),   CMFCDynamicLayout::SizeHorizontalAndVertical(100,80));
  manager.AddItem(IDC_BUT_RUN,    CMFCDynamicLayout::MoveVertical(100),  CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_BUT_ALL,    CMFCDynamicLayout::MoveVertical(100),  CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_BUT_EDIT,   CMFCDynamicLayout::MoveVertical(100),  CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_BUT_NEW,    CMFCDynamicLayout::MoveVertical(100),  CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_BUT_REMOVE, CMFCDynamicLayout::MoveVertical(100),  CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_BUT_UP,     CMFCDynamicLayout::MoveVertical(100),  CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_BUT_DOWN,   CMFCDynamicLayout::MoveVertical(100),  CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_BUT_MUTATE, CMFCDynamicLayout::MoveVertical(100),  CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDOK,           CMFCDynamicLayout::MoveHorizontalAndVertical(100,100),CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDCANCEL,       CMFCDynamicLayout::MoveHorizontalAndVertical(100,100),CMFCDynamicLayout::SizeNone());
}

void 
KwattaAppDlg::AppendSysMenu()
{
  // IDM_ABOUTBOX must be in the system command range.
  ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
  ASSERT (IDM_ABOUTBOX < 0xF000);

  CMenu* pSysMenu = GetSystemMenu(FALSE);
  if(pSysMenu != nullptr)
  {
    CString strAboutMenu("About Kwatta...");
    if(!strAboutMenu.IsEmpty())
    {
      pSysMenu->AppendMenu(MF_SEPARATOR);
      pSysMenu->AppendMenu(MF_STRING,IDM_ABOUTBOX,strAboutMenu);
    }
  }
}

void
KwattaAppDlg::InitButtons()
{
  m_buttonRun   .SetIconImage(IDI_RUN);
  m_buttonRunAll.SetIconImage(IDI_RUNALL);
  m_buttonEdit  .SetIconImage(IDI_EDIT);
  m_buttonNew   .SetIconImage(IDI_NEW);
  m_buttonRemove.SetIconImage(IDI_DELETE);
  m_buttonUP    .SetIconImage(IDI_UP);
  m_buttonDN    .SetIconImage(IDI_DOWN);
  m_buttonMutate.SetIconImage(IDI_MUTATE);

  m_buttonChoose.SetStyle("dir");
  m_buttonOK    .SetStyle("ok");
  m_buttonCancel.SetStyle("can");

  EnableToolTips();

  RegisterTooltip(m_buttonRun,    "Run the testset");
  RegisterTooltip(m_buttonRunAll, "Run all the testsets in the suite");
  RegisterTooltip(m_buttonNew,    "Add a new testset");
  RegisterTooltip(m_buttonEdit,   "Edit the testset");
  RegisterTooltip(m_buttonRemove, "Delete the testset");
  RegisterTooltip(m_buttonUP,     "Move current testset one row UP");
  RegisterTooltip(m_buttonDN,     "Move current testset one row DOWN");
  RegisterTooltip(m_buttonMutate, "Change the directory or filename of the testset.");
}

void
KwattaAppDlg::DoStarter()
{
  if(m_testsuite.IsEmpty())
  {
    StarterDlg starter(this);
    INT_PTR res = starter.DoModal();
    if(res == IDOK)
    {
      CString path = starter.GetChosenSuite();
      if(!path.IsEmpty())
      {
        m_testsuite = path;
        UpdateData(FALSE);
      }
    }
    else if(res == IDNO)
    {
      // Ending the program
      EndDialog(IDNO);
      return;
    }
  }
  if(!m_testsuite.IsEmpty())
  {
    ReadSuite();
  }
}

void
KwattaAppDlg::ReadSuite()
{
  if(m_testsuite.IsEmpty())
  {
    return;
  }
  try
  {
    EnsureFile ensure;
    CString base = ensure.DirectoryPart(m_testsuite);
    Reset();
    m_suite = new TestSuite(base);

    m_suite->ReadFromXML(m_testsuite);
    theApp.SetBaseDirectory(base);

    m_description = m_suite->GetDescription();
    FillGrid();
    UpdateData(FALSE);
  }
  catch(StdException& ex)
  {
    CString error;
    error.Format("Cannot read the test suite definition file: %s\n"
                 "Error found: %s"
                 ,m_testsuite.GetString()
                 ,ex.GetErrorMessage().GetString());
    StyleMessageBox(this,error,"ERROR",MB_OK|MB_ICONERROR);
  }
}

void
KwattaAppDlg::InitGrid()
{
  m_list.SetRowCount(1);
  m_list.SetColumnCount(4);
  m_list.SetFixedRowCount(1);
  //m_list.SetFixedColumnCount(1);
  m_list.SetEditable(FALSE);
  m_list.SetSingleColSelection(TRUE);
  m_list.SetSingleRowSelection(TRUE);

  m_list.GetCell(0,0)->SetText("Testset");
  m_list.GetCell(0,1)->SetText("Run this testset");
  m_list.GetCell(0,2)->SetText("Name of the testset");
  m_list.GetCell(0,3)->SetText("Result of last run");

  m_list.SetColumnWidth(0,100);
  m_list.SetColumnWidth(1,120);
  m_list.SetColumnWidth(2,400);
  m_list.SetColumnWidth(3,300);

  // Create image list from bitmap
  m_images.Create(MAKEINTRESOURCE(IDB_STATUS),16,1,RGB(256,256,256));
  m_list.SetImageList(&m_images);
}

void
KwattaAppDlg::FillGrid()
{
  Tests& tests = m_suite->GetAllTests();
  Tests::iterator it = tests.begin();

  m_list.SetRowCount(1);
  m_list.Refresh();
  m_list.SetRowCount((int)m_suite->GetAllTests().size() + 1);

  // fill rows/cols with text
  for(int row = 1; row < m_list.GetRowCount(); row++)
  {
    for (int col = 0; col < m_list.GetColumnCount(); col++)
    {
      Test& test = it->second;

      GV_ITEM item;
      item.mask    = GVIF_TEXT | GVIF_FORMAT;
      item.row     = row;
      item.col     = col;
      item.nFormat = DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS | DT_NOPREFIX;

      switch(col)
      {
        case 0: item.strText.Format("Test %d", it->first); 
                break;
        case 1: item.strText.Format("%s", test.m_active ? "Active" : "Inactive");
                break;
        case 2: item.strText = test.m_name;
                break;
        case 3: item.strText = test.m_lastResult.IsEmpty() ? CString("(No result yet)") : test.m_lastResult;
                item.mask |= GVIF_IMAGE;
                item.iImage = test.m_lastResult == "OK" ? 1 : 0;
                break;
      }
      m_list.SetItem(&item);
      m_list.SetItemState(row,col,m_list.GetItemState(row,col) & GVIS_READONLY);
    }

    // Turn of read-only
    m_list.SetItemState(row, 1, m_list.GetItemState(row, 1) & ~GVIS_READONLY);
    // Change cell type to checkbox
    m_list.SetCellType(row, 1, RUNTIME_CLASS(CGridCellCheck));
    CGridCellCheck* check = (CGridCellCheck*) m_list.GetCell(row,1);
    check->SetCheck(it->second.m_active);

    // Next row in the test suite
    ++it;
    if(it == tests.end())
    {
      break;
    }
  }
  // m_list.AutoSize();
}

void
KwattaAppDlg::ReadParameters(CString p_file)
{
  // read the definition of the GLOBAL (!) parameters
  try
  {
    m_parameters->ReadFromXML(p_file,true);
  }
  catch(StdException& ex)
  {
    StyleMessageBox(this,ex.GetErrorMessage(),PRODUCT_NAME,MB_OK | MB_ICONERROR);
    theApp.GetMainWnd()->PostMessage(WM_QUIT,0,0);
  }
}

bool
KwattaAppDlg::CreateEmptyTestSet(CString p_directory,CString p_testname)
{
  CString path = theApp.GetBaseDirectoryClean();
  path += "\\";
  path += p_directory;

  if(CreateDirectory(path,nullptr) == FALSE)
  {
    StyleMessageBox(this,"Failed to create a directory for the new testset!","Kwatta",MB_OK|MB_ICONERROR);
    return false;
  }
  TestSet set;
  set.SetName(p_testname);
  CString filename = path + "\\" + p_testname + ".xset";
  set.SetFilename(filename);

  if(!set.WriteToXML())
  {
    StyleMessageBox(this,"Failed to create a new test-set definition file","Kwatta",MB_OK|MB_ICONERROR);
    return false;
  }
  return true;
}

bool 
KwattaAppDlg::RemoveTestsetDirectory(CString p_path)
{
  char buffer[MAX_PATH];
  memset(&buffer,0,MAX_PATH);
  strcpy_s(buffer,MAX_PATH,p_path.GetString());
  buffer[p_path.GetLength() + 1] = 0;
  buffer[p_path.GetLength() + 2] = 0;

  SHFILEOPSTRUCT operation;
  memset(&operation,0,sizeof SHFILEOPSTRUCT);
  operation.hwnd   = GetSafeHwnd();
  operation.wFunc  = FO_DELETE;
  operation.fFlags = FOF_NO_UI;
  operation.pFrom  = (PCZZSTR) buffer;
  if(SHFileOperation(&operation) != 0 && GetLastError())
  {
    CString reason = GetLastErrorAsString();
    CString warning;
    warning.Format("Could not remove the test directory: [%s] Reason: %s",p_path.GetString(),reason.GetString());
    StyleMessageBox(this,warning,"Kwatta",MB_OK|MB_ICONERROR);
    return false;
  }
  return true;
}

void 
KwattaAppDlg::SwitchActive(int p_row,CString p_testname)
{
  Test* test = m_suite->FindTest(p_testname);
  if(!test)
  {
    return;
  }
  // Switch active state
  test->m_active = !test->m_active;

  // Show in the grid
  CGridCellCheck* check = (CGridCellCheck*)m_list.GetCell(p_row,1);
  check->SetCheck(test->m_active);
}

void
KwattaAppDlg::SetUpDownButtons(int p_row)
{
  int  rows = m_list.GetRowCount();
  bool activeUP = p_row > 1;
  bool activeDN = p_row < (rows - 1) && p_row > 0;

  m_buttonUP.EnableWindow(activeUP);
  m_buttonDN.EnableWindow(activeDN);
}

void
KwattaAppDlg::SetResult(int p_row, bool p_ok)
{
  GV_ITEM item;
  item.mask    = GVIF_TEXT | GVIF_IMAGE;
  item.row     = p_row;
  item.col     = 3;
  item.strText = p_ok ? "OK" : "ERROR";
  item.iImage  = p_ok ? 1 : 0;
  m_list.SetItem(&item);
  m_list.Invalidate();
}

void
KwattaAppDlg::SetActive(int p_row,bool p_active)
{
  CGridCellCheck* check = (CGridCellCheck*)m_list.GetCell(p_row, 1);
  if(check)
  {
    check->SetCheck(p_active);
  }
  m_list.GetCell(p_row,1)->SetText(p_active ? "Active" : "Inactive");
}

void 
KwattaAppDlg::LoadParameters()
{
  if(m_parameters == nullptr)
  {
    m_parameters = new Parameters();
    CString filename = theApp.GetBaseDirectory() + "Parameters.xpar";

    ReadParameters(filename);
  }
}

void 
KwattaAppDlg::RunTestset(CString p_testname,int p_index)
{
  // Find the test and start it
  Test* test = m_suite->FindTest(p_testname);
  if(test)
  {
    int result = theApp.StartTestRunner(*test,GetSafeHwnd(),p_index);

    // Save in the testsuite
    test->m_lastResult = result ? "OK" : "ERROR";
    m_suite->WriteToXML(true);

    // Show on the grid
    GV_ITEM item;
    item.mask    = GVIF_TEXT | GVIF_IMAGE;
    item.row     = p_index;
    item.col     = 3;
    item.strText = result ? "OK" : "ERROR";
    item.iImage  = result ? 1 : 0;
    m_list.SetItem(&item);
    m_list.Invalidate();
  }
}

//////////////////////////////////////////////////////////////////////////
// 
// KwattaAppDlg message handlers
//
//////////////////////////////////////////////////////////////////////////

void 
KwattaAppDlg::OnSysCommand(UINT nID,LPARAM lParam)
{
  if((nID & 0xFFF0) == IDM_ABOUTBOX)
  {
    OnAbout();
  }
  else
  {
    StyleDialog::OnSysCommand(nID,lParam);
  }
}

void
KwattaAppDlg::OnAbout()
{
  AboutDlg dlgAbout(this);
  dlgAbout.DoModal();
}

void 
KwattaAppDlg::OnEnChangeSuite()
{
  UpdateData();
  Reset();
  ReadSuite();
}

void 
KwattaAppDlg::OnBnClickedButChoose()
{
  DocFileDialog dlg(GetSafeHwnd()
                   ,true
                   ,"Choose a Kwatta test suite"
                   ,"xtest"
                   ,m_testsuite
                   ,0
                   ,"Kwatta test suite *.xtest|*.xtest|All files|*.*");
  if(dlg.DoModal() == IDOK)
  {
    CString path = dlg.GetChosenFile();
    if(!path.IsEmpty())
    {
      m_testsuite = path;
      UpdateData(FALSE);
      ReadSuite();
    }
  }
}

void 
KwattaAppDlg::OnEnChangeDescription()
{
  UpdateData();
  if(m_suite)
  {
    m_suite->SetDescription(m_description);
  }
}

void 
KwattaAppDlg::OnBnClickedButSuite()
{
  LoadParameters();
  SystemDlg dlg(this,m_parameters);
  dlg.DoModal();
  // TODO: Add your control notification handler code here
}

void KwattaAppDlg::OnBnClickedButParam()
{
  LoadParameters();
  SearchVarDlg dlg(this,m_parameters,true,false,false,false,false);
  dlg.DoModal();

  if(dlg.GetSaved())
  {
    m_parameters->WriteToXML();
  }
}

void 
KwattaAppDlg::OnBnClickedButProc()
{
  ProceduresDlg dlg(this,theApp.GetBaseDirectory());
  dlg.DoModal();

  if(dlg.GetDoPurge())
  {
    ReadSuite();
  }
}

void
KwattaAppDlg::OnGridDblClick(NMHDR* pNMHDR, LRESULT* pResult)
{
  LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
  CString testname = m_list.GetItemText(pNMLV->iItem, 2 /*NAME*/);

  if(!m_suite)
  {
    return;
  }

  // Find the test and start it
  for (auto& test : m_suite->GetAllTests())
  {
    if (test.second.m_name.Compare(testname) == 0)
    {
      if(pNMLV->iSubItem == 1)
      {
        SwitchActive(pNMLV->iItem,testname);
      }
      else if(pNMLV->iSubItem > 1)
      {
        OnBnClickedButEdit();
      }
      return;
    }
  }
}

void 
KwattaAppDlg::OnBnClickedButRun()
{
  if (!m_suite)
  {
    return;
  }

  CCellID cell = m_list.GetFocusCell();
  CString testname = m_list.GetItemText(cell.row,2);

  RunTestset(testname,cell.row);
}

void 
KwattaAppDlg::OnBnClickedButAll()
{
  if(!m_suite)
  {
    return;
  }

  int rows = m_list.GetRowCount();
  for(int index = 1; index < rows; ++index)
  {
    CString testname = m_list.GetItemText(index,2);
    RunTestset(testname,index);
  }
}

void 
KwattaAppDlg::OnBnClickedButEdit()
{
  CCellID cell = m_list.GetFocusCell();
  CString testname = m_list.GetItemText(cell.row, 2 /*NAME*/);

  if(!m_suite)
  {
    return;
  }

  // Find the test and start it
  for(auto& test : m_suite->GetAllTests())
  {
    if(test.second.m_name.Compare(testname) == 0)
    {
      theApp.StartTestEditor(test.second,GetSafeHwnd(),cell.row);
      // Status will be updated by the WM_READYTEST message
      return;
    }
  }
}

void 
KwattaAppDlg::OnBnClickedButNew()
{
  NewTestDlg dlg(this);
  if(dlg.DoModal() != IDOK)
  {
    return;
  }
  CString directory = dlg.GetDirectory();
  CString testname  = dlg.GetTestName();
  bool    active    = dlg.GetActive();

  // Add the test to the display list
  int row = m_list.InsertRow("Test");
  CString testnumber;
  testnumber.Format("Test %d",row);
  CString activeTxt = active ? "Active" : "Inactive";

  m_list.GetCell(row,0)->SetText(testnumber);
  m_list.GetCell(row,1)->SetText(activeTxt);
  m_list.GetCell(row,2)->SetText(testname);
  m_list.GetCell(row,3)->SetText("(No result yet)");

  m_list.SetCellType(row,1,RUNTIME_CLASS(CGridCellCheck));
  CGridCellCheck* check = (CGridCellCheck*)m_list.GetCell(row,1);
  check->SetCheck(active);

  // Add the test-set to the test suite
  Test test;
  test.m_directory = directory;
  test.m_filename  = testname + ".xset";
  test.m_name      = testname;
  test.m_active       = active;

  m_suite->AddTest(test);

  // Now go create a directory and an initial test set
  if(CreateEmptyTestSet(directory,testname))
  {
    m_suite->WriteToXML();
  }

  // Refresh the list
  m_list.Refresh();
}

void 
KwattaAppDlg::OnBnClickedButRemove()
{
  CCellID id = m_list.GetFocusCell();
  CString testname = m_list.GetCell(id.row,2)->GetText();

  CString question;
  question.Format("Do you wish to completely remove test set [%s] ?",testname.GetString());
  if(StyleMessageBox(this,question,"Kwatta",MB_YESNO|MB_DEFBUTTON2|MB_ICONQUESTION) == IDNO)
  {
    return;
  }

  // Remove the test directory
  Test* test = m_suite->FindTest(testname);
  if(!test)
  {
    StyleMessageBox(this,"Cannot find the test in the test suite","Kwatta",MB_OK|MB_ICONERROR);
    return;
  }
  CString path = theApp.GetBaseDirectoryClean() + "\\" + test->m_directory;
  if(!RemoveTestsetDirectory(path))
  {
    return;
  }

  // Remove from the suite
  if(!m_suite->RemoveTest(testname))
  {
    StyleMessageBox(this,"Could not remove the remove the test set from the test-suite","Kwatta",MB_OK|MB_ICONERROR);
    return;
  }

  // Save the suite
  m_suite->WriteToXML();

  // Refresh the list
  m_list.DeleteRow(id.row);
  m_list.Refresh();
}

void
KwattaAppDlg::OnGridItemChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
  LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

  if (pNMLV->uNewState)
  {
    SetUpDownButtons(pNMLV->iItem);
  }
  *pResult = 1;
}

void
KwattaAppDlg::OnBnClickedButUP()
{
  CCellID id = m_list.GetFocusCell();
  int row = id.row;
  if (row >= 2)
  {
    Tests& set  = m_suite->GetAllTests();
    Test& lower = set[row - 1];
    Test& upper = set[row - 0];
    std::swap(lower,upper);

    CString name = m_list.GetCell(row - 1, 1)->GetText();
    CString file = m_list.GetCell(row - 1, 2)->GetText();

    m_list.GetCell(row - 1, 1)->SetText(m_list.GetCell(row, 1)->GetText());
    m_list.GetCell(row - 1, 2)->SetText(m_list.GetCell(row, 2)->GetText());
    m_list.GetCell(row - 1, 3)->SetText("");

    m_list.GetCell(row, 1)->SetText(name);
    m_list.GetCell(row, 2)->SetText(file);
    m_list.GetCell(row, 3)->SetText("");

    SetResult(row,     set[row - 0].m_lastResult == "OK");
    SetResult(row - 1, set[row - 1].m_lastResult == "OK");
    SetActive(row,     set[row - 0].m_active);
    SetActive(row - 1, set[row - 1].m_active);

    SetUpDownButtons(--id.row);
    m_list.Refresh();
    m_list.SetFocusCell(id);
  }
}

void 
KwattaAppDlg::OnBnClickedButDN()
{
  CCellID id = m_list.GetFocusCell();
  int row = id.row;
  if (row < (m_list.GetRowCount() - 1))
  {
    Tests& set  = m_suite->GetAllTests();
    Test& lower = set[row + 0];
    Test& upper = set[row + 1];
    std::swap(lower, upper);


    CString name = m_list.GetCell(row + 1, 1)->GetText();
    CString file = m_list.GetCell(row + 1, 2)->GetText();

    m_list.GetCell(row + 1, 1)->SetText(m_list.GetCell(row, 1)->GetText());
    m_list.GetCell(row + 1, 2)->SetText(m_list.GetCell(row, 2)->GetText());
    m_list.GetCell(row + 1, 3)->SetText("");

    m_list.GetCell(row, 1)->SetText(name);
    m_list.GetCell(row, 2)->SetText(file);
    m_list.GetCell(row, 3)->SetText("");

    SetResult(row,     set[row + 0].m_lastResult == "OK");
    SetResult(row + 1, set[row + 1].m_lastResult == "OK");
    SetActive(row,     set[row + 0].m_active);
    SetActive(row + 1, set[row + 1].m_active);

    SetUpDownButtons(++id.row);
    m_list.Refresh();
    m_list.SetFocusCell(id);
  }
}

void 
KwattaAppDlg::OnBnClickedButMutate()
{
  CCellID cell = m_list.GetFocusCell();
  CString testname = m_list.GetItemText(cell.row,2 /*NAME*/);

  if(!m_suite)
  {
    return;
  }

  // Find the test and start it
  Test* test = m_suite->FindTest(testname);
  if(test)
  {
    CString directory = test->m_directory;
    CString filename  = test->m_filename;

    MutateNamesDlg dlg(this,directory,filename);
    dlg.DoModal();

    CString newDirectory = dlg.GetDirectory();
    CString newFilename  = dlg.GetFilename();
    CString newTestname  = dlg.GetTestname();

    if(directory.CompareNoCase(newDirectory) || filename.CompareNoCase(newFilename))
    {
      m_suite->ChangeTestDirectory(testname,newDirectory);
      m_suite->ChangeTestFilename (testname,newFilename);
      m_suite->ChangeTestTestname (testname,newTestname);

      m_list.GetCell(cell.row,cell.col)->SetText(newTestname);
      m_list.Invalidate();

      m_suite->WriteToXML();
    }
  }
}

LRESULT
KwattaAppDlg::OnReadyTest(WPARAM wParam,LPARAM lParam)
{
  if(!m_suite)
  {
    return 0;
  }
  int row    = (int)wParam;
  int result = (int)lParam;
  CString testname = m_list.GetItemText(row, 2 /*NAME*/);

  // Find the test update the status
  for(auto& test : m_suite->GetAllTests())
  {
    if (test.second.m_name.Compare(testname) == 0)
    {
      // Save in the testsuite
      test.second.m_lastResult = result ? "OK" : "ERROR";

      // Show on the grid: Name first
      TestSet set;
      CString path = theApp.GetBaseDirectory() + test.second.m_directory + "\\" + test.second.m_filename;
      set.ReadFromXML(path);
      m_list.GetCell(row, 2)->SetText(set.GetName());
      test.second.m_name = set.GetName();

      // Show status of the test set
      GV_ITEM item;
      item.mask    = GVIF_TEXT | GVIF_IMAGE;
      item.row     = row;
      item.col     = 3;
      item.strText = result ? "OK" : "ERROR";
      item.iImage  = result ? 1 : 0;
      m_list.SetItem(&item);
      m_list.Invalidate();

      // Name and status could be altered
      m_suite->WriteToXML(true);

      // Ready
      break;
    }
  }
  return 0;
}

void 
KwattaAppDlg::OnBnClickedOk()
{
  if(m_suite)
  {
    if(!m_suite->WriteToXML())
    {
      StyleMessageBox(this,"Could not write the test suite file!",KWATTA,MB_OK|MB_ICONERROR);
      return;
    }
  }
  StyleDialog::OnOK();
}

void 
KwattaAppDlg::OnBnClickedCancel()
{
  StyleDialog::OnCancel();
}

void
KwattaAppDlg::OnExit()
{
  if(m_suite)
  {
    m_suite->WriteToXML();
  }
  StyleDialog::OnOK();
}

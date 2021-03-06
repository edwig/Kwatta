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
// This File   : TestEditorDlg.cpp
// What it does: Main dialog to edit one (1) test step (command-line)
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TestEditor.h"
#include "TestEditorDlg.h"
#include "CombiEditorDlg.h"
#include "NewStepDlg.h"
#include "MutateDlg.h"
#include <AboutDlg.h>
#include "afxdialogex.h"
#include <StdException.h>
#include <TestStepCMD.h>
#include <TestStepNET.h>
#include <TestStepSQL.h>
#include <ValidateCMD.h>
#include <ValidateNET.h>
#include <validateSQL.h>
#include <ExtraMessages.h>
#include <ExtraExtensions.h>
#include <EnsureFile.h>
#include <algorithm>
#include <io.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// TestEditorDlg dialog

TestEditorDlg::TestEditorDlg(CWnd* pParent /*=nullptr*/)
              :StyleDialog(IDD_TESTEDITOR_DIALOG, pParent)
              ,m_changed(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void TestEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	StyleDialog::DoDataExchange(pDX);
  DDX_Control(pDX,IDC_NAME,          m_editName,m_name);
  DDX_Control(pDX,IDC_DOCUMENTATION, m_editDocumentation,m_documentation);
  DDX_Control(pDX,IDC_RESULT_OK,     m_pictureOK);
  DDX_Control(pDX,IDC_RESULT_ERROR,  m_pictureERROR);
  DDX_Control(pDX,IDC_GRID,          m_grid);
  DDX_Control(pDX,IDC_BUT_RUN,       m_buttonRun);
  DDX_Control(pDX,IDC_BUT_ALL,       m_buttonRunAll);
  DDX_Control(pDX,IDC_BUT_EDIT,      m_buttonEdit);
  DDX_Control(pDX,IDC_BUT_EDIT,      m_buttonEdit);
  DDX_Control(pDX,IDC_BUT_COPY,      m_buttonCopy);
  DDX_Control(pDX,IDC_BUT_NEW,       m_buttonNew);
  DDX_Control(pDX,IDC_BUT_DELETE,    m_buttonDelete);
  DDX_Control(pDX,IDC_BUT_UP,        m_buttonUp);
  DDX_Control(pDX,IDC_BUT_DOWN,      m_buttonDn);
  DDX_Control(pDX,IDC_BUT_MUTATE,    m_buttonMutate);
  DDX_Control(pDX,IDOK,              m_buttonOK);
  DDX_Control(pDX,IDCANCEL,          m_buttonCancel);
}

BEGIN_MESSAGE_MAP(TestEditorDlg, StyleDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
  ON_MESSAGE(WM_READYTEST,            OnReadyTest)
  ON_MESSAGE(WM_READYVALI,            OnReadyValidation)
  ON_EN_KILLFOCUS(IDC_NAME,           OnEnChangeName)
  ON_EN_KILLFOCUS(IDC_DOCUMENTATION,  OnEnChangeDocumentation)
  ON_NOTIFY(LVN_ITEMCHANGING,IDC_GRID,OnGridItemChanging)
  ON_NOTIFY      (NM_DBLCLK,IDC_GRID, OnGridDblClick)
  ON_BN_CLICKED  (IDC_BUT_RUN,        OnBnClickedRun)
  ON_BN_CLICKED  (IDC_BUT_ALL,        OnBnClickedRunAll)
  ON_BN_CLICKED  (IDC_BUT_EDIT,       OnBnClickedEdit)
  ON_BN_CLICKED  (IDC_BUT_COPY,       OnBnClickedCopy)
  ON_BN_CLICKED  (IDC_BUT_NEW,        OnBnClickedNew)
  ON_BN_CLICKED  (IDC_BUT_DELETE,     OnBnClickedDelete)
  ON_BN_CLICKED  (IDC_BUT_UP,         OnBnClickedUp)
  ON_BN_CLICKED  (IDC_BUT_DOWN,       OnBnClickedDown)
  ON_BN_CLICKED  (IDC_BUT_MUTATE,     OnBnClickedMutate)
  ON_BN_CLICKED  (IDOK,               OnOK)
  ON_BN_CLICKED  (IDCANCEL,           OnCancel)

  ON_COMMAND(ID_MENU_THEMELIME,       OnStyleLime)
  ON_COMMAND(ID_MENU_THEMESKYBLUE,    OnStyleSkyblue)
  ON_COMMAND(ID_MENU_THEMEPURPLE,     OnStylePurple)
  ON_COMMAND(ID_MENU_THEMEMODERATE,   OnStyleModerateGray)
  ON_COMMAND(ID_MENU_THEMEPURE,       OnStylePureGray)
  ON_COMMAND(ID_MENU_THEMEBLACK,      OnStyleBlackWhite)
  ON_COMMAND(ID_MENU_ABOUT,           OnAbout)
  ON_COMMAND(ID_MENU_EXIT,            OnExit)

END_MESSAGE_MAP()

// TestEditorDlg message handlers

BOOL TestEditorDlg::OnInitDialog()
{
	StyleDialog::OnInitDialog();
  SetWindowText("Testset Editor");
  ShowMinMaxButton();
  ShowMinMaxButton();
  SetSysMenu(IDR_MENU);
  ShowSysMenu();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

  m_pictureOK   .LoadBitmaps(MAKEINTRESOURCE(IDB_OK));
  m_pictureERROR.LoadBitmaps(MAKEINTRESOURCE(IDB_ERROR));

	// extra initialization here
  InitButtons();
  InitLijst();
  InitTestSet();
  LoadTest();
  SetResult();

  m_buttonOK.SetStyle("ok");
  m_buttonCancel.SetStyle("can");
  SetCanResize();
  UpdateData(FALSE);

  m_changed = false;
  return TRUE;  // return TRUE  unless you set the focus to a control
}

void
TestEditorDlg::SetupDynamicLayout()
{
  StyleDialog::SetupDynamicLayout();

  CMFCDynamicLayout& manager = *GetDynamicLayout();
#ifdef _DEBUG
  manager.AssertValid();
#endif
  manager.AddItem(IDC_GRP_TEST,     CMFCDynamicLayout::MoveNone(),                        CMFCDynamicLayout::SizeHorizontalAndVertical(100,20));
  manager.AddItem(IDC_NAME,         CMFCDynamicLayout::MoveNone(),                        CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_DOCUMENTATION,CMFCDynamicLayout::MoveNone(),                        CMFCDynamicLayout::SizeHorizontalAndVertical(100,20));
  manager.AddItem(IDC_RESULT_OK,    CMFCDynamicLayout::MoveHorizontalAndVertical(100,20), CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_RESULT_ERROR, CMFCDynamicLayout::MoveHorizontalAndVertical(100,20), CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_GRP_STEPS,    CMFCDynamicLayout::MoveVertical(20),                  CMFCDynamicLayout::SizeHorizontalAndVertical(100,80));
  manager.AddItem(IDC_GRID,         CMFCDynamicLayout::MoveVertical(20),                  CMFCDynamicLayout::SizeHorizontalAndVertical(100,80));
  manager.AddItem(IDC_BUT_RUN,      CMFCDynamicLayout::MoveVertical(100),                 CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_BUT_ALL,      CMFCDynamicLayout::MoveVertical(100),                 CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_BUT_EDIT,     CMFCDynamicLayout::MoveVertical(100),                 CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_BUT_COPY,     CMFCDynamicLayout::MoveVertical(100),                 CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_BUT_NEW,      CMFCDynamicLayout::MoveVertical(100),                 CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_BUT_DELETE,   CMFCDynamicLayout::MoveVertical(100),                 CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_BUT_UP,       CMFCDynamicLayout::MoveVertical(100),                 CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_BUT_DOWN,     CMFCDynamicLayout::MoveVertical(100),                 CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_BUT_MUTATE,   CMFCDynamicLayout::MoveVertical(100),                 CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDOK,             CMFCDynamicLayout::MoveHorizontalAndVertical(100,100),CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDCANCEL,         CMFCDynamicLayout::MoveHorizontalAndVertical(100,100),CMFCDynamicLayout::SizeNone());
}

void
TestEditorDlg::SetResult()
{
  bool result = m_testset.GetTotalResult();

  m_pictureOK   .ShowWindow(result ? SW_SHOW : SW_HIDE);
  m_pictureERROR.ShowWindow(result ? SW_HIDE : SW_SHOW);
}

void
TestEditorDlg::InitButtons()
{
  m_buttonRun   .SetIconImage(IDI_RUN);
  m_buttonRunAll.SetIconImage(IDI_RUNALL);
  m_buttonEdit  .SetIconImage(IDI_EDIT);
  m_buttonCopy  .SetIconImage(IDI_COPY);
  m_buttonNew   .SetIconImage(IDI_NEW);
  m_buttonDelete.SetIconImage(IDI_DELETE);
  m_buttonUp    .SetIconImage(IDI_UP);
  m_buttonDn    .SetIconImage(IDI_DOWN);
  m_buttonMutate.SetIconImage(IDI_MUTATE);

  EnableToolTips();

  RegisterTooltip(IDC_BUT_RUN,    "Run the test step");
  RegisterTooltip(IDC_BUT_ALL,    "Run all the test steps");
  RegisterTooltip(IDC_BUT_EDIT,   "Edit the test step");
  RegisterTooltip(IDC_BUT_COPY,   "Copy the test step to a new step");
  RegisterTooltip(IDC_BUT_NEW,    "Create a new test step");
  RegisterTooltip(IDC_BUT_DELETE, "Delete the test step");
  RegisterTooltip(IDC_BUT_UP,     "Move current test step one row UP");
  RegisterTooltip(IDC_BUT_DOWN,   "Move current test step one row DOWN");
  RegisterTooltip(IDC_BUT_MUTATE, "Change the filename of a teststep or validation.");
}

void
TestEditorDlg::InitLijst()
{
  // Set the columns
  m_grid.SetColumnCount(6);
  m_grid.SetRowCount(1);
  m_grid.SetFixedRowCount(1);
  m_grid.SetFixedColumnCount(1);
  m_grid.SetEditable(FALSE);
  m_grid.GetCell(0, COL_NUMBER     )->SetText("Number");
  m_grid.GetCell(0, COL_GLOBAL_TEST)->SetText("GL");
  m_grid.GetCell(0, COL_NAME_STEP  )->SetText("Test step");
  m_grid.GetCell(0, COL_GLOBAL_VALI)->SetText("GL");
  m_grid.GetCell(0, COL_NAME_VALI  )->SetText("Validation");
  m_grid.GetCell(0, COL_RESULT     )->SetText("Status");
  m_grid.SetSingleRowSelection();
  m_grid.SetSortColumn(0);

  SetColumnWidths();

  // Create image list from bitmap
  m_images.Create(MAKEINTRESOURCE(IDB_STATUS), 16, 1, RGB(256, 256, 256));
  m_grid.SetImageList(&m_images);
}

void
TestEditorDlg::SetColumnWidths()
{
  m_grid.SetColumnWidth(0, 80);
  m_grid.SetColumnWidth(1, 32);
  m_grid.SetColumnWidth(2, 380);
  m_grid.SetColumnWidth(3, 32);
  m_grid.SetColumnWidth(4, 380);
  m_grid.SetColumnWidth(5, 100);
}

void
TestEditorDlg::InitTestSet()
{
  CString filename = theApp.GetTestFilename();

  // Read in the definition file for a test step
  try
  {
    m_testset.ReadFromXML(filename);
  }
  catch (StdException& ex)
  {
    StyleMessageBox(this,ex.GetErrorMessage(),PRODUCT_NAME,MB_OK|MB_ICONERROR);
    theApp.GetMainWnd()->PostMessage(WM_QUIT, 0, 0);
  }
  SetTitle();
}

void
TestEditorDlg::SetTitle()
{
  CString filename = theApp.GetTestFilename();
  EnsureFile ensure;
  CString name = ensure.FilenamePart(filename);

  CString title("Testset Editor - ");
  title += name;

  SetWindowText(title);
}

void
TestEditorDlg::SetTextImage(int p_row,int p_col,CString p_text,int p_image)
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
TestEditorDlg::LoadTest()
{
  m_name          = m_testset.GetName();
  m_documentation = m_testset.GetDocumentation();

  int index = 0;
  for(auto& run : m_testset.GetTestRuns())
  {
    CString number;
    number.Format("%d",++index);

    int row = m_grid.InsertRow(number);
    SetTextImage(row,COL_GLOBAL_TEST,"",run.m_global ? COL_STATUS_GLOBAL : COL_STATUS_LOCAL);
    m_grid.GetCell(row,2)->SetText(run.m_name.IsEmpty() ? run.m_filename : run.m_name);

    if(run.m_validations.size() == 1)
    {
      TRValidation vali = run.m_validations.front();
      SetTextImage(row,COL_GLOBAL_VALI,"",vali.m_global ? COL_STATUS_GLOBAL : COL_STATUS_LOCAL);
      m_grid.GetCell(row,4)->SetText(vali.m_name.IsEmpty() ? vali.m_filename : vali.m_name);
    }
    if(run.m_validations.size() > 1)
    {
      m_grid.GetCell(row,4)->SetText(MULTIPLE_VAL);
    }

    // Set result column
    SetTextImage(row,COL_RESULT,run.m_lastResult,run.m_lastResult.CompareNoCase("OK") == 0 ? COL_STATUS_OK : COL_STATUS_ERROR);
  }

  // If something read-in
  if(index)
  {
    m_grid.AutoSize();
    SetColumnWidths();
    // Set focus to first cell
    m_grid.SetFocusCell(1,1);
  }
}

bool
TestEditorDlg::SaveTest()
{
  if(m_changed)
  {
    if(m_testset.WriteToXML())
    {
      m_changed = false;
      return true;
    }
    return false;
  }
  return true;
}

void
TestEditorDlg::TryChangeTestGlobalLocal(int p_row,TRun& p_test)
{
  CString ask;
  ask.Format("Do you want to change the test step to a [%s] step?",p_test.m_global ? "local" : "global");
  if(StyleMessageBox(this,ask,PRODUCT_NAME,MB_YESNO|MB_DEFBUTTON2|MB_ICONQUESTION) == IDNO)
  {
    return;
  }

  if(p_test.m_global)
  {
    // It was global. Try to make it local
    if(theApp.DemoteTestStep(this, p_test.m_filename))
    {
      p_test.m_global = false;
      m_changed = true;
      SaveTest();
    }
  }
  else
  {
    // It was local. Try to make it global
    if(theApp.PromoteTestStep(this, p_test.m_filename))
    {
      p_test.m_global = true;
      m_changed = true;
      SaveTest();
    }
  }
  SetTextImage(p_row,COL_GLOBAL_TEST,"",p_test.m_global ? COL_STATUS_GLOBAL : COL_STATUS_LOCAL);
  m_grid.Invalidate();
}

void
TestEditorDlg::TryChangeValiGlobalLocal(int p_row,TRValidation& p_vali)
{
  CString ask;
  ask.Format("Do you want to change the validation to a [%s] validation?",p_vali.m_global ? "local" : "global");
  if (StyleMessageBox(this,ask,PRODUCT_NAME,MB_YESNO|MB_DEFBUTTON2|MB_ICONQUESTION) == IDNO)
  {
    return;
  }

  if(p_vali.m_global)
  {
    // It was global. Try to make it local
    if(theApp.DemoteValidation(this, p_vali.m_filename))
    {
      p_vali.m_global = false;
      m_changed = true;
      SaveTest();
    }
  }
  else
  {
    // It was local. Try to make it global
    if(theApp.PromoteValidation(this,p_vali.m_filename))
    {
      p_vali.m_global = true;
      m_changed = true;
      SaveTest();
    }
  }
  SetTextImage(p_row,COL_GLOBAL_VALI,"",p_vali.m_global ? COL_STATUS_GLOBAL : COL_STATUS_LOCAL);
  m_grid.Invalidate();
}

//////////////////////////////////////////////////////////////////////////
//
// MESSAGE HANDLERS
//
//////////////////////////////////////////////////////////////////////////

void TestEditorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		AboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
  else if((nID & 0xFFF) == SC_CLOSE)
  {
    OnOK();
  }
	else
	{
		StyleDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void TestEditorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		StyleDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR TestEditorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void
TestEditorDlg::EditCell(CCellID p_id)
{
  if(p_id.row < 1)
  {
    return;
  }
  // This is our test
  TRun& test = m_testset.GetTestRun(p_id.row - 1);

  if(p_id.col == COL_GLOBAL_TEST)
  {
    TryChangeTestGlobalLocal(p_id.row,test);
  }
  else if(p_id.col == COL_NAME_STEP)
  {
    theApp.StartStepEditor(test.m_filename,test.m_global,m_testset.GetValidationsByFile(test.m_filename),GetSafeHwnd(),p_id.row);
  }
  else if(p_id.col == COL_GLOBAL_VALI)
  {
    CString validation = m_grid.GetItemText(p_id.row, COL_NAME_VALI);
    if(validation.Compare(MULTIPLE_VAL))
    {
      TRValidation& val = test.m_validations.front();
      TryChangeValiGlobalLocal(p_id.row,val);
    }
  }
  else if(p_id.col == COL_NAME_VALI)
  {
    CString stepname   = m_grid.GetItemText(p_id.row,COL_NAME_STEP);
    CString validation = m_grid.GetItemText(p_id.row,COL_NAME_VALI);

    SHORT shift = GetKeyState(VK_SHIFT) & 0xFF80;
    if(shift || validation.Compare(MULTIPLE_VAL) == 0)
    {
      CombiEditorDlg dlg(this,m_testset,stepname,p_id.row);
      dlg.DoModal();
      if(dlg.GetIsChanged())
      {
        m_changed = true;
        SaveTest();
      }
    }
    else
    {
      TRValidation& val = test.m_validations.front();
      theApp.StartValidateEditor(val.m_filename,val.m_global,GetSafeHwnd(),p_id.row);
    }
  }
  else if(p_id.col == COL_RESULT)
  {
    theApp.StartResultViewer(test.m_filename,GetSafeHwnd(),p_id.row);
  }
  // else id.col == 0
}

void
TestEditorDlg::CopyTest(int p_row)
{
  TRun& run = m_testset.GetTestRun(p_row - 1);
  CString directory = theApp.GetBaseDirectory() + theApp.GetTestDirectory();
  CString filename  = run.m_filename;
  CString stepname  = run.m_name;
  CString newfile;
  CString valiName;
  CString valiFile;
  bool    stepGlobal = run.m_global;
  bool    valiGlobal = false;
  TestStep* step     = nullptr;

  CString ask;
  ask.Format("Do you want to copy the test step: %s?",stepname.GetString());
  if(StyleMessageBox(this,ask,KWATTA,MB_YESNO|MB_DEFBUTTON2|MB_ICONQUESTION) == IDNO)
  {
    return;
  }

  try
  {
    if(stepGlobal == false)
    {
      // Stepname extended with capital 'C'
      stepname += "C";
      // Filename extended with capital 'C' (can throw)
      newfile = ExtendFilename(filename);

      CString copyFrom = directory + filename;
      CString copyTo   = directory + newfile;
      if(CopyFile(copyFrom, copyTo, TRUE) == FALSE)
      {
        throw StdException("New teststep filename already exists. Rename that teststep first!");
      }
    }
    CString validate;
    if(!run.m_validations.empty())
    {
      valiName   = run.m_validations.front().m_name;
      valiFile   = run.m_validations.front().m_filename;
      valiGlobal = run.m_validations.front().m_global;

      if(valiGlobal == false)
      {
        CString newvalifile  = ExtendFilename(valiFile); // can throw
        valiName += "C";

        CString copyFrom = directory + valiFile;
        CString copyTo   = directory + newvalifile;
        if(CopyFile(copyFrom,copyTo,TRUE) == FALSE)
        {
          throw StdException("New validate filename already exists. Rename that validation step first!");
        }
      }
    }
  }
  catch (StdException& ex)
  {
    CString error;
    error.Format("Cannot copy test step [%s] Error: %s",stepname.GetString(),ex.GetErrorMessage().GetString());
    StyleMessageBox(this,error,KWATTA,MB_OK|MB_ICONERROR);
    return;
  }

  // Get the new test number
  int rows = m_grid.GetRowCount();
  CString rownum;
  rownum.Format("%d",rows);

  // Update the grid
  int row = m_grid.InsertRow(rownum);
  SetTextImage  (row,COL_GLOBAL_TEST,"",stepGlobal ? COL_STATUS_GLOBAL : COL_STATUS_LOCAL);
  m_grid.GetCell(row,COL_NAME_STEP)->SetText(stepname);
  SetTextImage  (row,COL_GLOBAL_VALI,"",valiGlobal ? COL_STATUS_GLOBAL : COL_STATUS_LOCAL);
  m_grid.GetCell(row,COL_NAME_VALI)->SetText(valiName);
  m_grid.Invalidate();

  // Update the test set
  TRun* newrun = m_testset.AddTestStep(stepname,newfile,run.m_global);
  if (!valiName.IsEmpty())
  {
    TRValidation vali;
    vali.m_global = valiGlobal;
    vali.m_name = valiName;
    vali.m_filename = valiFile;
    newrun->m_validations.push_back(vali);
  }
  // Re-save the total test set
  m_changed = true;
  SaveTest();
}

CString
TestEditorDlg::ExtendFilename(CString p_filename)
{
  int pos = p_filename.ReverseFind('.');
  if(pos)
  {
    CString basename  = p_filename.Left(pos);
    CString extension = p_filename.Mid(pos);

    return basename + "C" + extension;
  }
  throw StdException("Not a filename+extension to copy!");
}

void
TestEditorDlg::RunTest(CCellID p_id)
{
  if(p_id.row < 1)
  {
    return;
  }
  TRun& run = m_testset.GetTestRun(p_id.row - 1);
  CString filename = run.m_filename;
  int res = theApp.StartTestRunner(filename,run.m_global,m_testset.GetValidationsByFile(filename),GetSafeHwnd(),p_id.row);
  CString result = res ? "OK" : "ERROR";

  SetTextImage(p_id.row,COL_RESULT,result,res ? COL_STATUS_OK : COL_STATUS_ERROR);
  m_grid.Invalidate();

  // Save the result
  run.m_lastResult = result;
  m_changed = true;
  SaveTest();
  SetResult();

  // Tell it the Kwatta Suite app
  if(theApp.GetCallingHWND() && theApp.GetCallingRow())
  {
    ::PostMessage(theApp.GetCallingHWND(),WM_READYTEST,(WPARAM)theApp.GetCallingRow(),res);
  }
}

void
TestEditorDlg::ReParseTestStep(int p_run)
{
  TRun&   run      = m_testset.GetTestRun(p_run - 1);
  CString filename = theApp.GetBaseDirectory() + theApp.GetTestDirectory();
  filename        += run.m_filename;
  CString stepname = run.m_name;
  TestStep* step   = nullptr;
  try
  {
    step = ReadTestStep(filename);
  }
  catch(StdException& ex)
  {
    CString error;
    error.Format("Cannot read file [%s] Error: %s",filename,ex.GetErrorMessage());
    StyleMessageBox(this,error,"ERROR",MB_OK|MB_ICONERROR);
    return;
  }
  if(step && step->GetName() != stepname)
  {
    // Update the grid
    m_grid.GetCell(p_run,2)->SetText(step->GetName());
    m_grid.Invalidate();
    // Update the testset
    run.m_name = step->GetName();
    m_changed = true;
    SaveTest();
  }
  delete step;
}

void
TestEditorDlg::ReParseValidate(int p_run)
{
  TRun&   run      = m_testset.GetTestRun(p_run - 1);
  CString filename = run.m_filename;
  TSValSet* valis  = m_testset.GetValidationsByFile(filename);
  TRValidation& validation = valis->front();
  CString valiname = validation.m_name;
  CString valifile = validation.m_filename;
  Validate* vali = nullptr;
  try
  {
    vali = ReadValidate(theApp.GetBaseDirectory() + theApp.GetTestDirectory() + valifile);
  }
  catch(StdException& ex)
  {
    CString error;
    error.Format("Cannot read file [%s] Error: %s",valifile,ex.GetErrorMessage());
    StyleMessageBox(this,error,"ERROR",MB_OK|MB_ICONERROR);
    return;
  }
  if(vali->GetName() != valiname)
  {
    // Update the grid
    m_grid.GetCell(p_run,4)->SetText(vali->GetName());
    m_grid.Invalidate();
    // Update the test set
    valis->front().m_name = vali->GetName();
    m_changed = true;
    SaveTest();
  }
  delete vali;
}

int
TestEditorDlg::MakeNewTest(StepType p_type,bool p_global,CString p_name,CString p_filename)
{
  TestStep* step{nullptr};
  CString file(p_filename);
  file.MakeLower();

  // Create test step
  if(p_type == StepType::Step_command)
  {
    step = new TestStepCMD();
    if(file.Right(5).CompareNoCase(EXTENSION_TESTSTEP_CMD))
    {
      p_filename += EXTENSION_TESTSTEP_CMD;
    }
  }
  else if(p_type == StepType::Step_http)
  {
    step = new TestStepNET();
    if(file.Right(5).CompareNoCase(EXTENSION_TESTSTEP_NET))
    {
      p_filename += EXTENSION_TESTSTEP_NET;
    }
  }
  else if(p_type == StepType::Step_sql)
  {
    step = new TestStepSQL();
    if (file.Right(5).CompareNoCase(EXTENSION_TESTSTEP_SQL))
    {
      p_filename += EXTENSION_TESTSTEP_SQL;
    }
  }
  step->SetName(p_name);
  step->SetType((StepType)p_type);

  // Save to file
  if(p_global)
  {
    CString checkfile = theApp.GetBaseDirectory() + "Steps\\" + p_filename;
    if(_access(checkfile,0) < 0)
    {
      StyleMessageBox(this,"Global teststep does not exist: " + checkfile,"ERROR",MB_OK|MB_ICONERROR);
      return -1;
    }
  }
  else
  {
    if(!step->WriteToXML(theApp.GetBaseDirectory() + theApp.GetTestDirectory() + p_filename))
    {
      StyleMessageBox(this,"Cannot create step file: " + p_filename,"ERROR",MB_OK|MB_ICONERROR);
      return -1;
    }
  }
  // Put in test set
  m_testset.AddTestStep(step->GetName(), p_filename,p_global);
  m_testset.WriteToXML();

  // Put in grid
  int index = m_grid.GetRowCount();
  CString number;
  number.Format("%d",index);
  int row = m_grid.InsertRow(number);

  SetTextImage(row,COL_GLOBAL_TEST,"",p_global ? COL_STATUS_GLOBAL : COL_STATUS_LOCAL);
  m_grid.GetCell(row,2)->SetText(p_name);
  m_grid.Invalidate();

  return index;
}

void
TestEditorDlg::MakeNewVali(StepType p_type,bool p_global,CString p_name,CString p_filename,int p_row)
{
  // Make testrun validation
  TRValidation vali;
  vali.m_filename = p_filename;
  vali.m_global   = p_global;
  vali.m_name     = p_name;

  // Put in the testset
  TRun& run = m_testset.GetTestRun(p_row - 1);
  run.m_validations.push_back(vali);

  if(!p_global)
  {
    // Create the file
    CString path = theApp.GetBaseDirectory() + theApp.GetTestDirectory() + p_filename;

    bool result = false;
    if(p_type == StepType::Step_command)
    {
      ValidateCMD val;
      val.SetName(p_name);
      val.SetFilename(path);
      result = val.WriteToXML(path);
    }
    else if(p_type == StepType::Step_http)
    {
      ValidateNET val;
      val.SetName(p_name);
      val.SetFilename(path);
      result = val.WriteToXML(path);
    }
    else if(p_type == StepType::Step_sql)
    {
      ValidateSQL val;
      val.SetName(p_name);
      val.SetFilename(path);
      result = val.WriteToXML(path);
    }
    if (!result)
    {
      StyleMessageBox(this,"Cannot create the global validation file: " + path,"ERROR",MB_OK|MB_ICONERROR);
      return;
    }
  }
  else
  {
    // Check the global file
    CString path = theApp.GetBaseDirectory() + "Validations\\" + p_filename;
    if(_access(path,0) < 0)
    {
      StyleMessageBox(this,"Global validation file does not exist: " + p_filename,"ERROR",MB_OK|MB_ICONERROR);
      return;
    }
  }

  // Put in the grid
  SetTextImage(p_row,COL_GLOBAL_VALI,"",p_global ? COL_STATUS_GLOBAL : COL_STATUS_LOCAL);
  CString name = m_grid.GetCell(p_row,4)->GetText();
  if(name.IsEmpty())
  {
    m_grid.GetCell(p_row,4)->SetText(p_name);
  }
  else
  {
    m_grid.GetCell(p_row,4)->SetText(MULTIPLE_VAL);
  }
  m_grid.Invalidate();

  m_changed = true;
}

void
TestEditorDlg::DeleteResult(int p_row)
{
  TRun&   run  = m_testset.GetTestRun(p_row);
  CString file = run.m_filename;
  file.Replace(EXTENSION_TESTSTEP_CMD,EXTENSION_RESULT_CMD);
  file.Replace(EXTENSION_TESTSTEP_NET,EXTENSION_RESULT_NET);
  file.Replace(EXTENSION_TESTSTEP_SQL,EXTENSION_RESULT_SQL);

  CString path = theApp.GetBaseDirectory() + theApp.GetTestDirectory();
  path += file;
  if(DeleteFile(path))
  {
    run.m_lastResult.Empty();

    SetTextImage(p_row+1,COL_RESULT,"",COL_STATUS_ERROR);
    m_grid.Invalidate();
  }
}

void
TestEditorDlg::DeleteValidation(int p_row)
{
  TRun& run = m_testset.GetTestRun(p_row);
  for(auto& val : run.m_validations)
  {
    if(val.m_global == false)
    {
      CString file = val.m_filename;
      CString path = theApp.GetBaseDirectory() + theApp.GetTestDirectory();
      path += file;
      DeleteFile(path);
    }
  }
  run.m_validations.clear();
  m_grid.GetCell(p_row + 1,COL_NAME_VALI)->SetText("");
}

void
TestEditorDlg::DeleteTestStep(int p_row)
{
  TRun&   run  = m_testset.GetTestRun(p_row);

  if(run.m_global == false)
  {
    CString file = run.m_filename;
    CString path = theApp.GetBaseDirectory() + theApp.GetTestDirectory();
    path += file;
    DeleteFile(path);
  }
  m_testset.DeleteStep(p_row);
}

void
TestEditorDlg::DeleteCell(CCellID p_id)
{
  int row = p_id.row - 1;

  // Check if we are on a valid row
  if(row < 0)
  {
    return;
  }
  if (p_id.col < 3)
  {
    if(StyleMessageBox(this,"Do you want to delete the complete test step?","Question",MB_YESNO | MB_DEFBUTTON2) == IDYES)
    {
      DeleteResult(row);
      DeleteValidation(row);
      DeleteTestStep(row);

      m_grid.DeleteRow(p_id.row);
    }
    else return;
  }
  else if(p_id.col == 3 || p_id.col == 4)
  {
    if(StyleMessageBox(this,"Do you want to delete the validation(s)?","Question",MB_YESNO|MB_DEFBUTTON2) == IDYES)
    {
      // Delete the validation
      DeleteValidation(row);
    }
    else return;
  }
  else
  {
    if(StyleMessageBox(this,"Do you want to delete the test result?","Question",MB_YESNO|MB_DEFBUTTON2) == IDYES)
    {
      // Delete the result
      DeleteResult(row);
    }
    else return;
  }
  m_changed = true;
  SaveTest();
  SetResult();

  m_grid.Invalidate();
}

void
TestEditorDlg::SetUpDownButtons(int p_row)
{
  // Check for mismatch click
  if(p_row < 0)
  {
    return;
  }
  int  rows = m_grid.GetRowCount();
  bool activeUP = p_row > 1;
  bool activeDN = p_row < (rows - 1) && p_row > 0;

  m_buttonUp.EnableWindow(activeUP);
  m_buttonDn.EnableWindow(activeDN);
}

void
TestEditorDlg::SetResult(int p_row,bool p_ok)
{
  SetTextImage(p_row,COL_RESULT,p_ok ? "OK" : "ERROR",p_ok ? COL_STATUS_OK : COL_STATUS_ERROR);
  m_grid.Invalidate();
}

//////////////////////////////////////////////////////////////////////////
//
// Event handlers
//
//////////////////////////////////////////////////////////////////////////

void
TestEditorDlg::OnAbout()
{
  AboutDlg dlgAbout(this);
  dlgAbout.DoModal();
}

LRESULT
TestEditorDlg::OnReadyTest(WPARAM wParam,LPARAM lParam)
{
  TRACE("Ready row: %d\n",wParam);
  ReParseTestStep((int)wParam);
  if(lParam >= 0)
  {
    SetResult((int)wParam,(bool)lParam);
  }
  PumpMessage();
  return 0;
}

LRESULT
TestEditorDlg::OnReadyValidation(WPARAM wParam,LPARAM lParam)
{
  TRACE("Ready row: %d\n",wParam);
  ReParseValidate((int)wParam);
  PumpMessage();
  return 0;
}

void
TestEditorDlg::OnEnChangeName()
{
  UpdateData();
  if(m_name.Compare(m_testset.GetName()))
  {
    m_testset.SetName(m_name);
    m_changed = true;
  }
}

void
TestEditorDlg::OnEnChangeDocumentation()
{
  UpdateData();
  if(m_documentation.Compare(m_testset.GetDocumentation()))
  {
    m_testset.SetDocumentation(m_documentation);
    m_changed = true;
  }
}

void 
TestEditorDlg::OnGridDblClick(NMHDR* pNMHDR,LRESULT* pResult)
{
  LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
  CCellID id = m_grid.GetFocusCell();
  EditCell(id);
}

void 
TestEditorDlg::OnBnClickedRun()
{
  CCellID id = m_grid.GetFocusCell();
  RunTest(id);
}

void
TestEditorDlg::OnBnClickedRunAll()
{
  for(int index = 1; index < m_grid.GetRowCount();++index)
  {
    CCellID cell(index,3);
    m_grid.SetFocusCell(cell);
    OnBnClickedRun();
    PumpMessage();
  }
}

void
TestEditorDlg::OnBnClickedEdit()
{
  CCellID id = m_grid.GetFocusCell();
  EditCell(id);
}

void 
TestEditorDlg::OnBnClickedCopy()
{
  CCellID id = m_grid.GetFocusCell();
  CopyTest(id.row);
}

void
TestEditorDlg::OnBnClickedNew()
{
  NewStepDlg dlg(this);
  if(dlg.DoModal() == IDOK)
  {
    int row = MakeNewTest(dlg.GetType(),dlg.GetStepGlobal(),dlg.GetName(),dlg.GetFilename());
    if(row > 0)
    {
      MakeNewVali(dlg.GetValiType(),dlg.GetValiGlobal(),dlg.GetValiName(),dlg.GetValiFile(),row);
    }
    m_changed = true;
    SaveTest();
  }
}

void
TestEditorDlg::OnBnClickedDelete()
{
  CCellID id = m_grid.GetFocusCell();
  DeleteCell(id);
}

void 
TestEditorDlg::OnGridItemChanging(NMHDR* pNMHDR, LRESULT* pResult)
{
  LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

  if(pNMLV->uNewState)
  {
    SetUpDownButtons(pNMLV->iItem);
  }
  *pResult = 1;
}

void
TestEditorDlg::OnBnClickedUp()
{
  CCellID id = m_grid.GetFocusCell();
  int row = id.row;
  if(row >= 2)
  {
    TRunSet& set = m_testset.GetTestRuns();
    std::iter_swap(set.begin() + row - 2,set.begin() + row - 1);

    CString type1 = m_grid.GetCell(row - 1,1)->GetText();
    CString name  = m_grid.GetCell(row - 1,2)->GetText();
    CString type2 = m_grid.GetCell(row - 1,3)->GetText();
    CString vali  = m_grid.GetCell(row - 1,4)->GetText();

    m_grid.GetCell(row - 1,1)->SetText(m_grid.GetCell(row,1)->GetText());
    m_grid.GetCell(row - 1,2)->SetText(m_grid.GetCell(row,2)->GetText());
    m_grid.GetCell(row - 1,3)->SetText(m_grid.GetCell(row,3)->GetText());
    m_grid.GetCell(row - 1,4)->SetText(m_grid.GetCell(row,4)->GetText());
    m_grid.GetCell(row - 1,5)->SetText("");

    m_grid.GetCell(row,1)->SetText(type1);
    m_grid.GetCell(row,2)->SetText(name);
    m_grid.GetCell(row,3)->SetText(type2);
    m_grid.GetCell(row,4)->SetText(vali);
    m_grid.GetCell(row,5)->SetText("");

    SetResult(row,    set[row-1].m_lastResult == "OK");
    SetResult(row - 1,set[row-2].m_lastResult == "OK");

    SetUpDownButtons(--id.row);
    m_grid.Refresh();
    m_grid.SetFocusCell(id);

    m_changed = true;
  }
}

void
TestEditorDlg::OnBnClickedDown()
{
  CCellID id = m_grid.GetFocusCell();
  int row = id.row;
  if(row < (m_grid.GetRowCount() - 1))
  {
    TRunSet& set = m_testset.GetTestRuns();
    std::iter_swap(set.begin() + row,set.begin() + row - 1);

    CString type1 = m_grid.GetCell(row + 1,1)->GetText();
    CString name  = m_grid.GetCell(row + 1,2)->GetText();
    CString type2 = m_grid.GetCell(row + 1,3)->GetText();
    CString vali  = m_grid.GetCell(row + 1,4)->GetText();

    m_grid.GetCell(row + 1,1)->SetText(m_grid.GetCell(row,1)->GetText());
    m_grid.GetCell(row + 1,2)->SetText(m_grid.GetCell(row,2)->GetText());
    m_grid.GetCell(row + 1,3)->SetText(m_grid.GetCell(row,3)->GetText());
    m_grid.GetCell(row + 1,4)->SetText(m_grid.GetCell(row,4)->GetText());
    m_grid.GetCell(row + 1,5)->SetText("");

    m_grid.GetCell(row,1)->SetText(type1);
    m_grid.GetCell(row,2)->SetText(name);
    m_grid.GetCell(row,3)->SetText(type2);
    m_grid.GetCell(row,4)->SetText(vali);
    m_grid.GetCell(row,5)->SetText("");

    SetResult(row,    set[row - 1].m_lastResult == "OK");
    SetResult(row + 1,set[row    ].m_lastResult == "OK");

    SetUpDownButtons(++id.row);
    m_grid.Refresh();
    m_grid.SetFocusCell(id);

    m_changed = true;
  }
}

void 
TestEditorDlg::OnBnClickedMutate()
{
  CCellID id = m_grid.GetFocusCell();
  int row = id.row;
  if (row >= 1)
  {
    if(id.col < 3)
    {
      TRun& run = m_testset.GetTestRuns()[row - 1];
      if(run.m_global)
      {
        StyleMessageBox(this,"Global test step cannot be edited!",KWATTA,MB_OK|MB_ICONWARNING);
        return;
      }
      else
      {
        CString filename = run.m_filename;

        MutateDlg dlg(this, "testset", filename);
        dlg.DoModal();

        CString newfile = dlg.GetFilename();
        if(newfile.CompareNoCase(filename))
        {
          run.m_filename = newfile;
          m_testset.WriteToXML();
        }
      }
    }
    else if(id.col > 2 && id.col < 5)
    {
      CString text = m_grid.GetCell(id.row,id.col)->GetText();
      if(text.Compare(MULTIPLE_VAL))
      {
        TRun& run = m_testset.GetTestRuns()[row - 1];
        TRValidation& vali = run.m_validations.front();
        if(vali.m_global)
        {
          StyleMessageBox(this,"Global validation cannot be edited!",KWATTA,MB_OK|MB_ICONWARNING);
          return;
        }
        else
        {
          CString filename = vali.m_filename;

          MutateDlg dlg(this, "validation", filename);
          dlg.DoModal();

          CString newfile = dlg.GetFilename();
          if (newfile.CompareNoCase(filename))
          {
            vali.m_filename = newfile;
            m_testset.WriteToXML();
          }
        }
      }
      else
      {
        StyleMessageBox(this,"Change the filename by choosing the combined-validation editor first (use the edit button).",KWATTA,MB_OK|MB_ICONEXCLAMATION);
      }
    }
  }
}

void
TestEditorDlg::PumpMessage()
{
  // Message pump that also pumps the WM_USER + 200 messages
  // Otherwise we will *NOT* see the completed tests
  MSG msg;
  UINT ticks = GetTickCount();
  while(GetTickCount() - ticks < 500 &&
       (PeekMessage(&msg,NULL,WM_MOVE,WM_APP,PM_REMOVE)))
  {
    try
    {
      ::TranslateMessage(&msg);
      ::DispatchMessage(&msg);
    }
    catch (StdException& er)
    {
      // How now, brown cow?
      UNREFERENCED_PARAMETER(er);
    }
  }
}

void
TestEditorDlg::OnOK()
{
  if(!SaveTest())
  {
    return;
  }
  OnExit();
}

void 
TestEditorDlg::OnCancel()
{
  if(m_changed)
  {
    if(StyleMessageBox(this,"The testset has changes made to it. Save the testset?",KWATTA,MB_YESNO|MB_DEFBUTTON1|MB_ICONQUESTION) == IDYES)
    {
      if(!SaveTest())
      {
        return;
      }
    }
  }
  OnExit();
}

void 
TestEditorDlg::OnExit()
{
  int result = 1;

  for(auto& test : m_testset.GetTestRuns())
  {
    if(test.m_lastResult != "OK")
    {
      result = 0;
      break;
    }
  }
  EndDialog(result);
}

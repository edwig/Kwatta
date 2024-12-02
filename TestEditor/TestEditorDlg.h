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
// This File   : TestEditorDlg.h
// What it does: Main dialog to edit one (1) test step (command-line)
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Testset.h"

#define MULTIPLE_VAL _T("<MULTIPLE VALIDATIONS>")

// Column numbers in the TestEditor
#define COL_NUMBER        0
#define COL_GLOBAL_TEST   1
#define COL_NAME_STEP     2
#define COL_GLOBAL_VALI   3
#define COL_NAME_VALI     4
#define COL_RESULT        5
// Status index in the image bitmap
#define COL_STATUS_ERROR  0
#define COL_STATUS_OK     1
#define COL_STATUS_GLOBAL 2
#define COL_STATUS_LOCAL  3

typedef struct _newstep
{
  StepType m_type;
  CString  m_default;
}
NewStepInfo;

// TestEditorDlg dialog
class TestEditorDlg : public StyleDialog
{
// Construction
public:
	TestEditorDlg(CWnd* pParent = nullptr);	// standard constructor

  int  MakeNewTest(StepType p_type,bool p_global,CString p_name,CString p_filename);
  void MakeNewVali(StepType p_type,bool p_global,CString p_name,CString p_filename,int p_row);

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TESTEDITOR_DIALOG };
#endif

protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual BOOL OnInitDialog() override;
  virtual void SetupDynamicLayout() override;

// Implementation
protected:
  void            SetTitle();
  void            InitButtons();
  void            InitLijst();
  void            InitTestSet();
  void            LoadTest();
  bool            SaveTest();
  void            SetColumnWidths();
  void            SetResult();
  void            EditCell(CCellID p_id);
  void            CopyTest(int p_row);
  void            RunTest(CCellID p_id);
  void            DeleteCell(CCellID p_id);
  void            ReParseTestStep(int p_run);
  void            ReParseValidate(int p_run);
  void            DeleteResult(int p_row);
  void            DeleteValidation(int p_row);
  void            DeleteTestStep(int p_row);
  CString         ExtendFilename(CString p_filename);
  void            SetUpDownButtons(int p_row);
  void            SetResult(int p_row,bool p_ok);
  void            SetTextImage(int p_row,int p_col,CString p_text,int p_image);
  void            TryChangeTestGlobalLocal(int p_row,TRun& p_test);
  void            TryChangeValiGlobalLocal(int p_row,TRValidation& p_vali);
  void            PumpMessage();

  CString         m_name;
  CString         m_documentation;
  TestSet         m_testset;
  bool            m_changed;
  StyleEdit       m_editName;
  StyleEdit       m_editDocumentation;
  StyleGridCtrl   m_grid;
  StyleButton     m_buttonRun;
  StyleButton     m_buttonRunAll;
  StyleButton     m_buttonEdit;
  StyleButton     m_buttonCopy;
  StyleButton     m_buttonNew;
  StyleButton     m_buttonMVal;
  StyleButton     m_buttonDelete;
  StyleButton     m_buttonUp;
  StyleButton     m_buttonDn;
  StyleButton     m_buttonMutate;
  StyleButton     m_buttonLoadtest;
  StyleButton     m_buttonOK;
  StyleButton     m_buttonCancel;

  CBitmapButton   m_pictureOK;
  CBitmapButton   m_pictureERROR;
  CImageList      m_images;
  CBitmap         m_bitmap;

  DECLARE_MESSAGE_MAP()
  
  // Generated message map functions
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
  afx_msg void OnEnChangeName();
  afx_msg void OnEnChangeDocumentation();
  afx_msg void OnGridItemChanging(NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void OnGridDblClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg HCURSOR OnQueryDragIcon();
  afx_msg LRESULT OnReadyTest(WPARAM wParam,LPARAM lParam);
  afx_msg LRESULT OnReadyValidation(WPARAM wParam,LPARAM lParam);
  afx_msg void OnBnClickedRun();
  afx_msg void OnBnClickedRunAll();
  afx_msg void OnBnClickedEdit();
  afx_msg void OnBnClickedCopy();
  afx_msg void OnBnClickedNew();
  afx_msg void OnBnClickedMulVal();
  afx_msg void OnBnClickedDelete();
  afx_msg void OnBnClickedUp();
  afx_msg void OnBnClickedDown();
  afx_msg void OnBnClickedMutate();
  afx_msg void OnBnClickedLoadtest();
  afx_msg void OnOK();
  afx_msg void OnCancel();
  afx_msg void OnExit();
  afx_msg void OnAbout();
};

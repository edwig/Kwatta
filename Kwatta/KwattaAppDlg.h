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

#pragma once
#include <StyleFrameWork.h>
#include <TestSuite.h>
#include <Parameters.h>

// KwattaAppDlg dialog

class KwattaAppDlg : public StyleDialog
{
  DECLARE_DYNAMIC(KwattaAppDlg)

public:
  KwattaAppDlg(CWnd* p_parent,CString p_suiteFile);
  virtual ~KwattaAppDlg();

protected:
  virtual void  DoDataExchange(CDataExchange* pDX) override;
  virtual BOOL  OnInitDialog() override;
  virtual void  SetupDynamicLayout() override;

  void Reset();
  void ReadSuite();
  void InitButtons();
  void InitGrid();
  void FillGrid();
  void DoStarter();
  void LoadParameters();
  void AppendSysMenu();
  void ReadParameters(CString p_file);
  bool CreateEmptyTestSet(CString p_directory,CString p_testname);
  bool RemoveTestsetDirectory(CString p_path);
  void SwitchActive(int p_row,CString p_testname);
  void SetUpDownButtons(int p_row);
  void SetResult(int p_row,bool p_ok);
  void SetActive(int p_row,bool p_active);

  CString       m_testsuite;
  CString       m_description;

  StyleEdit     m_editTestsuite;
  StyleEdit     m_editDescription;
  StyleGridCtrl m_list;
  CImageList    m_images;

  StyleButton   m_buttonChoose;
  StyleButton   m_buttonSuite;
  StyleButton   m_buttonParam;
  StyleButton   m_buttonProc;
  StyleButton   m_buttonRun;
  StyleButton   m_buttonRunAll;
  StyleButton   m_buttonEdit;
  StyleButton   m_buttonNew;
  StyleButton   m_buttonRemove;
  StyleButton   m_buttonUP;
  StyleButton   m_buttonDN;
  StyleButton   m_buttonMutate;
  StyleButton   m_buttonOK;
  StyleButton   m_buttonCancel;

  TestSuite*    m_suite      { nullptr };
  Parameters*   m_parameters { nullptr };

  DECLARE_MESSAGE_MAP()
public:
  afx_msg void    OnSysCommand(UINT nID,LPARAM lParam);
  afx_msg LRESULT OnReadyTest(WPARAM wParam,LPARAM lParam);
  afx_msg void OnEnChangeSuite();
  afx_msg void OnEnChangeDescription();
  afx_msg void OnBnClickedButChoose();
  afx_msg void OnBnClickedButSuite();
  afx_msg void OnBnClickedButParam();
  afx_msg void OnBnClickedButProc();
  afx_msg void OnGridItemChanged   (NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void OnGridDblClick      (NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void OnBnClickedButRun();
  afx_msg void OnBnClickedButAll();
  afx_msg void OnBnClickedButEdit();
  afx_msg void OnBnClickedButNew();
  afx_msg void OnBnClickedButRemove();
  afx_msg void OnBnClickedButUP();
  afx_msg void OnBnClickedButDN();
  afx_msg void OnBnClickedButMutate();
  afx_msg void OnBnClickedOk();
  afx_msg void OnBnClickedCancel();
  afx_msg void OnAbout();
  afx_msg void OnExit();
};

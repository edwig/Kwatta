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
// This File   : LoadTestDlg.h
// What it does: Enter and run a loadtest for the current teststep
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "TestSet.h"
#include "resource.h"

class LoadTestDlg : public StyleDialog
{
  DECLARE_DYNAMIC(LoadTestDlg)

public:
  LoadTestDlg(CWnd*     p_parent
             ,CString   p_name
             ,CString   p_filename
             ,bool      p_global
             ,TSValSet* p_valset
             ,HWND      p_report
             ,int       p_row);
  virtual ~LoadTestDlg();

private:
  virtual void LoadTestDlg::DoDataExchange(CDataExchange* pDX) override;
  virtual BOOL OnInitDialog() override;

  void SetTotal();

  // Test to run
  CString   m_name;
  CString   m_testfilename;
  bool      m_global;
  TSValSet* m_validationSet;
  HWND      m_reportTo;
  int       m_row;

  // Interface
  int m_parallel  {  2 };     // Number of parallel runs
  int m_persecond {  5 };     // Number of parallel starts per second
  int m_seconds   { 10 };     // Run this number of seconds
  int m_totalcalls{  0 };     // Total runs (parallel * persecond * seconds)

  StyleEdit         m_editName;
  StyleEdit         m_editParallel;
  StyleEdit         m_editPerSecond;
  StyleEdit         m_editSeconds;
  StyleEdit         m_editTotal;
  StyleProgressCtrl m_progress;
  StyleButton       m_buttonOK;
  StyleButton       m_buttonCancel;

  DECLARE_MESSAGE_MAP();

  afx_msg void OnTimer(UINT_PTR nID);
  afx_msg void OnEditParallel();
  afx_msg void OnEditPerSecond();
  afx_msg void OnEditSeconds();
  afx_msg void OnEditTotal();
  afx_msg void OnButtonOK();
  afx_msg void OnButtonCancel();
};

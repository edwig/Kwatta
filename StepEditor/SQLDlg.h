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
// This File   : SQLDlg.h
// What it does: Tab in the test editor for the SQL of a database test call
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "resource.h"

class TestStepSQL;
class Parameters;

// PayloadDlg dialog

class SQLDlg : public StyleTab
{
  DECLARE_DYNAMIC(SQLDlg)

public:
  SQLDlg(CWnd* p_parent);
  virtual ~SQLDlg();

  void InitTab(TestStepSQL* p_testStep, Parameters* p_parameters);
  void InitPayload();
  void StoreVariables();
  void EffectiveParameters();
  bool IsFilled();

  // Dialog Data
  enum { IDD = IDD_SQL };

protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual BOOL OnInitDialog() override;
  virtual void SetupDynamicLayout() override;

  CString       m_payload;
  StyleButton   m_buttonParam;
  StyleEdit     m_editPayload;

  TestStepSQL*  m_testStep { nullptr };
  Parameters*   m_parameters{ nullptr };

  DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnBnClickedParam();
  afx_msg void OnEnKillfocusPayload();
};


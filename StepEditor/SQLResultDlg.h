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
// This File   : SQLResultDlg.h
// What it does: Resulting columns of a SQL test call
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

class TestStepSQL;
class StepResultSQL;

// SQLResultDlg dialog

class SQLResultDlg : public StyleDialog
{
  DECLARE_DYNAMIC(SQLResultDlg)

public:
  SQLResultDlg(CWnd* pParent = nullptr);   // standard constructor
  virtual ~SQLResultDlg();
  void InitTab();
  void SetResult(StepResultSQL* p_result);

  // Dialog Data
  enum { IDD = IDD_SQLRESULT };

protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  virtual void SetupDynamicLayout() override;
  virtual BOOL OnInitDialog() override;
          void InitLijst();

  StepResultSQL*  m_result { nullptr };
  CString         m_speed;
  StyleGridCtrl   m_list;
  StyleEdit       m_editSpeed;
  StyleButton     m_buttonClear;

  DECLARE_MESSAGE_MAP()

  afx_msg void OnBnClickedClear();
};

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
// This File   : ProceduresDlg.h
// What it does: Start the suite global procedures (reset / reporting)
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <TestReport.h>

// ProceduresDlg dialog

class ProceduresDlg : public StyleDialog
{
	DECLARE_DYNAMIC(ProceduresDlg)

public:
	ProceduresDlg(CWnd* p_parent,CString p_baseDirectory);
	virtual ~ProceduresDlg();

  bool GetDoPurge() { return m_purge; }

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROCEDURES };
#endif

protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual BOOL OnInitDialog() override;
          void InitButtons();

  CString       m_baseDirectory;
  CString       m_resetOutcome;
  CString       m_reportOutcome;
  ReportDepth   m_depth{ ReportDepth::REPORT_TESTS };
  StyleButton   m_buttonReset;
  StyleButton   m_buttonReport;
  StyleEdit     m_editResetOutcome;
  StyleEdit     m_editReportOutcome;
  StyleComboBox m_comboDepth;
  StyleButton   m_buttonOK;
  StyleButton   m_buttonCancel;
  bool          m_purge { false };

	DECLARE_MESSAGE_MAP()

  afx_msg void OnBnClickedReset();
  afx_msg void OnBnClickedReport();
  afx_msg void OnCbnCloseupDepth();
};

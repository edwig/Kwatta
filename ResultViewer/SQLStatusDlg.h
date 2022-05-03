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
// This program: ResultViewer
// This File   : SQLStatusDlg.h
// What it does: Shows the status output of a SQL test command
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "StepResultSQL.h"

// SQLStatusDlg dialog

class SQLStatusDlg : public StyleDialog
{
	DECLARE_DYNAMIC(SQLStatusDlg)

public:
	SQLStatusDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~SQLStatusDlg();
  void     LoadVariables(StepResultSQL* p_stepResult);

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SQL_STATUS };
#endif

protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual BOOL PreTranslateMessage(MSG* pMsg) override;
  virtual void SetupDynamicLayout();
  virtual BOOL OnInitDialog() override;

	DECLARE_MESSAGE_MAP()

  int       m_succeeded;
  int       m_returnedRows;
  int       m_returnedCols;
  CString   m_SQLState;
  CString   m_nativeError;
  StyleEdit m_editSucceeded;
  StyleEdit m_editRows;
  StyleEdit m_editCols;
  StyleEdit m_editState;
  StyleEdit m_editNative;

  StepResultSQL* m_stepResult{ nullptr };
};

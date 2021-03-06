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
// This program: SuiteLibrary
// This File   : SearchVarDlg.h
// What it does: Find and/or maintain global and local variables
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Parameters.h"
#include "GridCtrl.h"
#include "resource.h"

// SearchVarDlg dialog

class SearchVarDlg : public StyleDialog
{
	DECLARE_DYNAMIC(SearchVarDlg)

public:
  SearchVarDlg(CWnd*       p_parent
              ,Parameters* p_parameters
              ,bool        p_doGlobals
              ,bool        p_doLocals
              ,bool        p_doReturns
              ,bool        p_doBuffers
              ,bool        p_useUSE = true);
	virtual ~SearchVarDlg();

  ParType  GetResultType()     { return m_resultType;     }
  CString  GetChosenVariable() { return m_resultVariable; }
  bool     GetSaved()          { return m_saved;          }
  CString  GetVariable();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SEARCHVAR };
#endif

protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual void SetupDynamicLayout() override;
  virtual BOOL OnInitDialog() override;

  void InitPasswordProtect();
  void InitGrid();
  void LoadGrid();
  void LoadContext();
  void SaveParameters();
  void InsertRow(ParType p_type,CString p_name,CString p_value);
  bool CheckReadPassword(CString& p_name,CString& p_value);
  void CheckSavePassword(CString& p_name,CString& p_value);

	DECLARE_MESSAGE_MAP()

  Parameters*   m_parameters;
  CString       m_context;
  StyleEdit     m_editContext;
  StyleGridCtrl m_grid;
  StyleButton   m_buttonAdd;
  StyleButton   m_buttonDelete;
  StyleButton   m_buttonOK;
  StyleButton   m_buttonCancel;
  StyleButton   m_buttonSave;
  int           m_chosen  { -1    };
  bool          m_changed { false };
  bool          m_saved   { false };
  bool          m_doPasswordProtect { false };

  ParType       m_resultType { ParType::PAR_NONE };
  CString       m_resultVariable;

  bool          m_doGlobals;
  bool          m_doLocals;
  bool          m_doReturns;
  bool          m_doBuffers;
  bool          m_useUSEbutton;
public:
  afx_msg void OnLvnItemchangedGrid (NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void OnLvnEndLabelEditGrid(NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void OnBnClickedAdd();
  afx_msg void OnBnClickedDelete();
  afx_msg void OnBnClickedOk();
  afx_msg void OnBnClickedCancel();
  afx_msg void OnBnClickedSave();
};

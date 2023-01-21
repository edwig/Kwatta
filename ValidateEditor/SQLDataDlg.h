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
// This program: ValidateEditor
// This File   : SQLDataDlg.h
// What it does: Validation of returned data row of a SQL database statement
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once
#include <Validate.h>

class ValidateSQL;
class Parameters;

// SQLDataDlg dialog

class SQLDataDlg : public StyleTab
{
	DECLARE_DYNAMIC(SQLDataDlg)

public:
	SQLDataDlg(CWnd* p_parent);
	virtual ~SQLDataDlg();
  void     InitTab(ValidateSQL* p_validate,Parameters* p_parameters);
  void     StoreVariables();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SQL_DATA };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual BOOL OnInitDialog() override;
	virtual void SetupDynamicLayout() override;
	        void InitLijst();
          void LoadVariables();
          void FillCombos();
          void SetCombos();

	ValidateSQL* m_validate   { nullptr };
	Parameters*  m_parameters { nullptr };
	
  bool						m_useData        { false };
  BufferOperator	m_dataOperator   { BufferOperator::BOP_NOP };
  CString					m_effectiveData;
  CString					m_returnedDataVariable;

	StyleCheckbox		m_checkData;
	StyleGridCtrl   m_list;
	StyleButton			m_buttonNew;
	StyleButton			m_buttonDelete;
	StyleButton			m_buttonParm;
	StyleComboBox   m_comboOperator;
	StyleEdit				m_editEffective;
	StyleComboBox		m_comboVariable;
	StyleButton			m_buttonVariable;

	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnLvnItemchangedGrid(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedNew();
	afx_msg void OnBnClickedDelete();
	afx_msg void OnBnClickedParm();
	afx_msg void OnBnClickedUseData();
  afx_msg void OnBnClickedNewvar();
  afx_msg void OnEnChangeEffData();
  afx_msg void OnCbnSelchangeOperator();
  afx_msg void OnCbnSelchangeDataVariable();
};

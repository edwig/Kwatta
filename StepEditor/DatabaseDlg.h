#pragma once
#include "afxdialogex.h"

class TestStepSQL;
class Parameters;

// DatabaseDlg dialog

class DatabaseDlg : public StyleDialog
{
	DECLARE_DYNAMIC(DatabaseDlg)

public:
	DatabaseDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~DatabaseDlg();

	void InitTab(TestStepSQL* p_testStep,Parameters* p_parameters);
  void StoreVariables();
  void EffectiveParameters();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DATABASE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual BOOL OnInitDialog() override;
	        void InitButtons();

	TestStepSQL* m_testStep   { nullptr };
	Parameters*  m_parameters { nullptr };

	CString			 m_datasource;
	CString			 m_user;
	CString			 m_password;
	CString			 m_effDatasource;
	CString			 m_effUser;
	CString			 m_effPassword;

	StyleEdit	   m_editDatasource;
	StyleEdit		 m_editUser;
	StyleEdit		 m_editPassword;
	StyleButton  m_buttonDatasource;
	StyleButton  m_buttonUser;
	StyleButton  m_buttonPassword;
	StyleEdit		 m_editEffDatasource;
	StyleEdit		 m_editEffUser;
	StyleEdit		 m_editEffPassword;


	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeDatasource();
	afx_msg void OnBnClickedButDatasource();
	afx_msg void OnEnChangeEffDatasource();
	afx_msg void OnEnChangeUser();
	afx_msg void OnBnClickedButUser();
	afx_msg void OnEnChangeEffUser();
	afx_msg void OnEnChangePassword();
	afx_msg void OnBnClickedButPassword();
	afx_msg void OnEnChangeEffPassword();
};

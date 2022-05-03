#pragma once

class StepResultSQL;

// SQLOutputDlg dialog

class SQLOutputDlg : public StyleDialog
{
	DECLARE_DYNAMIC(SQLOutputDlg)

public:
	SQLOutputDlg(CWnd* p_parent);
	virtual ~SQLOutputDlg();
  void InitTab();
  void SetResult(StepResultSQL* p_result);

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SQLOUTPUT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual void SetupDynamicLayout() override;
	virtual BOOL OnInitDialog() override;

	StepResultSQL*  m_result { nullptr };
	int							m_rows   { 0 };
	int							m_cols   { 0 };
	CString					m_SQLState;
	CString					m_NativeError;

	StyleCheckbox   m_checkSucceeded;
	StyleEdit				m_editRows;
	StyleEdit				m_editCols;
	StyleEdit				m_editSqlState;
	StyleEdit				m_editNative;

	DECLARE_MESSAGE_MAP()
};

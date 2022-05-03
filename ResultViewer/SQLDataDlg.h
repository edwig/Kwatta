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
// This File   : SQLDataDlg.h
// What it does: Shows the resulting data row from a SQL test
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

class StepResultSQL;

// SQLDataDlg dialog

class SQLDataDlg : public StyleDialog
{
	DECLARE_DYNAMIC(SQLDataDlg)

public:
	SQLDataDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~SQLDataDlg();
	void LoadVariables(StepResultSQL* p_result);

	// Dialog Data
	enum { IDD = IDD_SQL_DATA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void SetupDynamicLayout() override;
	virtual BOOL OnInitDialog() override;
	void InitLijst();

	StyleGridCtrl m_list;

	DECLARE_MESSAGE_MAP()
};

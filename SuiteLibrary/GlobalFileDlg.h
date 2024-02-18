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
// This File   : GlobalFileDlg.h
// What it does: Displaying the contents of the global steps or global validation files
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "afxdialogex.h"
#include "TestStep.h"

// GlobalFileDlg dialog

class GlobalFileDlg : public StyleDialog
{
	DECLARE_DYNAMIC(GlobalFileDlg)

public:
	GlobalFileDlg(CWnd* p_parent,bool p_step,StepType p_type,CString p_directory);
	virtual ~GlobalFileDlg();

	CString GetChosenFile() { return m_chosen; }

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GLOBALFILE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual BOOL OnInitDialog() override;
          void InitGrid();
          void FillGrid();

	bool          m_step;
	StepType      m_type;
	CString       m_directory;
	CString       m_pattern;
	CString       m_chosen;

	StyleGridCtrl m_grid;
	StyleButton   m_buttonOK;
	StyleButton   m_buttonCancel;

	DECLARE_MESSAGE_MAP()

	afx_msg void OnLvnItemchangingGrid(NMHDR* pNMHDR,LRESULT* pResult);
	afx_msg void OnLvnDoubleClickGrid (NMHDR* pNMHDR,LRESULT* pResult);
	afx_msg void OnBnClickedOK();
	afx_msg void OnBnClickedCancel();
};

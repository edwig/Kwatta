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
// This program: TestEditor
// This File   : CombiEditorDlg.h
// What it does: Multiple (combined) validations for a teststep in the test-set
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <TestSet.h>

// CombeEditorDlg dialog

class CombiEditorDlg : public StyleDialog
{
	DECLARE_DYNAMIC(CombiEditorDlg)

public:
  CombiEditorDlg(CWnd* p_parent,TestSet& p_testset,CString p_stepname,int p_row);
	virtual ~CombiEditorDlg();

// Dialog Data
	enum { IDD = IDD_COMBIEDITOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual BOOL OnInitDialog() override;
          void FillGrid();
          void InitButtons();

  int           m_row;
  CString       m_name;
  CString       m_documentation;
  CString       m_stepName;

  TestSet&      m_testSet;

  StyleEdit     m_editName;
  StyleEdit     m_editDocu;
  StyleEdit     m_editStep;
  StyleGridCtrl m_grid;
  StyleButton   m_buttonAddValidation;
  StyleButton   m_buttonDelValidation;
  StyleButton   m_buttonMutValidation;
  StyleButton   m_buttonOK;
  StyleButton   m_buttonCancel;

	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnGridDblClick(NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void OnBnClickedAddValidation();
  afx_msg void OnBnClickedDelValidation();
  afx_msg void OnBnClickedMutValidation();
};

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
// This File   : ValidationsDlg.h
// What it does: Shows the tab with all validations for this test steps result
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "GridCtrl.h"
#include "StepResult.h"

// ValidationsDlg dialog

class ValidationsDlg : public StyleDialog
{
	DECLARE_DYNAMIC(ValidationsDlg)

public:
	ValidationsDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~ValidationsDlg();
  BOOL     OnInitDialog();
  void     LoadVariables(StepResult* p_stepResult);

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VALIDATIONS };
#endif

protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual BOOL PreTranslateMessage(MSG* pMsg) override;
  virtual void SetupDynamicLayout() override;
          void InitLijst();

	DECLARE_MESSAGE_MAP()

  StepResult*   m_stepResult { nullptr };
  StyleGridCtrl m_grid;
  CImageList    m_images;
  CBitmap       m_bitmap;

public:
  afx_msg void OnLvnItemchangedGrid(NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void OnLvnDoubleClickGrid(NMHDR* pNMHDR, LRESULT* pResult);
};

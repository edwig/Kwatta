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
// This File   : BodyDlg.h
// What it does: Defining the validation of a HTTP message body
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Parameters.h"
#include "ValidateIN.h"

// BodyDlg dialog

class BodyDlg : public StyleDialog
{
	DECLARE_DYNAMIC(BodyDlg)

public:
	BodyDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~BodyDlg();
  void     InitTab(ValidateIN* p_validate, Parameters* p_param);
  void     StoreVariables();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BODY };
#endif

protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual void SetupDynamicLayout() override;
  virtual BOOL OnInitDialog() override;
          void LoadVariables();
          void FillCombos();
          void SetCombos();

	DECLARE_MESSAGE_MAP()

  bool            m_useBody      { false };
  BufferOperator  m_bodyOperator { BufferOperator::BOP_NOP };
  CString         m_expectedBody;
  CString         m_effectiveBody;
  CString         m_bodyVariable;

  StyleCheckbox   m_buttonUseBody;
  StyleComboBox   m_comboOperator;
  StyleButton     m_buttonSearchvar;
  StyleEdit       m_editExpected;
  StyleEdit       m_editEffective;
  StyleComboBox   m_comboBodyVariable;
  StyleButton     m_buttonBodyParm;

  ValidateIN*     m_validate   { nullptr };
  Parameters*     m_parameters { nullptr };
public:
  afx_msg void OnBnClickedUseBody();
  afx_msg void OnCbnSelchangeOperator();
  afx_msg void OnBnClickedSearchvar();
  afx_msg void OnEnChangeBody();
  afx_msg void OnEnKillfocusBody();
  afx_msg void OnEnChangeEffective();
  afx_msg void OnCbnSelchangeBodyVariable();
  afx_msg void OnBnClickedBodyParm();
};

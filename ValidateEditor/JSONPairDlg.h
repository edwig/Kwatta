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
// This File   : JSONPairDlg.h
// What it does: Defining the validation of JSON pair in the HTTP response body
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "ValidateIN.h"
#include "Parameters.h"

// JSONPairDlg dialog

class JSONPairDlg : public StyleDialog
{
  DECLARE_DYNAMIC(JSONPairDlg)

public:
  JSONPairDlg(CWnd* pParent = nullptr);   // standard constructor
  virtual ~JSONPairDlg();
  void     InitTab(ValidateIN* p_validate,Parameters* p_parameters);
  void     StoreVariables();

// Dialog Data
  enum { IDD = IDD_JSON_PAIR};

protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual void SetupDynamicLayout() override;
  virtual BOOL OnInitDialog() override;
          void LoadVariables();
          void FillCombos();
          void SetCombos();

  DECLARE_MESSAGE_MAP()

  ValidateIN* m_validate;
  Parameters* m_parameters;

  bool           m_useJSON;
  BufferOperator m_jsonOperator;
  CString        m_jsonPath;
  CString        m_expectedJSON;
  CString        m_effectiveJSON;
  CString        m_jsonVariable;

  StyleEdit      m_editJsonPath;
  StyleEdit      m_editExpected;
  StyleEdit      m_editEffective;
  StyleCheckbox  m_buttonCheck;
  StyleComboBox  m_comboOperator;
  StyleButton    m_buttonVariable;
  StyleComboBox  m_comboJsonVariable;
  StyleButton    m_buttonJsonParm;

public:
  afx_msg void OnBnClickedUseJSON();
  afx_msg void OnEnChangeJsonPath();
  afx_msg void OnEnChangeJSON();
  afx_msg void OnEnKillfocusJSON();
  afx_msg void OnBnClickedNewvar();
  afx_msg void OnEnChangeEffStatus();
  afx_msg void OnCbnSelchangeOperator();
  afx_msg void OnCbnSelchangeJsonVariable();
  afx_msg void OnBnClickedJsonParm();
};

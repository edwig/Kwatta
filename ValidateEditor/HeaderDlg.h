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
// This File   : HeaderDlg.h
// What it does: Defining the validation of response headers of a HTTP internet message call
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "ValidateNET.h"
#include "Parameters.h"

// HeaderDlg dialog

class HeaderDlg : public StyleDialog
{
  DECLARE_DYNAMIC(HeaderDlg)

public:
  HeaderDlg(CWnd* pParent = nullptr);   // standard constructor
  virtual ~HeaderDlg();
  void     InitTab(ValidateNET* p_validate,Parameters* p_parameters);
  void     StoreVariables();

// Dialog Data
  enum { IDD = IDD_HEADER };

protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual void SetupDynamicLayout() override;
  virtual BOOL OnInitDialog() override;
          void LoadVariables();
          void FillCombos();
          void SetCombos();

  DECLARE_MESSAGE_MAP()

  ValidateNET* m_validate;
  Parameters* m_parameters;

  bool           m_useHeaderValue;
  BufferOperator m_headerOperator;
  CString        m_headerName;
  CString        m_expectedHeader;
  CString        m_effectiveHeader;
  CString        m_headerVariable;

  StyleEdit      m_editHeaderName;
  StyleEdit      m_editExpected;
  StyleEdit      m_editEffective;
  StyleCheckbox  m_buttonCheck;
  StyleComboBox  m_comboOperator;
  StyleButton    m_buttonVariable;
  StyleComboBox  m_comboHeaderVariable;
  StyleButton    m_buttonHeaderParm;

public:
  afx_msg void OnBnClickedUseHeader();
  afx_msg void OnEnChangeHeaderName();
  afx_msg void OnEnChangeHeader();
  afx_msg void OnEnKillfocusHeader();
  afx_msg void OnBnClickedNewvar();
  afx_msg void OnEnChangeEffStatus();
  afx_msg void OnCbnSelchangeOperator();
  afx_msg void OnCbnSelHeaderVariable();
  afx_msg void OnBnClickedHeaderParm();
};

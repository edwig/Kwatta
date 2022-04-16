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
// This File   : XMLNodeDlg.h
// What it does: Validation of an XML node in the HTTPMessage response body
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "ValidateNET.h"
#include "Parameters.h"

// XMLNodeDlg dialog

class XMLNodeDlg : public StyleDialog
{
  DECLARE_DYNAMIC(XMLNodeDlg)

public:
  XMLNodeDlg(CWnd* pParent = nullptr);   // standard constructor
  virtual ~XMLNodeDlg();
  void     InitTab(ValidateNET* p_validate,Parameters* p_parameters);
  void     StoreVariables();

// Dialog Data
  enum { IDD = IDD_XML_NODE};

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

  bool           m_useXML;
  BufferOperator m_xmlOperator;
  CString        m_xmlPath;
  CString        m_expectedXML;
  CString        m_effectiveXML;
  CString        m_xmlVariable;

  StyleEdit      m_editXmlPath;
  StyleEdit      m_editExpected;
  StyleEdit      m_editEffective;
  StyleCheckbox  m_buttonCheck;
  StyleComboBox  m_comboOperator;
  StyleButton    m_buttonVariable;
  StyleComboBox  m_comboXmlVariable;
  StyleButton    m_buttonXmlParm;

public:
  afx_msg void OnBnClickedUseXML();
  afx_msg void OnEnChangeXmlPath();
  afx_msg void OnEnChangeXML();
  afx_msg void OnEnKillfocusXML();
  afx_msg void OnBnClickedNewvar();
  afx_msg void OnEnChangeEffStatus();
  afx_msg void OnCbnSelchangeOperator();
  afx_msg void OnCbnSelchangeXmlVariable();
  afx_msg void OnBnClickedXmlParm();
};

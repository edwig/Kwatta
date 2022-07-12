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
// This program: StepEditor
// This File   : MutliPartDlg.h
// What it does: Editing one of the parts of a multipart/form-data buffer
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

class MultiPart;

// MultiPartDlg dialog

class MultiPartDlg : public StyleDialog
{
  DECLARE_DYNAMIC(MultiPartDlg)

public:
  MultiPartDlg(CWnd* p_parent,MultiPart* p_part);
  virtual ~MultiPartDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
  enum { IDD = IDD_MULTIPART };
#endif

protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual BOOL OnInitDialog() override;

  void InitButtons();
  void InitContent();
  void UseMultiPart();
  void SaveMultiPart();

  MultiPart* m_part;

  CString       m_name;
  CString       m_charset;
  CString       m_contentID;
  CString       m_contentType;
  CString       m_boundary;
  bool          m_isFile;
  CString       m_LongFilename;
  CString       m_shortFilename;
  CString       m_data;

  StyleEdit     m_editName;
  StyleEdit     m_editCharset;
  StyleEdit     m_editContent;
  StyleComboBox m_comboContent;
  StyleCheckbox m_checkIsFile;
  StyleButton   m_buttonMulti;
  StyleEdit     m_editBoundary;
  StyleEdit     m_editLongFilename;
  StyleEdit     m_editShortFilename;
  StyleButton   m_buttonFile;
  StyleEdit     m_editData;
  StyleButton   m_buttonOK;
  StyleButton   m_buttonCancel;

  DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnEnChangeName();
  afx_msg void OnEnChangeCharset();
  afx_msg void OnEnChangeContentID();
  afx_msg void OnCbnSelchangeContent();
  afx_msg void OnBnClickedIsfile();
  afx_msg void OnBnClickedMulti();
  afx_msg void OnEnChangeFullfile();
  afx_msg void OnBnClickedButtFile();
  afx_msg void OnEnChangeShortfile();
  afx_msg void OnEnChangeData();
  afx_msg void OnBnClickedOk();
  afx_msg void OnBnClickedCancel();
};

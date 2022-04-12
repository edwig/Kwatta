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
// This File   : FormDataDlg.h
// What it does: Editing the request body as if a Multipart/Form-data buffer
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <MultiPartBuffer.h>

// FormDataDlg dialog

class FormDataDlg : public StyleDialog
{
	DECLARE_DYNAMIC(FormDataDlg)

public:
	FormDataDlg(CWnd* p_parent,CString p_body,CString p_content);
	virtual ~FormDataDlg();

	MultiPartBuffer* GetBuffer() { return &m_buffer; }

// Dialog Data
	enum { IDD = IDD_FORMDATA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual BOOL OnInitDialog() override;
	virtual void SetupDynamicLayout() override;

	void InitButtons();
	void InitGrid();
	void ParsePayload();
	void FillGrid();
	void FillRow(MultiPart* p_part,int p_row,bool p_new = true);

	MultiPartBuffer m_buffer;
  CString				  m_orignalBody;
	CString					m_contentType;
  ////
  CString				m_boundary;
	CString				m_inCharset;
	bool 					m_useCharset;
	bool					m_useExtensions;
  StyleEdit			m_editBoundary;
	StyleButton		m_buttonGenerate;
	StyleEdit			m_editCharset;
	StyleCheckbox m_checkUseCharset;
	StyleCheckbox m_checkUseExtensions;
	StyleGridCtrl m_grid;
	StyleButton   m_buttonNew;
	StyleButton   m_buttonDelete;
	StyleButton   m_buttonEdit;
	StyleButton	  m_buttonOK;
	StyleButton	  m_buttonCancel;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeBoundary();
	afx_msg void OnEnChangeCharset();
	afx_msg void OnBnClickedGenerate();
	afx_msg void OnBnClickedUsechar();
	afx_msg void OnBnClickedExtens();
	afx_msg void OnGridDblClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedNew();
	afx_msg void OnBnClickedDelete();
	afx_msg void OnBnClickedEdit();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};

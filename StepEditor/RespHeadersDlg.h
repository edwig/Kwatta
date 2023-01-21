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
// This File   : RespHeadersDlg.h
// What it does: Resulting headers of the response of a HTTP internet test call
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

class TestStepNET;
class StepResultNET;

// RespHeadersDlg dialog

class RespHeadersDlg : public StyleTab
{
	DECLARE_DYNAMIC(RespHeadersDlg)

public:
	RespHeadersDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~RespHeadersDlg();
  void InitTab();
  void SetResult(StepResultNET* p_result);

// Dialog Data
	enum { IDD = IDD_RESP_HEADERS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  virtual void SetupDynamicLayout() override;
  virtual BOOL OnInitDialog() override;
  void InitLijst();

  StyleGridCtrl m_list;

	DECLARE_MESSAGE_MAP()
};

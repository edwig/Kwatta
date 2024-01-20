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
// This File   : NewStepNamesDlg.h
// What it does: Defining the second step of the new teststep wizard
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

// NewStepNamesDlg dialog

class NewStepNamesDlg : public StyleTab
{
	DECLARE_DYNAMIC(NewStepNamesDlg)

public:
	NewStepNamesDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~NewStepNamesDlg();

  virtual bool InitStyleTab (void* p_data) override;
  virtual bool CheckStyleTab(void* p_data) override;
  virtual bool SaveStyleTab (void* p_data) override;

  bool         GetStepGlobal()  { return m_stepGlobal;  }
  CString      GetStepName()    { return m_stepName;    }
  CString      GetStepFile()    { return m_stepFile;    }

  // Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NEWSTEP_NAMES };
#endif

protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual void SetupDynamicLayout() override;

  CString StripExtension(CString p_filename);
  void    PresetFilenames();
  void    CheckFilename();
  void    ResetPage();

  StepType     m_stepType { StepType::Step_command };
  StyleButton  m_buttonStepGlobal;
  StyleEdit    m_editName;
  StyleEdit    m_editFile;

  CString      m_stepName;
  CString      m_stepFile;
  bool         m_stepGlobal{false};
  
  DECLARE_MESSAGE_MAP()

  afx_msg void OnBnClickedStepGlobal();
  afx_msg void OnEnChangeName();
  afx_msg void OnEnChangeFilename();
};

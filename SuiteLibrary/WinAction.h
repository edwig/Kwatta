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
// This program: SuiteLibrary
// This File   : WinAction.h
// What it does: A single action to perform in a Windows UI teststep
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

class TestStepWIN;
class WinActionsDlg;
class WINRunner;

enum class WinUIAction
{
   WA_Nothing = 0
  ,WA_Start
  ,WA_Close
  ,WA_CaretPos
  ,WA_Click
  ,WA_DblClick
  ,WA_RClick
  ,WA_Char
  ,WA_String
  ,WA_Present
  ,WA_Activate
  ,WA_Focus
  ,WA_TextArea
};

// Helper structure to locate windows in a window-search
typedef struct
{
  HWND    m_hwnd;
  CString m_partialname;
  CString m_partialClass;
  int     m_dialogID;
  bool    m_fromstart;
  bool    m_toend;
  bool    m_all;
}
SearchWindow;

//////////////////////////////////////////////////////////////////////////
//
//  WinActionZ
//
//////////////////////////////////////////////////////////////////////////

class WinAction
{
public:
  WinAction() = default;
 ~WinAction() = default;

  int PerformAction(CString& p_log,CString& p_errors,UINT& p_error);

  // Helper functions
  [[nodiscard]]        bool FillSearchPattern  (SearchWindow& p_search,int p_part);
  [[nodiscard]]        bool IterateChildWindows(SearchWindow& p_search,int p_level,CString& p_log,CString& p_errors);
  [[nodiscard]] static bool MatchWindowName    (SearchWindow* p_search,int p_ctrlID,CString& p_name,CString& p_classname);

private:
  friend TestStepWIN;         // May mutate
  friend WinActionsDlg;       // May mutate
  friend WINRunner;           // May read

  // Helper functions
  void GetSystemKey(CString& p_input,CString& p_tosend,int& p_virtkey);
  int  FindPattern(bool p_activate,CString& p_log,CString& p_errors,UINT& p_error);
  int  ActivateWindow(HWND p_hwnd, CString& p_log,CString& p_errors,UINT& p_error);

  // All actions
  int ActionStartProgram  (CString& p_log,CString& p_errors,UINT& p_error);
  int ActionCloseProgram  (CString& p_log,CString& p_errors,UINT& p_error);
  int ActionWindowPresent (CString& p_log,CString& p_errors,UINT& p_error);
  int ActionActivateWindow(CString& p_log,CString& p_errors,UINT& p_error);
  int ActionWindowFocus   (CString& p_log,CString& p_errors,UINT& p_error);
  int ActionWindowTxtArea (CString& p_log,CString& p_errors,UINT& p_error);
  int ActionWindowCaretPos(CString& p_log,CString& p_errors,UINT& p_error);
  int ActionSendOneChar   (CString& p_log,CString& p_errors,UINT& p_error);
  int ActionSendString    (CString& p_log,CString& p_errors,UINT& p_error);
  int ActionMouseClick    (CString& p_log,CString& p_errors,UINT& p_error);
  int ActionMouseDblClick (CString& p_log,CString& p_errors,UINT& p_error);
  int ActionMouseRClick   (CString& p_log,CString& p_errors,UINT& p_error);

  // Sending a keyboard character and mouse info
  int SendSystemKey(CString key,CString& p_log,CString& p_errors,UINT& p_error);
  int SendString   (CString str,CString& p_log,CString& p_errors,UINT& p_error);
  int SendMouseClick(bool p_dbl,bool p_left,CString& p_log,CString& p_errors,UINT& p_error);
  
  WinUIAction m_action;       // Action to perform
  CString     m_pattern;      // Name pattern to find MS-Window
  CString     m_argument1;    // Argument 1 as a string
  CString     m_argument2;    // Argument 2 as a string
  CString     m_argument3;    // Argument 3 as a string
  CString     m_wait;         // Wait time after the action
  // Effective
  CString     m_effectivePattern;
  CString     m_effectiveArgument1;
  CString     m_effectiveArgument2;
  CString     m_effectiveArgument3;
  CString     m_effectiveWait;
  // Processing data
  HWND        m_hwnd { NULL };
};


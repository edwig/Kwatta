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
  XString m_partialname;
  XString m_partialClass;
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

  int PerformAction(XString& p_log,XString& p_errors,UINT& p_error);

  // Helper functions
  [[nodiscard]]        bool FillSearchPattern  (SearchWindow& p_search,int p_part);
  [[nodiscard]]        bool IterateChildWindows(SearchWindow& p_search,int p_level,XString& p_log,XString& p_errors);
  [[nodiscard]] static bool MatchWindowName    (SearchWindow* p_search,int p_ctrlID,XString& p_name,XString& p_classname);

private:
  friend TestStepWIN;         // May mutate
  friend WinActionsDlg;       // May mutate
  friend WINRunner;           // May read

  // Helper functions
  void GetSystemKey(XString& p_input,XString& p_tosend,int& p_virtkey);
  int  FindPattern(bool p_activate,XString& p_log,XString& p_errors,UINT& p_error);
  int  ActivateWindow(HWND p_hwnd, XString& p_log,XString& p_errors,UINT& p_error);

  // All actions
  int ActionStartProgram  (XString& p_log,XString& p_errors,UINT& p_error);
  int ActionCloseProgram  (XString& p_log,XString& p_errors,UINT& p_error);
  int ActionWindowPresent (XString& p_log,XString& p_errors,UINT& p_error);
  int ActionActivateWindow(XString& p_log,XString& p_errors,UINT& p_error);
  int ActionWindowFocus   (XString& p_log,XString& p_errors,UINT& p_error);
  int ActionWindowTxtArea (XString& p_log,XString& p_errors,UINT& p_error);
  int ActionWindowCaretPos(XString& p_log,XString& p_errors,UINT& p_error);
  int ActionSendOneChar   (XString& p_log,XString& p_errors,UINT& p_error);
  int ActionSendString    (XString& p_log,XString& p_errors,UINT& p_error);
  int ActionMouseClick    (XString& p_log,XString& p_errors,UINT& p_error);
  int ActionMouseDblClick (XString& p_log,XString& p_errors,UINT& p_error);
  int ActionMouseRClick   (XString& p_log,XString& p_errors,UINT& p_error);

  // Sending a keyboard character and mouse info
  int SendSystemKey(XString key,XString& p_log,XString& p_errors,UINT& p_error);
  int SendString   (XString str,XString& p_log,XString& p_errors,UINT& p_error);
  int SendMouseClick(bool p_dbl,bool p_left,XString& p_log,XString& p_errors,UINT& p_error);
  
  WinUIAction m_action;       // Action to perform
  XString     m_pattern;      // Name pattern to find MS-Window
  XString     m_argument1;    // Argument 1 as a string
  XString     m_argument2;    // Argument 2 as a string
  XString     m_argument3;    // Argument 3 as a string
  XString     m_wait;         // Wait time after the action
  // Effective
  XString     m_effectivePattern;
  XString     m_effectiveArgument1;
  XString     m_effectiveArgument2;
  XString     m_effectiveArgument3;
  XString     m_effectiveWait;
  // Processing data
  HWND        m_hwnd { NULL };
};


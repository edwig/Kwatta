1///////////////////////////////////////////////////////////////////////////////////////////////////////
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
// This File   : WinAction.cpp
// What it does: A single action to perform in a Windows UI teststep
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WinAction.h"
#include "VirtualKeys.h"
#include <ExecuteProcess.h>
#include <stdlib.h>

// Forward declaration for EnumWindows
static BOOL IterateTopLevelWindows(HWND p_hwnd,LPARAM p_param);

int
WinAction::PerformAction(CString& p_log,CString& p_errors,UINT& p_error)
{
  switch (m_action)
  {
    case WinUIAction::WA_Start:         return ActionStartProgram  (p_log,p_errors,p_error); break;
    case WinUIAction::WA_Close:         return ActionCloseProgram  (p_log,p_errors,p_error); break;
    case WinUIAction::WA_CaretPos:      return ActionWindowCaretPos(p_log,p_errors,p_error); break;
    case WinUIAction::WA_Click:         return ActionMouseClick    (p_log,p_errors,p_error); break;
    case WinUIAction::WA_DblClick:      return ActionMouseDblClick (p_log,p_errors,p_error); break;
    case WinUIAction::WA_RClick:        return ActionMouseRClick   (p_log,p_errors,p_error); break;
    case WinUIAction::WA_Char:          return ActionSendOneChar   (p_log,p_errors,p_error); break;
    case WinUIAction::WA_String:        return ActionSendString    (p_log,p_errors,p_error); break;
    case WinUIAction::WA_Present:       return ActionWindowPresent (p_log,p_errors,p_error); break;
    case WinUIAction::WA_Activate:      return ActionActivateWindow(p_log,p_errors,p_error); break;
    case WinUIAction::WA_Focus:         return ActionWindowFocus   (p_log,p_errors,p_error); break;
    case WinUIAction::WA_TextArea:      return ActionWindowTxtArea (p_log,p_errors,p_error); break;
    default:                            break;
  }
  p_log += _T("Nothing done\n");
  p_errors.AppendFormat(_T("Unknown action [%d] (%s) (%s) (%s)\n"),(int)m_action,m_argument1.GetString(),m_argument2.GetString(),m_argument3.GetString());
  p_error = 1;
  return 1;
}

//////////////////////////////////////////////////////////////////////////
// 
// PRIVATE
// Perform the Windows UI actions
//
//////////////////////////////////////////////////////////////////////////

int
WinAction::ActionStartProgram(CString& p_log, CString& p_errorString, UINT& p_error)
{
  XString errors;

  p_log.AppendFormat(_T("Start program [%s] with arguments [%s]\n"), m_pattern.GetString(), m_argument1.GetString());

  int res = ExecuteProcess(m_pattern, m_argument1, false, errors, SW_SHOW, false, true);
  if (!errors.IsEmpty())
  {
    p_errorString += errors;
    p_error = res;
    return 1;
  }
  p_log.AppendFormat(_T("Result of started program: [%d]\n"), res);
  return 0;
}

int
WinAction::ActionCloseProgram(CString& p_log,CString& p_errors,UINT& p_error)
{
  if(FindPattern(false,p_log,p_errors,p_error) == 0)
  {
    p_log += _T("Posted a WM_CLOSE. Window MAY close!\n");
    ::PostMessage(m_hwnd,WM_CLOSE,NULL,NULL);
    return 0;
  }
  return 1;
}

int
WinAction::ActionActivateWindow(CString& p_log,CString& p_errors,UINT& p_error)
{
  p_log.AppendFormat(_T("Try to activate window [%s]\n"),m_pattern.GetString());

  if(m_pattern.Find('/') > 0)
  {
    p_errors += _T("Can only activate a TOP level window!\n");
    return (p_error = ERROR_INVALID_PARAMETER);
  }
  if(FindPattern(true,p_log,p_errors,p_error) == 0)
  {
    return 0;
  }
  return ERROR_NOT_FOUND;
}

int
WinAction::ActionWindowPresent(CString& p_log,CString& p_errors,UINT& p_error)
{
  p_log.AppendFormat(_T("Testing if window [%s] is present.\n"),m_pattern.GetString());

  int counter = _ttoi(m_argument1);
  int wait    = _ttoi(m_argument2);
  if(counter <     3) counter = 3;
  if(counter >  1000) counter = 1000;
  if(wait    <   500) wait    = 500;
  if(wait    > 10000) wait    = 10000;

  int notfound = 0;
  do
  {
    notfound = FindPattern(false,p_log,p_errors,p_error);
    if(notfound)
    {
      if(--counter == 0)
      {
        break;
      }
      Sleep(wait);
    }
  }
  while(notfound);

  p_log.AppendFormat(_T("Window [%s] is %s present.\n"),m_pattern.GetString(),notfound ? _T("NOT") : _T("NOW"));
  if(notfound && !p_error)
  {
    p_error = ERROR_NOT_FOUND;
    p_errors += _T("Window not found: ") + m_pattern;
  }
  return notfound;
}

int 
WinAction::ActionWindowFocus(CString& p_log,CString& p_errors,UINT& p_error)
{
  if(FindPattern(true,p_log,p_errors,p_error) == 0)
  {
    p_log += _T("Posted a WM_SETFOCUS. Window MAY gain the keyboard focus!\n");
    ::PostMessage(m_hwnd,WM_SETFOCUS,NULL,NULL);
    return 0;
  }
  return 1;
}

int 
WinAction::ActionWindowTxtArea(CString& p_log,CString& p_errors,UINT& p_error)
{
  p_log.AppendFormat(_T("Reading TEXT area of window [%s].\n"),m_pattern.GetString());

  if(FindPattern(false,p_log,p_errors,p_error) == 0)
  {
    // OK. OVERWRITING the previous log. We need this one as a result
    CString text;
    int length = (int)::SendMessage(m_hwnd, WM_GETTEXTLENGTH, 0, 0);
    if(length > 0)
    {
      TCHAR* buffer = text.GetBufferSetLength(length + 1);
      ::SendMessage(m_hwnd,WM_GETTEXT,(WPARAM)(length + 1),(LPARAM)buffer);
      text.ReleaseBufferSetLength(length);
    }
    p_log = text;
    return 0;
  }
  return 1;
}

int
WinAction::ActionSendOneChar(CString& p_log,CString& p_errors,UINT& p_error)
{
  if(!m_pattern.IsEmpty())
  {
    // Try to locate the (sub) window for a character keystroke
    int res = FindPattern(true,p_log,p_errors,p_error);
    if(res)
    {
      return res;
    }
  }
  return SendSystemKey(m_argument1,p_log,p_errors,p_error);
}

int
WinAction::ActionSendString(CString& p_log,CString& p_errors,UINT& p_error)
{
  if(!m_pattern.IsEmpty())
  {
    // Try to locate the (sub) window for a character keystroke
    int res = FindPattern(true,p_log,p_errors,p_error);
    if(res)
    {
      return res;
    }
  }
  return SendString(m_argument1,p_log,p_errors,p_error);
}

int 
WinAction::ActionMouseClick(CString& p_log,CString& p_errors,UINT& p_error)
{
  if(!m_pattern.IsEmpty())
  {
    // Try to locate the (sub) window for a character keystroke
    int res = FindPattern(false,p_log,p_errors,p_error);
    if(res)
    {
      return res;
    }
  }
  return SendMouseClick(false,true,p_log,p_errors,p_error);
}

int 
WinAction::ActionMouseDblClick(CString& p_log,CString& p_errors,UINT& p_error)
{
  if(!m_pattern.IsEmpty())
  {
    // Try to locate the (sub) window for a character keystroke
    int res = FindPattern(false,p_log,p_errors,p_error);
    if(res)
    {
      return res;
    }
  }
  return SendMouseClick(true,true,p_log,p_errors,p_error);
}

int
WinAction::ActionMouseRClick(CString& p_log,CString& p_errors,UINT& p_error)
{
  if(!m_pattern.IsEmpty())
  {
    // Try to locate the (sub) window for a character keystroke
    int res = FindPattern(false,p_log,p_errors,p_error);
    if(res)
    {
      return res;
    }
  }
  return SendMouseClick(true,false,p_log,p_errors,p_error);
}

int
WinAction::ActionWindowCaretPos(CString& p_log,CString& p_errors,UINT& p_error)
{
  if(FindPattern(false,p_log,p_errors,p_error) == 0)
  {
    CEdit* edit = dynamic_cast<CEdit*>(CWnd::FromHandle(m_hwnd));
    if(edit)
    {
      int begin = _ttoi(m_argument1);
      int end   = _ttoi(m_argument2);

      // Make it possible to calculate relative to the end
      if(begin < -1 || end < -1)
      {
        int length = (int)::SendMessage(m_hwnd,WM_GETTEXTLENGTH,0,0);
        if(begin < -1) begin = (length - begin >= 0) ? length + begin : -1;
        if(end < -1)   end   = (length - end   >= 0) ? length + end   : -1;
      }
      // Does EM_SETSEL with the following combinations
      // begin = -1 (end unspecified)   => Remove the selection
      // begin = 0 && end = -1          => Select all text
      // begin >= 0 && end >= begin     => Select these characters
      // begin >= 0 && end == begin     => Set the caret position
      ::SendMessage(m_hwnd,EM_SETSEL,     begin,end);
      ::SendMessage(m_hwnd,EM_SCROLLCARET,begin,end);
      return 0;
    }
    p_errors += _T("Window is not an EDIT (derived) control!\n");
  }
  return (p_error = ERROR_NOT_FOUND);
}

//////////////////////////////////////////////////////////////////////////
//
// Helper functions
//
//////////////////////////////////////////////////////////////////////////

int
WinAction::FindPattern(bool p_activate,CString& p_log,CString& p_errors,UINT& p_error)
{
  // Starting the search
  p_log.AppendFormat(_T("Test if window [%s] is present\n"),m_pattern.GetString());

  SearchWindow search;
  if(!FillSearchPattern(search,0))
  {
    p_errors += _T("No top-level window pattern to search for!\n");
    p_error = ERROR_NOT_FOUND;
    return 1;
  }

  if(EnumWindows(IterateTopLevelWindows,(LPARAM)&search))
  {
    p_log += _T("Top-level window enumerated search done.\n");
  }
  if(!search.m_hwnd)
  {
    p_log.AppendFormat(_T("Top-level window NOT FOUND!\n"));
    p_error = ERROR_NOT_FOUND;
    return 1;
  }
  m_hwnd = search.m_hwnd; // Last found window
  p_log.AppendFormat(_T("Top-level window is present. HWND = [%llX]\n"),m_hwnd);

  // Check / Try to activate the window
  if(p_activate)
  {
    ActivateWindow(m_hwnd,p_log,p_errors,p_error);
  }

  // More child windows to find?
  if(m_pattern.Find('/') > 0)
  {
    // Recurse into child windows for a search
    if(!IterateChildWindows(search,1,p_log,p_errors))
    {
      p_error = ERROR_NOT_FOUND;
      p_errors.AppendFormat(_T("Next partial name NOT found [%s]\n"),search.m_partialname.GetString());
      return 1;
    }
    p_log.AppendFormat(_T("Next child window found [%s] HWND = [%llX]\n"),search.m_partialname.GetString(),m_hwnd);
  }
  return 0;
}

// Called by EnumWindows for a top-level window
BOOL IterateTopLevelWindows(HWND p_hwnd,LPARAM p_param)
{
  SearchWindow* search = reinterpret_cast<SearchWindow*>(p_param);

  TCHAR windowname[2 * MAX_PATH];
  TCHAR classname [2 * MAX_PATH];
  ::GetWindowText(p_hwnd,windowname,2 * MAX_PATH);
  ::GetClassName (p_hwnd,classname, 2 * MAX_PATH);
 
  CString name(windowname);
  CString nclass(classname);

  // Top level windows have NO control-ID
  if(WinAction::MatchWindowName(search,0,name,nclass))
  {
    search->m_hwnd = p_hwnd;
    return FALSE; // Stop further search
  }
  // No match, continue searching
  return TRUE;
}

bool 
WinAction::IterateChildWindows(SearchWindow& p_search,int p_level,CString& p_log,CString& p_errors)
{
  // Next recursive level found?
  if(!FillSearchPattern(p_search,p_level))
  {
    return true;
  }

  // Search past all children has first priority
  if(p_search.m_all)
  {
    // Descend into grand-children
    HWND child = GetWindow(m_hwnd,GW_CHILD);
    while(child)
    {
      HWND push = m_hwnd;
      m_hwnd = child;
      if(IterateChildWindows(p_search,p_level + 1,p_log,p_errors))
      {
        return true;
      }
      m_hwnd = push;
      // Next child window
      child = GetWindow(child,GW_HWNDNEXT);
    }
  }
  else
  {
    // First level search in my direct child windows
    HWND child = GetWindow(m_hwnd,GW_CHILD);
    while(child)
    {
      TCHAR classname[2 * MAX_PATH];
      ::GetClassName(child,classname,2 * MAX_PATH);
      CString nclass(classname);
      CString text;

      int length = (int)::SendMessage(child,WM_GETTEXTLENGTH,0,0);
      if(length > 0)
      {
        TCHAR* buffer = text.GetBufferSetLength(length + 1);
        ::SendMessage(child,WM_GETTEXT,(WPARAM)(length + 1),(LPARAM)buffer);
        text.ReleaseBufferSetLength(length);
      }
      // Possibly getting a control-id or zero
      int controlID = GetDlgCtrlID(child);

      if(MatchWindowName(&p_search,controlID,text,nclass))
      {
        m_hwnd = child;
        // See if we are not the last in the search
        if(FillSearchPattern(p_search,p_level + 1))
        {
          return IterateChildWindows(p_search,p_level + 1,p_log,p_errors);
        }
        return true;
      }
      // Next child window
      child = GetWindow(child,GW_HWNDNEXT);
    }
  }
  return false;
}

int
WinAction::ActivateWindow(HWND p_hwnd,CString& p_log,CString& p_errors,UINT& p_error)
{
  if(IsHungAppWindow(p_hwnd))
  {
    p_errors.AppendFormat(_T("Cannot activate a HUNG window [%llX]\n"),p_hwnd);
    return (p_error = ERROR_WAKE_SYSTEM);
  }
  if(IsIconic(p_hwnd))
  {
    p_log.AppendFormat(_T("SYSCOMMAND restore window HWND [%llX]\n"),p_hwnd);
    SendMessage(p_hwnd,WM_SYSCOMMAND,SC_RESTORE,0);
  }
  if(GetForegroundWindow() != p_hwnd)
  {
    // Force window into the foreground
    p_log.AppendFormat(_T("Force window into the foreground HWND [%llX]\n"),p_hwnd);
    BOOL totop  = BringWindowToTop(p_hwnd);
    BOOL setpos = SetWindowPos(p_hwnd,HWND_TOP,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
    BOOL fgrnd  = SetForegroundWindow(p_hwnd);
    if(totop || setpos || fgrnd)
    {
      p_log += _T("Window brought to the foreground.\n");
      return 0;
    }
    p_errors += _T("Window NOT in the foreground. Cannot be used for keyboard.\n");
    return (p_error = ERROR_WAKE_SYSTEM);
  }
  p_log += _T("Window IS foreground window.\n");
  return 0;
}

bool 
WinAction::FillSearchPattern(SearchWindow& p_search,int p_part)
{
  CString pattern(m_pattern);
  while(p_part)
  {
    if(int pos = pattern.Find('/'); pos > 0)
    {
      pattern = pattern.Mid(pos + 1);
    }
    else
    {
      pattern.Empty();
      break;
    }
    // Next part
    --p_part;
  }
  // Remove everything after requested pattern
  if(int pos = pattern.Find('/'); pos > 0)
  {
    pattern = pattern.Left(pos);
  }
  if(pattern.IsEmpty())
  {
    return false;
  }
  // Fill the search parameter
  p_search.m_partialname.Empty();
  p_search.m_partialClass.Empty();
  p_search.m_dialogID    = 0;
  p_search.m_hwnd        = NULL;
  p_search.m_fromstart   = true;
  p_search.m_toend       = true;
  p_search.m_all         = false;
  
  // Look for dialog ID "{n}" or a partial class name "{identifier}"
  if(pattern.Left(1) == _T("{") && pattern.Right(1) == _T("}"))
  {
    CString partial = pattern.Mid(1,pattern.GetLength() - 2);
    if(_ttoi(partial) > 0)
    {
      p_search.m_dialogID = _ttoi(partial);
    }
    else
    {
      p_search.m_partialClass = partial;
    }
  }
  else
  {
    p_search.m_partialname = pattern;
  }
  if(pattern == _T("*"))
  {
    p_search.m_all = true;
  }
  else
  {
    if(pattern.Left(1) == _T("*"))
    {
      p_search.m_partialname = pattern.Mid(1);
      p_search.m_fromstart = false;
    }
    if(pattern.Right(1) == _T("*"))
    {
      p_search.m_partialname = p_search.m_partialname.Left(p_search.m_partialname.GetLength() - 1);
      p_search.m_toend = false;
    }
  }
  // Top level window not filled in. '*' is NOT a valid top level search
  if(p_part == 0 && p_search.m_partialname.IsEmpty() && p_search.m_partialClass.IsEmpty())
  {
    return false;
  }
  return true;
}

bool
WinAction::MatchWindowName(SearchWindow* p_search,int p_ctrlID,CString& p_name,CString& p_classname)
{
  // STEP 1: Try control ID first
  if(p_search->m_dialogID && p_search->m_dialogID == p_ctrlID)
  {
    return true;
  }
  // STEP 2: Try (partial) classname 
  if(!p_search->m_partialClass.IsEmpty())
  {
    if(p_classname.Find(p_search->m_partialClass) >= 0)
    {
      return true;
    }
    return false;
  }
  // STEP 3: Somewhere within the text
  if(!p_search->m_fromstart && !p_search->m_toend)
  {
    // Must be somewhere within the name
    if(p_name.Find(p_search->m_partialname) >= 0)
    {
      return true;
    }
  }
  else if(!p_search->m_toend)
  {
    // STEP 4: Must be at the start of the name
    if(p_name.Find(p_search->m_partialname) == 0)
    {
      return true;
    }
  }
  else
  {
    // STEP 5: Must be at the end
    p_name = p_name.Right(p_search->m_partialname.GetLength());
    if(p_name == p_search->m_partialname)
    {
      return true;
    }
  }
  // Nope: not found
  return false;
}

// Scan a (possibly virtual) key from the input string
// Return the left over of the string along with
// the first keystroke and the virtual key code (VK_*)
void
WinAction::GetSystemKey(CString& p_input,CString& p_tosend,int& p_virtkey)
{
  // Reset input
  p_tosend.Empty();
  p_virtkey = 0;

  // Test if we have something to do
  if(p_input.IsEmpty())
  {
    return;
  }
  // Special key escaped
  if(p_input.GetAt(0) == _T('\\') && p_input.GetAt(1) == '{')
  {
    p_tosend = _T("{");
    p_input  = p_input.Mid(1);
    return;
  }
  // Regular character
  if(p_input.GetAt(0) == '{')
  {
    if(int pos = p_input.Find('}'); pos > 1)
    {
      CString vkey = p_input.Mid(1,pos - 1);
      int code = GetVirtualKeyCode(vkey);
      if (code > 0)
      {
        p_tosend  = p_input.Left(pos + 1);
        p_input   = p_input.Mid(pos + 1);
        p_virtkey = code;
        return;
      }
    }
  }
  // Regular character
  p_tosend = p_input.Left(1);
  p_input  = p_input.Mid(1);
}

int
WinAction::SendSystemKey(CString key,CString& p_log,CString& p_errors,UINT& p_error)
{
  CString tosend(key);
  CString send;
  int vkey = 0;
  int result = 0;

  GetSystemKey(tosend,send,vkey);

  if(vkey == VK_CONTROL || vkey == VK_LCONTROL || vkey == VK_RCONTROL ||
     vkey == VK_MENU    || vkey == VK_LMENU    || vkey == VK_RMENU    ||
     vkey == VK_SHIFT   || vkey == VK_LSHIFT   || vkey == VK_RSHIFT   ||
     vkey == VK_LWIN    || vkey == VK_RWIN )
  {
    CString second;
    int vkey2;
    GetSystemKey(tosend,second,vkey2);

    if(!vkey2)
    {
      vkey2 = _totupper(second.GetAt(0));
    }
    p_log.AppendFormat(_T("Sending one character [%s%s]\n"),send.GetString(),second.GetString());
    result += SendInputKey((WORD)vkey,(WORD)vkey2);

    // Yield processor
    // Sleep after EACH character
    Sleep(g_keyboardSleep);

    return result;
  }

  // Single virtual key case
  if(vkey)
  {
    p_log.AppendFormat(_T("Sending one character [%s]\n"),key.GetString());
    return SendInputKey((WORD)vkey);
  }

  if(send.GetLength() > 1)
  {
    // Error: More than one character
    p_errors.AppendFormat(_T("Cannot output more than 1 character. Found: %s\n"),tosend.GetString());
    return (p_error = ERROR_INVALID_PARAMETER);
  }

  // Getting the HWND with the keyboard focus
  HWND hwnd = m_hwnd ? m_hwnd : ::GetFocus();
  TCHAR ckey = send.GetAt(0);

  // Regular non-virtual key
  p_log.AppendFormat(_T("Sending one character [%c] to window [%llX]\n"),ckey,hwnd);
  ::PostMessage(hwnd,WM_CHAR,(WPARAM)ckey,(LPARAM)1);

  // Yield processor
  // Sleep after EACH character
  Sleep(g_keyboardSleep);

  return 0;
}

int
WinAction::SendString(CString str,CString& p_log,CString& p_errors,UINT& p_error)
{
  int res = 0;
  p_log.AppendFormat(_T("Sending string of characters: %s\n"),str.GetString());

  while(str.GetLength())
  {
    CString send;
    int vkey = 0;
    GetSystemKey(str,send,vkey);

    if(vkey == VK_CONTROL || vkey == VK_LCONTROL || vkey == VK_RCONTROL ||
       vkey == VK_MENU    || vkey == VK_LMENU    || vkey == VK_RMENU    ||
       vkey == VK_SHIFT   || vkey == VK_LSHIFT   || vkey == VK_RSHIFT   ||
       vkey == VK_LWIN    || vkey == VK_RWIN )
    {
      CString second;
      int vkey2;
      GetSystemKey(str,second,vkey2);

      if(!vkey2)
      {
        vkey2 = _totupper(second.GetAt(0));
      }
      p_log.AppendFormat(_T("Sending one character [%s%s]\n"),send.GetString(),second.GetString());
      res += SendInputKey((WORD)vkey,(WORD)vkey2);

      Sleep(0); // Yield processor
      // Sleep after EACH character
      Sleep(g_keyboardSleep);
    }
    else
    {
      res += SendSystemKey(send,p_log,p_errors,p_error);
    }
    // Break of after error
    if(res)
    {
      return res;
    }
  }
  return 0;
}

int 
WinAction::SendMouseClick(bool p_dbl,bool p_left,CString& p_log,CString& p_errors,UINT& p_error)
{
  CRect windowRect;
  GetWindowRect(m_hwnd,windowRect);

  int x = _ttoi(m_argument1);
  int y = _ttoi(m_argument2);

  p_log.AppendFormat(_T("Mouse coordinates relative to [%s]\n"),  m_hwnd ? _T("window") : _T("desktop"));
  p_log.AppendFormat(_T("Sending a %s %smouse click to [%d,%d]\n"),p_dbl ? _T("double") : _T("single"),p_left ? _T("") : _T("right "),x,y);

  x += (x >= 0) ? windowRect.left : windowRect.right;
  y += (y >= 0) ? windowRect.top  : windowRect.bottom;

  p_log.AppendFormat(_T("Absolute mouse coordinates [%d,%d]\n"),x,y);
  MapScreenPositionToMousePosition(x,y,m_hwnd);
  p_log.AppendFormat(_T("Absolute mouse position [%d,%d]\n"), x, y);

  if(p_dbl)
  {
    SendInputMouseClick(x,y,p_left);
    Sleep(GetDoubleClickTime() / 2);
  }
  int res = SendInputMouseClick(x,y,p_left);
  if(res)
  {
    p_error = res;
    p_errors += _T("Cannot send mouse coordinates/click\n");
  }
  return res;
}

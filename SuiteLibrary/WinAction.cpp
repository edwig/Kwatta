﻿///////////////////////////////////////////////////////////////////////////////////////////////////////
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

// Forward declarations
BOOL IterateTopLevelWindows(HWND p_hwnd, LPARAM p_param);
// Sleeping after each simulated keyboard press
extern int g_keyboardSleep;

int
WinAction::PerformAction(CString& p_log, CString& p_errors, UINT& p_error)
{
  switch (m_action)
  {
  case WinUIAction::WA_Start:         return ActionStartProgram(p_log, p_errors, p_error); break;
  case WinUIAction::WA_Close:         return ActionCloseProgram(p_log, p_errors, p_error); break;
  case WinUIAction::WA_AbsolutePos:   break;
  case WinUIAction::WA_RelativePos:   break;
  case WinUIAction::WA_Click:         return ActionMouseClick   (p_log, p_errors, p_error); break;
  case WinUIAction::WA_DblClick:      return ActionMouseDblClick(p_log, p_errors, p_error); break;
  case WinUIAction::WA_Char:          return ActionSendOneChar  (p_log, p_errors, p_error); break;
  case WinUIAction::WA_String:        return ActionSendString   (p_log, p_errors, p_error); break;
  case WinUIAction::WA_Present:       return ActionWindowPresent(p_log, p_errors, p_error); break;
  case WinUIAction::WA_Focus:         return ActionWindowFocus  (p_log, p_errors, p_error); break;
  case WinUIAction::WA_HasStyle:      break;
  case WinUIAction::WA_TextArea:      return ActionWindowTxtArea(p_log, p_errors, p_error); break;
  default:                            break;
  }
  p_log += "Nothing done\n";
  p_errors.AppendFormat("Unknown action [%d] (%s) (%s) (%s)", (int)m_action, m_argument1.GetString(), m_argument2.GetString(), m_argument3.GetString());
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

  p_log.AppendFormat("Start program [%s] with arguments [%s]\n", m_pattern.GetString(), m_argument1.GetString());

  int res = ExecuteProcess(m_pattern, m_argument1, false, errors, SW_SHOW, false, true);
  if (!errors.IsEmpty())
  {
    p_errorString += errors;
    p_error = res;
    return 1;
  }
  p_log.AppendFormat("Result of started program: [%d]\n", res);
  return 0;
}

int
WinAction::ActionCloseProgram(CString& p_log,CString& p_errors,UINT& p_error)
{
  if(ActionWindowPresent(p_log,p_errors,p_error) == 0)
  {
    p_log += "Posted a WM_CLOSE. Window MAY close!";
    ::PostMessage(m_hwnd,WM_CLOSE,NULL,NULL);
    return 0;
  }
  return 1;
}

int
WinAction::ActionWindowPresent(CString& p_log,CString& p_errors,UINT& p_error)
{
  // Starting the search
  p_log.AppendFormat("Test if window [%s] is present\n",m_pattern.GetString());

  SearchWindow search;
  if(!FillSearchPattern(search,0))
  {
    p_errors += "No top-level window pattern to search for!";
    p_error = ERROR_NOT_FOUND;
    return 1;
  }

  if(EnumWindows(IterateTopLevelWindows,(LPARAM)&search))
  {
    p_log += "Top-level window enumerated search done.\n";
  }
  if(!search.m_hwnd)
  {
    p_log.AppendFormat("Top-level window NOT FOUND!\n");
    p_error = ERROR_NOT_FOUND;
    return 1;
  }
  m_hwnd = search.m_hwnd; // Last found window
  p_log.AppendFormat("Top-level window is present. HWND = [%llX]\n",m_hwnd);

  int index = 1;
  while(FillSearchPattern(search,index))
  {
    if(!IterateChildWindows(search,p_log,p_errors))
    {
      p_error = ERROR_NOT_FOUND;
      return 1;
    }
    // Next pattern to search
    ++index;
  }
  return 0;
}

int 
WinAction::ActionWindowFocus(CString& p_log,CString& p_errors,UINT& p_error)
{
  if(ActionWindowPresent(p_log,p_errors,p_error) == 0)
  {
    p_log += "Posted a WM_SETFOCUS. Window MAY gain the keyboard focus!";
    ::PostMessage(m_hwnd,WM_SETFOCUS,NULL,NULL);
    return 0;
  }
  return 1;
}

int 
WinAction::ActionWindowTxtArea(CString& p_log,CString& p_errors,UINT& p_error)
{
  if(ActionWindowPresent(p_log,p_errors,p_error) == 0)
  {
    int length = (int)::SendMessage(m_hwnd, WM_GETTEXTLENGTH, 0, 0);
    if(length > 0)
    {
      CString text;
      char* buffer = text.GetBufferSetLength(length + 1);
      ::SendMessage(m_hwnd,WM_GETTEXT,(WPARAM)(length + 1),(LPARAM)buffer);
      text.ReleaseBufferSetLength(length);

      p_log = text;
    }
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
    int res = ActionWindowPresent(p_log,p_errors,p_error);
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
    int res = ActionWindowPresent(p_log,p_errors,p_error);
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
    int res = ActionWindowPresent(p_log,p_errors,p_error);
    if(res)
    {
      return res;
    }
  }
  return SendMouseClick(false,p_log,p_errors,p_error);
}

int 
WinAction::ActionMouseDblClick(CString& p_log,CString& p_errors,UINT& p_error)
{
  if(!m_pattern.IsEmpty())
  {
    // Try to locate the (sub) window for a character keystroke
    int res = ActionWindowPresent(p_log,p_errors,p_error);
    if(res)
    {
      return res;
    }
  }
  return SendMouseClick(true,p_log,p_errors,p_error);
}

//////////////////////////////////////////////////////////////////////////
//
// Helper functions
//
//////////////////////////////////////////////////////////////////////////

// Called by EnumWindows for a top-level window
BOOL IterateTopLevelWindows(HWND p_hwnd,LPARAM p_param)
{
  SearchWindow* search = reinterpret_cast<SearchWindow*>(p_param);

  char windowname[2 * MAX_PATH];
  ::GetWindowText(p_hwnd,windowname,2 * MAX_PATH);
  CString name(windowname);

  if(WinAction::MatchWindowName(search,name))
  {
    search->m_hwnd = p_hwnd;
    return FALSE; // Stop further search
  }
  // No match, continue searching
  return TRUE;
}

bool 
WinAction::IterateChildWindows(SearchWindow& p_search,CString& p_log,CString& p_errors)
{
  HWND child = GetWindow(m_hwnd,GW_CHILD);
  while(child)
  {
    int length = (int)::SendMessage(child,WM_GETTEXTLENGTH,0,0);
    if (length > 0)
    {
      CString text;
      char* buffer = text.GetBufferSetLength(length + 1);
      ::SendMessage(child,WM_GETTEXT,(WPARAM)(length + 1),(LPARAM)buffer);
      text.ReleaseBufferSetLength(length);

      if(MatchWindowName(&p_search,text))
      {
        p_log.AppendFormat("Next child window found [%s] HWND = [%llX]\n",p_search.m_partialname.GetString(),child);
        m_hwnd = child;
        return true;
      }
    }
    // Next child window
    child = GetWindow(child,GW_HWNDNEXT);
  }
  p_errors.AppendFormat("Next partial name NOT found [%s]\n",p_search.m_partialname.GetString());
  return false;
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
  p_search.m_hwnd        = NULL;
  p_search.m_partialname = pattern;
  p_search.m_fromstart   = true;
  p_search.m_toend       = true;
  if(pattern.Left(1) == "*")
  {
    p_search.m_partialname = pattern.Mid(1);
    p_search.m_fromstart = false;
  }
  if(pattern.Right(1) == "*")
  {
    p_search.m_partialname = p_search.m_partialname.Left(p_search.m_partialname.GetLength() - 1);
    p_search.m_toend = false;
  }

  // Top level window not filled in. '*' is NOT a valid top level search
  if(p_part == 0 && p_search.m_partialname.IsEmpty())
  {
    return false;
  }
  return true;
}

bool
WinAction::MatchWindowName(SearchWindow* p_search,CString& p_name)
{
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
    // Must be at the start of the name
    if(p_name.Find(p_search->m_partialname) == 0)
    {
      return true;
    }
  }
  else // Must be at the end
  {
    p_name = p_name.Right(p_search->m_partialname.GetLength());
    if(p_name == p_search->m_partialname)
    {
      return true;
    }
  }
  return false;
}

// Sending a keyboard character
int
WinAction::SendInputChar(WORD p_char1)
{
  INPUT inputs[2] = {};
  ZeroMemory(inputs, sizeof(inputs));

  inputs[0].type = INPUT_KEYBOARD;
  inputs[0].ki.wVk = p_char1;

  inputs[1].type = INPUT_KEYBOARD;
  inputs[1].ki.wVk = p_char1;
  inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;

  UINT uSent = SendInput(ARRAYSIZE(inputs),inputs,sizeof(INPUT));
  if(uSent != ARRAYSIZE(inputs))
  {
    return HRESULT_FROM_WIN32(GetLastError());
  }
  Sleep(0); // Yield processor
  Sleep(g_keyboardSleep);
  return 0;
}

int
WinAction::SendInputChar(WORD p_char1,WORD p_char2)
{
  INPUT inputs[4] = {};
  ZeroMemory(inputs, sizeof(inputs));

  inputs[0].type = INPUT_KEYBOARD;
  inputs[0].ki.wVk = p_char1;

  inputs[1].type = INPUT_KEYBOARD;
  inputs[1].ki.wVk = p_char2;

  inputs[2].type = INPUT_KEYBOARD;
  inputs[2].ki.wVk = p_char2;
  inputs[2].ki.dwFlags = KEYEVENTF_KEYUP;

  inputs[3].type = INPUT_KEYBOARD;
  inputs[3].ki.wVk = p_char1;
  inputs[3].ki.dwFlags = KEYEVENTF_KEYUP;

  UINT uSent = SendInput(ARRAYSIZE(inputs),inputs,sizeof(INPUT));
  if(uSent != ARRAYSIZE(inputs))
  {
    return HRESULT_FROM_WIN32(GetLastError());
  }
  Sleep(0); // Yield processor
  Sleep(g_keyboardSleep);
  return 0;
}

int
WinAction::SendInputMouseClick(int x,int y)
{
  INPUT inputs[1] = {};
  ZeroMemory(inputs,sizeof(inputs));

  inputs[0].type  = INPUT_MOUSE;
  inputs[0].mi.dx = x;
  inputs[0].mi.dy = y;
  inputs[0].mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;

  UINT uSent = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
  if (uSent != ARRAYSIZE(inputs))
  {
    return HRESULT_FROM_WIN32(GetLastError());
  }

  Sleep(100);

  inputs[0].type = INPUT_MOUSE;
  inputs[0].mi.dx = x;
  inputs[0].mi.dy = y;
  inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

  uSent = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
  if (uSent != ARRAYSIZE(inputs))
  {
    return HRESULT_FROM_WIN32(GetLastError());
  }

  Sleep(100);

  inputs[0].type  = INPUT_MOUSE;
  inputs[0].mi.dx = x;
  inputs[0].mi.dy = y;
  inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTUP;

  uSent = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
  if (uSent != ARRAYSIZE(inputs))
  {
    return HRESULT_FROM_WIN32(GetLastError());
  }

  return 0;
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
  if(p_input.GetAt(0) == '\\' && p_input.GetAt(1) == '{')
  {
    p_tosend = "{";
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
      vkey2 = toupper(second.GetAt(0));
    }
    p_log.AppendFormat("Sending one character [%s%s]\n",send.GetString(),second.GetString());
    result += SendInputChar((WORD)vkey,(WORD)vkey2);

    Sleep(0); // Yield processor
    Sleep(g_keyboardSleep);

    return result;
  }

  // Single virtual key case
  if(vkey)
  {
    p_log.AppendFormat("Sending one character [%s]\n",key.GetString());
    return SendInputChar((WORD)vkey);
  }

  if(send.GetLength() > 1)
  {
    // Error: More than one character
    p_errors.AppendFormat("Cannot output more than 1 character. Found: %s\n",tosend.GetString());
    return (p_error = ERROR_INVALID_PARAMETER);
  }

  // Getting the HWND with the keyboard focus
  HWND hwnd = m_hwnd ? m_hwnd : ::GetFocus();
  char ckey = send.GetAt(0);

  // Regular non-virtual key
  p_log.AppendFormat("Sending one character [%c] to window [%llX]\n",ckey,hwnd);
  ::PostMessage(hwnd,WM_KEYDOWN,(WPARAM)0,   (LPARAM)0);
  ::PostMessage(hwnd,WM_CHAR,   (WPARAM)ckey,(LPARAM)0);
  ::PostMessage(hwnd,WM_KEYUP,  (WPARAM)0,   (LPARAM)0);

  Sleep(0); // Yield processor
  Sleep(g_keyboardSleep);

  return 0;
}

int
WinAction::SendString(CString str,CString& p_log,CString& p_errors,UINT& p_error)
{
  int res = 0;
  p_log.AppendFormat("Sending string of characters: %s\n",str.GetString());

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
        vkey2 = toupper(second.GetAt(0));
      }
      p_log.AppendFormat("Sending one character [%s%s]\n",send.GetString(),second.GetString());
      res += SendInputChar((WORD)vkey,(WORD)vkey2);

      Sleep(0); // Yield processor
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

void
WinAction::MapScreenPositionToMousePosition(int& p_x,int& p_y)
{
  CRect rectWindow;
  GetWindowRect(m_hwnd,&rectWindow);

  // Try to get the work-area from the multi-monitor setup
  MONITORINFO mi;
  mi.cbSize = sizeof(MONITORINFO);
  CPoint point;
  if(p_x || p_y)
  {
    point.x = p_x;
    point.y = p_y;
  }
  else
  {
    point = rectWindow.CenterPoint();
  }
  CRect monitor;
  if(GetMonitorInfo(MonitorFromPoint(point,MONITOR_DEFAULTTONEAREST),&mi))
  {
    monitor = mi.rcWork;
  }
  else
  {
    // Fall back on system parameters (pre-Vista)
    ::SystemParametersInfo(SPI_GETWORKAREA,0,&monitor, 0);
  }

  // Mouse positions are relative to (0,0) -> (65535,65535)
  p_x <<= 16;
  p_x  /= monitor.Width();

  p_y <<= 16;
  p_y  /= monitor.Height();
}

int 
WinAction::SendMouseClick(bool p_dbl,CString& p_log,CString& p_errors,UINT& p_error)
{
  CRect windowRect;
  GetWindowRect(m_hwnd,windowRect);

  int x = atoi(m_argument1);
  int y = atoi(m_argument2);

  p_log.AppendFormat("Mouse coordinates relative to [%s]\n",m_hwnd ? "window" : "desktop");
  p_log.AppendFormat("Sending a %s mouse click to [%d,%d]\n",p_dbl ? "double" : "single",x,y);

  x += (x >= 0) ? windowRect.left : windowRect.right;
  y += (y >= 0) ? windowRect.top  : windowRect.bottom;

  p_log.AppendFormat("Absolute mouse coordinates [%d,%d]\n",x,y);
  MapScreenPositionToMousePosition(x, y);
  p_log.AppendFormat("Absolute mouse position [%d,%d]\n", x, y);

  if(p_dbl)
  {
    int wait = GetDoubleClickTime();
    SendInputMouseClick(x,y);
    Sleep(wait / 2);
  }
  int res = SendInputMouseClick(x,y);
  if(res)
  {
    p_error = res;
    p_errors += "Cannot send mouse coordinates";
  }
  return res;
}

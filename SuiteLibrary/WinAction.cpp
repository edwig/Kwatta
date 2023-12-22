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
// This File   : WinAction.cpp
// What it does: A single action to perform in a Windows UI teststep
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WinAction.h"
#include <ExecuteProcess.h>

// Forward declarations
BOOL IterateTopLevelWindows(HWND p_hwnd, LPARAM p_param);

int
WinAction::PerformAction(CString& p_log, CString& p_errors, UINT& p_error)
{
  switch (m_action)
  {
  case WinUIAction::WA_Start:         return ActionStartProgram(p_log, p_errors, p_error); break;
  case WinUIAction::WA_Close:         return ActionCloseProgram(p_log, p_errors, p_error); break;
  case WinUIAction::WA_AbsolutePos:   break;
  case WinUIAction::WA_RelativePos:   break;
  case WinUIAction::WA_Click:         break;
  case WinUIAction::WA_DblClick:      break;
  case WinUIAction::WA_Char:          break;
  case WinUIAction::WA_String:        break;
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
  p_log.AppendFormat("Top-level window is present. HWND = [%lX]\n",m_hwnd);

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
        p_log.AppendFormat("Next child window found [%s] HWND = [%lX]\n",p_search.m_partialname.GetString(),child);
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

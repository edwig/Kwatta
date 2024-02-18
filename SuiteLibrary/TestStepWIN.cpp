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
// This File   : TestStepSQL.cpp
// What it does: One (1) test step in MS-Windows User-Interface
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TestStepWIN.h"
#include "XMLMessage.h"
#include "StdException.h"
#include "ExtraExtensions.h"
#include <CrackURL.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// DTOR
TestStepWIN::~TestStepWIN()
{
  ResetActions();
}

void
TestStepWIN::ReadFromXML(CString p_filename)
{
  XMLMessage msg;
  TestStep::ReadFromXML(msg, p_filename);
  XMLElement* root = msg.GetRoot();

  // Find type
  XMLElement* typ = msg.FindElement(root,_T("Type"),false);
  if (typ)
  {
    CString type = typ->GetValue();
    if (type.Compare(_T("WIN")))
    {
      throw StdException(_T("QRUN file is not a WIN test: ") + p_filename);
    }
  }
  else
  {
    throw StdException(_T("Missing <Type> in QRUN file: ") + p_filename);
  }
  XMLElement* def = msg.FindElement(_T("Definition"));
  if (!def)
  {
    throw StdException(_T("Missing <Definition> in QRUN file: ") + p_filename);
  }
  // Fill in
  XMLElement* line = msg.FindElement(def,_T("Line"),false);
  while (line)
  {
    WinAction* action = new WinAction();
    // Fill in
    action->m_action    = StringToWinUIAction(msg.GetElement(line,_T("Action")));
    action->m_pattern   = msg.GetElement(line,_T("Pattern"));
    action->m_argument1 = msg.GetElement(line,_T("Argument1"));
    action->m_argument2 = msg.GetElement(line,_T("Argument2"));
    action->m_argument3 = msg.GetElement(line,_T("Argument3"));
    action->m_wait      = msg.GetElement(line,_T("Wait"));
    // Keep
    m_actions.push_back(action);

    // Next line
    line = msg.GetElementSibling(line);
  }
}

bool
TestStepWIN::WriteToXML(CString p_filename)
{
  XMLMessage msg;
  if (!TestStep::WriteToXML(msg,p_filename))
  {
    return false;
  }
  XMLElement* root = msg.GetRoot();

  // This is our SUB-CLASS type
  msg.AddElement(root,_T("Type"),XDT_String,_T("WIN"));

  // FILL IN
  XMLElement* def = msg.AddElement(root,_T("Definition"),XDT_String,_T(""));
  for (auto& action : m_actions)
  {
    XMLElement* act = msg.AddElement(def,_T("Line"),XDT_String,_T(""));

    msg.AddElement(act,_T("Action"),   XDT_String,WinUIActionToString(action->m_action));
    msg.AddElement(act,_T("Pattern"),  XDT_String,action->m_pattern);
    msg.AddElement(act,_T("Argument1"),XDT_String,action->m_argument1);
    msg.AddElement(act,_T("Argument2"),XDT_String,action->m_argument2);
    msg.AddElement(act,_T("Argument3"),XDT_String,action->m_argument3);
    msg.AddElement(act,_T("Wait"),     XDT_String,action->m_wait);
  }
  // Now save it
  return msg.SaveFile(p_filename);
}

// RE-Calculate the effective strings, returning the number of unbound parameters
int
TestStepWIN::EffectiveReplacements(Parameters* p_parameters,bool p_forDisplay)
{
  int unbound = TestStep::EffectiveReplacements(p_parameters,p_forDisplay);

  for(auto& action : m_actions)
  {
    unbound += p_parameters->Replace(action->m_pattern,action->m_effectivePattern,p_forDisplay);

    unbound += p_parameters->Replace(action->m_argument1,action->m_effectiveArgument1,p_forDisplay);
    unbound += p_parameters->Replace(action->m_argument2,action->m_effectiveArgument2,p_forDisplay);
    unbound += p_parameters->Replace(action->m_argument3,action->m_effectiveArgument3,p_forDisplay);
    unbound += p_parameters->Replace(action->m_wait,     action->m_effectiveWait,     p_forDisplay);
  }
  return unbound;
}

// Check our filenames extension
void
TestStepWIN::CheckFilename(CString p_filename)
{
  // Split of only the extension
  TCHAR extension[_MAX_EXT];
  _tsplitpath_s(p_filename,NULL,0,NULL,0,NULL,0,extension,_MAX_EXT);

  // Check that we have the right one
  if (_tcsncicmp(extension, EXTENSION_TESTSTEP_WIN, 5))
  {
    throw StdException(_T("A StepResult XML definition file must be saved as a *.WRUN"));
  }
}

// Reset effective parameter replacements
void
TestStepWIN::ResetEffective()
{
  for (auto& action : m_actions)
  {
    action->m_effectivePattern.Empty();
    action->m_effectiveArgument1.Empty();
    action->m_effectiveArgument2.Empty();
    action->m_effectiveArgument3.Empty();
    action->m_effectiveWait.Empty();
  }
}

// Translate ui-action to-from a string
CString
TestStepWIN::WinUIActionToString(WinUIAction p_action)
{
  CString action;
  switch (p_action)
  {
    case WinUIAction::WA_Start:       action = _T("Start");               break;
    case WinUIAction::WA_Close:       action = _T("Close");               break;
    case WinUIAction::WA_CaretPos:    action = _T("Caret position");      break;
    case WinUIAction::WA_Click:       action = _T("Mouse click");         break;
    case WinUIAction::WA_DblClick:    action = _T("Mouse double click");  break;
    case WinUIAction::WA_RClick:      action = _T("Mouse right click");   break;
    case WinUIAction::WA_Char:        action = _T("Character");           break;
    case WinUIAction::WA_String:      action = _T("String");              break;
    case WinUIAction::WA_Present:     action = _T("Present");             break;
    case WinUIAction::WA_Activate:    action = _T("Activate");            break;
    case WinUIAction::WA_Focus:       action = _T("Focus");               break;
    case WinUIAction::WA_TextArea:    action = _T("Text area");           break;
    case WinUIAction::WA_Nothing:     action = _T("Nothing");             break;
    default:                          break;

  }
  return action;
}

WinUIAction
TestStepWIN::StringToWinUIAction(CString p_action)
{
  if(p_action.CompareNoCase(_T("start"))              == 0) return WinUIAction::WA_Start;
  if(p_action.CompareNoCase(_T("close"))              == 0) return WinUIAction::WA_Close;
  if(p_action.CompareNoCase(_T("present"))            == 0) return WinUIAction::WA_Present;
  if(p_action.CompareNoCase(_T("activate"))           == 0) return WinUIAction::WA_Activate;
  if(p_action.CompareNoCase(_T("focus"))              == 0) return WinUIAction::WA_Focus;
  if(p_action.CompareNoCase(_T("caret position"))     == 0) return WinUIAction::WA_CaretPos;
  if(p_action.CompareNoCase(_T("mouse click"))        == 0) return WinUIAction::WA_Click;
  if(p_action.CompareNoCase(_T("mouse double click")) == 0) return WinUIAction::WA_DblClick;
  if(p_action.CompareNoCase(_T("mouse right click"))  == 0) return WinUIAction::WA_RClick;
  if(p_action.CompareNoCase(_T("character"))          == 0) return WinUIAction::WA_Char;
  if(p_action.CompareNoCase(_T("string"))             == 0) return WinUIAction::WA_String;
  if(p_action.CompareNoCase(_T("text area"))          == 0) return WinUIAction::WA_TextArea;
  if(p_action.CompareNoCase(_T("nothing"))            == 0) return WinUIAction::WA_Nothing;

  return WinUIAction::WA_Nothing;
}

// Reset the list with actions
void
TestStepWIN::ResetActions()
{
  for (auto& action : m_actions)
  {
    delete action;
  }
  m_actions.clear();
}

WinActionList&
TestStepWIN::GetActions()
{
  return m_actions;
}

void
TestStepWIN::SetAction(WinAction* p_action)
{
  m_actions.push_back(p_action);
}

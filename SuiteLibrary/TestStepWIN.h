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
// This File   : TestStepWIN.h
// What it does: One (1) test step for a MS-Windows User-Interface test
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "TestStep.h"
#include "WinAction.h"
#include <vector>

using WinActionList = std::vector<WinAction*>;

class TestStepWIN : public TestStep
{
public:
  TestStepWIN() = default;
  virtual ~TestStepWIN();

  virtual void    ReadFromXML(CString p_filename) override;
  virtual bool    WriteToXML (CString p_filename) override;
  // RE-Calculate the effective strings, returning the number of unbound parameters
  virtual int     EffectiveReplacements(Parameters* p_parameters, bool p_forDisplay);
  // Check our filenames extension
  virtual void    CheckFilename(CString p_filename);
  // Reset effective parameter replacements
  virtual void    ResetEffective();
  // Reset the list with actions
  virtual void    ResetActions();

  // Translate ui-action to-from a string
  CString         WinUIActionToString(WinUIAction p_action);
  WinUIAction     StringToWinUIAction(CString p_action);

  // GETTERS
  WinActionList&  GetActions();

  // SETTERS
  void            SetAction(WinAction* p_action);

private:
  // All actions to perform
  WinActionList   m_actions;
};

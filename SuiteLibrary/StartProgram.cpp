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
// This File   : StartProgram.cpp
// What it does: Starting a program
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Stdafx.h"
#include "StartProgram.h"
#include "GetExePath.h"
#include <io.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

int
StartProgram(CString p_program
            ,CString p_arguments
            ,bool    p_currentdir
            ,bool    p_waitForIdle
            ,bool    p_waitForExit
            ,bool    p_show      /*= false*/
            ,PDWORD  p_processID /*= nullptr*/)
{
  if(p_currentdir)
  {
    CString pad = GetExePath();
    p_program = pad + p_program;

    if(_access(p_program,04) == -1)
    {
      // We do not find a program
      CString error;
      error.Format("Cannot find the program: %s",p_program.GetString());
      StyleMessageBox(NULL,error,"ERROR",MB_OK|MB_ICONERROR);
      return -3;
    }
  }
  PROCESS_INFORMATION	processInfo;
  ZeroMemory(&processInfo, sizeof(processInfo));
  STARTUPINFO	startupInfo;
  ZeroMemory(&startupInfo, sizeof(startupInfo));
  startupInfo.cb = sizeof(startupInfo);
  startupInfo.dwFlags = STARTF_USESHOWWINDOW;
  startupInfo.wShowWindow = p_show ? SW_SHOW : SW_HIDE;

  // Create a command line 
  char commandLine[MAX_COMMANDLINE];
  sprintf_s(commandLine,MAX_COMMANDLINE,"\"%s\" %s",p_program.GetString(),p_arguments.GetString());
  BOOL res = CreateProcess((LPCTSTR)NULL          // Program to start or NULL
                          ,(LPSTR)  commandLine   // Command line with arguments only
                          ,NULL         				  // Security
                          ,NULL  				          // ThreadAttributes
                          ,FALSE                  // Inherit handles
                          ,NORMAL_PRIORITY_CLASS  // Priority
                          ,NULL 				          // Environment
                          ,NULL            				// Current dir
                          ,&startupInfo           // Startupinfo
                          ,&processInfo		        // procesinfo
                          );
  DWORD exitCode = 0;
  if(res)
  {
    // Wait until the program can process the input
    if(p_waitForIdle)
    {
      WaitForInputIdle(processInfo.hProcess,60000);
    }
    if(p_waitForExit)
    {
      if(WaitForSingleObject(processInfo.hProcess,90000) == WAIT_OBJECT_0)
      {
        GetExitCodeProcess(processInfo.hProcess,&exitCode);
      }
    }
    else if(p_processID)
    {
      // Only if we do not wait for the exit, but we want the proces ID
      *p_processID = processInfo.dwProcessId;
    }
    ::CloseHandle(processInfo.hProcess);
    ::CloseHandle(processInfo.hThread);
  }
  else
  {
    // Cannot start! Rights?
    CString error;
    error.Format("Cannot start the program: %s",p_program.GetString());
    StyleMessageBox(NULL,error,"ERROR",MB_OK|MB_ICONERROR);
    exitCode = (DWORD)-3;
  }
  return exitCode;
}

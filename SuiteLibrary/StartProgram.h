#pragma once

// Max length of command line in NT technology
#define MAX_COMMANDLINE 8192

int
StartProgram(CString p_program
            ,CString p_arguments
            ,bool    p_currentdir
            ,bool    p_waitForIdle
            ,bool    p_waitForExit
            ,bool    p_show      = false
            ,PDWORD  p_processID = nullptr);


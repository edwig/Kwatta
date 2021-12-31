#pragma once

//////////////////////////////////////////////////////////////////////////
//
// Extra MS-Windows Messages
//
// Used for communication between the editors
//
//////////////////////////////////////////////////////////////////////////

#define WM_READYTEST (WM_USER + 100 + 1)  // A TestStep completed
#define WM_READYVALI (WM_USER + 100 + 2)  // A Validation completed
#define WM_READYSET  (WM_USER + 100 + 3)  // A Test SET completed


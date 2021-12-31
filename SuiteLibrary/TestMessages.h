#pragma once

#define WM_TEST_TESTNAME   (WM_USER+1)    // WParam = char* testname
#define WM_TEST_STEPNAME   (WM_USER+2)    // WParam = char* stepname
#define WM_TEST_TOTAL      (WM_USER+3)    // WParam = percent
#define WM_TEST_PROGRESS   (WM_USER+4)    // WParam = percent
#define WM_TEST_UPDATE     (WM_USER+5)    // No params
#define WM_TEST_ENDING     (WM_USER+6)    // No params

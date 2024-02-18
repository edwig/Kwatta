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
// This File   : VirtualKeys.cpp
// What it does: Translation of a formal VIRTUAL KEY definition to the correct scancode
//               Can send VIRTUAL mouse movements and does a button click (left / right)
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VirtualKeys.h"
#include <WinUser.h>

// Definition structure
typedef struct _vkey
{
  TCHAR* m_vkey_name;
  int   m_vkey_code;
}
VirtualKey;

// Sleeping after each simulated keyboard press
int g_keyboardSleep = 20;

// All names and virtual codes
static VirtualKey vkeys[] =
{
   { _T("BACK"),        VK_BACK           }
  ,{ _T("TAB"),         VK_TAB            }
  ,{ _T("CLEAR"),       VK_CLEAR          }
  ,{ _T("RETURN"),      VK_RETURN         }
  ,{ _T("ENTER"),       VK_RETURN         }
  ,{ _T("SHIFT"),       VK_SHIFT          }
  ,{ _T("CTRL"),        VK_CONTROL        }
  ,{ _T("ALT"),         VK_MENU           }
  ,{ _T("PAUSE"),       VK_PAUSE          }
  ,{ _T("CAPITAL"),     VK_CAPITAL        }
  ,{ _T("KANA"),        VK_KANA           }
  ,{ _T("HANGEUL"),     VK_HANGEUL        }
  ,{ _T("HANGUL"),      VK_HANGUL         }
  ,{ _T("IME_ON"),      VK_IME_ON         }
  ,{ _T("JUNJA"),       VK_JUNJA          }
  ,{ _T("FINAL"),       VK_FINAL          }
  ,{ _T("HANJA"),       VK_HANJA          }
  ,{ _T("KANJI"),       VK_KANJI          }
  ,{ _T("IME_OFF"),     VK_IME_OFF        }
  ,{ _T("ESCAPE"),      VK_ESCAPE         }
  ,{ _T("CONVERT"),     VK_CONVERT        }
  ,{ _T("NONCONVERT"),  VK_NONCONVERT     }
  ,{ _T("ACCEPT"),      VK_ACCEPT         }
  ,{ _T("MODECHANGE"),  VK_MODECHANGE     }
  ,{ _T("SPACE"),       VK_SPACE          }
  ,{ _T("PGUP"),        VK_PRIOR          }
  ,{ _T("PRIOR"),       VK_PRIOR          }
  ,{ _T("NEXT"),        VK_NEXT           }
  ,{ _T("PGDOWN"),      VK_NEXT           }
  ,{ _T("END"),         VK_END            }
  ,{ _T("HOME"),        VK_HOME           }
  ,{ _T("LEFT"),        VK_LEFT           }
  ,{ _T("UP"),          VK_UP             }
  ,{ _T("RIGHT"),       VK_RIGHT          }
  ,{ _T("DOWN"),        VK_DOWN           }
  ,{ _T("SELECT"),      VK_SELECT         }
  ,{ _T("PRINT"),       VK_PRINT          }
  ,{ _T("EXECUTE"),     VK_EXECUTE        }
  ,{ _T("SNAPSHOT"),    VK_SNAPSHOT       }
  ,{ _T("INSERT"),      VK_INSERT         }
  ,{ _T("DELETE"),      VK_DELETE         }
  ,{ _T("DEL"),         VK_DELETE         }
  ,{ _T("HELP"),        VK_HELP           }
  ,{ _T("LWIN"),        VK_LWIN           }
  ,{ _T("RWIN"),        VK_RWIN           }
  ,{ _T("WIN"),         VK_LWIN           }
  ,{ _T("APPS"),        VK_APPS           }
  ,{ _T("SLEEP"),       VK_SLEEP          }
  ,{ _T("NUMPAD0"),     VK_NUMPAD0        }
  ,{ _T("NUMPAD1"),     VK_NUMPAD1        }
  ,{ _T("NUMPAD2"),     VK_NUMPAD2        }
  ,{ _T("NUMPAD3"),     VK_NUMPAD3        }
  ,{ _T("NUMPAD4"),     VK_NUMPAD4        }
  ,{ _T("NUMPAD5"),     VK_NUMPAD5        }
  ,{ _T("NUMPAD6"),     VK_NUMPAD6        }
  ,{ _T("NUMPAD7"),     VK_NUMPAD7        }
  ,{ _T("NUMPAD8"),     VK_NUMPAD8        }
  ,{ _T("NUMPAD9"),     VK_NUMPAD9        }
  ,{ _T("MULTIPLY"),    VK_MULTIPLY       }
  ,{ _T("ADD"),         VK_ADD            }
  ,{ _T("SEPARATOR"),   VK_SEPARATOR      }
  ,{ _T("SUBTRACT"),    VK_SUBTRACT       }
  ,{ _T("DECIMAL"),     VK_DECIMAL        }
  ,{ _T("DIVIDE"),      VK_DIVIDE         }
  ,{ _T("F1"),          VK_F1             }
  ,{ _T("F2"),          VK_F2             }
  ,{ _T("F3"),          VK_F3             }
  ,{ _T("F4"),          VK_F4             }
  ,{ _T("F5"),          VK_F5             }
  ,{ _T("F6"),          VK_F6             }
  ,{ _T("F7"),          VK_F7             }
  ,{ _T("F8"),          VK_F8             }
  ,{ _T("F9"),          VK_F9             }
  ,{ _T("F10"),         VK_F10            }
  ,{ _T("F11"),         VK_F11            }
  ,{ _T("F12"),         VK_F12            }
  ,{ _T("F13"),         VK_F13            }
  ,{ _T("F14"),         VK_F14            }
  ,{ _T("F15"),         VK_F15            }
  ,{ _T("F16"),         VK_F16            }
  ,{ _T("F17"),         VK_F17            }
  ,{ _T("F18"),         VK_F18            }
  ,{ _T("F19"),         VK_F19            }
  ,{ _T("F20"),         VK_F20            }
  ,{ _T("F21"),         VK_F21            }
  ,{ _T("F22"),         VK_F22            }
  ,{ _T("F23"),         VK_F23            }
  ,{ _T("F24"),         VK_F24            }
  ,{ _T("NUMLOCK"),     VK_NUMLOCK        }
  ,{ _T("SCROLL"),      VK_SCROLL         }
  ,{ _T("FJ_JISHO"),    VK_OEM_FJ_JISHO   }
  ,{ _T("FJ_MASSHOU"),  VK_OEM_FJ_MASSHOU }
  ,{ _T("FJ_TOUROKU"),  VK_OEM_FJ_TOUROKU }
  ,{ _T("FJ_LOYA"),     VK_OEM_FJ_LOYA    }
  ,{ _T("FJ_ROYA"),     VK_OEM_FJ_ROYA    }
  ,{ _T("LSHIFT"),      VK_LSHIFT         }
  ,{ _T("RSHIFT"),      VK_RSHIFT         }
  ,{ _T("LCTRL"),       VK_LCONTROL       }
  ,{ _T("RCTRL"),       VK_RCONTROL       }
  ,{ _T("LALT"),        VK_LMENU          }
  ,{ _T("RALT"),        VK_RMENU          }
};

// Performance mapping to be used runtime
using  VKeyMap = std::map<CString,int>;
static VKeyMap g_vkeymap;

// Init the key map
static void InitVirtualKeymap()
{
  // One (1) time init
  if(!g_vkeymap.empty())
  {
    return;
  }
  VirtualKey* keys = vkeys;
  for(int index = 0;index < (sizeof(vkeys) / sizeof(VirtualKey));++index)
  {
    g_vkeymap[keys->m_vkey_name] = keys->m_vkey_code;
    ++keys;
  }
}

// Translate name to virtual-key code
// e.g "F1" -> VK_F1
int GetVirtualKeyCode(CString p_name)
{
  int vkey = -1;

  p_name.MakeUpper();
  InitVirtualKeymap();
  VKeyMap::iterator it = g_vkeymap.find(p_name);
  if(it != g_vkeymap.end())
  {
    vkey = it->second;
  }
  return vkey;
}

// https://learn.microsoft.com/en-us/windows/win32/inputdev/wm-char
// https://learn.microsoft.com/en-us/windows/win32/inputdev/about-keyboard-input#keystroke-message-flags

// Sending a keyboard character
int
SendInputKey(WORD p_char1)
{
  INPUT inputs[1] = {};
  ZeroMemory(inputs,sizeof(inputs));

  // Key / Scancode for WM_KEYDOWN
  inputs[0].type       = INPUT_KEYBOARD;
  inputs[0].ki.wVk     = p_char1;
  inputs[0].ki.wScan   = (WORD)MapVirtualKey(p_char1,MAPVK_VK_TO_VSC);
  inputs[0].ki.dwFlags = 0;

  UINT uSent = SendInput(1,inputs,sizeof(INPUT));
  if(uSent != ARRAYSIZE(inputs))
  {
    return HRESULT_FROM_WIN32(GetLastError());
  }

  // Keyboard works better if timestamps are slightly apart!!
  Sleep(rand() % 15 + 5);

  // Key / Scancode for WM_KEYUP
  inputs[0].type       = INPUT_KEYBOARD;
  inputs[0].ki.wVk     = p_char1;
  inputs[0].ki.wScan   = (WORD)MapVirtualKey(p_char1,MAPVK_VK_TO_VSC);
  inputs[0].ki.dwFlags = KEYEVENTF_KEYUP;

  uSent = SendInput(1,inputs,sizeof(INPUT));
  if(uSent != ARRAYSIZE(inputs))
  {
    return HRESULT_FROM_WIN32(GetLastError());
  }

  // Yield processor
  // Sleep after EACH character
  Sleep(g_keyboardSleep);

  return 0;
}

// Sending two keys. First most likely a virtual key
int
SendInputKey(WORD p_char1,WORD p_char2)
{
  INPUT inputs[1] = {};
  ZeroMemory(inputs,sizeof(inputs));

  // Key / Scancode for first prefix key
  inputs[0].type       = INPUT_KEYBOARD;
  inputs[0].ki.wVk     = p_char1;
  inputs[0].ki.wScan   = (WORD)MapVirtualKey(p_char1,MAPVK_VK_TO_VSC);
  inputs[0].ki.dwFlags = 0;

  UINT uSent = SendInput(1,inputs,sizeof(INPUT));
  if(uSent != ARRAYSIZE(inputs))
  {
    return HRESULT_FROM_WIN32(GetLastError());
  }

  // Keyboard works better if timestamps are slightly apart!!
  Sleep(rand() % 15 + 5);

  // Key / Scancode for second key (after the prefix) for the WM_KEYDOWN
  inputs[0].type       = INPUT_KEYBOARD;
  inputs[0].ki.wVk     = p_char2;
  inputs[0].ki.wScan   = (WORD)MapVirtualKey(p_char2,MAPVK_VK_TO_VSC);
  inputs[0].ki.dwFlags = 0;

  uSent = SendInput(1,inputs,sizeof(INPUT));
  if(uSent != ARRAYSIZE(inputs))
  {
    return HRESULT_FROM_WIN32(GetLastError());
  }

  // Keyboard works better if timestamps are slightly apart!!
  Sleep(rand() % 15 + 5);

  // Key / Scancode for the SECOND char for the WM_KEYUP
  inputs[0].type       = INPUT_KEYBOARD;
  inputs[0].ki.wVk     = p_char2;
  inputs[0].ki.wScan   = (WORD)MapVirtualKey(p_char2,MAPVK_VK_TO_VSC);
  inputs[0].ki.dwFlags = KEYEVENTF_KEYUP;

  uSent = SendInput(1,inputs,sizeof(INPUT));
  if(uSent != ARRAYSIZE(inputs))
  {
    return HRESULT_FROM_WIN32(GetLastError());
  }

  // Keyboard works better if timestamps are slightly apart!!
  Sleep(rand() % 15 + 5);

  // Key / Scancode for the FIRST char for the WM_KEYUP
  inputs[0].type = INPUT_KEYBOARD;
  inputs[0].ki.wVk     = p_char1;
  inputs[0].ki.wScan   = (WORD)MapVirtualKey(p_char1,MAPVK_VK_TO_VSC);
  inputs[0].ki.dwFlags = KEYEVENTF_KEYUP;

  uSent = SendInput(1,inputs,sizeof(INPUT));
  if(uSent != ARRAYSIZE(inputs))
  {
    return HRESULT_FROM_WIN32(GetLastError());
  }

  // Yield processor
  // Sleep after EACH character
  Sleep(g_keyboardSleep);
  return 0;
}

void
MapScreenPositionToMousePosition(int& p_x,int& p_y,HWND p_hwnd /*=NULL*/)
{
  CRect rectWindow;
  GetWindowRect(p_hwnd,&rectWindow);

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

// Sending virtual mouse click to the system
// Moves the mouse to this position and then emits a button click
int
SendInputMouseClick(int x,int y,bool p_left /*=true*/)
{
  INPUT inputs[1] = {};
  ZeroMemory(inputs,sizeof(inputs));

  inputs[0].type  = INPUT_MOUSE;
  inputs[0].mi.dx = x;
  inputs[0].mi.dy = y;
  inputs[0].mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;

  UINT uSent = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
  if(uSent != ARRAYSIZE(inputs))
  {
    return HRESULT_FROM_WIN32(GetLastError());
  }

  Sleep(rand() % 15 + 5);

  inputs[0].type = INPUT_MOUSE;
  inputs[0].mi.dx = x;
  inputs[0].mi.dy = y;
  inputs[0].mi.dwFlags = p_left ? MOUSEEVENTF_LEFTDOWN : MOUSEEVENTF_RIGHTDOWN;

  uSent = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
  if(uSent != ARRAYSIZE(inputs))
  {
    return HRESULT_FROM_WIN32(GetLastError());
  }

  Sleep(rand() % 15 + 5);

  inputs[0].type  = INPUT_MOUSE;
  inputs[0].mi.dx = x;
  inputs[0].mi.dy = y;
  inputs[0].mi.dwFlags = p_left ? MOUSEEVENTF_LEFTUP : MOUSEEVENTF_RIGHTUP;

  uSent = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
  if(uSent != ARRAYSIZE(inputs))
  {
    return HRESULT_FROM_WIN32(GetLastError());
  }
  return 0;
}

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
  char* m_vkey_name;
  int   m_vkey_code;
}
VirtualKey;

// All names and virtual codes
static VirtualKey vkeys[] =
{
   { "BACK",        VK_BACK           }
  ,{ "TAB",         VK_TAB            }
  ,{ "CLEAR",       VK_CLEAR          }
  ,{ "RETURN",      VK_RETURN         }
  ,{ "ENTER",       VK_RETURN         }
  ,{ "SHIFT",       VK_SHIFT          }
  ,{ "CTRL",        VK_CONTROL        }
  ,{ "ALT",         VK_MENU           }
  ,{ "PAUSE",       VK_PAUSE          }
  ,{ "CAPITAL",     VK_CAPITAL        }
  ,{ "KANA",        VK_KANA           }
  ,{ "HANGEUL",     VK_HANGEUL        }
  ,{ "HANGUL",      VK_HANGUL         }
  ,{ "IME_ON",      VK_IME_ON         }
  ,{ "JUNJA",       VK_JUNJA          }
  ,{ "FINAL",       VK_FINAL          }
  ,{ "HANJA",       VK_HANJA          }
  ,{ "KANJI",       VK_KANJI          }
  ,{ "IME_OFF",     VK_IME_OFF        }
  ,{ "ESCAPE",      VK_ESCAPE         }
  ,{ "CONVERT",     VK_CONVERT        }
  ,{ "NONCONVERT",  VK_NONCONVERT     }
  ,{ "ACCEPT",      VK_ACCEPT         }
  ,{ "MODECHANGE",  VK_MODECHANGE     }
  ,{ "SPACE",       VK_SPACE          }
  ,{ "PGUP",        VK_PRIOR          }
  ,{ "PRIOR",       VK_PRIOR          }
  ,{ "NEXT",        VK_NEXT           }
  ,{ "PGDOWN",      VK_NEXT           }
  ,{ "END",         VK_END            }
  ,{ "HOME",        VK_HOME           }
  ,{ "LEFT",        VK_LEFT           }
  ,{ "UP",          VK_UP             }
  ,{ "RIGHT",       VK_RIGHT          }
  ,{ "DOWN",        VK_DOWN           }
  ,{ "SELECT",      VK_SELECT         }
  ,{ "PRINT",       VK_PRINT          }
  ,{ "EXECUTE",     VK_EXECUTE        }
  ,{ "SNAPSHOT",    VK_SNAPSHOT       }
  ,{ "INSERT",      VK_INSERT         }
  ,{ "DELETE",      VK_DELETE         }
  ,{ "DEL",         VK_DELETE         }
  ,{ "HELP",        VK_HELP           }
  ,{ "LWIN",        VK_LWIN           }
  ,{ "RWIN",        VK_RWIN           }
  ,{ "WIN",         VK_LWIN           }
  ,{ "APPS",        VK_APPS           }
  ,{ "SLEEP",       VK_SLEEP          }
  ,{ "NUMPAD0",     VK_NUMPAD0        }
  ,{ "NUMPAD1",     VK_NUMPAD1        }
  ,{ "NUMPAD2",     VK_NUMPAD2        }
  ,{ "NUMPAD3",     VK_NUMPAD3        }
  ,{ "NUMPAD4",     VK_NUMPAD4        }
  ,{ "NUMPAD5",     VK_NUMPAD5        }
  ,{ "NUMPAD6",     VK_NUMPAD6        }
  ,{ "NUMPAD7",     VK_NUMPAD7        }
  ,{ "NUMPAD8",     VK_NUMPAD8        }
  ,{ "NUMPAD9",     VK_NUMPAD9        }
  ,{ "MULTIPLY",    VK_MULTIPLY       }
  ,{ "ADD",         VK_ADD            }
  ,{ "SEPARATOR",   VK_SEPARATOR      }
  ,{ "SUBTRACT",    VK_SUBTRACT       }
  ,{ "DECIMAL",     VK_DECIMAL        }
  ,{ "DIVIDE",      VK_DIVIDE         }
  ,{ "F1",          VK_F1             }
  ,{ "F2",          VK_F2             }
  ,{ "F3",          VK_F3             }
  ,{ "F4",          VK_F4             }
  ,{ "F5",          VK_F5             }
  ,{ "F6",          VK_F6             }
  ,{ "F7",          VK_F7             }
  ,{ "F8",          VK_F8             }
  ,{ "F9",          VK_F9             }
  ,{ "F10",         VK_F10            }
  ,{ "F11",         VK_F11            }
  ,{ "F12",         VK_F12            }
  ,{ "F13",         VK_F13            }
  ,{ "F14",         VK_F14            }
  ,{ "F15",         VK_F15            }
  ,{ "F16",         VK_F16            }
  ,{ "F17",         VK_F17            }
  ,{ "F18",         VK_F18            }
  ,{ "F19",         VK_F19            }
  ,{ "F20",         VK_F20            }
  ,{ "F21",         VK_F21            }
  ,{ "F22",         VK_F22            }
  ,{ "F23",         VK_F23            }
  ,{ "F24",         VK_F24            }
  ,{ "NUMLOCK",     VK_NUMLOCK        }
  ,{ "SCROLL",      VK_SCROLL         }
  ,{ "FJ_JISHO",    VK_OEM_FJ_JISHO   }
  ,{ "FJ_MASSHOU",  VK_OEM_FJ_MASSHOU }
  ,{ "FJ_TOUROKU",  VK_OEM_FJ_TOUROKU }
  ,{ "FJ_LOYA",     VK_OEM_FJ_LOYA    }
  ,{ "FJ_ROYA",     VK_OEM_FJ_ROYA    }
  ,{ "LSHIFT",      VK_LSHIFT         }
  ,{ "RSHIFT",      VK_RSHIFT         }
  ,{ "LCTRL",       VK_LCONTROL       }
  ,{ "RCTRL",       VK_RCONTROL       }
  ,{ "LALT",        VK_LMENU          }
  ,{ "RALT",        VK_RMENU          }
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

// Translate name to virtual-keycode
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

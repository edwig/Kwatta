#include "stdafx.h"
#include "GetExePath.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

static char g_staticAddress;

CString GetExePath()
{
  char buffer[_MAX_PATH + 1];

  // Getting the module handle, if any
  // If it fails, the process names will be retrieved
  // Thus we get the *.DLL handle in IIS instead of a
  // %systemdrive\system32\inetsrv\w3wp.exe path
  HMODULE hmodule = NULL;
  GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
                    GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT
                   ,static_cast<LPCTSTR>(&g_staticAddress)
                   ,&hmodule);

  // Retrieve the path
  GetModuleFileName(hmodule, buffer, _MAX_PATH);
  CString applicatiePlusPad = buffer;

  int slashPositie = applicatiePlusPad.ReverseFind('\\');
  if (slashPositie == 0)
  {
    return "";
  }
  return applicatiePlusPad.Left(slashPositie + 1);
}

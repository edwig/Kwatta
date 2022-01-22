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
// This File   : SuiteLibrary.h
// What it does: Auto linking to the internal library
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#define KWATTA_VERSION_MAJOR  1
#define KWATTA_VERSION_MINOR  0
#define KWATTA_VERSION_SP     5
#define KWATTA_VERSION_BUILD  101

#define KWATTA                "Kwatta"
#define KWATTA_VERSION        "1.0.5"
#define KWATTA_YEAR           "2022"

// Used for environment variables
#define KWATTA_PASSWORD       "KWATTA_PASSWORD"

// Used for global objects
#define GLOBAL_COLOR          RGB(200,191,231)

// Selecting the right library to link with automatically
// So we do not need to worry about which library to use in the linker settings
#if defined _M_IX86
#define KWATTA_PLATFORM "x86"
#else
#define KWATTA_PLATFORM "x64"
#endif

#if defined _DEBUG
#define KWATTA_CONFIGURATION "D"
#else
#define KWATTA_CONFIGURATION "R"
#endif 

#ifndef KWATTA_NOAUTOLINK
#pragma comment(lib,"SuiteLibrary_"                        KWATTA_PLATFORM KWATTA_CONFIGURATION ".lib")
#pragma message("Automatically linking with SuiteLibrary_" KWATTA_PLATFORM KWATTA_CONFIGURATION ".lib")
#endif 

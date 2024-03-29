﻿///////////////////////////////////////////////////////////////////////////////////////////////////////
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
// This File   : VirtualKeys.h
// What it does: Translation of a formal VIRTUAL KEY definition to the correct scancode
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

// Sleeping after each simulated keyboard press
extern int g_keyboardSleep;

// Translate name to virtual-keycode. e.g "F1" -> VK_F1
int  GetVirtualKeyCode(CString p_name);
// Calculate mouse coordinates from screen pixel coordinates
void MapScreenPositionToMousePosition(int& p_x,int& p_y,HWND p_hwnd = NULL);

// Sending a keyboard character
int  SendInputKey(WORD p_char1);
// Sending two keys. First most likely a virtual key
int  SendInputKey(WORD p_char1,WORD p_char2);
// Sending virtual mouse click to the system
int  SendInputMouseClick(int x,int y,bool p_left = true);

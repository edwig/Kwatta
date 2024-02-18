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
// This File   : StreamBuffer.h
// What it does: In- Output buffer for a command line test step
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

class StreamBuffer
{
public:
  StreamBuffer() = default;
 ~StreamBuffer();

  void Reset();
  void SetBuffer(_TUCHAR*  p_buffer,unsigned int  p_length);
  void AddBuffer(_TUCHAR*  p_buffer,unsigned int  p_length);
  void GetBuffer(_TUCHAR*& p_buffer,unsigned int& p_length);
  void SetUse(bool p_use);
  bool GetUse() { return m_use; };

//  CString AsCDataNode();

private:
  bool         m_use    { false   };
  _TUCHAR*     m_buffer { nullptr };
  unsigned int m_length { 0       };
};
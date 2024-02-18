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
// This File   : StreamBuffer.cpp
// What it does: In- Output buffer for a command line test step
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StreamBuffer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

StreamBuffer::~StreamBuffer()
{
  if(m_buffer)
  {
    delete [] m_buffer;
    m_buffer = nullptr;
  }
}

void
StreamBuffer::Reset()
{
  if(m_buffer || m_length)
  {
    delete [] m_buffer;
    m_buffer = nullptr;
    m_length = 0;
    m_use    = false;
  }
}

// Make a copy of the buffer
void 
StreamBuffer::SetBuffer(_TUCHAR* p_buffer, unsigned int p_length)
{
  Reset();

  if(p_length)
  {
    m_buffer = new _TUCHAR[p_length + 1];
    memcpy(m_buffer,p_buffer,p_length * sizeof(_TUCHAR));
    m_buffer[p_length] = 0;
    m_length = p_length;

    m_use = true;
  }
}

void 
StreamBuffer::AddBuffer(_TUCHAR* p_buffer,unsigned int p_length)
{
  if(m_length == 0)
  {
    SetBuffer(p_buffer, p_length);
    return;
  }

  // Add to the buffer
  _TUCHAR* buffer = new _TUCHAR[m_length + p_length + 1];
  memcpy(buffer,m_buffer,m_length * sizeof(_TUCHAR));
  memcpy(&buffer[m_length],p_buffer,p_length * sizeof(_TUCHAR));
  m_length += p_length;
  buffer[m_length] = 0;

  delete[] m_buffer;
  m_buffer = buffer;
}

// Getting the buffer elements
void 
StreamBuffer::GetBuffer(_TUCHAR*& p_buffer,unsigned int& p_length)
{
  p_buffer = m_buffer;
  p_length = m_length;
}

// Setting the used status
void 
StreamBuffer::SetUse(bool p_use)
{
  if(p_use == false)
  {
    Reset();
  }
  m_use = p_use;
}


// CString 
// StreamBuffer::AsCDataNode()
// {
//   CString value;
// 
//   for(unsigned int ind = 0; ind < m_length; ++ind)
//   {
//     unsigned char ch = m_buffer[ind];
//     if(ch >= 32 && ch <= 127)
//     {
//       switch (ch)
//       {
//         case '&': value += "&amp;";  break;
//         case '<': value += "&lt;";   break;
//         case '>': value += "&gt;";   break;
//         case '\'':value += "&apos;"; break;
//         case '\"':value += "&quot;"; break;
//         default:  value += ch;      break;
//       }
//     }
//     else
//     {
//       value.AppendFormat("&#%03d;",(int)ch);
//     }
//   }
//   return value;
// }

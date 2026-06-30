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
// This File   : StepResultIN.h
// What it does: Result of one (1) internet step in a test set
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "StepResult.h"
#include "TestStepNET.h"

class StepResultNET : public StepResult
{
public:
  StepResultNET() = default;
  virtual ~StepResultNET() = default;

  // Interface with the file system
  virtual void ReadFromXML(XString p_filename);  // Throws in case of an error
  virtual bool WriteToXML (XString p_filename);
  virtual void CheckFilename(XString p_filename);
  virtual void ResetEffective();
  virtual void Reset();

  // GETTERS
  int         GetStatus()         { return m_status;        }
  int         GetOSError()        { return m_osError;       }
  XString     GetOSErrorString()  { return m_osErrorString; }
  XString     GetBody()           { return m_body;          }
  UrlHeaders& GetHeaders()        { return m_headers;       }
  XString     GetBearerToken()    { return m_bearerToken;   }
  XString     GetHeader(XString p_header);
  XString     GetRawResponse();

  // SETTERS
  void        SetStatus(int p_status)  { m_status  = p_status;      }
  void        SetOSError(int p_error)  { m_osError = p_error;       }
  void        SetHeader(INPair& pair)  { m_headers.push_back(pair); }
  void        SetBody(XString p_body)  { m_body    = p_body;        }
  void        SetBearerToken(XString p_token)   { m_bearerToken   = p_token; }
  void        SetOSErrorString(XString p_error) { m_osErrorString = p_error; }

protected:
  int         m_status { 0 };
  int         m_osError{ 0 };
  XString     m_osErrorString;
  UrlHeaders  m_headers;
  XString     m_body;
  XString     m_bearerToken;
  XString     m_bodyFile;
};


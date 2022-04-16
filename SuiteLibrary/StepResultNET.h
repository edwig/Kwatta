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
  virtual void ReadFromXML(CString p_filename);  // Throws in case of an error
  virtual bool WriteToXML (CString p_filename);
  virtual void CheckFilename(CString p_filename);
  virtual void ResetEffective();

  // GETTERS
  int         GetStatus()         { return m_status;        }
  int         GetOSError()        { return m_osError;       }
  CString     GetOSErrorString()  { return m_osErrorString; }
  CString     GetBody()           { return m_body;          }
  UrlHeaders& GetHeaders()        { return m_headers;       }
  CString     GetBearerToken()    { return m_bearerToken;   }
  CString     GetHeader(CString p_header);
  CString     GetRawResponse();

  // SETTERS
  void        SetStatus(int p_status)  { m_status  = p_status;      }
  void        SetOSError(int p_error)  { m_osError = p_error;       }
  void        SetHeader(INPair& pair)  { m_headers.push_back(pair); }
  void        SetBody(CString p_body)  { m_body    = p_body;        }
  void        SetBearerToken(CString p_token)   { m_bearerToken   = p_token; }
  void        SetOSErrorString(CString p_error) { m_osErrorString = p_error; }

protected:
  int         m_status { 0 };
  int         m_osError{ 0 };
  CString     m_osErrorString;
  UrlHeaders  m_headers;
  CString     m_body;
  CString     m_bearerToken;
};


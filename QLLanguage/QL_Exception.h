//////////////////////////////////////////////////////////////////////////
//
// QL Language exception
// ir. W.E. Huisman (c) 2018
//
//////////////////////////////////////////////////////////////////////////

#pragma once

const int EXCEPTION_BY_ERROR = 2;

class QLException
{
public:
  QLException(TCHAR* p_message);
  QLException(TCHAR* p_message, int p_code);
  QLException(XString p_message,int p_code);
 ~QLException();

  // Get the message
  XString GetMessage()  { return m_message; };
  // Get the code
  int     GetCode()     { return m_code; };
  // Get the error
  XString GetErrorMessage();
private:
  int     m_code;
  XString m_message;
};
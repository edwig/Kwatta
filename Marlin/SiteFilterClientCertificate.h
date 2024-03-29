/////////////////////////////////////////////////////////////////////////////////
//
// SourceFile: SiteFilterClientCertificate.h
//
// Marlin Server: Internet server/client
// 
// Copyright (c) 2014-2024 ir. W.E. Huisman
// All rights reserved
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
#pragma once
#include "SiteFilter.h"

class SiteFilterClientCertificate : public SiteFilter
{
public:
  explicit SiteFilterClientCertificate(unsigned p_priority,XString p_name);
  virtual ~SiteFilterClientCertificate();

  // Handle the filter
  virtual bool Handle(HTTPMessage* p_message) override;
  virtual void SetSite(HTTPSite* p_site) override;

  // MANDATORY: Set Client certificate name and/or thumbprint to expect
  void    SetClientCertificate(LPCTSTR p_name,LPCTSTR p_thumbprint);
  // OPTIONAL: Set Request client certificate: used to deactivate temporarily
  void    SetRequestCertificate(bool p_request);

  bool    GetRequestCertificate()           { return m_request;       };
  XString GetClientCertificateName()        { return m_certName;      };
  XString GetClientCertificateThumbprint()  { return m_certThumbprint;};

private:
  void    FreeCertificate();
  DWORD   ReceiveClientCertificate(HTTPMessage* p_message);
  bool    CheckCertificateStatus();
  bool    CheckClientCertificate();

  bool    m_request { true };
  XString m_certName;
  XString m_certThumbprint;
};

inline void
SiteFilterClientCertificate::SetRequestCertificate(bool p_request)
{
  m_request = p_request;
}


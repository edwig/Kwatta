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
// This File   : TestStepSQL.h
// What it does: One (1) test step for a SQL statement. Subclass for a HTTP call
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "TestStep.h"
#include <vector>

class TestStepSQL : public TestStep
{
public:
  TestStepSQL() = default;
  virtual ~TestStepSQL() = default;

  // Interface with the file system
  virtual void    ReadFromXML(XString p_filename) override;  // Throws in case of an error
  virtual bool    WriteToXML(XString p_filename) override;
  // RE-Calculate the effective strings, returning the number of unbound parameters
  virtual int     EffectiveReplacements(Parameters* p_parameters, bool p_forDisplay);
  // Check our filenames extension
  virtual void    CheckFilename(XString p_filename);
  // Reset effective parameter replacements
  virtual void    ResetEffective();

  // SETTERS
  void            SetDatasource(XString p_source)     { m_datasource = p_source;      }
  void            SetUser(XString p_user)             { m_user       = p_user;        }
  void            SetPassword(XString p_password)     { m_password   = p_password;    }
  void            SetSQL(XString p_sql)               { m_sql        = p_sql;         }
  void            SetCredential(XString p_credential);

  // GETTERS
  XString         GetDataSource()                     { return m_datasource;          }
  XString         GetUser()                           { return m_user;                }
  XString         GetPassword()                       { return m_password;            }
  XString         GetSQL()                            { return m_sql;                 }
  XString         GetEffectiveDatasource()            { return m_effectiveDatasource; }
  XString         GetEffectiveUser()                  { return m_effectiveUser;       }
  XString         GetEffectivePassword()              { return m_effectivePassword;   }
  XString         GetEffectiveSQL()                   { return m_effectiveSQL;        }
  XString         GetCredential()                     { return m_credential;          }

protected:
  // Credential template
  XString         m_credential;
  // Credentials
  XString         m_datasource;
  XString         m_user;
  XString         m_password;
  XString         m_sql;
  // Effective after parameter replacement
  XString         m_effectiveDatasource;
  XString         m_effectiveUser;
  XString         m_effectivePassword;
  XString         m_effectiveSQL;
};

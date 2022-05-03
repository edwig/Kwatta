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
  virtual void    ReadFromXML(CString p_filename) override;  // Throws in case of an error
  virtual bool    WriteToXML(CString p_filename) override;
  // RE-Calculate the effective strings, returning the number of unbound parameters
  virtual int     EffectiveReplacements(Parameters* p_parameters, bool p_forDisplay);
  // Check our filenames extension
  virtual void    CheckFilename(CString p_filename);
  // Reset effective parameter replacements
  virtual void    ResetEffective();

  // SETTERS
  void            SetDatasource(CString p_source)     { m_datasource = p_source;      }
  void            SetUser(CString p_user)             { m_user       = p_user;        }
  void            SetPassword(CString p_password)     { m_password   = p_password;    }
  void            SetSQL(CString p_sql)               { m_sql        = p_sql;         }
  // GETTERS
  CString         GetDataSource()                     { return m_datasource;          }
  CString         GetUser()                           { return m_user;                }
  CString         GetPassword()                       { return m_password;            }
  CString         GetSQL()                            { return m_sql;                 }
  CString         GetEffectiveDatasource()            { return m_effectiveDatasource; }
  CString         GetEffectiveUser()                  { return m_effectiveUser;       }
  CString         GetEffectivePassword()              { return m_effectivePassword;   }
  CString         GetEffectiveSQL()                   { return m_effectiveSQL;        }

protected:
  CString         m_datasource;
  CString         m_user;
  CString         m_password;
  CString         m_sql;
  // Effective after parameter replacement
  CString         m_effectiveDatasource;
  CString         m_effectiveUser;
  CString         m_effectivePassword;
  CString         m_effectiveSQL;
};

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
// This File   : ValidateIN.cpp
// What it does: A validation step for a teststep. The validation of a HTTP step
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ValidateIN.h"
#include "Parameters.h"
#include "ExtraExtensions.h"
#include <JSONMessage.h>
#include <JSONPath.h>
#include <XPath.h>
#include <StdException.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

void
ValidateIN::ReadFromXML(CString p_filename)
{
  XMLMessage msg;
  Validate::ReadFromXML(msg,p_filename);

  // Status value validation
  XMLElement* retval = msg.FindElement("CheckStatus");
  if(retval)
  {
    m_checkStatus    = msg.GetElementBoolean(retval,"Check");
    m_statusOperator = StringToReturnOperator(msg.GetElement(retval,"Operator"));
    m_expectedStatus = msg.GetElement(retval,"Expected");
    m_statusVariable = msg.GetElement(retval,"StatusVariable");
  }

  // Header value validation
  XMLElement* header = msg.FindElement("CheckHeader");
  if(header)
  {
    m_checkHeader    = msg.GetElementBoolean(header,"Check");
    m_headerOperator = StringToBufferOperator(msg.GetElement(header,"Operator"));
    m_expectedHeader = msg.GetElement(header,"Expected");
    m_verifyHeader   = msg.GetElement(header,"Header");
    m_headerVariable = msg.GetElement(header,"HeaderVariable");
  }

  // Body validation
  XMLElement* body = msg.FindElement("CheckBody");
  if(body)
  {
    m_checkBody    = msg.GetElementBoolean(body,"Check");
    m_bodyOperator = StringToBufferOperator(msg.GetElement(body,"Operator"));
    m_expectedBody = msg.GetElement(body,"Expected");
    m_bodyVariable = msg.GetElement(body,"BodyVariable");
  }

  // Header value validation
  XMLElement* xml = msg.FindElement("CheckXML");
  if(xml)
  {
    m_checkXML      = msg.GetElementBoolean(xml,"Check");
    m_xmlOperator   = StringToBufferOperator(msg.GetElement(xml,"Operator"));
    m_expectedXML   = msg.GetElement(xml,"Expected");
    m_verifyXmlPath = msg.GetElement(xml,"XmlPath");
    m_xmlVariable   = msg.GetElement(xml,"XmlVariable");
  }

  // Header value validation
  XMLElement* json = msg.FindElement("CheckJSON");
  if(json)
  {
    m_checkJSON      = msg.GetElementBoolean(json,"Check");
    m_jsonOperator   = StringToBufferOperator(msg.GetElement(json,"Operator"));
    m_expectedJSON   = msg.GetElement(json,"Expected");
    m_verifyJSONPath = msg.GetElement(json,"JsonPath");
    m_jsonVariable   = msg.GetElement(json,"JsonVariable");
  }
}

bool
ValidateIN::WriteToXML(CString p_filename)
{
  XMLMessage msg;
  if(!Validate::WriteToXML(msg,p_filename))
  {
    return false;
  }
  XMLElement* root = msg.GetRoot();

  // Status value
  XMLElement* elem = msg.AddElement(root,"CheckStatus",XDT_String,"");
  msg.SetElement(elem,"Check",m_checkStatus);
  msg.SetElement(elem,"Operator",ReturnOperatorToString(m_statusOperator));
  msg.SetElement(elem,"StatusVariable",m_statusVariable);
  msg.AddElement(elem,"Expected",XDT_CDATA,m_expectedStatus);

  // Header value
  elem = msg.AddElement(root,"CheckHeader",XDT_String,"");
  msg.SetElement(elem,"Check",m_checkHeader);
  msg.SetElement(elem,"Operator",BufferOperatorToString(m_headerOperator));
  msg.AddElement(elem,"Expected",      XDT_CDATA, m_expectedHeader);
  msg.AddElement(elem,"Header",        XDT_String,m_verifyHeader);
  msg.AddElement(elem,"HeaderVariable",XDT_String,m_headerVariable);

  // Body value
  elem = msg.AddElement(root,"CheckBody",XDT_String,"");
  msg.SetElement(elem,"Check",m_checkBody);
  msg.SetElement(elem,"Operator",BufferOperatorToString(m_bodyOperator));
  msg.AddElement(elem,"Expected",    XDT_CDATA,m_expectedBody);
  msg.AddElement(elem,"BodyVariable",XDT_String,m_bodyVariable);

  // XML node value
  elem = msg.AddElement(root,"CheckXML",XDT_String,"");
  msg.SetElement(elem,"Check",m_checkXML);
  msg.SetElement(elem,"Operator",BufferOperatorToString(m_xmlOperator));
  msg.AddElement(elem,"Expected",   XDT_CDATA, m_expectedXML);
  msg.AddElement(elem,"XmlPath",    XDT_String,m_verifyXmlPath);
  msg.AddElement(elem,"XmlVariable",XDT_String,m_xmlVariable);

  // JSON pair value
  elem = msg.AddElement(root,"CheckJSON",XDT_String,"");
  msg.SetElement(elem,"Check",m_checkJSON);
  msg.SetElement(elem,"Operator",BufferOperatorToString(m_jsonOperator));
  msg.AddElement(elem,"Expected",    XDT_CDATA, m_expectedJSON);
  msg.AddElement(elem,"JsonPath",    XDT_String,m_verifyJSONPath);
  msg.AddElement(elem,"JsonVariable",XDT_String,m_jsonVariable);

  // Now save it
  return msg.SaveFile(m_filename);
}

// RE-Calculate the effective strings, returning the number of unbound parameters
int
ValidateIN::EffectiveReplacements(Parameters* p_parameters,bool p_forDisplay)
{
  int unbound = 0;

  unbound += p_parameters->Replace(m_expectedStatus, m_effectiveStatus,     p_forDisplay);
  unbound += p_parameters->Replace(m_expectedHeader, m_effectiveHeaderValue,p_forDisplay);
  unbound += p_parameters->Replace(m_expectedBody,   m_effectiveBody,       p_forDisplay);
  unbound += p_parameters->Replace(m_expectedXML,    m_effectiveXMLValue,   p_forDisplay);
  unbound += p_parameters->Replace(m_expectedJSON,   m_effectiveJSONValue,  p_forDisplay);

  return unbound;
}

// Check our filenames extension
void
ValidateIN::CheckFilename(CString p_filename)
{
  // Split of only the extension
  char extension[_MAX_EXT];
  _splitpath_s(p_filename,NULL,0,NULL,0,NULL,0,extension,_MAX_EXT);

  // Check that we have the right one
  if(_strnicmp(extension,EXTENSION_VALIDATE_IN,5))
  {
    throw StdException("A Validation XML definition file must be saved as a *.IVAL");
  }
}

bool
ValidateIN::ValidateStatusValue(Parameters* p_parameters,int p_status)
{
  // If we need not check it, all-OK
  if(!m_checkStatus)
  {
    return true;
  }

  // Convert expected result to an int
  int expected = atoi(m_effectiveStatus);

  // Check result, depending on the operator
  bool result = false;
  switch(m_statusOperator)
  {
    case ReturnOperator::ROP_NOP:         result = false;                break;
    case ReturnOperator::ROP_EQUAL:       result = p_status == expected; break;
    case ReturnOperator::ROP_GREATER:     result = p_status >  expected; break;
    case ReturnOperator::ROP_GREATEREQUAL:result = p_status >= expected; break;
    case ReturnOperator::ROP_SMALLER:     result = p_status <  expected; break;
    case ReturnOperator::ROP_SMALLEREQUAL:result = p_status <= expected; break;
    case ReturnOperator::ROP_NOTEQUAL:    result = p_status != expected; break;
    case ReturnOperator::ROP_BETWEEN:     result = Between(m_effectiveStatus,p_status); break;
    case ReturnOperator::ROP_IN:          result = ValueIn(m_effectiveStatus,p_status); break;
  }
  // Save as result of the test
  if(result && !m_statusVariable.IsEmpty())
  {
    CString status;
    status.Format("%d", p_status);
    p_parameters->OverwriteReturnParameter(m_statusVariable,status);
  }
  return result;
}

bool
ValidateIN::ValidateHeaderValue(Parameters* p_parameters,CString p_value)
{
  // See if we must check the output value
  if(!m_checkHeader)
  {
    return true;
  }

  bool result = false;
  switch(m_headerOperator)
  {
    case BufferOperator::BOP_NOP:       result = false;                                         break;
    case BufferOperator::BOP_ISEMPTY:   result = p_value.IsEmpty();                             break;
    case BufferOperator::BOP_NOTEMPTY:  result = p_value.IsEmpty() == false;                    break;
    case BufferOperator::BOP_EXACT:     result = p_value.Compare(m_effectiveHeaderValue) == 0;  break;
    case BufferOperator::BOP_CONTAINS:  result = p_value.Find(m_effectiveHeaderValue)    >= 0;  break;
    case BufferOperator::BOP_BEGINSWITH:result = p_value.Find(m_effectiveHeaderValue)    == 0;  break;
    case BufferOperator::BOP_NOTFOUND:  result = p_value.Find(m_effectiveHeaderValue)     < 0;  break;
    case BufferOperator::BOP_ENDSWITH:  result = p_value.Right(m_effectiveHeaderValue.GetLength()).Compare(m_effectiveHeaderValue) == 0; break;
    case BufferOperator::BOP_FILEMATCH: result = FileMatch(p_value,m_effectiveHeaderValue);     break;
    case BufferOperator::BOP_EXISTFILE: result = FileExist(p_value);                            break;
  }
  // Save as result of the test
  if(result && !m_headerVariable.IsEmpty())
  {
    p_parameters->OverwriteReturnParameter(m_headerVariable,p_value);
  }
  return result;
}

bool
ValidateIN::ValidateBodyValue(Parameters* p_parameters,CString p_body)
{
  // See if we must check the output value
  if(!m_checkBody)
  {
    return true;
  }

  bool result = false;
  switch(m_bodyOperator)
  {
    case BufferOperator::BOP_NOP:       result = false;                                 break;
    case BufferOperator::BOP_ISEMPTY:   result = p_body.IsEmpty();                      break;
    case BufferOperator::BOP_NOTEMPTY:  result = p_body.IsEmpty() == false;             break;
    case BufferOperator::BOP_EXACT:     result = p_body.Compare(m_effectiveBody) == 0;  break;
    case BufferOperator::BOP_CONTAINS:  result = p_body.Find(m_effectiveBody)    >= 0;  break;
    case BufferOperator::BOP_BEGINSWITH:result = p_body.Find(m_effectiveBody)    == 0;  break;
    case BufferOperator::BOP_NOTFOUND:  result = p_body.Find(m_effectiveBody)     < 0;  break;
    case BufferOperator::BOP_ENDSWITH:  result = p_body.Right(m_effectiveBody.GetLength()).Compare(m_effectiveBody) == 0; break;
    case BufferOperator::BOP_FILEMATCH: result = FileMatch(p_body,m_effectiveBody);     break;
    case BufferOperator::BOP_EXISTFILE: result = FileExist(p_body);                     break;
  }
  // Save as result of the test
  if(result && !m_bodyVariable.IsEmpty())
  {
    p_parameters->OverwriteBufferParameter(m_bodyVariable,p_body);
  }
  return result;
}

bool
ValidateIN::ValidateXMLValue(Parameters* p_parameters,CString p_xml)
{
  // See if we must check the output value
  if(!m_checkXML)
  {
    return true;
  }

  // Parse as a XML message string
  XMLMessage msg;
  msg.ParseMessage(p_xml);
  if(msg.GetInternalError() != XmlError::XE_NoError)
  {
    return false;
  }
  CString node;

  XPath path(&msg,m_verifyXmlPath);
  if (path.GetNumberOfMatches() > 0)
  {
    node = path.GetFirstResult()->GetValue();
  }
  else
  {
    return false;
  }
  bool result = false;
  switch(m_xmlOperator)
  {
    case BufferOperator::BOP_NOP:       result = false;                                  break;
    case BufferOperator::BOP_ISEMPTY:   result = node.IsEmpty();                         break;
    case BufferOperator::BOP_NOTEMPTY:  result = node.IsEmpty() == false;                break;
    case BufferOperator::BOP_EXACT:     result = node.Compare(m_effectiveXMLValue) == 0; break;
    case BufferOperator::BOP_CONTAINS:  result = node.Find(m_effectiveXMLValue)    >= 0; break;
    case BufferOperator::BOP_BEGINSWITH:result = node.Find(m_effectiveXMLValue)    == 0; break;
    case BufferOperator::BOP_NOTFOUND:  result = node.Find(m_effectiveXMLValue)     < 0; break;
    case BufferOperator::BOP_ENDSWITH:  result = node.Right(m_effectiveXMLValue.GetLength()).Compare(m_effectiveXMLValue) == 0; break;
    case BufferOperator::BOP_FILEMATCH: result = FileMatch(node,m_effectiveXMLValue);    break;
    case BufferOperator::BOP_EXISTFILE: result = FileExist(node);                        break;
  }
  // Save as result of the test
  if(result && !m_xmlVariable.IsEmpty())
  {
    p_parameters->OverwriteReturnParameter(m_xmlVariable,node);
  }
  return result;
}

bool
ValidateIN::ValidateJSONValue(Parameters* p_parameters,CString p_json)
{
  // See if we must check the output value
  if(!m_checkJSON)
  {
    return true;
  }

  // Parse as a XML message string
  JSONMessage msg;
  msg.ParseMessage(p_json);
  if(msg.GetErrorState())
  {
    return false;
  }

  // Find the value from this pair
  CString value;
  JSONPath path(msg,m_verifyJSONPath);
  if(path.GetNumberOfMatches() >= 1)
  {
    value = path.GetFirstResultForceToString();
  }
  else
  {
    return false;
  }

  bool result = false;
  switch(m_jsonOperator)
  {
    case BufferOperator::BOP_NOP:       result = false;                                    break;
    case BufferOperator::BOP_ISEMPTY:   result = value.IsEmpty();                          break;
    case BufferOperator::BOP_NOTEMPTY:  result = value.IsEmpty() == false;                 break;
    case BufferOperator::BOP_EXACT:     result = value.Compare(m_effectiveJSONValue) == 0; break;
    case BufferOperator::BOP_CONTAINS:  result = value.Find(m_effectiveJSONValue)    >= 0; break;
    case BufferOperator::BOP_BEGINSWITH:result = value.Find(m_effectiveJSONValue)    == 0; break;
    case BufferOperator::BOP_NOTFOUND:  result = value.Find(m_effectiveJSONValue)     < 0; break;
    case BufferOperator::BOP_ENDSWITH:  result = value.Right(m_effectiveJSONValue.GetLength()).Compare(m_effectiveJSONValue) == 0; break;
    case BufferOperator::BOP_FILEMATCH: result = FileMatch(value,m_effectiveJSONValue);    break;
    case BufferOperator::BOP_EXISTFILE: result = FileExist(value);                         break;
  }
  // Save as result of the test
  if(result && !m_jsonVariable.IsEmpty())
  {
    p_parameters->OverwriteReturnParameter(m_jsonVariable,value);
  }
  return result;
}

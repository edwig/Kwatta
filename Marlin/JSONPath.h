/////////////////////////////////////////////////////////////////////////////////
//
// SourceFile: JSONPath.h
//
// Marlin Server: Internet server/client
// 
// Copyright (c) 2014-2022 ir. W.E. Huisman
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

//////////////////////////////////////////////////////////////////////////////
//
// JSONPath
//
// Implementation of IETF draft
// See: https://datatracker.ietf.org/doc/draft-ietf-jsonpath-base/
// 
// The draft is still incomplete. Elementary elements are missing:
// - How to escape charaters in tokens (notably the '.' and '[]' delimiters
// - The definition of the wildcard is ambiguous "*" or ".*"
// - The definition of the slice indicator is 'wobbely' [0:2:1] means 
//   only ellements 0 and 1. The end :2: is not included in the result set
// 
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//
// IMPLEMENTS:
// -----------
// $.one.two.three          -> object selection
// $.one[2].three           -> array (3th element).object selection
// $['one']['two']['three'] -> object selection
// $['one'][2]['three]      -> array (3th element).object selection
// $                        -> Whole document (selects the root)
// $..*                     -> Whole document (selects the root)
// $.one..something         -> Recursive find 'something' under 'one'
// $.one.two.*              -> All elements of 'two'
// $.one.two[:4]            -> Slice operator, array elements 0,1,2,3
// $.one.two[1:8:2]         -> Slice operator, array elements 2,4,6
// $.one.two[:8:2]          -> Slice operator, array elements 0,2,4,6
// $.one.two[2::3]          -> Slice operator, array elements 3,6,9,...
// $.one.two[3,4,7]         -> Union operator, array elements 4,5,8
//
// STILL TO BE IMPLEMENTED:
// ------------------------
// - Union operators for object names 
//   $.one['two','five']    -> Selects object pairs 'two' and 'five' from object 'one'
// - The combination of slice and union operators
//   $.one.two[2,4,12:17]   -> Selects array elements 3,5,13,14,15,16
// - Query operator '?'               (mentioned but not implemented in the draft)
// - Expression selection with (...)  (mentioned but not implemented in the draft)
// 
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "JSONPointer.h"
#include <vector>

class JSONMessage;
using JPResults = std::vector<JSONvalue*>;

class JSONPath
{
public:
  JSONPath(bool p_originOne = false);
  JSONPath(JSONMessage* p_message,CString p_path,bool p_originOne = false);
  JSONPath(JSONMessage& p_message,CString p_path,bool p_originOne = false);
 ~JSONPath();

  // Our main purpose: evaluate the path in the message
  bool Evaluate() noexcept;

  // SETTERS + Re-Evaluate
  bool SetPath(CString p_path) noexcept;
  bool SetMessage(JSONMessage* p_message) noexcept;

  // GETTERS
  CString      GetPath() const;
  JSONMessage* GetJSONMessage() const;
  JPStatus     GetStatus() const;
  // Results from the path evaluation
  unsigned     GetNumberOfMatches() const;
  JSONvalue*   GetFirstResult() const;
  CString      GetFirstResultForceToString(bool p_whitespace = false) const;
  JSONvalue*   GetResult(int p_index) const;
  CString      GetErrorMessage() const;

private:
  // Internal procedures
  void Reset();
  void PresetStatus();
  bool ParseLevel(CString& p_parsing);
  bool FindDelimiterType(CString& p_parsing);
  bool GetNextToken(CString& p_parsing,CString& p_token,bool& p_isIndex);
  void ProcessWildcard();
  void ProcessSlice(CString p_token);
  void ProcessUnion(CString p_token);

  // DATA
  CString      m_path;
  JSONMessage* m_message   { nullptr };
  int          m_origin    { 0       };
  JPStatus     m_status    { JPStatus::JP_None };
  char         m_delimiter { '/'};
  JSONvalue*   m_searching { nullptr };
  bool         m_recursive { false   };
  CString      m_errorInfo;

  // RESULT Pointers
  JPResults    m_results;
};


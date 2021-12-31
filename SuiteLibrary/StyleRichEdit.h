/////////////////////////////////////////////////////////////////////////////////
//
// SourceFile: RichEdit.h
//
// Mailer to post an e-mail to the DARPA-net
// See the "Handleiding PostMail6.docx" for a manual description
// 
// Copyright (c) 2006-2017 ir. W.E. Huisman
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
#include <richedit.h>
#include <afxcmn.h>

// Override the CRichtEditCtrl so that we 
// can paint our own (modern-look) border around it
//
class StyleRichEdit : public CRichEditCtrl
{
  DECLARE_DYNAMIC(StyleRichEdit)

public:
  StyleRichEdit();
  // Getting the RTF text out of the control
  CString GetRTFText();
  // Setting RTF text in the control
  void    SetRTFText(CString p_text);
  void    SetBorderColor(int p_color);
  void    SetBorderSize(int p_size);

  DECLARE_MESSAGE_MAP()
protected:
  // Paint our own border
  afx_msg void OnPaint();
  afx_msg void OnHScroll(UINT nSBCode,UINT nPos,CScrollBar* pScrollBar);
  afx_msg void OnVScroll(UINT nSBCode,UINT nPos,CScrollBar* pScrollBar);
private:
  int m_borderColor { RGB(127,127,127) };  // Gray border
  int m_borderSize  { 1 };                 // Size of one line
};

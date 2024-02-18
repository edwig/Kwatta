/////////////////////////////////////////////////////////////////////////////////
//
// SourceFile: StyleRichEdit.cpp
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
#include "stdafx.h"
#include "StyleRichEdit.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(StyleRichEdit,CRichEditCtrl)

StyleRichEdit::StyleRichEdit()
{
  m_borderColor = RGB(127,127,127);
}

BEGIN_MESSAGE_MAP(StyleRichEdit,CRichEditCtrl)
  ON_WM_PAINT()
  ON_WM_HSCROLL()
  ON_WM_VSCROLL()
END_MESSAGE_MAP()

void
StyleRichEdit::SetBorderColor(int p_color)
{
  if(m_borderColor != p_color)
  {
    m_borderColor = p_color;
    Invalidate();
  }
}

void
StyleRichEdit::SetBorderSize(int p_size)
{
  if(m_borderSize != p_size)
  {
    m_borderSize = p_size;
    Invalidate();
  }
}

void StyleRichEdit::OnPaint()
{
  CRichEditCtrl::OnPaint();

  CDC* dc = GetDC();
  CRect rect;
  GetClientRect(&rect);
  for(int index = 0; index < m_borderSize; ++index)
  {
    dc->Draw3dRect(rect,m_borderColor,m_borderColor);
    rect.DeflateRect(1,1);
  }
  ReleaseDC(dc);
}

DWORD __stdcall 
EditStreamInCallback(DWORD_PTR dwCookie,LPBYTE pbBuff,LONG cb,LONG *pcb)
{
  CString *psBuffer = (CString *)dwCookie;

  if(cb > psBuffer->GetLength())
  {
    cb = psBuffer->GetLength();
  }
  for(int i=0;i<cb;++i)
  {
    *(pbBuff+i) = (BYTE) psBuffer->GetAt(i);
  }
  *pcb = cb;
  *psBuffer = psBuffer->Mid(cb);
  return 0;
}

DWORD __stdcall 
EditStreamOutCallback(DWORD_PTR dwCookie,LPBYTE pbBuff,LONG cb,LONG *pcb)
{
  CString thisWrite;
  thisWrite.GetBufferSetLength(cb);
  CString *psBuffer = (CString *)dwCookie;

  // Getting all RTF chars
  for (int i=0;i<cb;++i) 
  {
    thisWrite.SetAt(i,*(pbBuff+i));
  }

  // Place in buffer
  *psBuffer += thisWrite;

  // Free temporary
  *pcb = thisWrite.GetLength();
  thisWrite.ReleaseBuffer();
  return 0;
}

// Getting the RTF text out of the control
CString 
StyleRichEdit::GetRTFText()
{
  CString readText;
  EDITSTREAM es;

  es.dwCookie    = (DWORD_PTR)&readText;    // Pass a pointer to the CString to the callback function 
  es.pfnCallback = EditStreamOutCallback;   // Specify the pointer to the callback function.
  es.dwError     = 0;

  // Perform the streaming
  StreamOut(SF_RTF,es); 

  return readText;
}

// Setting RTF text in the control
// Make sure p_text begins with "{\rtf1....}" !!
void
StyleRichEdit::SetRTFText(CString p_text)
{
  EDITSTREAM es;
  es.dwCookie    = (DWORD_PTR)&p_text;    // Pass a pointer to the CString to the callback function
  es.pfnCallback = EditStreamInCallback;  // Specify the pointer to the callback function
  es.dwError     = 0;

  // Perform the streaming
  StreamIn(SF_RTF,es); 
}

void 
StyleRichEdit::OnHScroll(UINT nSBCode,UINT nPos,CScrollBar* pScrollBar)
{
  CRichEditCtrl::OnHScroll(nSBCode,nPos,pScrollBar);
  Invalidate();
}

void 
StyleRichEdit::OnVScroll(UINT nSBCode,UINT nPos,CScrollBar* pScrollBar)
{
  CRichEditCtrl::OnVScroll(nSBCode,nPos,pScrollBar);
  Invalidate();
}

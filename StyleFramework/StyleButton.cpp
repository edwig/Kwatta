////////////////////////////////////////////////////////////////////////
//
// File: StyleButton.cpp
// Function: Buttons and IMG button to quickly set an icon resource
//
//   _____ _         _ _             ______                                           _    
//  / ____| |       | (_)           |  ____|                                         | |   
// | (___ | |_ _   _| |_ _ __   __ _| |__ _ __ __ _ _ __ ___   _____      _____  _ __| | __
//  \___ \| __| | | | | | '_ \ / _` |  __| '__/ _` | '_ ` _ \ / _ \ \ /\ / / _ \| '__| |/ /
//  ____) | |_| |_| | | | | | | (_| | |  | | | (_| | | | | | |  __/\ V  V / (_) | |  |   < 
// |_____/ \__|\__, |_|_|_| |_|\__, |_|  |_|  \__,_|_| |_| |_|\___| \_/\_/ \___/|_|  |_|\_\
//              __/ |           __/ |                                                      
//             |___/           |___/                                                       
//
//
// Author: ir. W.E. Huisman
// For license: See the file "LICENSE.txt" in the root folder
//
#include "stdafx.h"
#include "StyleButton.h"
#include "StyleColors.h"
#include "StyleMessageBox.h"
#include "StyleMacros.h"
#include "StyleFonts.h"
#include "StyleEdit.h"
#include "CWSExpander.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace ThemeColor;

//////////////////////////////////////////////////////////////////////////
//
// SERVICE FUNCTIONS FOR STYLE BUTTON
//
//////////////////////////////////////////////////////////////////////////

static bool left_button_busy = false;

static void LButtonUpBusy(bool p_busy)
{
  left_button_busy = p_busy;
}

static bool LButtonMayUp()
{
  return !left_button_busy;
}

static void PlaceRectInRect(const CRect& mRect,CRect& rect,int pos = 0,int p_margins = 0)
{
  CRect inRect(mRect);
  inRect.DeflateRect(2 * p_margins,2 * p_margins);
  // Place rect in upper left corner of inRect
  // And use the margins
  int leavex = (inRect.Width()  - rect.Width());
  int leavey = (inRect.Height() - rect.Height());

  double prop = 0;
  // 20 - 30   // Proportional scaling
  // 30 - 40   // Enlarge, but do not make smaller
  if(pos >= 20 && pos < 40)  // Proportional
  {
    if(leavex != 0 && leavey != 0)
    {
      prop = __min((double)leavex / (double)rect.Width(),(double)leavey / (double)rect.Height());
      prop = prop / 2.0;
      if(prop != 0.0 && !(pos > 30 && prop > 0.0))
      {
        rect.InflateRect((int)(rect.Width() * prop),(int)(rect.Height() * prop));
        leavex = (inRect.Width() - rect.Width());
        leavey = (inRect.Height() - rect.Height());
      }
    }
    if(pos > 30) pos -= 30;
    if(pos > 20) pos -= 20;
  }
  rect.OffsetRect(inRect.left - rect.left,inRect.top - rect.top);

  switch(pos) // text position
  {
    default:
    case 0:   break;
    case 1:   rect.OffsetRect(leavex / 2,0);
              break;
    case 2:   rect.OffsetRect(leavex,0);
              break;
    case 3:   rect.OffsetRect(4,leavey / 2);
              break;
    case 4:   rect.OffsetRect(leavex / 2,leavey / 2);
              break;
    case 5:   rect.OffsetRect(leavex,leavey / 2);
              break;
    case 6:   rect.OffsetRect(0,leavey);
              break;
    case 7:   rect.OffsetRect(leavex / 2,leavey);
              break;
    case 8:   rect.OffsetRect(leavex,leavey);
              break;
    case 10:  // all
              rect = inRect;
              break;
    case 11:  // Above
              rect.right = inRect.right;
              break;
    case 12:  // Right above
              rect.OffsetRect(leavex,0);
              rect.bottom = inRect.bottom;
              break;
    case 13:  // Below length
              rect.OffsetRect(0,leavey);
              rect.right = inRect.right;
              break;
    case 14:  // left high
              rect.bottom = inRect.bottom;
              break;
  }
  //rect.IntersectRect(&rect,&inRect);
}

//////////////////////////////////////////////////////////////////////////
//
// StyleButton
//
//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(StyleButton,CButton);

BEGIN_MESSAGE_MAP(StyleButton, CButton)
  ON_WM_CREATE()
  ON_WM_DESTROY()
  ON_WM_SIZE()
  ON_WM_ERASEBKGND()
  ON_WM_MOUSEMOVE()  
  ON_WM_LBUTTONUP()
  ON_WM_LBUTTONDOWN()
  ON_MESSAGE(WM_MOUSEHOVER,   OnMouseHover)
  ON_MESSAGE(WM_MOUSELEAVE,   OnMouseLeave)
  ON_MESSAGE(BM_SETSTYLE,     OnSetStyle)
  ON_MESSAGE(WM_NORMALBUTTON, OnNormalButton)
  ON_WM_PAINT()
END_MESSAGE_MAP()

StyleButton::StyleButton()
            :m_over(false)
            ,m_mandatory(false)
            ,m_error(false)
            ,m_defaultButton(false)
            ,m_style(0)
            ,m_bold(false)
{
}

StyleButton::StyleButton(CString p_type, bool pInError)
            :m_error(pInError)
            ,m_over(false)
            ,m_mandatory(false)
            ,m_defaultButton(false)
            ,m_style(0)
            ,m_bold(false)
{
  TranslateStyle(p_type);
}

StyleButton::~StyleButton()
{
  DestroyWindow();
}

BOOL
StyleButton::DestroyWindow()
{
  if (m_hWnd)
  {
    if (m_icon)
    {
      DestroyIcon(m_icon);
    }
    m_icon = nullptr;
    DestroyIcon(SetIcon(nullptr));
  }
  return CButton::DestroyWindow();
}

void
StyleButton::TranslateStyle(CString p_style)
{
  m_style = 0;

       if(p_style.Compare("ok")   == 0) m_style = IDI_OK;
  else if(p_style.Compare("can")  == 0) m_style = IDI_CANCEL;
  else if(p_style.Compare("rem")  == 0) m_style = IDI_REMOVE;
  else if(p_style.Compare("save") == 0) m_style = IDI_SAVE;
  else if(p_style.Compare("cal")  == 0) m_style = IDI_CALENDAR;
  else if(p_style.Compare("hlp")  == 0) m_style = IDI_HELP;
  else if(p_style.Compare("dir")  == 0) m_style = IDI_DIRECTORY;
}

void
StyleButton::SetStyle(CString p_style)
{
  TranslateStyle(p_style);
  if(m_style)
  {
    SetIconImage(m_style);
  }
}

// Load a icon from a 16 X 16 bits bitmap (24 color plane)
void
StyleButton::SetImage(UINT p_bitmap)
{
  m_bitmap.LoadBitmap(p_bitmap);

  CImageList images;
  images.Create(16, 16, ILC_COLOR24 | ILC_MASK, 1, 1);
  images.Add(&m_bitmap, RGB(255, 255, 255));

  m_icon = images.ExtractIcon(0);
}

// Load an icon from a '1-icon' icon file
void
StyleButton::SetIconImage(UINT p_icon)
{
  m_icon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(p_icon));
}

//////////////////////////////////////////////////////////////////////////
//
// MESSAGE HANDLERS
//
//////////////////////////////////////////////////////////////////////////

int
StyleButton::OnCreate(LPCREATESTRUCT cs)
{
  int res = CButton::OnCreate(cs);
  if(m_style)
  {
    SetIconImage(m_style);
  }
  return res;
}

void
StyleButton::OnDestroy()
{
  DestroyIcon(SetIcon(NULL));
  CButton::OnDestroy();
}

void
StyleButton::OnSize(UINT nType, int cx, int cy)
{
  CButton::OnSize(nType, cx, cy);
}

LRESULT 
StyleButton::OnSetStyle(WPARAM wParam, LPARAM lParam)
{
  Invalidate();
  return 0;
}

LRESULT 
StyleButton::OnNormalButton(WPARAM wParam, LPARAM lParam)
{
  if ((WPARAM)this != wParam)
  {
    if (GetStyle() & BS_DEFPUSHBUTTON)
    {
      ModifyStyle(BS_DEFPUSHBUTTON, BS_PUSHBUTTON);
      Invalidate();
    }
  }
  return 0;
}

afx_msg BOOL
StyleButton::OnEraseBkgnd(CDC* pdc)
{
  // No background erase
  return TRUE;
}

void StyleButton::OnPaint()
{
  if (GetUpdateRect(NULL, FALSE))
  {
    CPaintDC dc(this);

    CRect rect;
    GetClientRect(rect);
    UINT state  = GetState();
    DWORD style = GetStyle();

    CRect bmpRect(0, 0, WS(16), WS(16));
    CString txt;
    GetWindowText(txt);

    // If a button has no text, than regardless all, show an icon
    if(false) 
    {
      PlaceRectInRect(rect, bmpRect, 4);
    }
    else if(m_icon)
    {
      PlaceRectInRect(rect,bmpRect,3,2);
    }
    Draw(&dc, rect, m_over, state, style, m_icon, bmpRect, txt, m_error, true, m_mandatory, false /*themakleur*/, m_defaultButton);
  }
}

void 
StyleButton::OnMouseMove(UINT nFlags, CPoint point) 
{
  TRACKMOUSEEVENT mouseEvent;

  mouseEvent.cbSize      = sizeof(TRACKMOUSEEVENT);
  mouseEvent.dwFlags     = TME_HOVER | TME_LEAVE;
  mouseEvent.dwHoverTime = 1;
  mouseEvent.hwndTrack   = m_hWnd;

  _TrackMouseEvent(&mouseEvent);

  CButton::OnMouseMove(nFlags, point);
}

LRESULT 
StyleButton::OnMouseHover(WPARAM wParam,LPARAM lParam)
{
  m_over = true;
  Invalidate();
  return 0;
}

LRESULT 
StyleButton::OnMouseLeave(WPARAM wParam,LPARAM lParam)
{
  m_over = false;
  Invalidate();
  return 0;
}

void
StyleButton::OnLButtonDown(UINT nFlags, CPoint point)
{
  LButtonUpBusy(false);
  CButton::OnLButtonDown(nFlags, point);
}

void
StyleButton::OnLButtonUp(UINT nFlags, CPoint point)
{
  if(!LButtonMayUp())
  {
    return;
  }

  CWnd* parent = GetParent();
  MessageDialog* message = dynamic_cast<MessageDialog*> (parent);
  if (message)
  {
    message->PushThisButton(GetDlgCtrlID(), point);
  }
  else
  {
    CButton::OnLButtonUp(nFlags, point);
  }
}

//////////////////////////////////////////////////////////////////////////
//
// FUNCTIONS
//
//////////////////////////////////////////////////////////////////////////

BOOL
StyleButton::EnableWindow(BOOL p_enable)
{
  BOOL result = CWnd::EnableWindow(p_enable);
  Invalidate();
  return result;
}

void
StyleButton::StartCalendar(int p_control)
{
  CRect rect;
  GetWindowRect(&rect);
  int left = rect.left != 1 ? rect.left : 0;

  // GetDlgItem does not work in the StyleFrame!
  CWnd* wnd = GetParent()->GetDescendantWindow(p_control);
  if(wnd)
  {
    wnd->SendMessage(WM_LBUTTONDBLCLK, left, rect.bottom);
  }
}

void
StyleButton::StartCalendar(StyleEdit* p_edit)
{
  CRect rect;
  GetWindowRect(&rect);
  int left = rect.left != 1 ? rect.left : 0;

  p_edit->SendMessage(WM_LBUTTONDBLCLK,left,rect.bottom);
}

//////////////////////////////////////////////////////////////////////////
//
// PROTECTED
//
//////////////////////////////////////////////////////////////////////////

LRESULT 
StyleButton::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
  try
  {
    // We catch a TAB key here and remove the BS_DEFPUSHBUTTON style
    if(wParam == VK_TAB) 
    {
      DWORD style = GetStyle();
      UINT  state = GetState();      
      if ((style & BS_DEFPUSHBUTTON) != 0 && (state & BST_FOCUS) != 0)
      {
        ModifyStyle(BS_DEFPUSHBUTTON, BS_PUSHBUTTON);
      }
    }
    return CButton::WindowProc(message, wParam, lParam);
  }
  catch(CString& er)
  {
    // Cannot use StyleMessagebox (as StyleButton is a part of that!)
    AfxMessageBox(er,MB_OK|MB_ICONERROR);
    return 0;
  }
}

void 
StyleButton::Draw(CDC*    pDC
                 ,CRect   rect
                 ,bool    over
                 ,UINT    state
                 ,DWORD   style
                 ,HICON   hicon
                 ,CRect   bmpRect
                 ,CString txt
                 ,bool    pInError
                 ,bool    dialog
                 ,bool    p_mandatory
                 ,bool    p_themeColor
                 ,bool    p_default)
{
  // Get the colors
  COLORREF outline;
  COLORREF filling;
  COLORREF textcolor;

  if ((style & WS_DISABLED) == 0)
  {
    if ((state & BST_PUSHED) != 0)
    {
      if(over)
      {
        filling   = ClrControlPressed;
        textcolor = ClrControlTextPressed;
        outline   = ClrControlFramePressed;
      }
      else
      {
        filling   = ThemeColor::GetColor(Colors::ColorWindowFrame); // ClrControlNormal;
        textcolor = ThemeColor::GetColor(Colors::AccentColor3);     // ClrControlTextNormal;
        outline   = ClrControlFrameNormal;
      }
    }
    else if (((state & BST_FOCUS) != 0) || (style & BS_DEFPUSHBUTTON) != 0)
    {
      if (pInError)
      {
        filling   = ClrWindowFrameError;
        textcolor = ClrWindowFrameTextError;
        outline   = ClrWindowFrameError;
      }
      else
      {
        if (over || (style & BS_DEFPUSHBUTTON))
        {
          filling   = ThemeColor::GetColor(Colors::AccentColor1);  // ClrControlDefault;
          textcolor = ClrControlTextHover;
          outline   = ClrControlFrameHover;
        }
        else
        {
          filling   = ThemeColor::GetColor(Colors::ColorCtrlBackground);   // ClrControlNormal;
          textcolor = ThemeColor::GetColor(Colors::AccentColor1);          //ClrControlTextNormal;
          outline   = ThemeColor::GetColor(Colors::AccentColor1); 
        }
      }
    }
    else if (over != 0)
    {
      filling   = ClrControlHover;
      textcolor = ClrControlTextHover;
      outline   = ClrControlFrameHover;
    }
    else if (p_default != 0)
    {
      filling   = ClrControlHover;
      textcolor = ClrControlTextHover;
      outline   = ClrControlFrameHover;
    }

    else
    {
      filling   = ThemeColor::GetColor(Colors::ColorButtonBackground);  // ClrControlNormal      = UsersBackground
      textcolor = ThemeColor::GetColor(Colors::ColorButtonText);        // ClrControlTextNormal
      outline   = ThemeColor::GetColor(Colors::AccentColor1);           // ClrControlFrameNormal
    }
  }
  else
  {
    outline   = ClrControlFrameDisabled;
    filling   = ClrControlDisabled;
    textcolor = ClrControlTextDisabled;
  }

  if (p_mandatory)
  {
    outline = ThemeColor::NoWhite(ThemeColor::GetColor(Colors::AccentColor1)); //ClrEditFrameVerplicht;
  }
  if (pInError)
  {
    outline = ClrEditFrameError;
  }
  // Draw the edge
  CBrush br(outline);
  pDC->FrameRect(rect, &br);
  rect.DeflateRect(1, 1);
  if (pInError)
  {
    pDC->FrameRect(rect, &br);
    rect.DeflateRect(1, 1);
  }

  // Draw the background
  pDC->FillSolidRect(rect, filling);


  // Draw the icon image
  if (hicon != NULL)
  {
    if ((style & WS_DISABLED) == 0)
    {
      if (p_themeColor)
      {
        CWSExpander(pDC, rect).DrawIcon(hicon
                                       ,ThemeColor::GetColor(Colors::ColorWindowFrame)
                                       ,ThemeColor::GetColor(Colors::ColorComboActive));
        // Alternative without scaling
        // bmpRect.DeflateRect(1, 1);
        // br.DeleteObject();
        // br.CreateSolidBrush(ClrUitklapLijstBtn);
        // pDC->DrawState(bmpRect.TopLeft(), bmpRect.Size(), hicon, DST_ICON | DSS_MONO, &br);
      }
      else
      {
        int b = WS(16);
        if(bmpRect.left < 1)
        {
          b = WS(16) - abs(bmpRect.left);
          if (b < 0) b = 0;
          bmpRect.left = 1;
        }
        if(b > 0)
        {
          DrawIconEx(pDC->m_hDC, bmpRect.left, bmpRect.top, hicon, b, b, 0, NULL, DI_NORMAL);
        }
      }
    }
    else
    {
      CWSExpander(pDC, rect).DrawIcon(hicon,ThemeColor::GetColor(Colors::ColorWindowFrame),textcolor);
      // Alternative without scaling
      // br.DeleteObject();
      // br.CreateSolidBrush(textcolor);
      // pDC->DrawState(bmpRect.TopLeft(), bmpRect.Size(), hicon, DST_ICON | DSS_MONO, &br);
    }
  }
  
  // Paint the text
  pDC->SetTextColor(textcolor);

  CFont* org = pDC->SelectObject(m_bold ? &STYLEFONTS.DialogTextFontBold : &STYLEFONTS.DialogTextFont);
  pDC->DrawText(txt, &rect, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
  pDC->SelectObject(org);
}

////////////////////////////////////////////////////////////////////////
//
// File: StyleStepper.cpp
// Function: Stepper like wizard to guide the user through a mutation
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
#include "StyleStepper.h"
#include "StyleColors.h"
#include "StyleFonts.h"

using namespace ThemeColor;

IMPLEMENT_DYNAMIC(StyleStepper,StyleDialog);

StyleStepper::StyleStepper(CWnd*   p_parentWnd  /*= nullptr     */
                          ,UINT    p_IDTemplate /*= IDD_STEPPER */
                          ,CString p_caption    /*= ""          */
                          ,bool    p_sysmenu    /*= false       */
                          ,bool    p_status     /*= false       */)
             :StyleDialog(p_IDTemplate
                         ,p_parentWnd
                         ,true
                         ,p_sysmenu
                         ,p_status)
             ,m_caption(p_caption)
             ,m_activePage(0)
             ,m_data(nullptr)
{
  m_textPrior = _T("Previous");
  m_textNext  = _T("Next");
  m_textReady = _T("Ready");

  LOGFONT lf = StyleFonts::MakeLOGFONTFromString(_T("Verdana;28;800"));
  m_stepFont.CreateFontIndirect(&lf);
}

StyleStepper::~StyleStepper()
{
}

void StyleStepper::DoDataExchange(CDataExchange* pDX)
{
  StyleDialog::DoDataExchange(pDX);

  DDX_Control(pDX,IDC_PRIOR,m_buttonPrior);
  DDX_Control(pDX,IDC_NEXT, m_buttonNext);
}

BEGIN_MESSAGE_MAP(StyleStepper,StyleDialog)
  ON_WM_SIZE()
  ON_WM_PAINT()
  ON_BN_CLICKED(IDC_PRIOR,&StyleStepper::OnBnClickedPrior)
  ON_BN_CLICKED(IDC_NEXT, &StyleStepper::OnBnClickedNext)
END_MESSAGE_MAP()

BOOL
StyleStepper::OnInitDialog()
{
  StyleDialog::OnInitDialog();
  SetCaption(m_caption);

  // Check if we really have a wizard!
  if(m_pages.size() < 2)
  {
    StyleMessageBox(this,"INTERNAL ERROR: Wizards must have a least 2 pages!","ERROR",MB_OK|MB_ICONERROR);
    OnOK();
    return TRUE;
  }
  InitPages();
  AdjustSteperSize();
  SetCanResize();
  ShowGripper();
  DisplayPage();

  UpdateData(FALSE);
  return TRUE;
}

void
StyleStepper::SetupDynamicLayout()
{
  StyleDialog::SetupDynamicLayout();

  CMFCDynamicLayout& manager = *GetDynamicLayout();
#ifdef _DEBUG
  manager.AssertValid();
#endif

  for(size_t index = 0;index < m_pages.size();++index)
  {
    HWND wnd = m_pages[index].m_page->GetSafeHwnd();
    manager.AddItem(wnd,manager.MoveNone(),manager.SizeHorizontalAndVertical(100,100));
  }
  manager.AddItem(IDC_PRIOR,manager.MoveHorizontalAndVertical(100,100),manager.SizeNone());
  manager.AddItem(IDC_NEXT,manager.MoveHorizontalAndVertical(100,100),manager.SizeNone());

  // Now resize all pages
  for(size_t index = 0;index < m_pages.size();++index)
  {
    m_pages[index].m_page->SetCanResize();
  }
  // Do the final adjustments
  manager.Adjust();
}

void
StyleStepper::AdjustSteperSize()
{
  // Get minimal size of the stepper
  CRect winrect;
  GetWindowRect(&winrect);
  CRect clientrect;
  GetClientRect(&clientrect);
  int minx = 0;
  int miny = 0;
  for(size_t index = 0;index < m_pages.size();++index)
  {
    CRect client;
    m_pages[index].m_page->GetWindowRect(client);
    if(minx < client.Width())  minx = client.Width();
    if(miny < client.Height()) miny = client.Height();
  }

  // Recalculate new positions
  miny += STEPPER_TOP + STEPPER_BOTTOM;
  int newright  = winrect.left + minx + (winrect.Width()  - clientrect.Width());
  int newbottom = winrect.top  + miny + (winrect.Height() - clientrect.Height());

  // Move the buttons up and left
  MoveButton(m_buttonPrior,winrect.right - newright,winrect.bottom - newbottom);
  MoveButton(m_buttonNext, winrect.right - newright,winrect.bottom - newbottom);

  // Final resize of the window
  winrect.right  = newright;
  winrect.bottom = newbottom;
  MoveWindow(winrect);
}

void
StyleStepper::MoveButton(StyleButton& p_button,int x,int y)
{
  CRect rect;
  p_button.GetWindowRect(&rect);
  CRect parent;
  GetWindowRect(&parent);
  
  int w = rect.Width();
  int h = rect.Height();

  rect.left   = rect.left - parent.left - x - 7;
  rect.right  = rect.left + w;
  rect.top    = rect.top  - parent.top  - y - (STEPPER_BOTTOM / 2);
  rect.bottom = rect.top  + h;
  p_button.MoveWindow(rect,TRUE);
}

void 
StyleStepper::SetCaption(CString p_caption)
{
  m_caption = p_caption;
  if(GetSafeHwnd())
  {
    SetWindowText(m_caption);
  }
}

void
StyleStepper::AddPage(StyleTab* p_page,UINT p_resource)
{
  StepperTab page;
  page.m_page = p_page;
  page.m_resource = p_resource;

  m_pages.push_back(page);
}

void
StyleStepper::InitPages()
{
  // Create all the pages
  for(auto page : m_pages)
  {
    BOOL created = page.m_page->Create(page.m_resource,this);
    if(created)
    {
      page.m_page->ShowWindow(SW_HIDE);
    }
  }

  // Init all the pages
  for(size_t index = 0;index < m_pages.size();++index)
  {
    if(!m_pages[index].m_page->InitStyleTab(m_data))
    {
      return;
    }
  }
}

void
StyleStepper::DisplayPage()
{
  // Set button text and icons
  m_buttonPrior.SetIconImage(IDI_PRIOR);
  m_buttonNext. SetIconImage(m_activePage == (GetPagesCount() - 1) ? IDI_OK : IDI_NEXT);
  m_buttonPrior.EnableWindow(m_activePage > 0);
  // Button text
  m_buttonPrior.SetWindowText(m_textPrior);
  m_buttonNext .SetWindowText(m_activePage == (GetPagesCount() -1) ? m_textReady : m_textNext);
  
  // Show correct page
  for(size_t index = 0;index < m_pages.size();++index)
  {
    if(m_activePage == index)
    {
      m_pages[index].m_page->ShowWindow(SW_SHOW);
    }
    else
    {
      m_pages[index].m_page->ShowWindow(SW_HIDE);
    }
  }
  // Repaint everything
  Invalidate();

  CRect client;
  GetClientRect(&client);
  ResizePages(client.Width(),client.Height());
}

void
StyleStepper::OnSize(UINT p_type,int cx,int cy)
{
  StyleDialog::OnSize(p_type,cx,cy);
  ClearStepperArea();
}

void
StyleStepper::ResizePages(int cx,int cy)
{

  // Where we expect to see our pages
  CRect pagerect(0,STEPPER_TOP,cx,cy - STEPPER_BOTTOM);
  for(size_t index = 0;index < m_pages.size();++index)
  {
    if(m_pages[index].m_page->GetSafeHwnd())
    {
      CWnd* parent = m_pages[index].m_page->GetParent();


      m_pages[index].m_page->MoveWindow(pagerect,TRUE);
    }
  }
}

void
StyleStepper::OnBnClickedPrior()
{
  if(m_activePage > 0)
  {
    --m_activePage;
    DisplayPage();
  }
}

void
StyleStepper::OnBnClickedNext()
{
  if(m_activePage == GetPagesCount() - 1)
  {
    // Last page. This is the final OK!
    TryComplete();
  }
  if(m_activePage < (unsigned)(GetPagesCount() - 1))
  {
    TryPageForward();
  }
}

void
StyleStepper::TryPageForward()
{
  if(m_pages[m_activePage].m_page->CheckStyleTab(m_data))
  {
    // Simply next page
    ++m_activePage;
    m_pages[m_activePage].m_page->InitStyleTab(m_data);
    DisplayPage();
  }
}

void
StyleStepper::TryComplete()
{
  // Recheck all pages
  for(size_t index = 0;index < m_pages.size();++index)
  {
    if(!m_pages[index].m_page->CheckStyleTab(m_data))
    {
      return;
    }
  }
  // Save all pages
  for(size_t index = 0;index < m_pages.size();++index)
  {
    if(!m_pages[index].m_page->SaveStyleTab(m_data))
    {
      return;
    }
  }
  // Final OK
  OnOK();
}

void
StyleStepper::OnPaint()
{
  StyleDialog::OnPaint();

  Draw();
}

void
StyleStepper::ClearStepperArea()
{
  CDC* dc = GetDC();
  CRect client;
  GetClientRect(&client);

  client.bottom =  client.top + STEPPER_TOP;
  COLORREF oldBack = dc->SetBkColor(ThemeColor::GetColor(Colors::ColorWindowFrame));
  dc->ExtTextOut(0,0,ETO_OPAQUE,client,"",0);

  dc->SetBkColor(oldBack);
  ReleaseDC(dc);
}

void
StyleStepper::Draw()
{
  CDC* dc = GetDC();
  CRect client;
  GetClientRect(&client);

  int count  = (int) m_pages.size();
  int margin = STEPPER_MARGIN;
  int middle = STEPPER_TOP / 2;
  int radius = (STEPPER_TOP - (2 * STEPPER_MARGIN)) / 2;
  int xpos   = margin + radius;
  int dx     = (client.Width() - (2 * margin) - (2 * radius)) / (count - 1);

  // Draw the middle line (GRAY)

  int color1 = ThemeColor::GetColor(Colors::AccentColor1);
  int color2 = RGB(0x7F,0x7F,0x7F);

  CBrush  brush1(color1);
  CBrush  brush2(color2);
  CPen    pen;
  pen.CreatePen(PS_NULL,0,RGB(0,0,0));
  CPen*   oldPen   = dc->SelectObject(&pen);
  CBrush* oldBrush = dc->SelectObject(&brush1);

  for(int page = 0;page < (count - 1); ++page)
  {
    if(page == m_activePage)
    {
      // Draw lines in GRAY color for unfinished pages
      dc->SelectObject(&brush2);
    }
    CRect line(xpos,middle - (STEPPER_LINEWIDTH / 2),xpos + dx,middle + (STEPPER_LINEWIDTH / 2));
    dc->Rectangle(line);
    xpos += dx;
  }


  // Draw circles in the style color up to activePage
  dc->SelectObject(&brush1);
  xpos = margin + radius;

  for(int page = 0;page < count; ++page)
  {
    CRect circle(xpos - radius,middle - radius,xpos + radius,middle + radius);
    dc->Ellipse(circle);
    xpos += dx;
    if(page == m_activePage)
    {
      // Draw circles in GRAY color for unfinished pages
      dc->SelectObject(&brush2);
    }
  }

  // Number the steps
  xpos = margin + radius;
  CFont* oldFont = dc->SelectObject(&m_stepFont);
  for(int page = 0;page < count; ++page)
  {
    CString number;
    number.Format("%d",page + 1);
    CSize size = dc->GetTextExtent(number);
    CRect pos(xpos - size.cx / 2,middle - size.cy / 2,xpos + size.cx / 2,middle + size.cy / 2);
    dc->SetTextColor(RGB(0xFF,0xFF,0xFF));
    dc->SetBkColor  (page <= (int)m_activePage ? color1 : color2);
    dc->ExtTextOut(xpos - size.cx / 2,middle - size.cy / 2,ETO_CLIPPED | ETO_OPAQUE,&pos,number,nullptr);
    xpos += dx;
  }

  // Reset DC
  dc->SelectObject(oldBrush);
  dc->SelectObject(oldPen);
  dc->SelectObject(oldFont);
  ReleaseDC(dc);
}


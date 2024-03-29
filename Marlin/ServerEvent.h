/////////////////////////////////////////////////////////////////////////////////
//
// SourceFile: ServerEvent.h
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

// Server push event
class ServerEvent
{
public:
  // Standard constructors
  ServerEvent() 
    :m_id(0)
  { 
  }
  
  explicit ServerEvent(XString p_event)
    :m_event(p_event)
    ,m_id(0)
  { 
  }

  explicit ServerEvent(const ServerEvent* p_event)
    :m_id   (p_event->m_id)
    ,m_event(p_event->m_event)
    ,m_data (p_event->m_data)
  {
  }

  // Event data
  UINT    m_id;
  XString m_event;
  XString m_data;
};

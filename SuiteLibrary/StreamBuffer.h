#pragma once

class StreamBuffer
{
public:
  StreamBuffer() = default;
 ~StreamBuffer();

  void Reset();
  void SetBuffer(unsigned char*  p_buffer,unsigned int  p_length);
  void AddBuffer(unsigned char*  p_buffer,unsigned int  p_length);
  void GetBuffer(unsigned char*& p_buffer,unsigned int& p_length);
  void SetUse(bool p_use);
  bool GetUse() { return m_use; };

//  CString AsCDataNode();

private:
  bool           m_use    { false   };
  unsigned char* m_buffer { nullptr };
  unsigned int   m_length { 0       };
};
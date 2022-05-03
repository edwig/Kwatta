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
// This File   : ResetAll.cpp
// What it does: Reset all test results (suite, tests, steps)
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ResetAll.h"
#include "TestSet.h"
#include "TestStep.h"
#include "TestStepCMD.h"
#include "TestStepNET.h"
#include "TestSuite.h"
#include "ExtraExtensions.h"
#include <StdException.h>
#include <io.h>

ResetAll::ResetAll(CString p_baseDirectory,CString p_testDirectory,CString p_step)
         :m_baseDirectory(p_baseDirectory)
         ,m_testDirectory(p_testDirectory)
         ,m_testStep(p_step)
{
}

bool
ResetAll::Reset(bool p_interface)
{
  m_interface = p_interface;

  if(!m_baseDirectory.IsEmpty())
  {
    if(_access(m_baseDirectory, 06) != 0)
    {
      m_error = "\nNo read/write rights on directory: " + m_baseDirectory;
      return false;
    }
  }
  if(m_testDirectory.IsEmpty())
  {
    ResetSuite();
  }
  else
  {
    if(m_testStep.IsEmpty())
    {
      ResetTestSet();
    }
    else
    {
      ResetTestStep();
    }
  }
  
  // Do the effective deletions
  DeleteList();
  
  return m_error.IsEmpty();
}

//////////////////////////////////////////////////////////////////////////
//
// PRIVATE
//
//////////////////////////////////////////////////////////////////////////

void
ResetAll::ResetSuite()
{
  CString pattern = m_baseDirectory + "*" + EXTENSION_SUITE;

  // Read in all known files
  WIN32_FIND_DATA data;
  HANDLE hFind = nullptr;
  if ((hFind = FindFirstFile(pattern, &data)) != INVALID_HANDLE_VALUE)
  {
    do
    {
      CString filename = CString(data.cFileName);
      try
      {
        TestSuite suite(m_baseDirectory);
        suite.ReadFromXML(filename);

        Tests& tests = suite.GetAllTests();

        for(auto& test : tests)
        {
          m_testDirectory = test.second.m_directory;
          ResetTestSet();

          // Reset the total set result;
          test.second.m_lastResult.Empty();
        }
        suite.WriteToXML(true);
      }
      catch(StdException& ex)
      {
        m_error += "\n" + ex.GetErrorMessage();
      }
    }
    while(FindNextFile(hFind,&data) != 0);
    FindClose(hFind);
  }
}

void
ResetAll::ResetTestSet()
{
  CString pattern = m_baseDirectory + m_testDirectory + "\\*" + EXTENSION_SET;

  // Read in all known files
  WIN32_FIND_DATA data;
  HANDLE hFind = nullptr;
  if((hFind = FindFirstFile(pattern,&data)) != INVALID_HANDLE_VALUE)
  {
    do
    {
      CString filename = m_baseDirectory + m_testDirectory + "\\" + CString(data.cFileName);
      try
      {
        TestSet testset;
        testset.ReadFromXML(filename);
        TRunSet& runs = testset.GetTestRuns();
        for(auto& trun : runs)
        {
          m_testStep = trun.m_filename;
          ResetTestStep();
          // Reset the step result !!!!
          trun.m_lastResult.Empty();

          // Add the result file to the delete list
          CString deleting = m_baseDirectory + m_testDirectory + "\\" + trun.m_filename;
          deleting.Replace(EXTENSION_TESTSTEP_CMD,EXTENSION_RESULT_CMD);
          deleting.Replace(EXTENSION_TESTSTEP_NET,EXTENSION_RESULT_NET);
          m_toBeDeleted.push_back(deleting);
        }
        testset.WriteToXML();
      }
      catch(StdException& ex)
      {
        m_error += "\n" + ex.GetErrorMessage();
      }
    }
    while (FindNextFile(hFind,&data) != 0);
    FindClose(hFind);
  }
}

void
ResetAll::ResetTestStep()
{
  CString path = m_baseDirectory + m_testDirectory + "\\" + m_testStep;

  try
  {
    TestStep* step = ReadTestStep(path);
    if (step->GetType() == StepType::Step_command)
    {
      TestStepCMD* clstep = reinterpret_cast<TestStepCMD*>(step);
      clstep->ResetEffective();
    }
    else
    {
      TestStepNET* instep = reinterpret_cast<TestStepNET*>(step);
      instep->ResetEffective();
    }
    step->WriteToXML(path);

    delete step;
  }
  catch (StdException& ex)
  {
    m_error += "\n" + ex.GetErrorMessage();
  }
}

// Delete all result files
void
ResetAll::DeleteList()
{
  for(auto file : m_toBeDeleted)
  {
    if(DeleteFile(file) == FALSE)
    {
      if(GetLastError() != ERROR_FILE_NOT_FOUND)
      {
        m_error += "\nCould not delete file: " + file;
      }
    }
  }
}

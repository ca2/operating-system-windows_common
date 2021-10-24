﻿#pragma once


namespace windows
{


   class CLASS_DECL_APEX_WINDOWS_COMMON process :
      virtual public ::operating_system::process
   {
   public:


      PROCESS_INFORMATION        m_pi;
      STARTUPINFOW               m_si;


      process();
      virtual ~process();


      virtual bool create_child_process(const ::string & pszCmdLine, bool bPiped, const ::string & pszDir = nullptr, ::enum_priority epriority = ::e_priority_none) override;

      virtual bool has_exited() override;

      virtual bool synch_elevated(const ::string & pszCmdLine,int iShow,const ::duration & durationTimeOut,bool * pbTimeOut) override;

      virtual bool kill() override;

   };


} // namespace windows

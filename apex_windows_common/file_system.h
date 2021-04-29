#pragma once


namespace windows
{


   class CLASS_DECL_APEX_WINDOWS file_system:
      virtual public ::file_system
   {
   public:


      file_system();
      virtual ~file_system();


      virtual ::e_status init_system() override;


      virtual ::e_status update_module_path() override;


   };


} // namespace windows




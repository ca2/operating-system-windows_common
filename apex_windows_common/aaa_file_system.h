#pragma once


namespace windows
{


   class CLASS_DECL_APEX_WINDOWS_COMMON file_system:
      virtual public ::file_system
   {
   public:


      file_system();
      virtual ~file_system();


      virtual void init_system() override;


      virtual void update_module_path() override;


   };


} // namespace windows




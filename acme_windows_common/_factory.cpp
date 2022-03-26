#include "framework.h"


//__FACTORY_EXCHANGE void acme_windows_common()
//FACTORY_ITEM(::windows_common::acme_directory)
//FACTORY_ITEM(::windows_common::acme_file)
//FACTORY_ITEM(::windows_common::acme_path)
//FACTORY_ITEM(::acme::windows_common::node)
//END_FACTORY()


//void CLASS_DECL_ACME __cdecl _ca2_purecall();


__FACTORY_EXPORT void acme_windows_common_factory(::factory::factory * pfactory)
{

   //_set_purecall_handler(_ca2_purecall);

   //pfactory->add_factory_item < ::windows::dir_system, ::dir_system >();
   //pfactory->add_factory_item < ::windows::file_system, ::file_system >();

   //pfactory->add_factory_item < ::windows::dir_context, ::dir_context >();
   //pfactory->add_factory_item < ::windows::file_context, ::file_context >();

   //add_factory_item < ::windows::stdio_file, ::file::text_file >();
   pfactory->add_factory_item < ::windows_common::file, ::file::file >();
   //pfactory->add_factory_item < ::windows::os_context, ::os_context >();
   //pfactory->add_factory_item < ::windows::pipe, ::operating_system::pipe >();
   //pfactory->add_factory_item < ::windows::process, ::operating_system::process >();

   ////add_factory_item < ::windows::console, ::console::console >();
   //pfactory->add_factory_item < ::windows::crypto, ::crypto::crypto >();
   //pfactory->add_factory_item < ::windows::ip_enum, ::net::ip_enum >();

   pfactory->add_factory_item < ::acme::windows_common::node, ::acme::node >();
   pfactory->add_factory_item < ::windows_common::acme_directory, ::acme_directory >();
   pfactory->add_factory_item < ::windows_common::acme_file, ::acme_path >();
   pfactory->add_factory_item < ::windows_common::acme_path, ::acme_path >();

   //pfactory->add_factory_item < ::windows::interprocess_communication_base, ::interprocess_communication::base >();
   //pfactory->add_factory_item < ::windows::interprocess_communication_rx, ::interprocess_communication::rx >();
   //pfactory->add_factory_item < ::windows::interprocess_communication_tx, ::interprocess_communication::tx >();
   //add_factory_item < ::windows::interprocess_communication, ::interprocess_communication::interprocess_communication >();


   //add_factory_item < ::windows::buffer, ::graphics::graphics >();
   //add_factory_item < ::windows::interaction_impl, ::user::interaction_impl >();

   //pfactory->add_factory_item < ::file::os_watcher, ::file::watcher >();
   //pfactory->add_factory_item < ::file::os_watch, ::file::watch >();

   //pfactory->add_factory_item < ::windows::file_context, ::file_context >();
   //add_factory_item < ::windows::copydesk, ::user::cop
   // 
   // 
   // ydesk >();
   ////add_factory_item < ::windows::shell, ::user::shell >();

   //pfactory->add_factory_item < ::windows::serial, ::serial::serial >();
   //pfactory->add_factory_item < ::windows::file_memory_map, ::file::memory_map >();

   //pfactory->add_factory_item < ::windows::pipe, ::operating_system::pipe >();
   //pfactory->add_factory_item < ::windows::file, ::file::file >();
   //pfactory->add_factory_item < ::windows::console, ::console::console >();

   //pfactory->add_factory_item < ::windows::dir_system, ::dir_system >();
   //pfactory->add_factory_item < ::windows::file_system, ::file_system >();

   //pfactory->add_factory_item < ::windows::dir_context, ::dir_context >();
   //pfactory->add_factory_item < ::windows::file_context, ::file_context >();

   //pfactory->add_factory_item < ::windows::stdio_file, ::file::text_file >();
   //pfactory->add_factory_item < ::windows::os_context, ::os_context >();
   //pfactory->add_factory_item < ::windows::future, ::operating_system::process >();

   //pfactory->add_factory_item < ::windows::crypto, ::crypto::crypto >();
   //pfactory->add_factory_item < ::windows::ip_enum, ::net::ip_enum >();

   //pfactory->add_factory_item < ::windows::file_memory_map, ::file::memory_map >();
   //pfactory->add_factory_item < ::windows::buffer, ::graphics::graphics >();
   //pfactory->add_factory_item < ::windows::interaction_impl, ::user::interaction_impl >();

   //pfactory->add_factory_item < ::file::os_watcher, ::file::watcher >();
   //pfactory->add_factory_item < ::file::os_watch, ::file::watch >();

   //pfactory->add_factory_item < ::windows::copydesk, ::user::copydesk >();
   //pfactory->add_factory_item < ::windows::shell, ::user::shell >();

   //pfactory->add_factory_item < ::windows_common::exception_translator, ::exception_translator >();
   //pfactory->add_factory_item < ::windows_common::exception_engine, ::exception_engine >();


}




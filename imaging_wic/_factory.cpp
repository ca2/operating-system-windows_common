#include "framework.h"


namespace imaging_wic
{

   class factory_exchange
   {
   public:

      
      factory_exchange();
      virtual ~factory_exchange();

   };


//#define new ACME_NEW
//
//
   factory_exchange::factory_exchange()
   {

      ::factory::add_factory_item < ::imaging_wic::context_image, ::context_image >();

   }

//
   factory_exchange::~factory_exchange()
   {

   }
//
//
} // namespace imaging_wic
//

__FACTORY_EXPORT void imaging_wic_factory(::factory::factory * pfactory)
{

   pfactory->add_factory_item < ::imaging_wic::context_image, ::context_image >();

}




//imaging_wic::factory_exchange g_imagingwicfactoryexchange;
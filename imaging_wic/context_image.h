#pragma once


namespace imaging_wic
{


   class CLASS_DECL_IMAGING_WIC context_image :
      virtual public ::context_image
   {
   public:




      __composite(handler_manager)        m_pmanagerImageLoad;


      context_image();
      ~context_image() override;


      virtual ::e_status initialize(::object * pobject) override;


      //virtual ::e_status _load_image(::context_image * pcontextimage, ::image * pimageParam, const ::payload & varFile, bool bSync, bool bCreateHelperMaps) override;
      ::e_status _load_image(image* pimage, const ::payload& varFile, const ::image::load_options & loadoptions = ::image::load_options()) override;

      ::e_status _load_image(::image * pimage, __pointer(image_frame_array) & pframea, ::memory & memory) override;
      ::e_status save_image(memory & memory, const ::image * pimage, const ::save_image * psaveimage) override;


      //::e_status _load_icon(::draw2d::icon * picon, const ::payload & varFile);


      //virtual ::e_status _load_image(::image* pimage, const ::payload& varFile, bool bSync, bool bCreateHelperMaps);


      //virtual ::e_status _load_image(::image* pimage, __pointer(image_frame_array)& pframea, ::memory_pointer pmemory);
      //virtual ::e_status save_image(memory& memory, const ::image* pimage, ::save_image* psaveimage);


      //virtual ::e_status load_cursor(::draw2d::cursor* pcursor, ::file::path path, bool bSync, bool bCache = true) override;

      //virtual ::e_status load_svg(::image* pimage, ::memory_pointer pmemory);


#ifdef _UWP
      virtual bool _desk_to_image(::image* pimage);
      virtual bool _desk_has_image();
      virtual bool _image_to_desk(const ::image* pimage);
#endif


      virtual bool _save_image(::file::file* pfile, const ::image* pimage, const ::save_image* psaveimage);

      void _os_load_image(::image * pimage, memory & memory) override;


   };


} // namespace imaging_wic




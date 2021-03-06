// Created by camilo on 2021-08-10 13:31 BRT <3ThomasBorregaardSĂ¸rensen!!
#pragma once


#include "aura/operating_system.h"
#include "aura_windows_common/_library.h"
#ifdef _UWP
#include "aura_universal_windows/_library.h"
#include <winrt/Windows.ApplicationModel.DataTransfer.h>
#endif
#ifdef WINDOWS_DESKTOP
#include "aura_windows/_library.h"
#endif
#include <wincodec.h>


#include "aura/graphics/image/save_image.h"


namespace imaging_wic
{


   comptr < IWICImagingFactory > get_imaging_factory();


   CLASS_DECL_IMAGING_WIC bool node_save_image(IStream * pstream, const ::image * pimage, const ::save_image * psaveimage);


#ifdef _UWP


   CLASS_DECL_IMAGING_WIC bool node_save_image(::winrt::Windows::Storage::Streams::IRandomAccessStream const & stream, const ::image * pimage, const ::save_image * psaveimage);


#endif


} // namespace imaging_wic


#include "factory_exchange.h"


comptr < IWICImagingFactory > wic_get_imaging_factory();


#include "context_image.h"


// Created by camilo on 2021-06-05 23:48 BRT <3ThomasBS_!!
#pragma once


namespace directx
{


   class CLASS_DECL_DIRECTX dxgi_debug :
      virtual public ::matter
   {
   public:


      Microsoft::WRL::ComPtr < IDXGIDebug >     m_pdxgidebug;


      static dxgi_debug *                       s_pdxgidebug;


      dxgi_debug();


      virtual void debug();


   };


} // namespace directx



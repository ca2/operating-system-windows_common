#include "framework.h"
#include "aura/user/_user.h"
#include "CustomRenderer.h"
#include "aura/graphics/draw2d/lock.h"
#include "aura/graphics/draw2d/device_lock.h"
#include "aura/graphics/draw2d/context_image.h"
#include <math.h>


void dpi_initialize(ID2D1Factory* pFactory);


#define d2d1_fax_options D2D1_FACTORY_OPTIONS // fax of merde
#define multi_threaded D2D1_FACTORY_TYPE_MULTI_THREADED // ???? muliple performance multi thread hidden option there exists cost uses?


//#if defined(WINDOWS_DESKTOP
//
//CLASS_DECL_AURA void directx_debug();
//
//#endif


namespace draw2d_direct2d
{


   graphics::state::state()
   {

   }


   graphics::state::~state()
   {

   }


   graphics::graphics()
   {

      m_ealphamodeDevice = ::draw2d::e_alpha_mode_none;

      clear_os_data();

      m_pthis = this;

      defer_create_mutex();

      m_pstate = __new(state);

      m_bSaveClip = false;

      m_hdcAttach = nullptr;

      m_iType     = 0;

      m_interpolationmode = D2D1_INTERPOLATION_MODE_HIGH_QUALITY_CUBIC;

      m_bitmapinterpolationmode = D2D1_BITMAP_INTERPOLATION_MODE_LINEAR;

   }


   graphics::~graphics()
   {

      destroy();

   }


   void graphics::assert_ok() const
   {

      object::assert_ok();

   }


   void graphics::dump(dump_context & dumpcontext) const
   {

      object::dump(dumpcontext);

      //dumpcontext << "get_handle1() = " << (::iptr) get_handle1();
      //dumpcontext << "\nm_hAttribDC = " << (::iptr) get_handle2();
      dumpcontext << "\nm_bPrinting = " << m_bPrinting;

      dumpcontext << "\n";

   }


   //void graphics::IsPrinting()
   //{

   //   return m_bPrinting;

   //}


   //void graphics::CreateDC(const ::string & lpszDriverName, const ::string & lpszDeviceName, const ::string & lpszOutput, const void * lpInitData)
   //{
   //   __throw(todo);
   //   //return Attach(::CreateDC(lpszDriverName, lpszDeviceName, lpszOutput, (const DEVMODE*)lpInitData));
   //}


   //void graphics::CreateIC(const ::string & lpszDriverName, const ::string & lpszDeviceName, const ::string & lpszOutput, const void * lpInitData)
   //{

   //   __throw(todo);

   //}


   void graphics::CreateCompatibleDC(::draw2d::graphics * pgraphics)
   {

      ::draw2d::lock draw2dlock;

      ::draw2d::device_lock devicelock(this);

      if (m_iType != 0)
      {

         destroy();

      }

      /*::direct2d::direct2d() = __new(::draw2d_direct2d::plugin);

      ::direct2d::direct2d()->initialize();*/

      HRESULT hr;

      comptr<ID2D1DeviceContext> pdevicecontextTemplate;

      if (FAILED(hr = ::direct2d::direct2d()->m_pd2device->CreateDeviceContext(
                      D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
                      //D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS,
                      &pdevicecontextTemplate)))
      {

         trace_hr("graphics::CreateCompatibleDC, CreateDeviceContext (1) ", hr);

         //return false;

         throw_status(error_failed);

      }

      comptr<ID2D1RenderTarget> prendertargetTemplate;

      auto psession = get_session();

      auto paurasession = psession->m_paurasession;

      auto puser = paurasession->user();

      auto pwindowing = puser->windowing();

      auto pdisplay = pwindowing->display();

      auto dpi = pdisplay->get_dpi();

      if (dpi <= 0.0)
      {

         ASSERT(false);

         throw_status(error_failed);

      }

      pdevicecontextTemplate->SetDpi(dpi, dpi);

      if (FAILED(hr = pdevicecontextTemplate->QueryInterface(IID_ID2D1RenderTarget,(void **)&prendertargetTemplate)))
      {

         trace_hr("graphics::CreateCompatibleDC, QueryInterface (2) ",hr);

         throw_status(error_failed);

      }

      D2D1_SIZE_U sizeu = D2D1::SizeU(1, 1);

      D2D1_PIXEL_FORMAT pixelformat;

      pixelformat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;

      pixelformat.format = DXGI_FORMAT_B8G8R8A8_UNORM;

      if (FAILED(hr = prendertargetTemplate->CreateCompatibleRenderTarget(
                      nullptr,
                      &sizeu,
                      &pixelformat,
         D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS_GDI_COMPATIBLE,
                      &m_pbitmaprendertarget)))
      {

         trace_hr("graphics::CreateCompatibleDC, CreateCompatibleRenderTarget (3) ", hr);

         throw_status(error_failed);

      }

      hr = m_pbitmaprendertarget.as(m_prendertarget);

      if(FAILED(hr))
      {

         m_pbitmaprendertarget = nullptr;

         throw_status(error_failed);

      }

      hr = m_pbitmaprendertarget.as(m_pdevicecontext);

      if(FAILED(hr))
      {

         m_prendertarget = nullptr;

         m_pbitmaprendertarget = nullptr;

         throw_status(error_failed);

      }

      __defer_construct(m_pbitmap);

      ID2D1Bitmap * pbitmap;

      hr = m_pbitmaprendertarget->GetBitmap(&pbitmap);

      if(FAILED(hr))
      {

         m_pbitmaprendertarget = nullptr;

         throw_status(error_failed);

      }

      m_pbitmap->attach(pbitmap);

      m_iType = 3;

      m_osdata[data_device_context] = m_pdevicecontext;

      m_osdata[data_render_target] = m_prendertarget;

      //return true;

   }


   //int graphics::GetDeviceCaps(double dIndex)
   //{

   //   __throw(todo);

   //}


   point_f64 graphics::GetBrushOrg()
   {
      //__throw(todo);
      //ASSERT(get_handle1() != nullptr);
      //::point_f64 point;
      //VERIFY(::GetBrushOrgEx(get_handle1(), &point));
      //return point;

      return ::point_f64();

   }

   
   point_f64 graphics::SetBrushOrg(double x, double y)
   {
      //__throw(todo);
      //ASSERT(get_handle1() != nullptr);
      //::point_f64 point;
      //VERIFY(::SetBrushOrgEx(get_handle1(), x, y, &point));
      //return point;

      return ::point_f64();

   }

   
   point_f64 graphics::SetBrushOrg(const ::point_f64 & point)
   {

      //__throw(todo);
      //ASSERT(get_handle1() != nullptr);
      //VERIFY(::SetBrushOrgEx(get_handle1(), point.x, point.y, &point));
      //return point;

      return ::point_f64();

   }


   //int graphics::EnumObjects(double dObjectType, int (CALLBACK* lpfn)(LPVOID, LPARAM), LPARAM lpData)
   //{
   //   __throw(todo);
   //   //ASSERT(get_handle2() != nullptr);
   //   //return ::EnumObjects(get_handle2(), nObjectType, (GOBJENUMPROC)lpfn, lpData);
   //}

   
   void graphics::_draw_blend(const ::image_drawing & imagedrawing)
   {

      //return ::draw2d::graphics::BitBltAlphaBlend(x, y, nWidth, nHeight, pgraphicsSrc, xSrc, ySrc);

      //if (m_pimageAlphaBlend->is_set())
      //{
      //   //return true;

      //   // Reference implementation

      //   ::rectangle_i32 rectangleAlphaBlend(m_pointAlphaBlend, m_pimageAlphaBlend->size());

      //   if (x < 0)
      //   {

      //      xSrc -= x;

      //      nWidth += x;

      //      x = 0;

      //   }

      //   if (y < 0)
      //   {

      //      ySrc -= y;

      //      nHeight += y;

      //      y = 0;

      //   }

      //   point_i32 ::point_f64(x, y);

      //   ::size_f64 size(nWidth, nHeight);

      //   ::rectangle_i32 rectangleBlend(point, ::size_f64);

      //   ::rectangle_i32 rectangleIntersect;

      //   if (rectangleIntersect.intersect(rectangleAlphaBlend, rectangleBlend))
      //   {

      //      ::image_pointer pimage1;

      //      pimage1 = create_image(::size_f64);

      //      ::rectangle_i32 rectangleDib1(::point_f64(), pimage1->m_size);

      //      pimage1->get_graphics()->set_alpha_mode(::draw2d::e_alpha_mode_set);

      //      pimage1->get_graphics()->fill_rectangle(rectangleDib1, argb(0, 0, 0, 0));

      //      if (!pimage1->from(::point_f64(), pgraphicsSrc, ::point_f64(xSrc, ySrc), ::size_f64))
      //      {

      //         return false;

      //      }

      //      ::image_pointer pimage2;

      //      pimage2 = create_image(::size_f64);

      //      pimage2->get_graphics()->set_alpha_mode(::draw2d::e_alpha_mode_set);

      //      pimage2->get_graphics()->fill_rectangle(rectangleDib1, argb(255, 0, 0, 0));

      //      if (!pimage2->from(::point_f64(), m_pimageAlphaBlend, point_i32 - m_pointAlphaBlend, rectangleIntersect.size()))
      //      {

      //         return false;

      //      }

      //      __pointer(::draw2d_direct2d::graphics) pgraphicsDib1 = pimage1->get_graphics();

      //      __pointer(::draw2d_direct2d::graphics) pgraphicsDib2 = pimage2->get_graphics();

      //      HRESULT hr = ((ID2D1DeviceContext *)pgraphicsDib2->get_os_data())->EndDraw();

      //      pgraphicsDib2->m_pdevicecontext->DrawImage(
      //      (ID2D1Bitmap *)pgraphicsDib1->get_current_bitmap()->get_os_data(),
      //      D2D1::Point2F(0.f, 0.f),
      //      d2d1::rectangle_f32(rectangleDib1),
      //      D2D1_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
      //      D2D1_COMPOSITE_MODE_SOURCE_OVER);

      //      if (SUCCEEDED(hr))
      //      {

      //         ((ID2D1DeviceContext *)pgraphicsDib2->get_os_data())->BeginDraw();

      //      }

      //      set_alpha_mode(::draw2d::e_alpha_mode_blend);

      //      BitBltRaw(x, y, nWidth, nHeight, pimage2->get_graphics(), 0, 0);

      //      return true;

      //   }

      //}

      //return false;

      if (m_pimageAlphaBlend->is_set())
      {

         auto rectangleTarget = imagedrawing.m_rectangleTarget;
         auto rectangleSource = imagedrawing.source_rectangle();


         auto x = rectangleTarget.left;
         auto y = rectangleTarget.top;
         auto xSrc = rectangleSource.left;
         auto ySrc = rectangleSource.top;
         auto nWidth = rectangleTarget.width();
         auto nHeight = rectangleTarget.height();

         if (x < 0)
         {

            xSrc -= x;

            nWidth += x;

            x = 0;

         }

         if (y < 0)
         {

            ySrc -= y;

            nHeight += y;

            y = 0;

         }


         ::rectangle_f64 rectangleIntersect(m_pointAlphaBlend, m_pimageAlphaBlend->size());

         ::rectangle_f64 rectangleBlt(::point_f64(x, y), ::size_f64(nWidth, nHeight));

         if (rectangleIntersect.intersect(rectangleIntersect, rectangleBlt))
         {

            //if (m_pointAlphaBlend.x < 0)
            //{

            //   xSrc += -m_pointAlphaBlend.x;

            //}
            //if (m_pointAlphaBlend.y < 0)
            //{

            //   ySrc += -m_pointAlphaBlend.y;

            //}

            // The following commented out code does not work well when there is clipping
            // and some calculations are not precise
            //if (m_pimage->is_set() && pgraphicsSrc->m_pimage->is_set())
            //{

            //   ::point_f64 pointOff = GetViewportOrg();

            //   x += pointOff.x;

            //   y += pointOff.y;

            //   return m_pimage->blend(::point_f64(x, y), pgraphicsSrc->m_pimage, ::point_f64(xSrc, ySrc), m_pimageAlphaBlend, point_i32(m_pointAlphaBlend.x - x, m_pointAlphaBlend.y - y), rectangleBlt.size());

            //}
            //else
            //{

               auto pimage1 = m_pcontext->context_image()->create_image(rectangleBlt.size());

               pimage1->get_graphics()->set_alpha_mode(::draw2d::e_alpha_mode_set);

               auto pimage = imagedrawing.image();

               pimage1->_draw_raw(::rectangle_f64(rectangleBlt.size()), pimage, ::point_f64(xSrc, ySrc));

               //if (!pimage1->_draw_raw(::rectangle_f64(rectangleBlt.size()), pimage , ::point_f64(xSrc, ySrc)))
               //{

               //   return false;

               //}

               pimage1->blend2(::point_f64(), m_pimageAlphaBlend, point_f64(x - m_pointAlphaBlend.x, y - m_pointAlphaBlend.y), rectangleBlt.size(), 255);

               image_drawing_options imagedrawingoptions;

               _draw_raw(rectangleTarget, pimage1, imagedrawingoptions, ::point_f64());

            //}

            //return true;

         }

      }

      //return false;
   }


   void graphics::TextOutAlphaBlend(double x, double y, const block & block)
   {

      if (block.is_empty())
      {

         throw_status(error_invalid_empty_argument);

      }

      ASSERT(m_pimageAlphaBlend->is_ok());
      
      // "Reference" implementation for TextOutAlphaBlend

      auto size = get_text_extent(block);

      ::rectangle_f64 rectangleAlphaBlend(m_pointAlphaBlend, m_pimageAlphaBlend->size());

      ::point_f64 point(x, y);

      ::rectangle_f64 rectangleText(point, size);

      ::rectangle_f64 rectangleIntersect;

      if (rectangleIntersect.intersect(rectangleAlphaBlend, rectangleText))
      {

         ::draw2d::lock draw2dlock;

         ::draw2d::device_lock devicelock(this);

         auto pimage1 = m_pcontext->context_image()->create_image(size);

         ::rectangle_f64 rectangleDib1(::point_f64(), size);

         pimage1->get_graphics()->set(get_current_font());

         pimage1->get_graphics()->set(get_current_brush());

         pimage1->get_graphics()->set_alpha_mode(::draw2d::e_alpha_mode_set);

         pimage1->get_graphics()->fill_rectangle(rectangleDib1, argb(0, 0, 0, 0));

         pimage1->get_graphics()->text_out(0, 0, block);

         ::image_pointer pimage2;

         pimage2->create(size);

         pimage2->get_graphics()->set_alpha_mode(::draw2d::e_alpha_mode_set);

         pimage2->get_graphics()->fill_rectangle(rectangleDib1, argb(255, 0, 0, 0));

         pimage2->_draw_raw(rectangleIntersect.size(), m_pimageAlphaBlend, __pointd(point - m_pointAlphaBlend));

         __pointer(::draw2d_direct2d::graphics) pgraphicsDib1 = pimage1->get_graphics();

         __pointer(::draw2d_direct2d::graphics) pgraphicsDib2 = pimage2->get_graphics();

         pimage2->unmap();

         HRESULT hrFlush = ((ID2D1DeviceContext *)pgraphicsDib2->get_os_data())->Flush();

         HRESULT hrEndDraw = ((ID2D1DeviceContext *)pgraphicsDib2->get_os_data())->EndDraw();

         pgraphicsDib1->m_pdevicecontext->DrawImage(
         (ID2D1Bitmap *)pgraphicsDib2->get_current_bitmap()->m_osdata[0],
         D2D1::Point2F(0.f, 0.f),
         d2d1::rectangle_f32(rectangleDib1),
         D2D1_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
         D2D1_COMPOSITE_MODE_DESTINATION_IN);

         if (SUCCEEDED(hrEndDraw))
         {

            ((ID2D1DeviceContext *)pgraphicsDib2->get_os_data())->BeginDraw();

         }

         set_alpha_mode(::draw2d::e_alpha_mode_blend);

         image_source imagesource(pimage1);

         ::rectangle_f64 rectangleTarget(::point_f64(x, y), pimage1->size());

         image_drawing_options imagedrawingoptions(rectangleTarget);

         image_drawing imagedrawing(imagedrawingoptions, imagesource);

         draw(imagedrawing);

         //return true;

      }

      //return false;

   }


   void graphics::set(::draw2d::bitmap* pbitmapParam)
   {

      ::draw2d::lock draw2dlock;

      ::draw2d::device_lock devicelock(this);

      __pointer(::draw2d_direct2d::bitmap) pbitmap = pbitmapParam;

      if (::is_null(pbitmapParam))
      {

         throw_status(error_invalid_empty_argument);

      }


      //if(m_prendertarget == nullptr)
      //{

        // CreateCompatibleDC(nullptr);

      //}

      //bool image::realize(::draw2d::graphics *) const
      //{

         //if (is_realized())
         //{

         //   unrealize();

         //}

         //if (is_realized())
         //{

         //   return false;

         //}

         //if (m_pbitmap.is_null()
         //   || m_pbitmapMap.is_null()
         //   || m_pgraphics.is_null()
         //   || m_pgraphicsMap.is_null())
         //{

         //   return false;

         //}

         //__pointer(::draw2d_direct2d::graphics) pgraphicsMap = m_pgraphicsMap;

         //__pointer(::draw2d_direct2d::graphics) pgraphics = m_pgraphics;

         //__pointer(::draw2d_direct2d::bitmap) pbitmap = m_pbitmap;

      //   m_pbitmaprendertarget = nullptr;

      //   m_iType = 11;

      //   ::size_i32 size = pbitmap->GetBitmapDimension();

      //   D2D1_SIZE_U sizeu = D2D1::SizeU(size.cx, size.cy);

      //   D2D1_PIXEL_FORMAT pixelformat;

      //   pixelformat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;

      //   pixelformat.format = DXGI_FORMAT_B8G8R8A8_UNORM;

      //   auto & prendertarget = m_prendertarget;

      //   HRESULT hr = prendertarget->CreateCompatibleRenderTarget(nullptr, &sizeu, &pixelformat, D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS_NONE, &m_pbitmaprendertarget);

      //   if (m_pbitmaprendertarget == nullptr)
      //   {

      //      return false;

      //   }

      //   if (FAILED(m_pbitmaprendertarget.as(m_prendertarget)))
      //   {

      //      m_pbitmaprendertarget = nullptr;

      //      return false;

      //   }

      //   if (FAILED(m_pbitmaprendertarget.as(m_pdevicecontext)))
      //   {

      //      m_pbitmaprendertarget = nullptr;

      //      m_prendertarget = nullptr;

      //      return false;

      //   }

      //   comptr<ID2D1Bitmap> pd2d1bitmap;

      //   m_pbitmaprendertarget->GetBitmap(&pd2d1bitmap);

      //   if (pbitmap->m_pbitmap == nullptr)
      //   {

      //      return false;

      //   }

      //   //pgraphics->m_pplugin = pgraphicsMap->m_pplugin;

      //   pbitmap->m_pbitmap = pd2d1bitmap;

      //   pbitmap->m_pbitmap.as(pbitmap->m_pbitmap1);

      //   pbitmap->m_osdata[0] = pbitmap->m_pbitmap;

      //   pbitmap->m_osdata[1] = pbitmap->m_pbitmap1;

      //   m_osdata[0] = m_pdevicecontext;

      //   m_osdata[1] = m_prendertarget;

      //   //D2D1_POINT_2U p;

      //   //p.x = 0;
      //   //p.y = 0;

      //   //D2D1_RECT_U srcRect;

      //   //srcRect.left = 0;
      //   //srcRect.right = this->width();
      //   //srcRect.top = 0;
      //   //srcRect.bottom = this->height();

      ////   return true;

      ////}

         m_pdevicecontext->SetTarget(pbitmap->m_pbitmap);

      m_pbitmap = pbitmap;

      m_iType = 3;

      //return ::success;

   }


   //::color::color graphics::GetNearestColor(const ::color::color & color)
   //{

   //   __throw(todo);

   //}


   //::u32 graphics::RealizePalette()
   //{

   //   __throw(todo);
   //   
   //}


   //void graphics::UpdateColors()
   //{

   //   __throw(todo);
   //   //::UpdateColors(get_handle1());

   //}


   //int graphics::GetPolyFillMode()
   //{
   //   __throw(todo);
   //   //return ::GetPolyFillMode(get_handle2());
   //}

   //int graphics::GetROP2()
   //{
   //   __throw(todo);
   //   //return ::GetROP2(get_handle2());
   //}

   //int graphics::GetStretchBltMode()
   //{
   //   __throw(todo);
   //   //return ::GetStretchBltMode(get_handle2());
   //}

   //int graphics::GetMapMode()
   //{
   //   __throw(todo);
   //   //return ::GetMapMode(get_handle2());
   //}

   //int graphics::GetGraphicsMode()
   //{
   //   __throw(todo);
   //   //return ::GetGraphicsMode(get_handle2());
   //}


   //void graphics::GetWorldTransform(XFORM* pXform)
   //{

   //   __throw(todo);

   //}


   void graphics::_get(::draw2d::matrix & matrix)
   {

      D2D1::Matrix3x2F m;

      m_prendertarget->GetTransform(&m);

      matrix = ::draw2d::matrix();

      matrix.a1 = m._11;
      matrix.a2 = m._12;
      matrix.b1 = m._21;
      matrix.b2 = m._22;
      matrix.c1 = m._31;
      matrix.c2 = m._32;

      //return true;

   }


   void graphics::_set(const ::draw2d::matrix & matrix)
   {
      
      if (!m_prendertarget)
      {

         throw_status(error_null_pointer);

      }

      D2D1::Matrix3x2F m;

      m._11 = (FLOAT) matrix.a1;
      m._12 = (FLOAT) matrix.a2;
      m._21 = (FLOAT) matrix.b1;
      m._22 = (FLOAT) matrix.b2;
      m._31 = (FLOAT) matrix.c1;
      m._32 = (FLOAT) matrix.c2;

      m_prendertarget->SetTransform(&m);

      //return true;

   }


   point_f64 graphics::GetViewportOrg()
   {

      return ::draw2d::graphics::GetViewportOrg();

   }


   size_f64 graphics::GetViewportExt()
   {

      __throw(todo);

      return nullptr;

   }

   
   point_f64 graphics::GetWindowOrg()
   {

      __throw(todo);

      return nullptr;

   }


   size_f64 graphics::GetWindowExt()
   {
      __throw(todo);
      //::size_f64::size_f64;
      //::GetWindowExtEx(get_handle2(), &::size_f64);
      //return ::size_f64;
      return nullptr;
   }


   point_f64 graphics::SetViewportOrg(const ::point_f64 & point)
   {

      return ::draw2d::graphics::SetViewportOrg(point.x, point.y);

   }


   ::size_f64 graphics::SetViewportExt(const ::size_f64 & size)
   {

      return ::draw2d::graphics::SetViewportExt(size.cx, size.cy);

   }


   point_f64 graphics::SetWindowOrg(const ::point_f64 & point)
   {

      return SetWindowOrg(point.x, point.y);

   }


   ::size_f64 graphics::set_window_ext(const ::size_f64 & size)
   {
      
      return set_window_ext(size.cx, size.cy);

   }


   void graphics::DPtoLP(::point_f64 * ppoints, count nCount)
   {

      __throw(todo);
      //::DPtoLP(get_handle2(), ppoints, nCount);

   }


   void graphics::DPtoLP(::rectangle_f64 * prectangle)
   {

      __throw(todo);
      //::DPtoLP(get_handle2(), (::point_f64 *)rectangle, 2);

   }


   void graphics::LPtoDP(::point_f64 * ppoints, count nCount)
   {

      __throw(todo);
      //::LPtoDP(get_handle2(), ppoints, nCount);

   }


   void graphics::LPtoDP(::rectangle_f64 * prectangle)
   {

      __throw(todo);
      //::LPtoDP(get_handle2(), (::point_f64 *)rectangle, 2);

   }


   void graphics::fill_region(::draw2d::region* pRgn, ::draw2d::brush* pBrush)
   {

      __throw(todo);
      //return ::FillRgn(get_handle1(), (HRGN)pRgn->get_os_data(), (HBRUSH)pBrush->get_os_data()) != false;

      //return false;

   }


   void graphics::frame_region(::draw2d::region* pRgn, ::draw2d::brush* pBrush, double nWidth, double nHeight)
   {

      __throw(todo);
      //return ::FrameRgn(get_handle1(), (HRGN)pRgn->get_os_data(), (HBRUSH)pBrush->get_os_data(), nWidth, nHeight) != false;

      //return false;

   }


   void graphics::invert_region(::draw2d::region* pregion)
   {
      __throw(todo);

      //ASSERT(get_handle1() != nullptr);

      //return ::InvertRgn(get_handle1(), (HRGN)pRgn->get_os_data()) != false;

      //return false;

   }


   void graphics::paint_region(::draw2d::region* pregion)
   {

      __throw(todo);
      //ASSERT(get_handle1() != nullptr);

      //return ::PaintRgn(get_handle1(), (HRGN)pRgn->get_os_data())  != false;

      //return false;

   }

   //void graphics::PtVisible(double x, double y)
   //{

   //   __throw(todo);
   //   //ASSERT(get_handle1() != nullptr);

   //   //return ::PtVisible(get_handle1(), x, y) != false;

   //}

   //void graphics::PtVisible(const ::point_f64 & point)
   //{

   //   ASSERT(get_handle1() != nullptr);

   //   return PtVisible(point.x, point.y);

   //} // call virtual


   //void graphics::rectVisible(const ::rectangle_i32 & rectangle)
   //{

   //   __throw(todo);

   //   //ASSERT(get_handle1() != nullptr);

   //   //return ::rectVisible(get_handle1(), rectangle) != false;

   //}


   point_f64 graphics::current_position()
   {

      __throw(todo);
      //ASSERT(get_handle2() != nullptr);
      //::point_f64 point;
      //VERIFY(::GetCurrentPositionEx(get_handle2(), &point));
      //return point;

      return nullptr;

   }


   //void graphics::Arc(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4)
   //{

   //   auto ppath = __create < ::draw2d::path > ();

   //   double pi = 3.1415927f;

   //   ::rectangle_f64 rectangle(x1, y1, x2, y2);

   //   double centerx    = (x2 + x1) / 2.0;
   //   double centery    = (y2 + y1) / 2.0;

   //   double start      = atan2(y3 - centery, x3 - centerx) * 180.0 / pi;
   //   double end        = atan2(y4 - centery, x4 - centerx) * 180.0 / pi;

   //   //ppath->begin_figure(false, ::draw2d::fill_mode_winding);
   //   ppath->begin_figure();
   //   ppath->add_arc(rectangle, (int) start, (int) fmod(end + 360.0 - start, 360.0));
   //   //ppath->end_figure(false);
   //   //ppath->close_figure();

   //   return this->path(ppath);

   //}

   void graphics::Arc(double x1, double y1, double w, double h, angle start, angle extends)
   {

      auto ppath = __create < ::draw2d::path > ();

      ::rectangle_i32 rectangle((::i32) x1, (::i32)y1, (::i32)(x1 + w), (::i32)(y1 + h));

      //ppath->begin_figure(false, ::draw2d::fill_mode_winding);
      ppath->begin_figure();
      ppath->add_arc(rectangle, start, extends);
      //ppath->end_figure(false);
      //ppath->close(false);

      return this->path(ppath);

   }


   void graphics::Arc(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4)
   {

      auto ppath = __create < ::draw2d::path > ();

      double pi = 3.1415927f;

      ::rectangle_i32 rectangle((::i32)x1, (::i32)y1, (::i32)x2, (::i32)y2);

      double centerx = (x2 + x1) / 2.0;
      double centery = (y2 + y1) / 2.0;

      double start = atan2(y3 - centery, x3 - centerx) * 180.0 / pi;
      double end = atan2(y4 - centery, x4 - centerx) * 180.0 / pi;

      //ppath->begin_figure(false, ::draw2d::fill_mode_winding);
      ppath->begin_figure();
      ppath->add_arc(rectangle, start, end - start);
      //ppath->end_figure(false);

      return this->path(ppath);

   }


   void graphics::Arc(const ::rectangle_f64 & rectangle, const ::point_f64 & pointStart, const ::point_f64 & pointEnd)
   {

      //ASSERT(get_handle1() != nullptr);

      Arc(rectangle.left, rectangle.top, rectangle.right, rectangle.bottom, pointStart.x, pointStart.y, pointEnd.x, pointEnd.y);


   }


   void graphics::polyline(const ::point_f64* ppoints, count nCount)
   {

      auto ppath = __create < ::draw2d::path > ();

      //ppath->begin_figure(false, ::draw2d::fill_mode_winding);

      ppath->begin_figure();

      ppath->add_lines(ppoints, nCount);

      //ppath->end_figure(false);

      //ppath->end_figure();

      this->draw_path(ppath);

   }


   void graphics::frame_rectangle(const ::rectangle_f64 & rectangleParam, ::draw2d::brush* pbrush)
   {

      if (m_prendertarget == nullptr)
      {

         throw_status(error_null_pointer);

      }

      D2D1_RECT_F rectangle;

      __copy(rectangle, rectangleParam);

      m_prendertarget->DrawRectangle(rectangle, pbrush->get_os_data < ID2D1Brush * > (this));

      //return true;

   }


   void graphics::invert_rectangle(const ::rectangle_f64 & rectangle)
   {

      __throw(todo);

      //return false;

   }


   //void graphics::DrawIcon(double x, double y, ::draw2d::icon * picon)
   //{
   //   __throw(todo);
   //   //ASSERT(get_handle1() != nullptr);

   //   //if(picon == nullptr)
   //   //   return false;

   //   //return ::DrawIcon(get_handle1(), x, y, picon->m_hicon) != false;

   //}

   //void graphics::DrawIcon(const ::point_f64 & point, ::draw2d::icon * picon)
   //{
   //   __throw(todo);
   //   //ASSERT(get_handle1() != nullptr);

   //   //if(picon == nullptr)
   //   //   return false;

   //   //return ::DrawIcon(get_handle1(), point.x, point.y, picon->m_hicon) != false;

   //}

//   void graphics::draw(const ::rectangle_f64 & rectangleTarget, ::draw2d::icon * picon)
//   {
//
//#ifdef _UWP
//
//      //throw interface_only_exception();
//      return false;
//
//#else
//
//      try
//      {
//
//         if(picon == nullptr)
//            return false;
//
//         if(m_prendertarget == nullptr)
//            return false;
//
//         bool bOk = false;
//
//         BITMAPINFO info;
//         ::color::color * pcolorref;
//
//         ZeroMemory(&info, sizeof (BITMAPINFO));
//
//         info.bmiHeader.biSize          = sizeof (BITMAPINFOHEADER);
//         info.bmiHeader.biWidth         = rectangleTarget.width();
//         info.bmiHeader.biHeight        = - rectangleTarget.height();
//         info.bmiHeader.biPlanes        = 1;
//         info.bmiHeader.biBitCount      = 32;
//         info.bmiHeader.biCompression   = BI_RGB;
//         info.bmiHeader.biSizeImage     = rectangleTarget.area() * 4;
//
//         HBITMAP hbitmap = ::CreateDIBSection(nullptr, &info, DIB_RGB_COLORS, (void **) &pcolorref, nullptr, 0);
//
//         HDC hdc = ::CreateCompatibleDC(nullptr);
//
//         HBITMAP hbitmapOld = (HBITMAP) ::SelectObject(hdc, hbitmap);
//
//         int istepIfAniCur = 0;
//         if(::DrawIconEx(hdc, 0, 0, (HICON) picon->m_picon, rectangleTarget.width(), rectangleTarget.height(), istepIfAniCur, nullptr, DI_IMAGE | DI_MASK))
//         {
//
//            ::SelectObject(hdc, hbitmapOld);
//
//            try
//            {
//
//               //Gdiplus::Bitmap b(cx, cy, cx * 4 , PixelFormat32bppARGB, (byte *) pcolorref);
//
//               ::draw2d::bitmap_pointer b(e_create);
//
//               b->CreateBitmap(this, rectangleTarget.size(), 1, 32, pcolorref, rectangleTarget.width() * sizeof(::color32_t));
//
//               D2D1_RECT_F rectangle_i32;
//
//               __copy(rectangle, rectangleTarget);
//               //rectangle.left   = (FLOAT) rectangle_i32;
//               //rectangle.top    = (FLOAT) y;
//               //rectangle.right  = rectangle.left    + cx;
//               //rectangle.bottom = rectangle.top     + cy;
//
//               m_prendertarget->DrawBitmap((ID2D1Bitmap *) b->get_os_data(), rectangle);
//
//            }
//            catch(...)
//            {
//            }
//
//         }
//
//         ::DeleteDC(hdc);
//
//         ::DeleteObject(hbitmap);
//
//         return bOk;
//
//      }
//      catch(...)
//      {
//      }
//
//      return false;
//
//#endif
//
//   }
//
//   void graphics::DrawState(const ::point_f64 & point, const ::size_f64& ::size_f64, HBITMAP hBitmap, ::u32 nFlags, HBRUSH hBrush)
//   {
//
//      __throw(todo);
//      //ASSERT(get_handle1() != nullptr);
//      //return ::DrawState(get_handle1(), hBrush, nullptr, (LPARAM)hBitmap, 0, point.x, point.y, size.cx, size.cy, nFlags|DST_BITMAP) != false;
//
//   }
//
//   void graphics::DrawState(const ::point_f64 & point, const ::size_f64& ::size_f64, ::draw2d::bitmap* pBitmap, ::u32 nFlags, ::draw2d::brush* pBrush)
//   {
//
//      __throw(todo);
//
//      //ASSERT(get_handle1() != nullptr);
//      //return ::DrawState(get_handle1(), (HBRUSH)pBrush->get_os_data(), nullptr, (LPARAM)pBitmap->get_os_data(), 0, point.x, point.y, size.cx, size.cy, nFlags|DST_BITMAP) != false;
//
//   }
//
//   void graphics::DrawState(const ::point_f64 & point, const ::size_f64& ::size_f64, HICON hIcon, ::u32 nFlags, HBRUSH hBrush)
//   {
//
//
//      __throw(todo);
//
//      //ASSERT(get_handle1() != nullptr);
//      //return ::DrawState(get_handle1(), hBrush, nullptr, (LPARAM)hIcon, 0, point.x, point.y, size.cx, size.cy, nFlags|DST_ICON) != false;
//
//   }
//
//
//   void graphics::DrawState(const ::point_f64 & point, const ::size_f64& ::size_f64, HICON hIcon, ::u32 nFlags, ::draw2d::brush* pBrush)
//   {
//
//
//      __throw(todo);
//
//      //ASSERT(get_handle1() != nullptr);
//      //return ::DrawState(get_handle1(), (HBRUSH)pBrush->get_os_data(), nullptr, (LPARAM)hIcon, 0, point.x, point.y, size.cx, size.cy, nFlags|DST_ICON) != false;
//
//   }
//
//   void graphics::DrawState(const ::point_f64 & point, const ::size_f64& ::size_f64, const ::string & lpszText, ::u32 nFlags, bool bPrefixText, double dTextLen, HBRUSH hBrush)
//   {
//
//      __throw(todo);
//
//      //ASSERT(get_handle1() != nullptr);
//      //return ::DrawState(get_handle1(), hBrush,  nullptr, (LPARAM)lpszText, (WPARAM)nTextLen, point.x, point.y, size.cx, size.cy, nFlags|(bPrefixText ? DST_PREFIXTEXT : DST_TEXT)) != false;
//
//   }
//
//   void graphics::DrawState(const ::point_f64 & point, const ::size_f64& ::size_f64, const ::string & lpszText, ::u32 nFlags, bool bPrefixText, double dTextLen, ::draw2d::brush* pBrush)
//   {
//
//      __throw(todo);
//
//      //ASSERT(get_handle1() != nullptr);
//      //return ::DrawState(get_handle1(), (HBRUSH)pBrush->get_os_data(), nullptr, (LPARAM)lpszText, (WPARAM)nTextLen, point.x, point.y, size.cx, size.cy, nFlags|(bPrefixText ? DST_PREFIXTEXT : DST_TEXT)) != false;
//
//   }
//
//#ifdef WINDOWS_DESKTOP
//
//   /*
//   void graphics::DrawState(const ::point_f64 & point, const ::size_f64& ::size_f64, DRAWSTATEPROC lpDrawProc, LPARAM lData, ::u32 nFlags, HBRUSH hBrush)
//   {
//
//      __throw(todo);
//
//      //ASSERT(get_handle1() != nullptr); return ::DrawState(get_handle1(), hBrush, lpDrawProc, lData, 0, point.x, point.y, size.cx, size.cy, nFlags|DST_COMPLEX) != false;
//
//   }
//
//   void graphics::DrawState(const ::point_f64 & point, const ::size_f64& ::size_f64, DRAWSTATEPROC lpDrawProc, LPARAM lData, ::u32 nFlags, ::draw2d::brush* pBrush)
//   {
//
//      __throw(todo);
//
//      //ASSERT(get_handle1() != nullptr); return ::DrawState(get_handle1(), (HBRUSH)pBrush->get_os_data(), lpDrawProc, lData, 0, point.x, point.y, size.cx, size.cy, nFlags|DST_COMPLEX) != false;
//
//   }
//   */
//
//#endif
//
//
   //void graphics::DrawEdge(::rectangle_f64 * prectangle, ::u32 nEdge, ::u32 nFlags)
   //{

   //   __throw(todo);

   //   ASSERT(get_handle1() != nullptr);

   //   //return ::DrawEdge(get_handle1(), rectangle, nEdge, nFlags) != false;

   //}


   //void graphics::DrawFrameControl(::rectangle_f64 * prectangle, ::u32 nType, ::u32 nState)
   //{

   //   __throw(todo);

   //   //ASSERT(get_handle1() != nullptr); return ::DrawFrameControl(get_handle1(), rectangle, nType, nState) != false;

   //}

   void graphics::Chord(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4)
   {

      __throw(todo);

      //ASSERT(get_handle1() != nullptr);

      //return ::Chord(get_handle1(), x1, y1, x2, y2, x3, y3, x4, y4) != false;

      //return false;

   }

   void graphics::Chord(const ::rectangle_f64 & rectangle, const ::point_f64 & pointStart, const ::point_f64 & pointEnd)
   {

      __throw(todo);

      //ASSERT(get_handle1() != nullptr);

      //return ::Chord(get_handle1(), rectangle.left, rectangle.top, rectangle.right, rectangle.bottom, pointStart.x, pointStart.y, pointEnd.x, pointEnd.y) != false;

      //return false;

   }


   void graphics::DrawFocusRect(const ::rectangle_f64 & rectangle)
   {

      __throw(todo);

//      ASSERT(get_handle1() != nullptr);

   }


   void graphics::fill_ellipse(const ::rectangle_f64 & rectangle)
   {

      if (m_pbrush.is_null())
      {

         //throw_status(error_null_pointer);return false;

      }

      auto pbrush = m_pbrush->get_os_data < ID2D1Brush * >(this);

      if (!pbrush)
      {

         //return false;

         throw_status(error_null_pointer);

      }

      D2D1_ELLIPSE ellipse;

      ellipse.point.x = (float)(rectangle.right + rectangle.left) / 2.f;
      ellipse.point.y = (float)(rectangle.bottom + rectangle.top) / 2.f;
      ellipse.radiusX = (float)(rectangle.right - rectangle.left) / 2.f;
      ellipse.radiusY = (float)(rectangle.bottom - rectangle.top) / 2.f;

      defer_primitive_blend();

      m_pdevicecontext->FillEllipse(&ellipse, pbrush);

      //return true;

   }


   void graphics::draw_ellipse(const ::rectangle_f64 & rectangle)
   {

      if (m_ppen.is_null())
      {

         //return false;

         throw_status(error_null_pointer);

      }

      auto pbrush = m_ppen->get_os_data < ID2D1Brush * >(this);

      if (!pbrush)
      {

         //return false;

         throw_status(error_null_pointer);

      }

      D2D1_ELLIPSE ellipse;

      ellipse.point.x = (float)(rectangle.right + rectangle.left) / 2.f;
      ellipse.point.y = (float)(rectangle.bottom + rectangle.top) / 2.f;
      ellipse.radiusX = (float)(rectangle.right - rectangle.left) / 2.f;
      ellipse.radiusY = (float)(rectangle.bottom - rectangle.top) / 2.f;

      defer_primitive_blend();

      m_pdevicecontext->DrawEllipse(&ellipse, pbrush,(FLOAT) m_ppen->m_dWidth);

      //return true;

   }


   void graphics::Pie(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4)
   {

      __throw(todo);

      //return false;

   }


   void graphics::Pie(const ::rectangle_f64 & rectangle, const ::point_f64 & pointStart, const ::point_f64 & pointEnd)
   {

      __throw(todo);

      //return false;

   }


   void graphics::polygon(const POINT_F64 * ppoints, count count)
   {

      if (count < 3)
      {

         //return false;

         throw_status(error_null_pointer);

      }

      comptr<ID2D1PathGeometry> pgeometry;

      HRESULT hr = ::direct2d::direct2d()->d2d1_factory1()->CreatePathGeometry(&pgeometry);

      {

         comptr<ID2D1GeometrySink> psink;

         pgeometry->Open(&psink);

         psink->SetFillMode(D2D1_FILL_MODE_WINDING);

         psink->BeginFigure(D2D1::Point2F((FLOAT) ppoints->x, (FLOAT)ppoints->y), D2D1_FIGURE_BEGIN_FILLED);

         for (index i = 1; i < count; i++)
         {

            psink->AddLine({ (FLOAT)ppoints[i].x, (FLOAT)ppoints[i].y });

         }

         psink->EndFigure(D2D1_FIGURE_END_CLOSED);

         psink->Close();

      }

      //bool bOk = 
      
      fill_path(pgeometry, m_pbrush);

      //bOk = bOk && 
      
      draw_path(pgeometry, m_ppen);

      //return bOk;

   }


   void graphics::draw_polygon(const POINT_F64 * ppoints, count count)
   {

      if (count < 3)
      {

         //return false;

         throw_status(error_invalid_empty_argument);

      }

      comptr<ID2D1PathGeometry> pgeometry;

      HRESULT hr = ::direct2d::direct2d()->d2d1_factory1()->CreatePathGeometry(&pgeometry);

      {

         comptr<ID2D1GeometrySink> psink;

         pgeometry->Open(&psink);

         psink->SetFillMode(D2D1_FILL_MODE_WINDING);

         psink->BeginFigure(D2D1::Point2F((FLOAT)ppoints->x, (FLOAT)ppoints->y), D2D1_FIGURE_BEGIN_FILLED);

         for (index i = 1; i < count; i++)
         {

            psink->AddLine({ (FLOAT)ppoints[i].x, (FLOAT)ppoints[i].y });

         }

         psink->EndFigure(D2D1_FIGURE_END_CLOSED);

         psink->Close();

      }

      //bool bOk = 
      
      draw_path(pgeometry, m_ppen);

      //return bOk;

   }


   void graphics::fill_polygon(const POINT_F64 * ppoints, count count)
   {

      if (count < 3)
      {

         //return false;

         throw_status(error_invalid_empty_argument);

      }

      comptr<ID2D1PathGeometry> pgeometry;

      HRESULT hr = ::direct2d::direct2d()->d2d1_factory1()->CreatePathGeometry(&pgeometry);

      {

         comptr<ID2D1GeometrySink> psink;

         pgeometry->Open(&psink);

         psink->SetFillMode(D2D1_FILL_MODE_WINDING);

         psink->BeginFigure(D2D1::Point2F((FLOAT)ppoints->x, (FLOAT)ppoints->y), D2D1_FIGURE_BEGIN_FILLED);

         for (index i = 1; i < count; i++)
         {

            psink->AddLine({ (FLOAT)ppoints[i].x, (FLOAT)ppoints[i].y });

         }

         psink->EndFigure(D2D1_FIGURE_END_CLOSED);

         psink->Close();

      }

      //bool bOk = 
      
      fill_path(pgeometry, m_pbrush);

      //return bOk;

   }


   void graphics::rectangle(const ::rectangle_f64 & rectangle)
   {

      //bool bOk1;

      //bOk1 = 
      
      fill_rectangle(rectangle);

      //bool bOk2;

      //bOk2 = 
      
      draw_rectangle(rectangle);

      //return bOk1 && bOk2;

   }


   void graphics::draw_rectangle(const ::rectangle_f64 & rectangleParam, ::draw2d::pen * ppen)
   {

      if (ppen == nullptr)
      {

         //return false;

         throw_status(error_null_pointer);

      }

      if (ppen->m_epen == ::draw2d::e_pen_null)
      {

         //return true;

         return;

      }

      D2D1_RECT_F rectangle;

      __copy(rectangle, rectangleParam);

      ::draw2d_direct2d::pen * ppen2 = dynamic_cast <::draw2d_direct2d::pen *> (ppen);

      defer_primitive_blend();

      m_prendertarget->DrawRectangle(&rectangle, ppen2->get_os_data < ID2D1Brush * >(this), (FLOAT)ppen->m_dWidth);

      //return true;

   }


   void graphics::fill_rectangle(const ::rectangle_f64 & rectangleParam, ::draw2d::brush * pbrush)
   {

      if (pbrush == nullptr)
      {

         throw_status(error_null_pointer);

      }

      if (pbrush->m_etype == ::draw2d::brush::e_type_null)
      {

         return;

      }

      D2D1_RECT_F rectangle;

      __copy(rectangle, rectangleParam);

      ::draw2d_direct2d::brush * pbrush2 = dynamic_cast <::draw2d_direct2d::brush *> (pbrush);

      defer_primitive_blend();

      m_prendertarget->FillRectangle(rectangle, pbrush2->get_os_data < ID2D1Brush * >(this));

      //return true;

   }


   void graphics::round_rectangle(const ::rectangle_f64 & rectangleParam, double dRadius)
   {

      D2D1_ROUNDED_RECT rectangle;

      __copy(rectangle, rectangleParam);

      rectangle.radiusX = (FLOAT) dRadius;

      if (m_pbrush.is_set() && m_pbrush->m_etype != ::draw2d::brush::e_type_null)
      {

         auto pbrush = m_pbrush.cast <::draw2d_direct2d::brush > ();

         defer_primitive_blend();

         m_prendertarget->FillRoundedRectangle(rectangle, pbrush->get_os_data < ID2D1Brush * > (this));

      }

      if (m_pbrush.is_set() && m_pbrush->m_etype != ::draw2d::brush::e_type_null)
      {

         auto ppen = m_ppen.cast <::draw2d_direct2d::pen >();
         
         defer_primitive_blend();

         m_prendertarget->DrawRoundedRectangle(rectangle, ppen->get_os_data < ID2D1Brush * > (this), (FLOAT) ppen->m_dWidth);

      }

      //return true;

   }


   //void graphics::PatBlt(double x, double y, double dWidth, double dHeight)
   //{

   //   __throw(todo);

   //}


   void graphics::_draw_raw(const ::rectangle_f64 & rectangleTarget, ::image * pimage, const image_drawing_options & imagedrawingoptions, const ::point_f64 & pointSrc)
   {

      ::draw2d::lock draw2dlock;

      ::draw2d::device_lock devicelock(this);

      //try
      //{

         if (pimage == nullptr || pimage->get_bitmap() == nullptr)
         {

            //return false;

            throw_status(error_null_pointer);

         }

         //if (pgraphicsSrc->get_current_bitmap() == nullptr)
         //{

         //   return false;

         //}

         //if (pgraphicsSrc->get_current_bitmap()->get_os_data() == nullptr)
         //{

         //   return false;

         //}

         double x = rectangleTarget.left;
         double y = rectangleTarget.top;
         double nWidth = rectangleTarget.width();
         double nHeight = rectangleTarget.height();
         double xSrc = pointSrc.x;
         double ySrc = pointSrc.y;

         if (x < 0)
         {

            xSrc -= x;

            nWidth += x;

            x = 0;

         }

         if (y < 0)
         {

            ySrc -= y;

            nHeight += y;

            y = 0;

         }

         if (get_current_bitmap() && get_current_bitmap()->is_set())
         {

            D2D1_SIZE_U sz = get_current_bitmap()->get_os_data < ID2D1Bitmap * >()->GetPixelSize();

            if (nWidth + x + GetViewportOrg().x > sz.width)
            {

               nWidth = sz.width - x - GetViewportOrg().x;

            }

            if (nHeight + y + GetViewportOrg().y > sz.height)
            {

               nHeight = sz.height - y - GetViewportOrg().y;

            }

         }

         {

            D2D1_SIZE_U sz = ((ID2D1Bitmap *)pimage->get_bitmap()->get_os_data())->GetPixelSize();

            if (nWidth + xSrc > sz.width)
            {

               nWidth = sz.width - xSrc;

            }

            if (nHeight + ySrc > sz.height)
            {

               nHeight = sz.height - ySrc;

            }

         }

         {

            D2D1_RECT_F rectangleTarget = D2D1::RectF((float)x, (float)y, (float)(x + nWidth), (float)(y + nHeight));

            D2D1_RECT_F rectangleSource = D2D1::RectF((float)xSrc, (float)ySrc, (float)(xSrc + nWidth), (float)(ySrc + nHeight));

            auto pd2d1bitmap = ((ID2D1Bitmap*)pimage->get_bitmap()->get_os_data());

            int cx = pd2d1bitmap->GetPixelSize().width;

            int cy = pd2d1bitmap->GetPixelSize().height;

            pimage->unmap();

            HRESULT hrFlush = ((ID2D1DeviceContext*)pimage->g()->get_os_data())->Flush();

            HRESULT hrEndDraw = ((ID2D1DeviceContext*)pimage->g()->get_os_data())->EndDraw();

            defer_primitive_blend();

            m_pdevicecontext->DrawBitmap(pd2d1bitmap, rectangleTarget, 1.0, D2D1_INTERPOLATION_MODE_NEAREST_NEIGHBOR, rectangleSource);

            if (SUCCEEDED(hrEndDraw))
            {

               ((ID2D1DeviceContext*)pimage->g()->get_os_data())->BeginDraw();

            }

         }

         //return true;

      //}
      //catch (...)
      //{

      //   return false;

      //}

   }


   void graphics::_stretch_raw(const ::rectangle_f64 & rectangleTarget, ::image * pimage, const image_drawing_options & imagedrawingoptions, const ::rectangle_f64 & rectangleSource)
   {

      //try
      //{

         double xDst = rectangleTarget.left;
         double yDst = rectangleTarget.top;
         double nDstWidth = rectangleTarget.width();
         double nDstHeight = rectangleTarget.height();
         double xSrc = rectangleSource.left;
         double ySrc = rectangleSource.top;
         double nSrcWidth = rectangleSource.width();
         double nSrcHeight = rectangleSource.height();

         if (::nok(pimage))
         {

            //return false;

            throw_status(error_failed);

         }

         if (pimage->get_bitmap() == nullptr)
         {

            //return false;

            throw_status(error_failed);

         }

         if (pimage->get_graphics() == nullptr)
         {

            //return false;

            throw_status(error_failed);

         }

         {

            D2D1_RECT_F rectangleTarget = D2D1::RectF((float)xDst, (float)yDst, (float)(xDst + nDstWidth), (float)(yDst + nDstHeight));

            D2D1_RECT_F rectangleSource = D2D1::RectF((float)xSrc, (float)ySrc, (float)(xSrc + nSrcWidth), (float)(ySrc + nSrcHeight));

            pimage->unmap();

            HRESULT hrFlush = ((ID2D1DeviceContext*)pimage->g()->get_os_data())->Flush();

            HRESULT hrEndDraw = ((ID2D1DeviceContext*)pimage->g()->get_os_data())->EndDraw();

            defer_primitive_blend();

            if (m_prendertarget != nullptr)
            {

               m_prendertarget->DrawBitmap((ID2D1Bitmap*)pimage->get_bitmap()->get_os_data(), &rectangleTarget, 1.0, m_bitmapinterpolationmode, &rectangleSource);

            }
            else
            {

               m_pdevicecontext->DrawBitmap((ID2D1Bitmap*)pimage->g()->get_current_bitmap()->get_os_data(), rectangleTarget, 1.0, m_interpolationmode, rectangleSource);

            }

            if (SUCCEEDED(hrEndDraw))
            {

               ((ID2D1DeviceContext*)pimage->g()->get_os_data())->BeginDraw();

            }

         }


         //else
         //{

         //   output_debug_string("direct2d graphics::StretchBltRaw hr failed");

         //}

         //return true;

      //}
      //catch (...)
      //{

      //}

      //return false;

   }


   //void graphics::_draw_raw(const ::rectangle_f64 & rectangleTarget, ::draw2d::graphics * pgraphicsSrc, const ::point_f64 & pointSrc)
   //{


   //}


   //void graphics::_stretch_raw(const ::rectangle_f64 & rectangleTarget, ::draw2d::graphics * pgraphicsSrc, const ::rectangle_f64 & rectangleSource)
   //{


   //}


   ::color::color graphics::GetPixel(double x, double y)
   {

      __throw(todo);


      return false;

   }


   ::color::color graphics::GetPixel(const ::point_f64 & point)
   {

      __throw(todo);

      return false;

   }


   ::color::color graphics::SetPixel(double x, double y, const ::color::color & color)
   {

      __throw(todo);

      return false;

   }


   ::color::color graphics::SetPixel(const ::point_f64 & point, const ::color::color & color)
   {

      __throw(todo);

      return false;

   }


//   void graphics::FloodFill(double x, double y, const ::color::color & color)
//   {
//
//      __throw(todo);
//
//   }
//
//
//   void graphics::ExtFloodFill(double x, double y, const ::color::color & color, ::u32 nFillType)
//   {
//
//      __throw(todo);
//
//   }


   //void graphics::text_out(double x, double y, const block & block)
   //{

   //   return text_out(x, y, (const ::string &) block.get_data(), block.get_size());

   //}


   //void graphics::text_out(double x, double y, const ::string & str)
   //{

   //   return text_out((int) x, (int) y, str, (int) str.get_length());

   //}


   //void graphics::ExtTextOut(double x, double y, ::u32 nOptions, const ::rectangle_f64 & rectangle, const ::string & lpszString, strsize nCount, LPINT lpDxWidths)
   //{

   //   __throw(todo);

   //}


   //void graphics::ExtTextOut(double x, double y, ::u32 nOptions, const ::rectangle_f64 & rectangle, const ::string & str, LPINT lpDxWidths)
   //{

   //   __throw(todo);

   //}


   //::size_f64 graphics::TabbedTextOut(double x, double y, const ::string & lpszString, strsize nCount, count nTabPositions, LPINT lpnTabStopPositions, i32 nTabOrigin)
   //{

   //   __throw(todo);

   //}


   //::size_f64 graphics::TabbedTextOut(double x, double y, const ::string & str, count nTabPositions, LPINT lpnTabStopPositions, i32 nTabOrigin)
   //{

   //   __throw(todo);

   //}


   //::size_f64 graphics::GetTabbedTextExtent(const ::string & lpszString, strsize nCount, count nTabPositions, LPINT lpnTabStopPositions)
   //{

   //   __throw(todo);

   //}


   //::size_f64 graphics::GetTabbedTextExtent(const ::string & str, count nTabPositions, LPINT lpnTabStopPositions)
   //{

   //   __throw(todo);

   //}


   //::size_f64 graphics::GetOutputTabbedTextExtent(const ::string & lpszString, strsize nCount, count nTabPositions, LPINT lpnTabStopPositions)
   //{

   //   __throw(todo);

   //   return nullptr;

   //}


   //::size_f64 graphics::GetOutputTabbedTextExtent(const ::string & str, count nTabPositions, LPINT lpnTabStopPositions)
   //{

   //   __throw(todo);

   //   return nullptr;

   //}


   //void graphics::GrayString(::draw2d::brush* pBrush, bool (CALLBACK* lpfnOutput)(HDC, LPARAM, int), LPARAM lpData, double dCount,double x, double y, double dWidth, double dHeight)
   //{

   //   __throw(todo);

   //}


   ::u32 graphics::GetTextAlign()
   {

      __throw(todo);

      return false;

   }


   //int graphics::GetTextFace(count nCount, char * lpszFacename)
   //{

   //   __throw(todo);

   //}


   //int graphics::GetTextFace(string & rString)
   //{

   //   __throw(todo);

   //}


   void graphics::get_text_metrics(::write_text::text_metric * lpMetrics)
   {

      if (m_pfont.is_null())
      {

         __construct(((graphics*)this)->m_pfont);

      }

      if (m_pfont.is_null())
      {

         //return false;

         throw_status(error_failed);

      }

      comptr<IDWriteFontCollection> pcollection;

      WCHAR name[256];
      ::u32 findex;
      BOOL exists;

      auto pwritetextformat = m_pfont->get_os_data < IDWriteTextFormat * >(this);

      if (::is_null(pwritetextformat))
      {

         lpMetrics->m_dAscent = 0;
         lpMetrics->m_dDescent = 0;
         lpMetrics->m_dHeight = 24;
         lpMetrics->m_dInternalLeading = 0;
         lpMetrics->m_dExternalLeading = 0;

         //return true;

         return;

      }

      pwritetextformat->GetFontFamilyName(name, 256);

      pwritetextformat->GetFontCollection(&pcollection);

      if (pcollection == nullptr)
      {

         lpMetrics->m_dAscent = 0;
         lpMetrics->m_dDescent = 0;
         lpMetrics->m_dHeight = 24;
         lpMetrics->m_dInternalLeading = 0;
         lpMetrics->m_dExternalLeading = 0;

         return;

         //return true;

      }

      pcollection->FindFamilyName(name, &findex, &exists);

      if (!exists)
      {

         pcollection->FindFamilyName(L"Arial", &findex, &exists);

         if (!exists)
         {

            lpMetrics->m_dAscent = 0;
            lpMetrics->m_dDescent = 0;
            lpMetrics->m_dHeight = 24;
            lpMetrics->m_dInternalLeading = 0;
            lpMetrics->m_dExternalLeading = 0;

            //return true;

            return;

         }

      }

      comptr<IDWriteFontFamily> ffamily;

      pcollection->GetFontFamily(findex, &ffamily);

      if (ffamily == nullptr)
      {

         lpMetrics->m_dAscent = 0;
         lpMetrics->m_dDescent = 0;
         lpMetrics->m_dHeight = 24;
         lpMetrics->m_dInternalLeading = 0;
         lpMetrics->m_dExternalLeading = 0;

         //return true;

         return;

      }

      comptr<IDWriteFont> pfont;

      ffamily->GetFirstMatchingFont(pwritetextformat->GetFontWeight(), pwritetextformat->GetFontStretch(), pwritetextformat->GetFontStyle(), &pfont);

      if (pfont == nullptr)
      {

         lpMetrics->m_dAscent = 0;
         lpMetrics->m_dDescent = 0;
         lpMetrics->m_dHeight = 24;
         lpMetrics->m_dInternalLeading = 0;
         lpMetrics->m_dExternalLeading = 0;

         //return true;

         return;

      }

      DWRITE_FONT_METRICS metrics;

      pfont->GetMetrics(&metrics);

      double ratio = pwritetextformat->GetFontSize() / (float)metrics.designUnitsPerEm;

      lpMetrics->m_dAscent = (::i32) (metrics.ascent * ratio);
      lpMetrics->m_dDescent = (::i32) (metrics.descent * ratio);
      lpMetrics->m_dInternalLeading = (::i32) 0;
      lpMetrics->m_dExternalLeading = (::i32) (metrics.lineGap * ratio);
      lpMetrics->m_dHeight = (::i32) ((metrics.ascent + metrics.descent + metrics.lineGap) * ratio);

      //return true;

   }


   void graphics::get_output_text_metrics(::write_text::text_metric * lpMetrics)
   {

      __throw(todo);

      //ASSERT(get_handle1() != nullptr);

      //return ::GetTextMetrics(get_handle1(), lpMetrics) != false;

      //return false;

   }


//   int graphics::GetTextCharacterExtra()
//   {
//
//      __throw(todo);
//
//      //ASSERT(get_handle2() != nullptr);
//
//      //return ::GetTextCharacterExtra(get_handle2());
//
//   }
//
//
//   void graphics::GetCharWidth(::u32 nFirstChar, ::u32 nLastChar, LPINT lpBuffer)
//   {
//
//      __throw(todo);
//
//      //ASSERT(get_handle2() != nullptr);
//
//      //return ::GetCharWidth(get_handle2(), nFirstChar, nLastChar, lpBuffer) != false;
//
//   }
//
//   void graphics::GetOutputCharWidth(::u32 nFirstChar, ::u32 nLastChar, LPINT lpBuffer)
//   {
//
//      __throw(todo);
//
//      //ASSERT(get_handle1() != nullptr);
//
//      //return ::GetCharWidth(get_handle1(), nFirstChar, nLastChar, lpBuffer) != false;
//
//   }
//
//   u32 graphics::GetFontLanguageInfo()
//   {
//
//      __throw(todo);
//
//      //ASSERT(get_handle1() != nullptr);
//
//      //return ::GetFontLanguageInfo(get_handle1());
//
//   }
//
//#ifdef WINDOWS_DESKTOP
//
//   /*   u32 graphics::GetCharacterPlacement(const ::string & lpString, double dCount, double dMaxExtent, LPGCP_RESULTS lpResults, u32 dwFlags)
//      {
//
//         __throw(todo);
//
//         //ASSERT(get_handle1() != nullptr);
//
//         //return ::GetCharacterPlacement(get_handle1(), lpString, nCount, nMaxExtent, lpResults, dwFlags);
//
//      }
//
//      u32 graphics::GetCharacterPlacement(string & str, double dMaxExtent, LPGCP_RESULTS lpResults, u32 dwFlags)
//      {
//
//         __throw(todo);
//
//         //ASSERT(get_handle1() != nullptr);
//
//         //return ::GetCharacterPlacement(get_handle1(), (const ::string &)str, (int) str.get_length(), nMaxExtent, lpResults, dwFlags);
//
//      }
//      */
//#endif
//
//   ::size_f64 graphics::GetAspectRatioFilter()
//   {
//
//      __throw(todo);
//
//      //ASSERT(get_handle2() != nullptr);
//      //::size_f64::size_f64;
//      //VERIFY(::GetAspectRatioFilterEx(get_handle2(), &::size_f64));
//      //return ::size_f64;
//
//   }
//
//   void graphics::ScrollDC(int dx, int dy, const ::rectangle_i32 & rectangleScroll, const ::rectangle_i32 & rectangleClip, ::draw2d::region* pRgnUpdate, RECTANGLE_I32 * lpRectUpdate)
//   {
//
//      __throw(todo);
//
//      //ASSERT(get_handle1() != nullptr);
//
//      //return ::ScrollDC(get_handle1(), dx, dy, lpRectScroll, lpRectClip, (HRGN)pRgnUpdate->get_os_data(), lpRectUpdate) != false;
//
//   }
//
//   // Printer Escape Functions
//   int graphics::Escape(double dEscape, double dCount, const ::string & lpszInData, LPVOID lpOutData)
//   {
//
//      __throw(todo);
//
//      //ASSERT(get_handle1() != nullptr);
//
//      //return ::Escape(get_handle1(), nEscape, nCount, lpszInData, lpOutData);
//
//   }

   // graphics 3.1 Specific functions
   ::u32 graphics::SetBoundsRect(const ::rectangle_f64 & rectangleBounds, ::u32 flags)
   {

      __throw(todo);

      //ASSERT(get_handle1() != nullptr);

      //return ::SetBoundsRect(get_handle1(), rectangleBounds, flags);

      return false;

   }


   ::u32 graphics::GetBoundsRect(::rectangle_f64 * prectBounds, ::u32 flags)
   {

      __throw(todo);

      //ASSERT(get_handle2() != nullptr);

      //return ::GetBoundsRect(get_handle2(), rectangleBounds, flags);

      return false;

   }


   //void graphics::ResetDC(const DEVMODE* lpDevMode)
   //{

   //   __throw(todo);

   //   //ASSERT(get_handle2() != nullptr);

   //   //return ::ResetDC(get_handle2(), lpDevMode) != nullptr;

   //}

#ifdef WINDOWS_DESKTOP

   /*   ::u32 graphics::GetOutlineTextMetrics(::u32 cbData, LPOUTLINETEXTMETRICW lpotm)
      {

         __throw(todo);

         //ASSERT(get_handle2() != nullptr);

         //return ::GetOutlineTextMetrics(get_handle2(), cbData, lpotm);

      }

      void graphics::GetCharABCWidths(::u32 nFirstChar, ::u32 nLastChar, LPABC lpabc)
      {

         __throw(todo);

         //ASSERT(get_handle2() != nullptr);

         //return ::GetCharABCWidths(get_handle2(), nFirstChar, nLastChar, lpabc) != false;

      }
      */
#endif

//   u32 graphics::GetFontData(u32 dwTable, u32 dwOffset, LPVOID lpData, u32 cbData)
//   {
//
//      __throw(todo);
//
//      //ASSERT(get_handle2() != nullptr);
//
//      //return ::GetFontData(get_handle2(), dwTable, dwOffset, lpData, cbData);
//
//   }
//
//#ifdef WINDOWS_DESKTOP
//
//   /*
//   int graphics::GetKerningPairs(double dPairs, LPKERNINGPAIR lpkrnpair)
//   {
//
//      __throw(todo);
//
//      //ASSERT(get_handle2() != nullptr);
//
//      //return ::GetKerningPairs(get_handle2(), nPairs, lpkrnpair);
//
//   }
//
//   u32 graphics::GetGlyphOutline(::u32 nChar, const ::e_align & ealign, const ::e_draw_text & edrawtext, LPGLYPHMETRICS lpgm,  u32 cbBuffer, LPVOID lpBuffer, const MAT2* lpmat2)
//   {
//
//      __throw(todo);
//
//      //ASSERT(get_handle2() != nullptr);
//
//      //return ::GetGlyphOutline(get_handle2(), nChar, nFormat, lpgm, cbBuffer, lpBuffer, lpmat2);
//
//   }
//
//   // ::userbase::document handling functions
//   int graphics::StartDoc(LPDOCINFO lpDocInfo)
//   {
//
//      __throw(todo);
//
//      //ASSERT(get_handle1() != nullptr);
//
//      //return ::StartDoc(get_handle1(), lpDocInfo);
//
//   }
//   */
//#endif

   int graphics::StartPage()
   {

      __throw(todo);

      //ASSERT(get_handle1() != nullptr);

      //return ::StartPage(get_handle1());

      return 0;

   }

   int graphics::EndPage()
   {

      __throw(todo);

      //ASSERT(get_handle1() != nullptr);

      //return ::EndPage(get_handle1());
      return 0;

   }


   //int graphics::SetAbortProc(bool (CALLBACK* lpfn)(HDC, int))
   //{

   //   __throw(todo);

   //   //ASSERT(get_handle1() != nullptr);

   //   //return ::SetAbortProc(get_handle1(), (ABORTPROC)lpfn);

   //}


   int graphics::AbortDoc()
   {

      __throw(todo);

      //ASSERT(get_handle1() != nullptr);

      //return ::AbortDoc(get_handle1());

      return 0;

   }


   int graphics::EndDoc()
   {

      __throw(todo);

      //ASSERT(get_handle1() != nullptr);

      //return ::EndDoc(get_handle1());

      return 0;

   }


//   void graphics::MaskBlt(double x, double y, double dWidth, double dHeight, ::draw2d::graphics * pgraphicsSrc, double xSrc, double ySrc, ::draw2d::bitmap& maskBitmap, double xMask, double yMask)
//   {
//
//      __throw(todo);
//
//      //ASSERT(get_handle1() != nullptr);
//
//      //return ::MaskBlt(get_handle1(), x, y, nWidth, nHeight, WIN_HDC(pgraphicsSrc), xSrc, ySrc,  (HBITMAP)maskBitmap.get_os_data(), xMask, yMask, dwRop) != false;
//
//   }
//
//
//   void graphics::PlgBlt(::point_f64 * lpPoint, ::draw2d::graphics * pgraphicsSrc, double xSrc, double ySrc, double dWidth, double dHeight, ::draw2d::bitmap& maskBitmap, double xMask, double yMask)
//   {
//
//      __throw(todo);
//
//      //ASSERT(get_handle1() != nullptr);
//
//      //return ::PlgBlt(get_handle1(), lpPoint, WIN_HDC(pgraphicsSrc), xSrc, ySrc, nWidth, nHeight, (HBITMAP)maskBitmap.get_os_data(), xMask, yMask) != false;
//
//   }


   //void graphics::SetPixelV(double x, double y, const ::color::color & color)
   //{

   //   __throw(todo);

   //   //ASSERT(get_handle1() != nullptr);

   //   //return ::SetPixelV(get_handle1(), x, y, crColor) != false;

   //   return false;

   //}

   //void graphics::SetPixelV(const ::point_f64 & point, const ::color::color & color)
   //{

   //   __throw(todo);

   //   //ASSERT(get_handle1() != nullptr);

   //   //return ::SetPixelV(get_handle1(), point.x, point.y, crColor) != false;

   //   return false;

   //}

   void graphics::AngleArc(double x, double y, double nRadius, angle fStartAngle, angle fSweepAngle)
   {

      __throw(todo);

      //ASSERT(get_handle1() != nullptr);

      //return ::AngleArc(get_handle1(), x, y, nRadius, fStartAngle, fSweepAngle) != false;

      //return false;

   }


   void graphics::ArcTo(const ::rectangle_f64 & rectangle, const ::point_f64 & pointStart, const ::point_f64 & pointEnd)
   {

      __throw(todo);

      //ASSERT(get_handle1() != nullptr);

      //return ArcTo(rectangle.left, rectangle.top, rectangle.right, rectangle.bottom, pointStart.x, pointStart.y, pointEnd.x, pointEnd.y);
      //return false;


   }

   //int graphics::GetArcDirection()
   //{

   //   __throw(todo);

   //   //ASSERT(get_handle2() != nullptr);

   //   //return ::GetArcDirection(get_handle2());

   //}

   void graphics::poly_polyline(const ::point_f64* ppoints, const ::i32 * lpPolyPoints, count nCount)
   {

      __throw(todo);

      //ASSERT(get_handle1() != nullptr);

      //return ::poly_polyline(get_handle1(), ppoints, lpPolyPoints, nCount) != false;
      //return false;


   }

#ifdef WINDOWS_DESKTOP
   /*
      void graphics::GetColorAdjustment(LPCOLORADJUSTMENT lpColorAdjust)
      {

         __throw(todo);

         //ASSERT(get_handle2() != nullptr);

         //return ::GetColorAdjustment(get_handle2(), lpColorAdjust) != false;

      }
      */
#endif


   void graphics::poly_bezier(const ::point_f64* ppoints, count nCount)
   {

      __throw(todo);

      //ASSERT(get_handle1() != nullptr);

      //return ::poly_bezier(get_handle1(), ppoints, nCount) != false;

      //return false;

   }


//   int graphics::DrawEscape(double dEscape, double dInputSize, const ::string & lpszInputData)
//   {
//
//      __throw(todo);
//
//      //ASSERT(get_handle1() != nullptr);
//
//      //return ::DrawEscape(get_handle1(), nEscape, nInputSize, lpszInputData);
//
//   }
//
//
//   int graphics::Escape(double dEscape, double dInputSize, const ::string & lpszInputData, double dOutputSize, char * lpszOutputData)
//   {
//
//      __throw(todo);
//
//      //ASSERT(get_handle1() != nullptr);
//
//      //return ::ExtEscape(get_handle1(), nEscape, nInputSize, lpszInputData, nOutputSize, lpszOutputData);
//
//   }
//
//
//#ifdef WINDOWS_DESKTOP
//   /*
//      void graphics::GetCharABCWidths(::u32 nFirstChar, ::u32 nLastChar, LPABCFLOAT lpABCF)
//      {
//
//         __throw(todo);
//
//         //ASSERT(get_handle2() != nullptr);
//         //
//         //return ::GetCharABCWidthsFloat(get_handle2(), nFirstChar, nLastChar, lpABCF) != false;
//
//      }
//      */
//#endif
//
//   void graphics::GetCharWidth(::u32 nFirstChar, ::u32 nLastChar, float* lpFloatBuffer)
//   {
//
//      __throw(todo);
//
//      //ASSERT(get_handle2() != nullptr);
//
//      //return ::GetCharWidthFloat(get_handle2(), nFirstChar, nLastChar, lpFloatBuffer) != false;
//
//   }


   void graphics::abort_path()
   {

      __throw(todo);

      //if(m_ppath != nullptr)
      //{
      //   delete m_ppath;
      //   m_ppath = nullptr;
      //}
      //return true;

      //return false;

   }


   void graphics::begin_path()
   {

      __throw(todo);

      //return false;

   }


   void graphics::close_figure()
   {

      __throw(todo);

      //ASSERT(m_ppath != nullptr);
      //return m_ppath->CloseFigure() == Gdiplus::Status::Ok;

      //return false;

   }


   void graphics::end_path()
   {

      __throw(todo);

      //if(m_ppath == nullptr)
      //   return false;

      //m_ppathPaint   = m_ppath;
      //m_ppath        = nullptr;

      //return true;

      //return false;

   }


   void graphics::fill_path()
   {

      __throw(todo);

      //return m_prendertarget->FillPath(direct2d_brush(), m_ppath) == Gdiplus::Status::Ok;

      //return false;


   }


   void graphics::flatten_path()
   {

      __throw(todo);

      //return m_ppath->Flatten() == Gdiplus::Status::Ok;
      //return false;


   }


   float graphics::GetMiterLimit()
   {

      __throw(todo);

      //ASSERT(get_handle1() != nullptr);
      //float fMiterLimit;
      //VERIFY(::GetMiterLimit(get_handle1(), &fMiterLimit));
      //return fMiterLimit;

      return 0.f;

   }


   //i32 graphics::GetPath(::point_f64 * ppoints, byte * lpTypes, ::count nCount)
   //{

   //   __throw(todo);

   //   //ASSERT(get_handle1() != nullptr);
   //   //return ::GetPath(get_handle1(), ppoints, lpTypes, nCount);

   //}


   void graphics::SetMiterLimit(float fMiterLimit)
   {

      __throw(todo);

      //ASSERT(get_handle1() != nullptr);
      //return ::SetMiterLimit(get_handle1(), fMiterLimit, nullptr) != false;

      //return false;

   }


   void graphics::stroke_and_fill_path()
   {

      __throw(todo);

      //bool bOk1 = m_prendertarget->FillPath(direct2d_brush(), m_ppathPaint) == Gdiplus::Status::Ok;

      //bool bOk2 = m_prendertarget->DrawPath(direct2d_pen(), m_ppathPaint) == Gdiplus::Status::Ok;

      //return bOk1 && bOk2;

      //return false;

   }


   void graphics::stroke_path()
   {

      __throw(todo);

//      return m_prendertarget->DrawPath(direct2d_pen(), m_ppathPaint) == Gdiplus::Status::Ok;

      //return false;

   }


   void graphics::widen_path()
   {

      __throw(todo);


//      return m_ppath->Widen(direct2d_pen()) == Gdiplus::Status::Ok;

      //return false;

   }


   void graphics::AddMetaFileComment(::u32 nDataSize, const byte* pCommentData)
   {

      __throw(todo);


      //ASSERT(get_handle1() != nullptr);
      //return ::GdiComment(get_handle1(), nDataSize, pCommentData) != false;

      //return false;

   }


   /*bool CALLBACK metaCallback(
   EmfPlusRecordType recordType,
   unsigned int flags,
   unsigned int dataSize,
   const unsigned char* pStr,
   void* callbackData)
   {
   // Play only EmfPlusRecordTypeFillEllipse records.
   if (recordType == EmfPlusRecordTypeFillEllipse)
   {
   // Explicitly cast callbackData as a metafile pointer, and use it to call
   // the PlayRecord method.
   static_cast < Metafile* > (callbackData)->PlayRecord(recordType, flags, dataSize, pStr);
   }
   return true;
   }

   VOID Example_EnumerateMetafile9(HDC hdc)
   {
   Graphics graphics(hdc);
   // Create a Metafile object from an existing disk metafile.
   Metafile* pMeta = new Metafile(L"SampleMetafile.emf", hdc);
   {
   // Fill a rectangle_i32 and an ellipse in pMeta.
   Graphics metaGraphics(pMeta);
   metaGraphics.fill_rectangle(&SolidBrush(Color(255, 0, 0, 0)), 0, 0, 100, 100);
   metaGraphics.FillEllipse(&SolidBrush(Color(255, 255, 0, 0)), 100, 0, 200, 100);
   }
   // Enumerate pMeta to the destination rectangle, passing pMeta as the callback data.
   graphics.EnumerateMetafile(
   pMeta,
   Rect(0, 0, 300, 50),
   metaCallback,
   pMeta);
   // Draw pMeta as an pimage->
   graphics.DrawImage(pMeta, Point(0, 150));
   delete pMeta;;
   }*/
   //void graphics::PlayMetaFile(HENHMETAFILE hEnhMF, const ::rectangle_f64 & rectangleBounds)
   //{

   //   __throw(todo);

   //   //Gdiplus::RectF rectangle_i32((Gdiplus::REAL) lpBounds->left, (Gdiplus::REAL) lpBounds->top, (Gdiplus::REAL) width(lpBounds), (Gdiplus::REAL) height(lpBounds));

   //   //Gdiplus::Metafile* pMeta = new Gdiplus::Metafile(hEnhMF, false);

   //   ////m_pgraphcis->EnumerateMetafile(pMeta, rectangle, metaCallback, PMETAHEADER);

   //   //bool bOk = m_prendertarget->DrawImage(pMeta, rectangle) == Gdiplus::Status::Ok;

   //   //delete pMeta;

   //   //return bOk ? true : false;
   //   ////return ::PlayEnhMetaFile(get_handle1(), hEnhMF, lpBounds);

   //}

   // true blend
   // COLOR_DEST = SRC_ALPHA * COLOR_SRC  + (1 - SRC_ALPHA) * COLOR_DST

   // double blend
   // COLOR_DEST = SRC_ALPHA * BLEND_ALPHA * COLOR_SRC  + (1 - SRC_ALPHA * BLEND_ALPHA) * COLOR_DST

   // Thank you
   // Jiju George T
   // Web Developer
   // India India
   // Member


   //void graphics::_alpha_blend_raw(const ::rectangle_f64 & rectangleTarget, ::draw2d::graphics * pgraphicsSrc, const ::rectangle_f64 & rectangleSource, double dRate)
   //{

   //   ::draw2d::device_lock devicelock(this);

   //   /*      float fA = (float) dRate;

   //         Gdiplus::ColorMatrix matrix = {
   //            1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
   //            0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
   //            0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
   //            0.0f, 0.0f, 0.0f, fA  , 0.0f,
   //            0.0f, 0.0f, 0.0f, 0.0f, 1.0f
   //         };

   //         Gdiplus::ImageAttributes attributes;

   //         attributes.SetColorMatrix(&matrix, Gdiplus::ColorMatrixFlagsDefault, Gdiplus::ColorAdjustTypeBitmap);

   //         Gdiplus::RectF dstRect((Gdiplus::REAL) xDest, (Gdiplus::REAL) yDest, (Gdiplus::REAL) nDestWidth, (Gdiplus::REAL) nDestHeight);

   //         m_prendertarget->DrawImage((Gdiplus::Bitmap *) pgraphicsSrc->get_current_bitmap()->get_os_data(), dstRect,
   //            (Gdiplus::REAL) xSrc, (Gdiplus::REAL) ySrc, (Gdiplus::REAL) nSrcWidth, (Gdiplus::REAL) nSrcHeight, Gdiplus::UnitPixel, &attributes);*/

   //   try
   //   {

   //      if(pgraphicsSrc == nullptr)
   //         return false;

   //      if(pgraphicsSrc->get_current_bitmap() == nullptr)
   //         return false;

   //      if(pgraphicsSrc->get_current_bitmap()->m_osdata[0] == nullptr)
   //         return false;

   //      //D2D1_RECT_F rectangleTarget = D2D1::RectF((float) xDst, (float) yDst, (float) (xDst + nDstWidth), (float) (yDst + nDstHeight));
   //      //D2D1_RECT_F rectangleSource = D2D1::RectF((float) xSrc, (float) ySrc, (float) (xSrc + nSrcWidth), (float) (ySrc + nSrcHeight));
   //      /*
   //               if (get_current_bitmap() != nullptr && get_current_bitmap()->get_os_data() != nullptr)
   //               {

   //                  D2D1_SIZE_U sz = ((ID2D1Bitmap *)get_current_bitmap()->get_os_data())->GetPixelSize();

   //                  if (natural(nDstWidth + xDst) > sz.width)
   //                     nDstWidth = sz.width - xDst;

   //                  if (natural(nDstHeight + yDst) > sz.height)
   //                     nDstHeight = sz.height - yDst;

   //               }

   //               {

   //                  D2D1_SIZE_U sz = ((ID2D1Bitmap *)pgraphicsSrc->get_current_bitmap()->get_os_data())->GetPixelSize();

   //                  if (natural(nSrcWidth + xSrc) > sz.width)
   //                     nSrcWidth = sz.width - xSrc;

   //                  if (natural(nSrcHeight + ySrc) > sz.height)
   //                     nSrcHeight = sz.height - ySrc;

   //               }*/

   //      D2D1_RECT_F rDst = D2D1::RectF((float)rectangleTarget.left, (float)rectangleTarget.top, (float) rectangleTarget.right, (float) rectangleTarget.bottom);
   //      D2D1_RECT_F rSrc = D2D1::RectF((float)rectangleSource.left, (float)rectangleSource.top, (float) rectangleSource.right, (float) rectangleSource.bottom);


   //      //dynamic_cast <::draw2d_direct2d::graphics *> (pgraphicsSrc)->SaveClip();

   //      HRESULT hr = ((ID2D1DeviceContext *) pgraphicsSrc->get_os_data())->EndDraw();

   //      defer_primitive_blend();

   //      if(m_pdevicecontext != nullptr)
   //      {

   //         m_pdevicecontext->DrawBitmap((ID2D1Bitmap*)pgraphicsSrc->get_current_bitmap()->get_os_data(), rDst, (float)dRate, m_interpolationmode, rSrc);

   //      }
   //      else
   //      {

   //         m_prendertarget->DrawBitmap((ID2D1Bitmap*)pgraphicsSrc->get_current_bitmap()->get_os_data(), rDst, (float)dRate, m_bitmapinterpolationmode, rSrc);

   //      }


   //      //auto pimage = (ID2D1Bitmap*)pgraphicsSrc->get_current_bitmap()->get_os_data();
   //      //auto ecomposite = m_ealphamode == ::draw2d::e_alpha_mode_blend ? D2D1_COMPOSITE_MODE_SOURCE_OVER : D2D1_COMPOSITE_MODE_SOURCE_IN;

   //      //m_pdevicecontext->DrawImage(pimage, pointDst, rectangleSource, m_interpolationmode, ecomposite);


   //      //hr = m_prendertarget->Flush();
   //      //flush();

   //      if(SUCCEEDED(hr))
   //      {

   //         ((ID2D1DeviceContext *)pgraphicsSrc->get_os_data())->BeginDraw();

   //         //dynamic_cast <::draw2d_direct2d::graphics *> (pgraphicsSrc)->RestoreClip();

   //      }

   //      return true;

   //   }
   //   catch(...)
   //   {
   //      return false;
   //   }



   //   return true;

   //}


   /*void graphics::alpha_blend(double xDest, double yDest, double dDestWidth, double dDestHeight,
   ::draw2d::graphics * pgraphicsSrc, double xSrc, double ySrc, double dSrcWidth, double dSrcHeight, BLENDFUNCTION blend)
   {

   __throw(not_implemented_exception());
   //if(get_handle1() == nullptr)
   // return false;


   if(m_pimageAlphaBlend->is_set())
   {


   ::rectangle_i32 rectangleIntersect(m_pointAlphaBlend, m_pimageAlphaBlend->size());


   ::image_pointer pimageWork = nullptr;
   ::image_pointer pimageWork2 = nullptr;
   //         ::image_pointer pimageWork3 = nullptr;
   ::image_pointer pimageWork4 = nullptr;


   ::point_f64 pointSrc(xSrc, ySrc);
   ::point_f64 pointDest(xDest, yDest);
   ::size_f64::size_f64(nDestWidth, nDestHeight);



   ::image_pointer pimage;
   if(imageWork == nullptr)
   {
   pimage = create_image(this);
   imageWork = pimage;
   }
   if(imageWork == nullptr)
   return false;
   if(!imageWork = create_image(::size_f64))
   return false;
   if(!imageWork.from(::point_f64(), pgraphicsSrc, pointSrc, ::size_f64))
   return false;




   ::image_pointer pimage2;
   if(imageWork2 == nullptr)
   {
   pimage2 = create_image(this);
   imageWork2 = pimage2;
   }


   ::image_pointer pimage4;
   if(imageWork4 == nullptr)
   {
   image4 = create_image(this);
   imageWork4 = image4;
   }
   if(imageWork4 == nullptr)
   return false;
   if(!imageWork4 = create_image(::size_f64))
   return false;


   imageWork4.Fill(255, 0, 0, 0);

   imageWork4.from(point_i32(maximum(0, m_pointAlphaBlend.x - xDest), maximum(0, m_pointAlphaBlend.y - yDest)),
   m_pimageAlphaBlend->get_graphics(), point_i32(maximum(0, xDest - m_pointAlphaBlend.x), maximum(0, yDest - m_pointAlphaBlend.y)), ::size_f64);

   imageWork.channel_multiply(::color::e_channel_alpha, imageWork4);


   keep < image > keep(&m_pimageAlphaBlend, nullptr, m_pimageAlphaBlend, true);


   return papplication->m_visual.imaging().true_blend(this, pointDest, ::size_f64, imageWork.get_graphics(), pointSrc);


   }

   return ::AlphaBlend(get_handle1(), xDest, yDest,
   nDestWidth, nDestHeight, WIN_HDC(pgraphicsSrc), xSrc, ySrc, nSrcWidth,
   nSrcHeight, blend) != false;
   }*/


//   void graphics::TransparentBlt(double xDest, double yDest, double dDestWidth, double dDestHeight, ::draw2d::graphics * pgraphicsSrc, double xSrc, double ySrc, double dSrcWidth, double dSrcHeight, ::u32 crTransparent)
//   {
//
//      __throw(todo);
//
//      //ASSERT(get_handle1() != nullptr);
//
//      //return ::TransparentBlt(get_handle1(), xDest, yDest, nDestWidth, nDestHeight, WIN_HDC(pgraphicsSrc), xSrc, ySrc, nSrcWidth, nSrcHeight, crTransparent) != false;
//
//   }
//
//   void graphics::GradientFill(TRIVERTEX* pVertices, ULONG nVertices, void * pMesh, ULONG nMeshElements, u32 dwMode)
//   {
//
//      __throw(todo);
//
//      //ASSERT(get_handle1() != nullptr);
//      //
//      //return ::GradientFill(get_handle1(), pVertices, nVertices, pMesh, nMeshElements, dwMode) != false;
//
//   }

   // This is aura API library.
   //
   //
   //
   //
   //
   //
   //
   //

   // Always Inline. Functions only in Win98/Win2K or later

   //inline ::color::color graphics::GetDCBrushColor()
   //{

   //   __throw(todo);

   //   //ASSERT(get_handle1() != nullptr);

   //   //return ::GetDCBrushColor(get_handle1());

   //}
   //inline ::color::color graphics::SetDCBrushColor(const ::color::color & color)
   //{

   //   __throw(todo);

   //   //ASSERT(get_handle1() != nullptr);

   //   //return ::SetDCBrushColor(get_handle1(), crColor);

   //}

   //inline ::color::color graphics::GetDCPenColor()
   //{

   //   __throw(todo);

   //   //ASSERT(get_handle1() != nullptr);

   //   //return ::GetDCPenColor(get_handle1());

   //}
   //inline ::color::color graphics::SetDCPenColor(const ::color::color & color)
   //{

   //   __throw(todo);

   //   //ASSERT(get_handle1() != nullptr);

   //   //return ::SetDCPenColor(get_handle1(), crColor);

   //}

#ifdef WINDOWS_DESKTOP
   /*
      inline void graphics::GetCharABCWidthsI(::u32 giFirst, ::u32 cgi, LPWORD pgi, LPABC lpabc)
      {

         __throw(todo);

         //ASSERT(get_handle1() != nullptr);

         //return ::GetCharABCWidthsI(get_handle1(), giFirst, cgi, pgi, lpabc) != false;

      }
      */
#endif

   //inline void graphics::GetCharWidthI(::u32 giFirst, ::u32 cgi, LPWORD pgi, LPINT lpBuffer)
   //{

   //   __throw(todo);

   //   //ASSERT(get_handle1() != nullptr);

   //   //return ::GetCharWidthI(get_handle1(), giFirst, cgi, pgi, lpBuffer) != false;

   //}

   //inline void graphics::GetTextExtentExPointI(LPWORD pgiIn, int cgi, double dMaxExtent, LPINT lpnFit, LPINT alpDx, LPSIZE32 LPSIZE32)
   //{

   //   __throw(todo);

   //   //ENSURE(LPSIZE32 != nullptr);
   //   //ASSERT(get_handle1() != nullptr);
   //   //return ::GetTextExtentExPointI(get_handle1(), pgiIn, cgi, nMaxExtent, lpnFit, alpDx, LPSIZE32) != false;

   //}


   //inline void graphics::GetTextExtentPointI(LPWORD pgiIn, int cgi, LPSIZE32 LPSIZE32)
   //{

   //   __throw(todo);

   //   //ENSURE(LPSIZE32 != nullptr);
   //   //ASSERT(get_handle1() != nullptr);
   //   //return ::GetTextExtentPointI(get_handle1(), pgiIn, cgi, LPSIZE32) != false;

   //}








#define HIMETRIC_INCH   2540    // HIMETRIC units per inch

   void graphics::DPtoHIMETRIC(::size_f64 * psize)
   {
//#ifdef WINDOWS_DESKTOP
//      ASSERT(__is_valid_address(psize, sizeof(::size_f64)));
//
//      double dMapMode;
//      if (this != nullptr && (nMapMode = GetMapMode()) < MM_ISOTROPIC &&
//            nMapMode != MM_TEXT)
//      {
//         // when using a constrained ::map mode, ::map against physical inch
//         ((::draw2d::graphics *)this)->SetMapMode(MM_HIMETRIC);
//         DPtoLP(psize);
//         ((::draw2d::graphics *)this)->SetMapMode(nMapMode);
//      }
//      else
//      {
//         // ::map against logical inch for non-constrained mapping modes
//         int cxPerInch, cyPerInch;
//         if (this != nullptr)
//         {
//            ASSERT_VALID(this);
//            ASSERT(get_handle1() != nullptr);  // no HDC attached or created?
//            cxPerInch = GetDeviceCaps(LOGPIXELSX);
//            cyPerInch = GetDeviceCaps(LOGPIXELSY);
//         }
//         else
//         {
//            //            cxPerInch = afxData.cxPixelsPerInch;
//            //          cyPerInch = afxData.cyPixelsPerInch;
//         }
//         ASSERT(cxPerInch != 0 && cyPerInch != 0);
//         psize->cx = psize->cx * HIMETRIC_INCH / cxPerInch;
//         psize->cy = psize->cy * HIMETRIC_INCH / cyPerInch;
//      }
//#endif
   }

   void graphics::HIMETRICtoDP(::size_f64 * psize)
   {
//#ifdef WINDOWS_DESKTOP
//      ASSERT(__is_valid_address(psize, sizeof(::size_f64)));
//
//      double dMapMode;
//      if (this != nullptr && (nMapMode = GetMapMode()) < MM_ISOTROPIC &&
//            nMapMode != MM_TEXT)
//      {
//         // when using a constrained ::map mode, ::map against physical inch
//         ((::draw2d::graphics *)this)->SetMapMode(MM_HIMETRIC);
//         LPtoDP(psize);
//         ((::draw2d::graphics *)this)->SetMapMode(nMapMode);
//      }
//      else
//      {
//         // ::map against logical inch for non-constrained mapping modes
//         int cxPerInch, cyPerInch;
//         if (this != nullptr)
//         {
//            ASSERT_VALID(this);
//            ASSERT(get_handle1() != nullptr);  // no HDC attached or created?
//            cxPerInch = GetDeviceCaps(LOGPIXELSX);
//            cyPerInch = GetDeviceCaps(LOGPIXELSY);
//         }
//         else
//         {
//            //            cxPerInch = afxData.cxPixelsPerInch;
//            //          cyPerInch = afxData.cyPixelsPerInch;
//         }
//         ASSERT(cxPerInch != 0 && cyPerInch != 0);
//         psize->cx = psize->cx * cxPerInch / HIMETRIC_INCH;
//         psize->cy = psize->cy * cyPerInch / HIMETRIC_INCH;
//      }
//#endif
   }

   /*void graphics::LPtoHIMETRIC(LPSIZE32 LPSIZE32)
   {
      ASSERT(__is_valid_address(LPSIZE32, sizeof(SIZE_I32)));

      LPtoDP(LPSIZE32);
      DPtoHIMETRIC(LPSIZE32);
   }

   void graphics::HIMETRICtoLP(LPSIZE32 LPSIZE32)
   {
      ASSERT(__is_valid_address(LPSIZE32, sizeof(SIZE_I32)));

      HIMETRICtoDP(LPSIZE32);
      DPtoLP(LPSIZE32);
   }*/

   /////////////////////////////////////////////////////////////////////////////
   // special graphics drawing primitives/helpers

   ::draw2d::brush* graphics::GetHalftoneBrush(::object * pobject)
   {
      /*
      ::aura::LockGlobals(CRIT_HALFTONEBRUSH);
      if (gen_HalftoneBrush == nullptr)
      {
      ::u16 grayPattern[8];
      for (int i = 0; i < 8; i++)
      grayPattern[i] = (::u16)(0x5555 << (i & 1));
      HBITMAP grayBitmap = CreateBitmap(8, 8, 1, 1, grayPattern);
      if (grayBitmap != nullptr)
      {
      gen_HalftoneBrush = ::CreatePatternBrush(grayBitmap);
      ::DeleteObject(grayBitmap);
      }
      }
      if (!gen_WingdixTerm)
      gen_WingdixTerm = (char)!atexit(&__win_gdi_x_term);
      ::aura::UnlockGlobals(CRIT_HALFTONEBRUSH);
      */

      return nullptr;
   }


   //void graphics::DrawDragRect(const ::rectangle_i32 & rectangle, const ::size_f64& ::size_f64, const ::rectangle_i32 & rectangleLast, const ::size_f64& sizeLast, ::draw2d::brush* pBrush, ::draw2d::brush* pBrushLast)
   //{

   //   throw interface_only_exception();

   //   /*
   //   ASSERT(__is_valid_address(rectangle, sizeof(::rectangle_f64), false));
   //   ASSERT(lpRectLast == nullptr ||
   //   __is_valid_address(lpRectLast, sizeof(::rectangle_f64), false));

   //   // first, determine the update region and select it
   //   ::draw2d::region rgnNew;
   //   ::draw2d::region rgnOutside, rgnInside;
   //   rgnOutside.CreateRectRgnIndirect(rectangle);
   //   const ::rectangle_i32 & rectangle = *rectangle;
   //   rectangle.inflate(-size.cx, -size.cy);
   //   rectangle.intersect(rectangle, rectangle);
   //   rgnInside.CreateRectRgnIndirect(rectangle);
   //   rgnNew.CreateRectRgn(0, 0, 0, 0);
   //   rgnNew.CombineRgn(&rgnOutside, &rgnInside, RGN_XOR);

   //   ::draw2d::brush* pBrushOld = nullptr;
   //   if (pBrush == nullptr)
   //   {
   //   pBrush = graphics::GetHalftoneBrush(this);
   //   }

   //   ENSURE(pBrush);

   //   if (pBrushLast == nullptr)
   //   {
   //   pBrushLast = pBrush;
   //   }

   //   ::draw2d::region rgnLast, rgnUpdate;
   //   if (lpRectLast != nullptr)
   //   {
   //   // find difference between new region and old region
   //   rgnLast.CreateRectRgn(0, 0, 0, 0);
   //   rgnOutside.SetRectRgn(lpRectLast);
   //   rectangle = *lpRectLast;
   //   rectangle.inflate(-sizeLast.cx, -sizeLast.cy);
   //   rectangle.intersect(rectangle, lpRectLast);
   //   rgnInside.SetRectRgn(rectangle);
   //   rgnLast.CombineRgn(&rgnOutside, &rgnInside, RGN_XOR);

   //   // only diff them if brushes are the same
   //   if (pBrush->get_os_data() == pBrushLast->get_os_data())
   //   {
   //   rgnUpdate.CreateRectRgn(0, 0, 0, 0);
   //   rgnUpdate.CombineRgn(&rgnLast, &rgnNew, RGN_XOR);
   //   }
   //   }
   //   if (pBrush->get_os_data() != pBrushLast->get_os_data() && lpRectLast != nullptr)
   //   {
   //   // brushes are different -- erase old region first
   //   SelectClipRgn(&rgnLast);
   //   get_clip_box(&rectangle);
   //   pBrushOld = SelectObject(pBrushLast);
   //   PatBlt(rectangle.left, rectangle.top, rectangle.width(), rectangle.height(), PATINVERT);
   //   SelectObject(pBrushOld);
   //   pBrushOld = nullptr;
   //   }

   //   // draw into the update/new region
   //   SelectClipRgn(rgnUpdate.get_os_data() != nullptr ? &rgnUpdate : &rgnNew);
   //   get_clip_box(&rectangle);
   //   pBrushOld = SelectObject(pBrush);
   //   PatBlt(rectangle.left, rectangle.top, rectangle.width(), rectangle.height(), PATINVERT);

   //   // cleanup DC
   //   if (pBrushOld != nullptr)
   //   SelectObject(pBrushOld);
   //   SelectClipRgn(nullptr);

   //   */

   //}


   //void graphics::fill_rectangle(const ::rectangle_f64 & rectangle, const ::color::color & color)
   //{

   //   return fill_rectangle(rectangle_f64(rectangle), color);

   //}


   //void graphics::draw_inset_3d_rectangle(const ::rectangle_f64 & rectangle, const ::color::color & colorTopLeft, const ::color::color & colorBottomRight, const ::e_border & eborder)
   //{

   //   draw_inset_3d_rectangle(rectangle_f64(rectangle), colorTopLeft, colorBottomRight, eborder);

   //}


   //void graphics::draw_inset_3d_rectangle(const ::rectangle_f64 & rectangle,  const ::color::color & colorTopLeft, const ::color::color & colorBottomRight, const ::e_border & eborder)
   //{

   //   double x = rectangle.left;
   //   double y = rectangle.top;
   //   double cx = ::width(rectangle);
   //   double cy = ::height(rectangle);

   //   if (eborder & e_border_top)
   //   {

   //      fill_solid_rect_dim(x, y, cx - 1.0, 1.0, colorTopLeft);

   //   }

   //   if (eborder & e_border_left)
   //   {

   //      fill_solid_rect_dim(x, y, 1.0, cy - 1.0, colorTopLeft);

   //   }

   //   if (eborder & e_border_right)
   //   {

   //      fill_solid_rect_dim(x + cx, y, -1.0, cy, colorBottomRight);

   //   }

   //   if (eborder & e_border_bottom)
   //   {

   //      fill_solid_rect_dim(x, y + cy, cx, -1.0, colorBottomRight);

   //   }

   //}


   void graphics::DeleteDC()
   {

      return destroy();

      /* if(get_handle() == nullptr)
          return false;

       return ::DeleteDC(Detach()) != false;*/
   }


   //int graphics::StartDoc(const ::string & lpszDocName)
   //{

   //   __throw(todo);

   //   //DOCINFO di;
   //   //__memset(&di, 0, sizeof(DOCINFO));
   //   //di.cbSize = sizeof(DOCINFO);
   //   //di.lpszDocName = lpszDocName;
   //   //return StartDoc(&di);
   //}


   int graphics::SaveDC()
   {

      m_prendertarget->GetTransform(&m_pstate->m_m);

      ::count iSaveDC = m_statea.get_size();

      m_statea.add(m_pstate);

      m_pstate = __new(state);

      //m_pstate->m_layerparameters = D2D1::LayerParameters();

      //m_prendertarget->PushLayer(m_pstate->m_layerparameters, nullptr);

      return (int) iSaveDC;

   }


   void graphics::RestoreDC(int nSavedDC)
   {

      if (nSavedDC < 0)
      {

         //return false;

         throw_status(error_failed);

      }

      if (nSavedDC >= m_statea.get_count())
      {

         //return false;

         throw_status(error_failed);

      }

      m_statea.add(m_pstate);

      for (index iState = m_statea.get_upper_bound(); iState > nSavedDC; iState--)
      {

         auto state = m_statea[iState];

         for (index iItem = state->m_maRegion.get_upper_bound(); iItem >= 0; iItem--)
         {

            m_prendertarget->PopLayer();

         }

         state->m_maRegion.erase_all();

         state->m_sparegionClip.erase_all();

         m_statea.erase_at(iState);

      }

      m_prendertarget->SetTransform(&m_statea[nSavedDC]->m_m);

      m_pstate = m_statea[nSavedDC];

      m_statea.set_size(nSavedDC);

      //return true;

   }


   //int graphics::SetPolyFillMode(int nPolyFillMode)
   //{

   //   __throw(todo);

   //}


   //int graphics::SetROP2(int nDrawMode)
   //{

   //   __throw(todo);

   //   //double dRetVal = 0;
   //   //if(get_handle1() != nullptr && get_handle1() != get_handle2())
   //   //   nRetVal = ::SetROP2(get_handle1(), nDrawMode);
   //   //if(get_handle2() != nullptr)
   //   //   nRetVal = ::SetROP2(get_handle2(), nDrawMode);
   //   //return nRetVal;
   //}

   void graphics::set_interpolation_mode(::draw2d::enum_interpolation_mode einterpolationmode)
   {
      if(einterpolationmode == ::draw2d::e_interpolation_mode_nearest_neighbor)
      {
         m_bitmapinterpolationmode     = D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR;
         m_interpolationmode           = D2D1_INTERPOLATION_MODE_NEAREST_NEIGHBOR;;
      }
      else if(einterpolationmode == ::draw2d::e_interpolation_mode_high_quality_bicubic)
      {
         m_bitmapinterpolationmode     = D2D1_BITMAP_INTERPOLATION_MODE_LINEAR;
         m_interpolationmode           = D2D1_INTERPOLATION_MODE_HIGH_QUALITY_CUBIC;
      }
      else
      {
         m_bitmapinterpolationmode     = D2D1_BITMAP_INTERPOLATION_MODE_LINEAR;
         m_interpolationmode           = D2D1_INTERPOLATION_MODE_LINEAR;
      }
      //return 1;
      /*double dRetVal = 0;
      if(get_handle1() != nullptr && get_handle1() != get_handle2())
      nRetVal = ::set_interpolation_mode(get_handle1(), nStretchMode);
      if(get_handle2() != nullptr)
      nRetVal = ::set_interpolation_mode(get_handle2(), nStretchMode);
      return nRetVal;*/
   }


   // ::color::color graphics::SetTextColor(const ::color::color & color)
//   {
   //  return set_color(crColor);
   //const ::color::color & colorRetVal = m_colorColor;
   //m_colorColor = crColor;
   /*      const ::color::color & colorRetVal = CLR_INVALID;
   if(get_handle1() != nullptr && get_handle1() != get_handle2())
   crRetVal = ::SetTextColor(get_handle1(), crColor);
   if(get_handle2() != nullptr)
   crRetVal = ::SetTextColor(get_handle2(), crColor);*/
   //return crRetVal;
   //}

   //int graphics::SetGraphicsMode(int iMode)
   //{

   //   __throw(todo);

   //   //double dRetVal = 0;
   //   //if(get_handle1() != nullptr && get_handle1() != get_handle2())
   //   //{
   //   //   nRetVal = ::SetGraphicsMode(get_handle1(), iMode);
   //   //}
   //   //if(get_handle2() != nullptr)
   //   //{
   //   //   nRetVal = ::SetGraphicsMode(get_handle2(), iMode);
   //   //}
   //   //return nRetVal;
   //}

   //void graphics::SetWorldTransform(const XFORM* pXform)
   //{

   //   __throw(todo);

   //   //bool nRetVal = 0;
   //   //if(get_handle1() != nullptr && get_handle1() != get_handle2())
   //   //{
   //   //   nRetVal = ::SetWorldTransform(get_handle1(), pXform) != false;
   //   //}
   //   //if(get_handle2() != nullptr)
   //   //{
   //   //   nRetVal = ::SetWorldTransform(get_handle2(), pXform) != false;
   //   //}
   //   //return nRetVal;
   //}

   //void graphics::ModifyWorldTransform(const XFORM* pXform,u32 iMode)
   //{

   //   __throw(todo);

   //   //bool nRetVal = 0;
   //   //if(get_handle1() != nullptr && get_handle1() != get_handle2())
   //   //{
   //   //   nRetVal = ::ModifyWorldTransform(get_handle1(), pXform, iMode) != false;
   //   //}
   //   //if(get_handle2() != nullptr)
   //   //{
   //   //   nRetVal = ::ModifyWorldTransform(get_handle2(), pXform, iMode) != false;
   //   //}
   //   //return nRetVal;
   //}


   //int graphics::SetMapMode(int nMapMode)
   //{

   //   return 0;

   //}


   point_f64 graphics::SetViewportOrg(double x, double y)
   {

      return ::draw2d::graphics::SetViewportOrg(x, y);

   }


   point_f64 graphics::OffsetViewportOrg(double dWidth, double dHeight)
   {

      return ::draw2d::graphics::OffsetViewportOrg(dWidth, dHeight);

   }


   ::size_f64 graphics::SetViewportExt(double x, double y)
   {

      __throw(todo);


      return nullptr;

   }


   ::size_f64 graphics::ScaleViewportExt(double xNum, double xDenom, double yNum, double yDenom)
   {

      return ::draw2d::graphics::ScaleViewportExt(xNum, xDenom, yNum, yDenom);

   }


   point_f64 graphics::SetWindowOrg(double x, double y)
   {

      __throw(todo);

      return nullptr;

   }


   point_f64 graphics::offset_window_org(double dWidth, double dHeight)
   {

      __throw(todo);

      return nullptr;

   }


   ::size_f64 graphics::set_window_ext(double x, double y)
   {

      __throw(todo);

      return nullptr;

   }


   ::size_f64 graphics::scale_window_ext(double xNum, double xDenom, double yNum, double yDenom)
   {

      __throw(todo);


      return nullptr;

   }


   int graphics::get_clip_box(::rectangle_f64 * prectangle)
   {
      return 0;
      //__throw(todo);

      //return ::get_clip_box(get_handle1(), rectangle);
   }

   
   void graphics::reset_clip()
   {

      ::draw2d::lock draw2dlock;

      ::draw2d::device_lock devicelock(this);

      for (index iState = m_statea.get_upper_bound(); iState >= 0; iState--)
      {

         auto state = m_statea[iState];

         for (index iItem = state->m_maRegion.get_upper_bound(); iItem >= 0; iItem--)
         {

            m_prendertarget->PopLayer();

         }

         state->m_maRegion.erase_all();

         state->m_sparegionClip.erase_all();

      }

      auto& state = m_pstate;

      for (index iItem = state->m_maRegion.get_upper_bound(); iItem >= 0; iItem--)
      {

         m_prendertarget->PopLayer();

      }

      state->m_maRegion.erase_all();

      state->m_sparegionClip.erase_all();

      //return ::success;

   }


   void graphics::add_shapes(const shape_array& shapea)
   {

      for (int i = 0; i < shapea.get_count(); i++)
      {

         if (i + 1 < shapea.get_count())
         {

            if (shapea[i + 1]->eshape() == e_shape_intersect_clip)
            {

               switch (shapea[i]->eshape())
               {
               //case e_shape_rect:
               //   intersect_clip(shapea[i]->shape < ::rectangle_i32>());
               //   break;
               case e_shape_rectangle:
                  intersect_clip(shapea[i]->shape < ::rectangle >());
                  break;
               //case e_shape_oval:
               //   intersect_clip(shapea[i]->shape < ::oval>());
               //   break;
               case e_shape_ellipse:
                  intersect_clip(shapea[i]->shape < ::ellipse >());
                  break;
               //case e_shape_polygon:
               //   intersect_clip(shapea[i]->shape < ::polygon_i32>());
               //   break;
               case e_shape_polygon:
                  intersect_clip(shapea[i]->shape < ::polygon >());
                  break;

               }

               i++;

            }

         }

      }

      //return ::success;

   }


   //void graphics::intersect_clip(const ::rectangle_f64 & rectangle)
   //{

   //   ::draw2d::device_lock devicelock(this);

   //   {

   //      auto pregion = __create < ::draw2d::region > ();

   //      auto rectangleClip = rectangle_i32 + m_pointAddShapeTranslate;

   //      pregion->create_rect(rectangleClip);

   //      D2D1::Matrix3x2F m = {};

   //      m_prendertarget->GetTransform(&m);

   //      m_pstate->m_sparegionClip.add(pregion);

   //      m_pstate->m_maRegion.add(m);

   //      ID2D1Geometry* pgeometry = (ID2D1Geometry*)pregion->get_os_data(this);

   //      auto layerparameters = D2D1::LayerParameters(
   //         D2D1::InfiniteRect(),
   //         pgeometry);

   //      m_prendertarget->PushLayer(layerparameters, nullptr);

   //   }

   //   return 0;

   //}


   void graphics::intersect_clip(const ::rectangle_f64 & rectangle)
   {

      ::draw2d::lock draw2dlock;

      ::draw2d::device_lock devicelock(this);

      auto pregion = __create < ::draw2d::region >();

      auto rectangleClip = rectangle + m_pointAddShapeTranslate;

      pregion->create_rectangle(rectangleClip);

      D2D1::Matrix3x2F m = {};

      m_prendertarget->GetTransform(&m);

      m_pstate->m_sparegionClip.add(pregion);

      m_pstate->m_maRegion.add(m);

      ID2D1Geometry* pgeometry = (ID2D1Geometry*)pregion->get_os_data(this);

      auto layerparameters = D2D1::LayerParameters(
         D2D1::InfiniteRect(),
         pgeometry);

      m_prendertarget->PushLayer(layerparameters, nullptr);

      //return 0;


   }




   //void graphics::intersect_clip(const ::oval& oval)
   //{

   //   ::draw2d::device_lock devicelock(this);

   //   {

   //      auto pregion = __create < ::draw2d::region > ();

   //      pregion->create_oval(oval);

   //      D2D1::Matrix3x2F m = {};

   //      m_prendertarget->GetTransform(&m);

   //      m_pstate->m_sparegionClip.add(pregion);

   //      m_pstate->m_maRegion.add(m);

   //      ID2D1Geometry* pgeometry = (ID2D1Geometry*)pregion->get_os_data(this);

   //      m_prendertarget->PushLayer(D2D1::LayerParameters(D2D1::InfiniteRect(), pgeometry), nullptr);

   //   }

   //   return 0;


   //}


   void graphics::intersect_clip(const ::ellipse & ellipse)
   {

      ::draw2d::lock draw2dlock;

      ::draw2d::device_lock devicelock(this);

      {

         auto pregion = __create < ::draw2d::region > ();

         pregion->create_ellipse(ellipse);

         D2D1::Matrix3x2F m = {};

         m_prendertarget->GetTransform(&m);

         m_pstate->m_sparegionClip.add(pregion);

         m_pstate->m_maRegion.add(m);

         ID2D1Geometry* pgeometry = (ID2D1Geometry*)pregion->get_os_data(this);

         m_prendertarget->PushLayer(D2D1::LayerParameters(D2D1::InfiniteRect(), pgeometry), nullptr);

      }

      //return 0;


   }


   //void graphics::intersect_clip(const ::polygon_i32& polygon_i32)
   //{

   //   ::draw2d::device_lock devicelock(this);

   //   {

   //      auto pregion = __create < ::draw2d::region > ();

   //      pregion->create_polygon(polygon_i32.get_data(), (::i32) polygon_i32.get_count(), ::draw2d::fill_mode_winding);

   //      D2D1::Matrix3x2F m = {};

   //      m_prendertarget->GetTransform(&m);

   //      m_pstate->m_sparegionClip.add(pregion);

   //      m_pstate->m_maRegion.add(m);

   //      ID2D1Geometry* pgeometry = (ID2D1Geometry*)pregion->get_os_data(this);

   //      m_prendertarget->PushLayer(D2D1::LayerParameters(D2D1::InfiniteRect(), pgeometry), nullptr);

   //   }

   //   return 0;


   //}


   void graphics::intersect_clip(const ::polygon_f64& polygon_i32)
   {

      ::draw2d::lock draw2dlock;

      ::draw2d::device_lock devicelock(this);

      {

         auto pregion = __create < ::draw2d::region > ();

         pregion->create_polygon(polygon_i32.get_data(), (::i32) polygon_i32.get_count(), ::draw2d::fill_mode_winding);

         D2D1::Matrix3x2F m = {};

         m_prendertarget->GetTransform(&m);

         m_pstate->m_sparegionClip.add(pregion);

         m_pstate->m_maRegion.add(m);

         ID2D1Geometry* pgeometry = (ID2D1Geometry*)pregion->get_os_data(this);

         m_prendertarget->PushLayer(D2D1::LayerParameters(D2D1::InfiniteRect(), pgeometry), nullptr);

      }

      //return 0;


   }


   //void graphics::

   //   }
   //   else
   //   {

   //      D2D1::Matrix3x2F m;

   //      m_prendertarget->GetTransform(&m);

   //      m_pstate->m_sparegionClip.add(pregion);

   //      m_pstate->m_maRegion.add(m);

   //      auto pgeometry = pregion->get_os_data < ID2D1Geometry * >(this);

   //      m_prendertarget->PushLayer(D2D1::LayerParameters(D2D1::InfiniteRect(),pgeometry),nullptr);

   //   }

   //   return 0;

   //   //__throw(todo);


   //   //if(pregion == nullptr)
   //   //{
   //   //   m_prendertarget->ResetClip();
   //   //}
   //   //else
   //   //{
   //   //   m_prendertarget->SetClip((Gdiplus::Region *) pregion->get_os_data());
   //   //}

   //   //return 0;

   //   ///*      double dRetVal = ERROR;
   //   //if(get_handle1() != nullptr && get_handle1() != get_handle2())
   //   //nRetVal = ::SelectClipRgn(get_handle1(), pRgn == nullptr ? nullptr : (HRGN) pRgn->get_os_data());
   //   //if(get_handle2() != nullptr)
   //   //nRetVal = ::SelectClipRgn(get_handle2(), pRgn == nullptr ? nullptr : (HRGN) pRgn->get_os_data());
   //   //return nRetVal;*/
   //}

   //int graphics::ExcludeClipRect(double x1, double y1, double x2, double y2)
   //{

   //   ::draw2d::device_lock devicelock(this);

   //   {

   //      ::u32 uMax = m_pdevicecontext->GetMaximumBitmapSize();

   //      ::draw2d::region_pointer regionBig(e_create);

   //      regionBig->create_rect_dim(-(int) uiMax/2, -(int)uiMax / 2, uiMax / 2, uiMax / 2);

   //      ::draw2d::region_pointer regionSmall(e_create);

   //      regionSmall->create_rect_dim(x1, y1, x2, y2);

   //      ::draw2d::region_pointer regionExclude(e_create);

   //      regionExclude->combine(regionBig, regionSmall, ::draw2d::e_combine_exclude);

   //      D2D1::Matrix3x2F m;

   //      m_prendertarget->GetTransform(&m);

   //      m_pstate->m_sparegionClip.add(regionExclude);

   //      m_pstate->m_maRegion.add(m);

   //      auto pgeometry = regionExclude->get_os_data < ID2D1Geometry * >(this);

   //      m_prendertarget->PushLayer(D2D1::LayerParameters(D2D1::InfiniteRect(), pgeometry), nullptr);

   //   }

   //   return 0;

   //}

   //int graphics::ExcludeClipRect(const ::rectangle_i32 & rectangle)
   //{

   //   return ExcludeClipRect(rectangle.left, rectangle.top, rectangle.right, rectangle.bottom);

   //}



   //int graphics::OffsetClipRgn(double x, double y)
   //{

   //   __throw(todo);

   //}


   //int graphics::OffsetClipRgn(const ::size_f64& ::size_f64)
   //{

   //   __throw(todo);

   //}


   ::u32 graphics::SetTextAlign(::u32 nFlags)
   {

      __throw(todo);

      return 0;

   }


   //int graphics::SetTextJustification(double dBreakExtra, double dBreakCount)
   //{

   //   __throw(todo);

   //}


   //int graphics::SetTextCharacterExtra(double dCharExtra)
   //{

   //   __throw(todo);

   //}


   //u32 graphics::SetMapperFlags(u32 dwFlag)
   //{

   //   __throw(todo);

   //}


   typedef u32 (CALLBACK* __GDIGETLAYOUTPROC)(HDC);
   typedef u32 (CALLBACK* __GDISETLAYOUTPROC)(HDC, u32);


   void graphics::apply_primitive_blend()
   {

      if (m_ealphamodeDevice != m_ealphamode)
      {

         m_ealphamodeDevice = m_ealphamode;

         if (m_ealphamodeDevice == ::draw2d::e_alpha_mode_blend)
         {

            m_pdevicecontext->SetPrimitiveBlend(D2D1_PRIMITIVE_BLEND_SOURCE_OVER);

         }
         else if (m_ealphamodeDevice == ::draw2d::e_alpha_mode_set)
         {

            m_pdevicecontext->SetPrimitiveBlend(D2D1_PRIMITIVE_BLEND_COPY);

         }

      }

   }


   void graphics::apply_text_rendering_hint()
   {

      try
      {

         if (m_prendertarget == nullptr)
         {

            return;

         }

         m_etextrenderinghintDevice = m_ewritetextrendering;

         switch (m_etextrenderinghintDevice)
         {
         case ::write_text::e_rendering_anti_alias:

            m_prendertarget->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);

            break;

         case ::write_text::e_rendering_anti_alias_grid_fit:
            
            m_prendertarget->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);

            break;

         case ::write_text::e_rendering_single_bit_per_pixel:
            
            m_prendertarget->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);

            break;

         case ::write_text::e_rendering_clear_type_grid_fit:

            m_prendertarget->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_CLEARTYPE);

            break;

         }

      }
      catch (...)
      {

      }

   }


   void graphics::defer_text_primitive_blend()
   {

      if (m_ealphamodeDevice != ::draw2d::e_alpha_mode_blend)
      {

         m_ealphamodeDevice = m_ealphamode;

         m_pdevicecontext->SetPrimitiveBlend(D2D1_PRIMITIVE_BLEND_SOURCE_OVER);

      }

   }


   oswindow graphics::get_window_handle()
   {

      if (::is_null(m_puserinteraction))
      {

         return nullptr;

      }

      return m_puserinteraction->get_oswindow();

   }


   u32 graphics::GetLayout()
   {

      __throw(todo);

      return 0;

   }


   u32 graphics::SetLayout(u32 dwSetLayout)
   {

      __throw(todo);

      return 0;

   }


   void graphics::ArcTo(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4)
   {

      __throw(todo);

      //return false;

   }


   //int graphics::SetArcDirection(double dArcDirection)
   //{

   //   __throw(todo);

   //}


   void graphics::polydraw(const ::point_f64* ppoints, const byte* lpTypes, count nCount)
   {

      __throw(todo);

      //return false;

   }


   void graphics::polyline_to(const ::point_f64* ppoints, count nCount)
   {

      __throw(todo);

      //return false;

   }


   void graphics::poly_bezier_to(const ::point_f64* ppoints, count nCount)
   {

      __throw(todo);

      //return false;

   }


   void graphics::SelectClipPath(int nMode)
   {

      //__throw(todo);

      //return false;

   }


   //int graphics::SelectClipRgn(::draw2d::region* pRgn, ::draw2d::enum_combine ecombine)
   //{

   //   return 0;

   //}

   /////////////////////////////////////////////////////////////////////////////
   // Special handling for metafile playback
#ifdef WINDOWS_DESKTOP
   int CALLBACK __enum_meta_file_procedure(HDC hDC,
                                           HANDLETABLE* pHandleTable, METARECORD* pMetaRec, double dHandles, LPARAM lParam)
   {
      
      ::draw2d::graphics * pgraphics = (::draw2d::graphics *)lParam;

      ASSERT_VALID(pgraphics);

      switch (pMetaRec->rdFunction)
      {
      // these records have effects different for each graphics derived class
      case META_SETMAPMODE:
         //(dynamic_cast<::draw2d_direct2d::graphics * >(pgraphics))->SetMapMode((int)(short)pMetaRec->rdParm[0]);
         break;
      case META_SETWINDOWEXT:
         (dynamic_cast<::draw2d_direct2d::graphics * >(pgraphics))->set_window_ext(
         (int)(short)pMetaRec->rdParm[1], (int)(short)pMetaRec->rdParm[0]);
         break;
      case META_SETWINDOWORG:
         (dynamic_cast<::draw2d_direct2d::graphics * >(pgraphics))->SetWindowOrg(
         (int)(short)pMetaRec->rdParm[1], (int)(short)pMetaRec->rdParm[0]);
         break;
      case META_SETVIEWPORTEXT:
         (dynamic_cast<::draw2d_direct2d::graphics * >(pgraphics))->SetViewportExt(
         (int)(short)pMetaRec->rdParm[1], (int)(short)pMetaRec->rdParm[0]);
         break;
      case META_SETVIEWPORTORG:
         (dynamic_cast<::draw2d_direct2d::graphics * >(pgraphics))->SetViewportOrg(
         (int)(short)pMetaRec->rdParm[1], (int)(short)pMetaRec->rdParm[0]);
         break;
      case META_SCALEWINDOWEXT:
         (dynamic_cast<::draw2d_direct2d::graphics * >(pgraphics))->scale_window_ext(
         (int)(short)pMetaRec->rdParm[3], (int)(short)pMetaRec->rdParm[2],
         (int)(short)pMetaRec->rdParm[1], (int)(short)pMetaRec->rdParm[0]);
         break;
      case META_SCALEVIEWPORTEXT:
         (dynamic_cast<::draw2d_direct2d::graphics * >(pgraphics))->ScaleViewportExt(
         (int)(short)pMetaRec->rdParm[3], (int)(short)pMetaRec->rdParm[2],
         (int)(short)pMetaRec->rdParm[1], (int)(short)pMetaRec->rdParm[0]);
         break;
      case META_OFFSETVIEWPORTORG:
         (dynamic_cast<::draw2d_direct2d::graphics * >(pgraphics))->OffsetViewportOrg(
         (int)(short)pMetaRec->rdParm[1], (int)(short)pMetaRec->rdParm[0]);
         break;
      case META_SAVEDC:
         (dynamic_cast<::draw2d_direct2d::graphics * >(pgraphics))->SaveDC();
         break;
      case META_RESTOREDC:
         (dynamic_cast<::draw2d_direct2d::graphics * >(pgraphics))->RestoreDC((int)(short)pMetaRec->rdParm[0]);
         break;
      case META_SETBKCOLOR:
      {
         
         auto pbrush = pgraphics->__create < ::draw2d::brush >();
         
         pbrush->create_solid(*(UNALIGNED ::color::color*)&pMetaRec->rdParm[0]);
         
         (dynamic_cast<::draw2d_direct2d::graphics * >(pgraphics))->set(pbrush);

      }
      break;
      case META_SETTEXTCOLOR:
      {
         auto pbrush = pgraphics->__create < ::draw2d::brush >();
         pbrush->create_solid(*(UNALIGNED ::color::color*)&pMetaRec->rdParm[0]);
         (dynamic_cast<::draw2d_direct2d::graphics * >(pgraphics))->set(pbrush);
      }
      break;

      // need to watch out for SelectObject(HFONT), for custom font mapping
      case META_SELECTOBJECT:
      {
         HGDIOBJ hObject = pHandleTable->objectHandle[pMetaRec->rdParm[0]];
         ::u32 nObjType = GetObjectType(hObject);
         if (nObjType == 0)
         {
            // object type is unknown, determine if it is a font
            HFONT hStockFont = (HFONT)::GetStockObject(SYSTEM_FONT);
            //HFONT hFontOld = (HFONT)::SelectObject((dynamic_cast<::draw2d_direct2d::graphics * >(pgraphics))->get_handle1(), hStockFont);
            //HGDIOBJ hObjOld = ::SelectObject((dynamic_cast<::draw2d_direct2d::graphics * >(pgraphics))->get_handle1(), hObject);
            //if (hObjOld == hStockFont)
            //{
            //   // got the stock object back, so must be selecting a font
            //   __throw(error_not_implemented);
            //   //                  (dynamic_cast<::draw2d_direct2d::graphics * >(pgraphics))->SelectObject(::draw2d_direct2d::font::from_handle_dup(pgraphics->get_application(), (HFONT)hObject));
            //   break;  // don't play the default record
            //}
            //else
            //{
            //   // didn't get the stock object back, so restore everything
            //   ::SelectObject((dynamic_cast<::draw2d_direct2d::graphics * >(pgraphics))->get_handle1(), hFontOld);
            //   ::SelectObject((dynamic_cast<::draw2d_direct2d::graphics * >(pgraphics))->get_handle1(), hObjOld);
            //}
            // and fall through to PlayMetaFileRecord...
         }
         else if (nObjType == OBJ_FONT)
         {
            // play back as graphics::SelectObject(::write_text::font*)
            //               (dynamic_cast<::draw2d_direct2d::graphics * >(pgraphics))->SelectObject(::draw2d_direct2d::font::from_handle_dup(pgraphics->get_application(), (HFONT)hObject));
            __throw(error_not_implemented);
            break;  // don't play the default record
         }
      }
      // fall through...

      default:
         //::PlayMetaFileRecord(hDC, pHandleTable, pMetaRec, nHandles);
         break;
      }

      return 1;
   }
#endif


   //void graphics::PlayMetaFile(HMETAFILE hMF)
   //{

   //   __throw(todo);

   //}


   void graphics::LPtoDP(::size_f64 * psize)
   {

      ASSERT(__is_valid_address(psize, sizeof(::size_f64)));

      ::size_f64 sizeWinExt = GetWindowExt();
      ::size_f64 sizeVpExt = GetViewportExt();
      psize->cx = psize->cx * abs(sizeVpExt.cx) / abs(sizeWinExt.cx);
      psize->cy = psize->cy * abs(sizeVpExt.cy) / abs(sizeWinExt.cy);

   }


   void graphics::DPtoLP(::size_f64 * psize)
   {
   
      ASSERT(__is_valid_address(psize, sizeof(::size_f64)));

      ::size_f64 sizeWinExt = GetWindowExt();
      ::size_f64 sizeVpExt = GetViewportExt();
      psize->cx = psize->cx * abs(sizeWinExt.cx) / abs(sizeVpExt.cx);
      psize->cy = psize->cy * abs(sizeWinExt.cy) / abs(sizeVpExt.cy);

   }


   //void graphics::draw_text(const ::string & lpszString, strsize nCount, const ::rectangle_i32 & rectangle, const ::e_align & ealign, const ::e_draw_text & edrawtext)
   //{

   //   if (nCount < 0)
   //   {

   //      nCount = strlen(lpszString) + nCount + 1;

   //   }

   //   return draw_text(string(lpszString, nCount), rectangle, nFormat);

   //}


   void graphics::draw_text(const ::string & str,const ::rectangle_f64 & rectangle, const ::e_align & ealign, const ::e_draw_text & edrawtext)
   {

      if (str.is_empty())
      {

         //return true;

         return;

      }

      if (str.is_empty())
      {

         //return false;

         throw_status(error_invalid_empty_argument);

      }

      if (m_pfont.is_null())
      {

         //return false;

         throw_status(error_null_pointer);

      }

      if (m_pbrush.is_null())
      {

         //return false;

         throw_status(error_null_pointer);

      }

      ID2D1Brush* pbrush = m_pbrush->get_os_data < ID2D1Brush * >(this);

      if (::is_null(pbrush))
      {

         //return false;

         throw_status(error_null_pointer);

      }

      IDWriteTextFormat* pfont = m_pfont->get_os_data < IDWriteTextFormat * >(this);

      if (::is_null(pfont))
      {

         //return false;

         throw_status(error_null_pointer);

      }

      if (ealign & e_align_right)
      {

         pfont->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);

      }
      else if (ealign & e_align_horizontal_center)
      {

         pfont->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);

      }
      else
      {

         pfont->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);

      }

      if (ealign & e_align_bottom)
      {

         pfont->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);

      }
      else if (ealign & e_align_vertical_center)
      {

         pfont->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

      }
      else
      {

         pfont->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

      }

      auto& text = m_pfont->m_mapText[str];

      if (text.m_wstr.is_empty())
      {

         text.m_wstr = str;

      }

      defer_text_primitive_blend();

      defer_text_rendering_hint();

      if (m_pfont->m_dFontWidth == 1.0)
      {

         D2D1_RECT_F rectangle_f32 = D2D1::RectF((FLOAT)rectangle.left, (FLOAT)rectangle.top, (FLOAT)rectangle.right, (FLOAT)rectangle.bottom);

         m_prendertarget->DrawText(text.m_wstr, (::u32)text.m_wstr.get_length(), pfont, &rectangle_f32, pbrush);

      }
      else
      {
         D2D1::Matrix3x2F m;

         m_prendertarget->GetTransform(&m);

         D2D1::Matrix3x2F mOriginal(m);

         D2D1_RECT_F rectangle_f32 = D2D1::RectF((FLOAT)0, (FLOAT)0, (FLOAT)width(rectangle), (FLOAT)height(rectangle));

         m = m * D2D1::Matrix3x2F::Translation((FLOAT)rectangle.left, (FLOAT)rectangle.top);

         m = m * D2D1::Matrix3x2F::Scale((FLOAT)m_pfont->m_dFontWidth, 1.f);

         m_prendertarget->SetTransform(&m);

         m_prendertarget->DrawText(text.m_wstr, (::u32)text.m_wstr.get_length(), pfont, &rectangle_f32, pbrush);

         m_prendertarget->SetTransform(&mOriginal);

      }

      //return 1;

   }


   size_f64 graphics::get_text_extent(const char * lpszString, strsize nCount, strsize iIndex)
   {

      if (nCount < 0)
      {

         nCount = strlen(lpszString) + nCount + 1;

      }

      size_f64 sz;

      //bool bOk = 
      
      get_text_extent(sz, lpszString, nCount, iIndex);

      //if (!bOk)
      //{

      //   return ::size_f64(0, 0);

      //}
      //else
      {

         return ::size_f64(sz.cx, sz.cy);

      }

   }


   size_f64 graphics::get_text_extent(const block & block)
   {

      size_f64 s;

      get_text_extent(s, (const char*)block.get_data(), block.get_size());

      //if (!get_text_extent(s, (const char *) block.get_data(), block.get_size()))
      //{

      //   return ::size_f64(0, 0);

      //}

      return s;

   }


   //::size_f64 graphics::GetOutputTextExtent(const char * lpszString, strsize nCount)
   //{

   //   __throw(todo);

   //   return nullptr;

   //}


   //::size_f64 graphics::GetOutputTextExtent(const ::string & str)
   //{

   //   __throw(todo);

   //   return nullptr;

   //}


   void graphics::get_text_extent(size_f64 & size, const char * lpszString, strsize nCount, strsize iIndex)
   {

      if (iIndex <= 0)
      {

         size.cx = 0;

         size.cy = 0;

         //return true;

         return;

      }

      string str;

      if (nCount < 0)
      {

         nCount = strlen(lpszString) + nCount + 1;

      }

      if (iIndex < 0)
      {

         iIndex = nCount;

      }

      if (iIndex > nCount && nCount >= 0)
      {

         iIndex = nCount;

      }

      str = string(lpszString, iIndex);

      if (m_pfont.is_null())
      {

         //return false;

         throw_status(error_null_pointer);

      }

      auto pfont = m_pfont->get_os_data < IDWriteTextFormat * >(this);

      if (pfont == nullptr)
      {

         //return false;
         throw_status(error_null_pointer);

      }

      auto& text = m_pfont->m_mapText[str];

      if (text.m_bSize)
      {

         size = text.m_size;

         //return true;

         return;

      }

      if (text.m_wstr.is_empty())
      {

         text.m_wstr = str;

      }

      comptr<IDWriteTextLayout> playout1;

      HRESULT hr;

      comptr<IDWriteTextLayout> playout;

      ::u32 uLength = (::u32)text.m_wstr.get_length();

      hr = ::direct2d::direct2d()->dwrite_factory()->CreateTextLayout(
           text.m_wstr,                // The string to be laid out and formatted.
           uLength,   // The length of the string.
           pfont,    // The text format to apply to the string (contains font information, etc).
           1024.f * 1024.f,               // The width of the on_layout box.
           1024.f * 1024.f,        // The height of the on_layout box.
           &playout  // The IDWriteTextLayout interface pointer.
           );

      if(playout == nullptr || FAILED(hr))
      {

         size.cx = 0;

         size.cy = 0;

         //return false;

         throw_status(error_null_pointer);

      }

      DWRITE_TEXT_METRICS m;

      playout->GetMetrics(&m);

      size.cx = (::i32) (m.widthIncludingTrailingWhitespace  * m_pfont->m_dFontWidth);

      size.cy = (::i32) m.height;

      text.m_size = size;

      text.m_bSize = true;

      //return true;

   }


   void graphics::get_text_extent(size_f64 & size, const char * lpszString, strsize nCount)
   {

      if (nCount < 0)
      {

         nCount = strlen(lpszString) + nCount + 1;

      }

      return get_text_extent(size, lpszString, nCount, nCount);

   }


   void graphics::get_text_extent(size_f64 & size, const ::string & str)
   {

      return get_text_extent(size, str, str.get_length());

   }


   void graphics::fill_rectangle(const ::rectangle_f64 & rectangleParam, const ::color::color & color)
   {

      if (!m_pdevicecontext)
      {

         //return false;

         throw_status(error_null_pointer);

      }

      auto pbrush = __create < ::draw2d::brush > ();

      pbrush->create_solid(color);

      auto posbrush = pbrush->get_os_data < ID2D1Brush * >(this);

      D2D1_RECT_F rectangle;

      __copy(rectangle, rectangleParam);

      defer_primitive_blend();

      m_pdevicecontext->FillRectangle(&rectangle, posbrush);

      //return true;

   }


   //void graphics::text_out(double x, double y, const block & block)
   //{

   //   if (block.is_empty())
   //   {

   //      return false;

   //   }

   //   return ::draw2d::graphics::text_out((double)x, (double)y, string(lpszString, nCount));

   //}


   void graphics::TextOutRaw(double x, double y, const block & block)
   {

      if (block.is_empty())
      {

         //return false;

         throw_status(error_invalid_empty_argument);

      }

      if (m_pfont.is_null())
      {

         //return false;

         throw_status(error_wrong_state);

      }

      IDWriteTextFormat* pfont = m_pfont->get_os_data < IDWriteTextFormat * >(this);

      if (pfont == nullptr)
      {

         //return false;

         throw_status(error_null_pointer);

      }

      if (m_pbrush.is_null())
      {

         throw_status(error_null_pointer);

      }

      ID2D1Brush* pbrush = m_pbrush->get_os_data < ID2D1Brush * >(this);

      if (::is_null(pbrush))
      {

         throw_status(error_null_pointer);

      }

      D2D1::Matrix3x2F m;

      D2D1::Matrix3x2F mOriginal;

      auto & text = m_pfont->m_mapText[block];

      ::size_f64 sizeText;
         
      if (text.m_bSize)
      {

         sizeText = text.m_size;

      }
      else
      {

         sizeText = get_text_extent(block);

      }

      D2D1_RECT_F rectangle_f32 = D2D1::RectF((FLOAT) 0, (FLOAT)0, (FLOAT)(0 + sizeText.cx * 2), (FLOAT)(0 + sizeText.cy * 2));

      HRESULT  hr = pfont->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);

      if(FAILED(hr))
      {

         trace_hr("text_out, SetTextAlignment",hr);

      }

      hr = pfont->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

      if(FAILED(hr))
      {

         trace_hr("text_out, SetTextAlignment",hr);

      }

      DWRITE_TRIMMING trim;

      __zero(trim);

      trim.granularity = DWRITE_TRIMMING_GRANULARITY_NONE;

      hr = pfont->SetTrimming(&trim, nullptr);

      if (FAILED(hr))
      {

         trace_hr("text_out, SetTextAlignment", hr);

      }

      hr = pfont->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);

      if (m_pfont->m_dFontWidth != 1.0)
      {

         m_prendertarget->GetTransform(&mOriginal);

         m = mOriginal;

         m._11 *= (FLOAT)m_pfont->m_dFontWidth;
         m._31 += (FLOAT)x;
         m._32 += (FLOAT)y;

      }
      else
      {

         rectangle_f32.left += (FLOAT)x;
         rectangle_f32.top += (FLOAT)y;
         rectangle_f32.right += (FLOAT)x;
         rectangle_f32.bottom += (FLOAT)y;

      }

      if (m_pfont->m_dFontWidth != 1.0)
      {

         m_prendertarget->SetTransform(&m);

      }

      const widechar * lpcwsz = text.m_wstr;

      strsize uiLen = text.m_wstr.get_length();

      defer_text_primitive_blend();

      defer_text_rendering_hint();

      if(lpcwsz != nullptr && uiLen > 0)
      {

         m_prendertarget->DrawText(lpcwsz,(int) uiLen,pfont,&rectangle_f32,pbrush);

      }

      if (m_pfont->m_dFontWidth != 1.0)
      {

         m_prendertarget->SetTransform(mOriginal);

      }

      //return true;

   }


   void graphics::draw_line(double x1, double y1, double x2, double y2, ::draw2d::pen * ppen)
   {

      D2D1_POINT_2F p1;

      p1.x = (FLOAT) x1;

      p1.y = (FLOAT) y1;

      D2D1_POINT_2F p2;

      p2.x = (FLOAT) x2;

      p2.y = (FLOAT) y2;

      ID2D1Brush * pbrush = m_ppen->get_os_data < ID2D1Brush * >(this);

      if (pbrush == nullptr)
      {

         //return false;

         throw_status(error_null_pointer);

      }

      defer_primitive_blend();

      m_prendertarget->DrawLine(p1, p2, pbrush, (FLOAT)(dynamic_cast < ::draw2d_direct2d::pen * > (m_ppen.m_p))->m_dWidth);

      m_point.x = x2;

      m_point.y = y2;

      //return true;

   }


   void graphics::line_to(double x, double y)
   {

      D2D1_POINT_2F p1;

      __copy(p1, m_point);

      D2D1_POINT_2F p2;

      p2.x = (FLOAT) x;

      p2.y = (FLOAT) y;

      ID2D1Brush * pbrush =  m_ppen->get_os_data < ID2D1Brush * >(this);

      if (pbrush == nullptr)
      {

         //return false;

         throw_status(error_null_pointer);

      }

      defer_primitive_blend();

      FLOAT fWidth = (FLOAT)(dynamic_cast <::draw2d_direct2d::pen *> (m_ppen.m_p))->m_dWidth;

      m_prendertarget->DrawLine(p1, p2, pbrush, fWidth);

      m_point.x = x;

      m_point.y = y;

      //return true;

   }


   void graphics::on_begin_draw()
   {

      reset_clip();

      ::draw2d::graphics::on_begin_draw();

      m_ealphamodeDevice = ::draw2d::e_alpha_mode_none;

      set_alpha_mode(::draw2d::e_alpha_mode_blend);

      //return true;

   }


   void graphics::set_alpha_mode(::draw2d::enum_alpha_mode ealphamode)
   {

      if (m_ealphamode != ealphamode)
      {

        ::draw2d::graphics::set_alpha_mode(ealphamode);

      }

      //return true;

   }


   //HDC graphics::get_handle() const
   //{

   //   __throw(todo);

   //   return

   //}


   //HDC graphics::get_handle1() const
   //{

   //   return get_handle();

   //}


   //HDC graphics::get_handle2() const
   //{

   //   return get_handle();

   //}


   void graphics::attach(void * pdata)
   {

      if(m_pdevicecontext != nullptr)
      {

         m_pdevicecontext = nullptr;

      }

      if(m_prendertarget != nullptr)
      {

         m_prendertarget = nullptr;

      }

      if(m_pbitmaprendertarget != nullptr)
      {

         m_pbitmaprendertarget = nullptr;

      }

      m_pdevicecontext = (ID2D1DeviceContext *) pdata;

      HRESULT hr = m_pdevicecontext.as(m_prendertarget);

      if(FAILED(hr))
      {

         m_pdevicecontext = nullptr;

         m_prendertarget = nullptr;

         //return false;

         throw_status(error_null_pointer);

      }

      hr = m_prendertarget.as(m_pbitmaprendertarget);

      if(FAILED(hr))
      {
         m_pbitmaprendertarget = nullptr;
      }

      m_osdata[data_device_context] = m_pdevicecontext;

      m_osdata[data_render_target] = m_prendertarget;

      //return true;

   }


   void * graphics::detach()
   {

      m_prendertarget = nullptr;

      m_pbitmaprendertarget = nullptr;

      m_osdata[data_device_context] = nullptr;

      m_osdata[data_render_target] = nullptr;

      return m_pdevicecontext.detach();

   }


//#ifdef WINDOWS_DESKTOP
//
//
//   void graphics::attach_hdc(HDC hdc)
//   {
//
//      HWND hwnd = ::WindowFromDC(hdc);
//
//      if (hwnd == nullptr)
//      {
//
//         return false;
//
//      }
//
//      ::rectangle_i32 rectangleClient;
//
//      ::GetClientRect(hwnd, rectangleClient);
//
//      // Create a DC render target.
//      D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
//                                            D2D1_RENDER_TARGET_TYPE_DEFAULT,
//                                            D2D1::PixelFormat(
//                                            DXGI_FORMAT_B8G8R8A8_UNORM,
//                                            D2D1_ALPHA_MODE_IGNORE),
//                                            0,
//                                            0,
//                                            D2D1_RENDER_TARGET_USAGE_NONE,
//                                            D2D1_FEATURE_LEVEL_DEFAULT
//                                            );
//
//      HRESULT hr = ::direct2d::direct2d()->d2d1_factory1()->CreateDCRenderTarget(&props,&m_pdcrendertarget);
//
//      if (FAILED(hr))
//      {
//
//         return false;
//
//      }
//
//      hr = m_pdcrendertarget->BindDC(hdc, rectangleClient);
//
//      m_pdcrendertarget.As(&m_prendertarget);
//
//      m_hdcAttach = hdc;
//
//      return true;
//
//   }
//
//
//   HDC graphics::detach_hdc()
//   {
//
//      if (m_hdcAttach == nullptr)
//      {
//
//         return nullptr;
//
//      }
//
//      HDC hdc = m_hdcAttach;
//
//      m_hdcAttach = nullptr;
//
//      return hdc;
//   }
//
//#endif


   void graphics::blur(bool bExpand, double dRadius, const ::rectangle_f64 & rectangle)
   {

      __throw(todo);

      //return false;

   }


   double graphics::get_dpix()
   {
      
      return 72.0;

   }


   void graphics::destroy()
   {

      ::draw2d::lock draw2dlock;

      ::draw2d::device_lock devicelock(this);

      if(m_player != nullptr)
      {

         m_prendertarget->PopLayer();

         m_player = nullptr;

      }

      m_ppathgeometryClip = nullptr;

      m_prendertarget = nullptr;

      m_pdevicecontext = nullptr;

      m_pbitmaprendertarget = nullptr;

      clear_os_data();

      //return ::success;

   }


   void graphics::clear_current_point()
   {

      //return ::success;

   }


   void graphics::draw_path(::draw2d::path * ppath)
   {

      return draw_path(ppath, m_ppen);

   }


   bool graphics::draw_path(ID2D1PathGeometry * pgeometry, ::draw2d::pen * ppen)
   {

      ::ID2D1Brush * pbrush = ppen->get_os_data < ID2D1Brush * >(this);

      if (pbrush == nullptr)
      {

         return false;

      }

      m_prendertarget->DrawGeometry(pgeometry, pbrush, (FLOAT)ppen->m_dWidth);

      return true;

   }


   bool graphics::fill_path(ID2D1PathGeometry * pgeometry, ::draw2d::brush * pbrushParam)
   {

      ::ID2D1Brush * pbrush = pbrushParam->get_os_data < ID2D1Brush * >(this);

      if (pbrush == nullptr)
      {

         return false;

      }

      m_prendertarget->FillGeometry(pgeometry, pbrush);

      return true;

   }


   void graphics::draw_path(::draw2d::path * ppath, ::draw2d::pen * ppen)
   {

      __stack(m_bOutline, true);

      ID2D1PathGeometry * pgeometry = ppath->get_os_data < ID2D1PathGeometry * >(this, path_hollow);

      defer_primitive_blend();

      if(pgeometry != nullptr)
      {

         draw_path(pgeometry, ppen);

      }

      //for(index i = 0; i < ppath->m_shapea.get_size(); i++)
      //{

      //   if(ppath->m_shapea[i]->eshape() == e_shape_text_out)
      //   {

      //      draw(ppath->m_shapea[i]->shape < ::write_text::text_out>(), ppen);

      //   }
      //   else if (ppath->m_shapea[i]->eshape() == e_shape_draw_text)
      //   {

      //      draw(ppath->m_shapea[i]->shape < ::write_text::draw_text>(), ppen);

      //   }

      //}

      //return true;

   }


   void graphics::fill_path(::draw2d::path * ppath)
   {

      if (ppath == nullptr)
      {

         //return false;

         throw_status(error_null_pointer);

      }

      __stack(m_bOutline, false);

      ID2D1PathGeometry * pgeometry = ppath->get_os_data < ID2D1PathGeometry * >(this, path_filled);

      defer_primitive_blend();

      if (pgeometry != nullptr)
      {

         fill_path(pgeometry, m_pbrush);

      }

      //for (index i = 0; i < ppath->m_shapea.get_size(); i++)
      //{

      //   if (ppath->m_shapea[i]->eshape() == ::e_shape_text_out)
      //   {

      //      fill(ppath->m_shapea[i]->shape < ::write_text::text_out >(), m_pbrush);

      //   }
      //   else if (ppath->m_shapea[i]->eshape() == ::e_shape_draw_text)
      //   {

      //      fill(ppath->m_shapea[i]->shape < ::write_text::draw_text >(), m_pbrush);

      //   }

      //}

      //return true;

   }


   void graphics::fill_path(::draw2d::path * ppath, ::draw2d::brush * pbrushParam)
   {

      if (ppath == nullptr)
      {

         //return false;

         throw_status(error_null_pointer);

      }

      ID2D1Brush * pbrush = pbrushParam->get_os_data < ID2D1Brush * >(this);

      if (pbrush == nullptr)
      {

         //return false;

         throw_status(error_null_pointer);

      }

      __stack(m_bOutline, false);

      ID2D1PathGeometry * pgeometry = ppath->get_os_data < ID2D1PathGeometry * >(this, path_filled);

      defer_primitive_blend();

      if (pgeometry != nullptr)
      {

         m_prendertarget->FillGeometry(pgeometry, pbrush);

      }

      //for (index i = 0; i < ppath->m_shapea.get_size(); i++)
      //{

      //   if (ppath->m_shapea[i]->eshape() == ::e_shape_text_out)
      //   {

      //      fill(ppath->m_shapea[i]->shape < ::write_text::text_out >(), pbrushParam);

      //   }
      //   else if (ppath->m_shapea[i]->eshape() == ::e_shape_draw_text)
      //   {

      //      fill(ppath->m_shapea[i]->shape < ::write_text::draw_text >(), pbrushParam);

      //   }

      //}

      //return true;

   }


   void graphics::path(::draw2d::path * ppath)
   {

      if (ppath == nullptr)
      {

         //return false;

         throw_status(error_null_pointer);

      }

      //bool bOk1 = 
      
      fill_path(ppath);

      //bool bOk2 = 
      
      draw_path(ppath);

      //return bOk1 && bOk2;

   }


   bool graphics::draw(const ::write_text::text_out & textout, ::draw2d::pen * ppen)
   {

      //wstring szOutline(textout.m_strText);

      //IDWriteTextFormat * pformat = textout.m_pfont->get_os_data < IDWriteTextFormat * > (this);

      //IDWriteFactory * pfactory = ::direct2d::direct2d()->dwrite_factory();

      //comptr<IDWriteTextLayout> playout;

      //HRESULT hr = pfactory->CreateTextLayout(
      //             szOutline,      // The string to be laid out and formatted.
      //             (::u32) szOutline.length(),  // The length of the string.
      //             pformat,  // The text format to apply to the string (contains font information, etc).
      //             4096,         // The width of the on_layout box.
      //             4096,        // The height of the on_layout box.
      //             &playout  // The IDWriteTextLayout interface pointer.
      //             );

      //if(playout == nullptr)
      //{

      //   return false;

      //}

      //CustomTextRenderer renderer(::direct2d::direct2d()->d2d1_factory1(),m_prendertarget,ppen->get_os_data < ID2D1Brush * >(this));

      //defer_text_primitive_blend();

      //defer_text_rendering_hint();

      //playout->Draw(nullptr, &renderer, (FLOAT) textout.m_point.x, (FLOAT) textout.m_point.y);

      return true;

   }


   bool graphics::fill(const ::write_text::text_out & textout, ::draw2d::brush * pbrush)
   {

      //wstring szOutline(textout.m_strText);

      //IDWriteTextFormat * pformat = textout.m_pfont->get_os_data < IDWriteTextFormat * >(this);

      //IDWriteFactory * pfactory = ::direct2d::direct2d()->dwrite_factory();

      //comptr<IDWriteTextLayout> playout;

      //HRESULT hr = pfactory->CreateTextLayout(
      //             szOutline,      // The string to be laid out and formatted.
      //             (::u32) szOutline.length(),  // The length of the string.
      //             pformat,  // The text format to apply to the string (contains font information, etc).
      //             4096,         // The width of the on_layout box.
      //             4096,        // The height of the on_layout box.
      //             &playout  // The IDWriteTextLayout interface pointer.
      //             );

      //if (playout == nullptr)
      //{
      //   
      //   return false;

      //}

      //auto posbrush = pbrush->get_os_data < ID2D1Brush * >(this);

      //if (posbrush)
      //{

      //   CustomTextRenderer renderer(::direct2d::direct2d()->d2d1_factory1(), m_prendertarget, nullptr, posbrush);

      //   defer_text_primitive_blend();

      //   defer_text_rendering_hint();

      //   playout->Draw(nullptr, &renderer, (FLOAT)textout.m_point.x, (FLOAT)textout.m_point.y);

      //}

      return true;

   }


   bool graphics::draw(const ::write_text::draw_text & drawtext, ::draw2d::pen* ppen)
   {

      //wstring szOutline(drawtext.m_strText);

      //IDWriteTextFormat* pformat = drawtext.m_pfont->get_os_data < IDWriteTextFormat* >(this);

      //IDWriteFactory* pfactory = ::direct2d::direct2d()->dwrite_factory();

      //IDWriteTextLayout* playout = nullptr;

      //HRESULT hr = pfactory->CreateTextLayout(
      //   szOutline,      // The string to be laid out and formatted.
      //   (::u32)szOutline.length(),  // The length of the string.
      //   pformat,  // The text format to apply to the string (contains font information, etc).
      //   4096,         // The width of the on_layout box.
      //   4096,        // The height of the on_layout box.
      //   &playout  // The IDWriteTextLayout interface pointer.
      //);

      //if (playout == nullptr)
      //{

      //   return false;

      //}

      //CustomTextRenderer renderer(::direct2d::direct2d()->d2d1_factory1(), m_prendertarget, ppen->get_os_data < ID2D1Brush* >(this));

      //defer_text_primitive_blend();

      //defer_text_rendering_hint();

      //playout->Draw(nullptr, &renderer, (FLOAT)drawtext.m_rectangle.left, (FLOAT)drawtext.m_rectangle.top);

      return true;

   }


   bool graphics::fill(const ::write_text::draw_text & drawtext, ::draw2d::brush* pbrush)
   {

      //wstring szOutline(drawtext.m_strText);

      //IDWriteTextFormat* pformat = drawtext.m_pfont->get_os_data < IDWriteTextFormat* >(this);

      //IDWriteFactory* pfactory = ::direct2d::direct2d()->dwrite_factory();

      //IDWriteTextLayout* playout = nullptr;

      //HRESULT hr = pfactory->CreateTextLayout(
      //   szOutline,      // The string to be laid out and formatted.
      //   (::u32)szOutline.length(),  // The length of the string.
      //   pformat,  // The text format to apply to the string (contains font information, etc).
      //   4096,         // The width of the on_layout box.
      //   4096,        // The height of the on_layout box.
      //   &playout  // The IDWriteTextLayout interface pointer.
      //);

      //if (playout == nullptr)
      //{

      //   return false;

      //}

      //auto posbrush = pbrush->get_os_data < ID2D1Brush* >(this);

      //if (posbrush)
      //{

      //   CustomTextRenderer renderer(::direct2d::direct2d()->d2d1_factory1(), m_prendertarget, nullptr, posbrush);

      //   defer_text_primitive_blend();

      //   defer_text_rendering_hint();

      //   playout->Draw(nullptr, &renderer, (FLOAT)drawtext.m_rectangle.left, (FLOAT)drawtext.m_rectangle.top);

      //}

      return true;

   }


   void graphics::flush()
   {

      if (!m_bBeginDraw)
      {

         //return false;

         throw_status(error_null_pointer);

      }

      HRESULT hr = m_prendertarget->Flush();

      //return SUCCEEDED(hr);

   }


   void graphics::debug()
   {

#ifdef _DEBUG
      directx_debug();
#endif

   }


   //void graphics::enum_fonts(::write_text::font_enumeration_item_array & itema)
   //{


   //}


   //void graphics::set_direct2d_plugin(::draw2d_direct2d::plugin * pplugin)
   //{

   //   ::direct2d::direct2d() = pplugin;

   //}


} // namespace draw2d_direct2d






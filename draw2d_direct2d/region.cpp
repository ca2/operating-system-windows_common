#include "framework.h"


namespace draw2d_direct2d
{


   region::region()
   {

      //m_bUpdated = false;

   }


   region::~region()
   {

      destroy();

   }


   //int region::GetRegionData(LPRGNDATA lpRgnData, int nDataSize) const
   //{

   //   throw ::exception(todo);

   //   //   ASSERT(get_os_data() != nullptr); return (int)::GetRegionData((HRGN)get_os_data(), nDataSize, lpRgnData);

   //}
   void region::SetRectRgn(int x1, int y1, int x2, int y2)
   {

      throw ::exception(todo);

      //ASSERT(get_os_data() != nullptr); ::SetRectRgn((HRGN)get_os_data(), x1, y1, x2, y2);
   }

   void region::SetRectRgn(const ::rectangle_i32 & rectangle)
   {

      throw ::exception(todo);

      //   ::SetRectRgn((HRGN)get_os_data(), rectangle.left, rectangle.top, rectangle.right, rectangle.bottom);
   }

   //int region::CombineRgn(const ::draw2d::region* pRgn1, const ::draw2d::region* pRgn2, int nCombineMode)
   //{

   //   throw ::exception(todo);

   //   //ASSERT(get_os_data() != nullptr); return ::CombineRgn((HRGN)get_os_data(), (HRGN)pRgn1->get_os_data(),
   //   //(HRGN)pRgn2->get_os_data(), nCombineMode);
   //}
   //int region::CopyRgn(const ::draw2d::region* pRgnSrc)
   //{

   //   throw ::exception(todo);

   //   //ASSERT(get_os_data() != nullptr); return ::CombineRgn((HRGN)get_os_data(), (HRGN)pRgnSrc->get_os_data(), nullptr, RGN_COPY);
   //}

   //bool region::EqualRgn(const ::draw2d::region* pRgn) const
   //{

   //   throw ::exception(todo);


   //   //   ASSERT(get_os_data() != nullptr);

   //   // return ::EqualRgn((HRGN)get_os_data(), (HRGN)pRgn->get_os_data()) != false;

   //}

   //int region::OffsetRgn(int x, int y)
   //{

   //   throw ::exception(todo);


   //   //ASSERT(get_os_data() != nullptr); return ::OffsetRgn((HRGN)get_os_data(), x, y);
   //}
   //int region::OffsetRgn(const ::point_i32 & point)
   //{

   //   throw ::exception(todo);


   //   //ASSERT(get_os_data() != nullptr); return ::OffsetRgn((HRGN)get_os_data(), point.x, point.y);
   //}
   //int region::GetRgnBox(RECTANGLE_I32 * prectangle) const
   //{

   //   throw ::exception(todo);


   //   //ASSERT(get_os_data() != nullptr); return ::GetRgnBox((HRGN)get_os_data(), rectangle);
   //}


   bool region::contains(::draw2d::graphics* pgraphics, int x, int y) const
   {

      ((region *) this)->defer_update(pgraphics, 0);

      BOOL b = false;

      HRESULT hr = m_pgeometry->FillContainsPoint(D2D1::Point2F((FLOAT) x, (FLOAT) y), nullptr, &b);

      if(FAILED(hr))
         return false;

      return b != false;

      //Gdiplus::PointF point_f32((Gdiplus::REAL) x, (Gdiplus::REAL) y);
      //
      //ASSERT(get_os_data() != nullptr); //return ::PtInRegion((HRGN)get_os_data(), x, y);

      //return m_pgeometry->IsVisible(point_f32)  != false;

   }


   bool region::contains(::draw2d::graphics* pgraphics, const ::point_i32 & point) const
   {

      return contains(pgraphics, point.x, point.y);

      //throw ::exception(todo);


      //return ::PtInRegion((HRGN)get_os_data(), point.x, point.y);
      //Gdiplus::PointF point_f32((Gdiplus::REAL) point.x, (Gdiplus::REAL) point.y);
      //
      //ASSERT(get_os_data() != nullptr); //return ::PtInRegion((HRGN)get_os_data(), x, y);

      //return m_pgeometry->IsVisible(point_f32)  != false;

   }


   bool region::RectInRegion(const ::rectangle_i32 & rectangle) const
   {

      throw ::exception(todo);


      //ASSERT(get_os_data() != nullptr); return ::rectInRegion((HRGN)get_os_data(), rectangle);

      //Gdiplus::RectF rectangle_f32((Gdiplus::REAL) rectangle.left, (Gdiplus::REAL) rectangle.top, (Gdiplus::REAL) (rectangle.right - rectangle.left), (Gdiplus::REAL) (rectangle.bottom - rectangle.top));
      //
      //ASSERT(get_os_data() != nullptr); //return ::PtInRegion((HRGN)get_os_data(), x, y);

      //return m_pgeometry->IsVisible(rectangle_f32)  != false;
      return false;

   }


   void region::create(::draw2d::graphics * pgraphics, i8 iCreate)
   {

      m_osdata[0] = get(pgraphics);

      //return m_pgeometry != nullptr;

   }


   ID2D1Geometry * region::get(::draw2d::graphics* pgraphics)
   {

      switch(m_eregion)
      {
      case ::draw2d::e_region_none:
      {

         ID2D1PathGeometry * ppathgeometry = nullptr;

         HRESULT hr = ::direct2d::direct2d()->d2d1_factory1()->CreatePathGeometry(&ppathgeometry);

         if(FAILED(hr))
            return nullptr;

         return ppathgeometry;

      }
      case ::draw2d::e_region_rect:
         return get_rectangle(pgraphics);
      case ::draw2d::e_region_ellipse:
         return get_ellipse(pgraphics);
      case ::draw2d::e_region_polygon:
         return get_polygon(pgraphics);
      case ::draw2d::e_region_poly_polygon:
         return get_polygon(pgraphics);
      case ::draw2d::e_region_combine:
         return get_combine(pgraphics);
      default:
         throw ::interface_only();
      }

      return nullptr;

   }


   ID2D1Geometry * region::get_rectangle(::draw2d::graphics* pgraphicsParam)
   {

      ID2D1RectangleGeometry * pgeometry = nullptr;

      auto pgraphics = __graphics(pgraphicsParam);

      if (!pgraphics)
      {

         return nullptr;

      }

      __pointer(rectangle_item) pitem = m_pitem;

      D2D1_RECT_F r;
      
      __copy(r, pitem->m_rectangle);

      ::direct2d::direct2d()->d2d1_factory1()->CreateRectangleGeometry(r, &pgeometry);

      return pgeometry;

   }


   ID2D1Geometry * region::get_ellipse(::draw2d::graphics* pgraphics)
   {

      D2D1_ELLIPSE ellipse;

      __pointer(ellipse_item) pitem = m_pitem;

      auto pointCenter = pitem->m_rectangle.center();
      auto sizeRadius = pitem->m_rectangle.size() / 2.0;

      ellipse.point.x = (float)pointCenter.x;
      ellipse.point.y = (float)pointCenter.y;
      ellipse.radiusX = (float)sizeRadius.cx;
      ellipse.radiusY = (float)sizeRadius.cy;

      ID2D1EllipseGeometry * pgeometry = nullptr;

      ::direct2d::direct2d()->d2d1_factory1()->CreateEllipseGeometry(ellipse, &pgeometry);

      return pgeometry;

   }


   ID2D1Geometry * region::get_polygon(::draw2d::graphics* pgraphics)
   {

      auto ppath = pgraphics->__create < ::draw2d::path > ();

      /*point_i32_array pa;

      for(int i = 0; i < m_nCount; i++)
      {
         pa.add(Gdiplus::PointF((Gdiplus::REAL) m_lppoints[i].x, (Gdiplus::REAL) m_lppoints[i].y));
      }*/

      /*
      if(m_efillmode == ::draw2d::e_fill_mode_alternate)
      {
         path.SetFillMode(Gdiplus::FillModeAlternate);
      }
      else
      {
         path.SetFillMode(Gdiplus::FillModeWinding);
      }
      */

      __pointer(polygon_item) pitem = m_pitem;

      ppath->begin_figure();
      ppath->add_polygon(pitem->m_polygon.get_data(), pitem->m_polygon.get_size());
      ppath->close_figure();

      ppath->get_os_data(pgraphics, path_filled);

      m_pgeometry = ::move((ID2D1PathGeometry *) ppath->detach());

      return m_pgeometry;

   }


   ID2D1Geometry * region::get_poly_polygon(::draw2d::graphics* pgraphics)
   {

      auto ppath = pgraphics->__create < ::draw2d::path > ();

      point_f64_array pa;

      /*if(m_efillmode == ::draw2d::e_fill_mode_alternate)
      {
         path.SetFillMode(Gdiplus::FillModeAlternate);
      }
      else
      {
         path.SetFillMode(Gdiplus::FillModeWinding);
      }*/

      int n = 0;

      __pointer(poly_polygon_item) pitem = m_pitem;

      for(int i = 0; i < pitem->m_polygona.get_size(); i++)
      {
         auto ppolygon = pitem->m_polygona[i];
         int jCount = ppolygon->get_size();
         //pa.erase_all();
         //for(int j = 0; j < jCount; j++)
         //{
         //   pa.add(point_f64(m_lppoints[n].x, m_lppoints[n].y));
         //   n++;
         //}
         //ppath->begin_figure(true, m_efillmode);
         ppath->begin_figure();
         ppath->add_lines(ppolygon->get_data(), (int) ppolygon->get_size());
         //ppath->end_figure(true);
         ppath->close_figure();
      }

      return (ID2D1PathGeometry *) ppath->detach();

   }


   ID2D1Geometry * region::get_combine(::draw2d::graphics* pgraphics)
   {

      comptr <ID2D1PathGeometry> ppathgeometry ;

      HRESULT hr = ::direct2d::direct2d()->d2d1_factory1()->CreatePathGeometry(&ppathgeometry);

      comptr < ID2D1GeometrySink > psink ;

      if (FAILED(hr))
      {

         return nullptr;

      }

      hr = ppathgeometry->Open(&psink);

      if(FAILED(hr))
      {

         return nullptr;

      }

      __pointer(combine_item) pitem = m_pitem;

      auto pgeometry1 = pitem->m_pregion1->get_os_data < ID2D1Geometry * >(pgraphics);

      auto pgeometry2 = pitem->m_pregion2->get_os_data < ID2D1Geometry * >(pgraphics);

      if(pitem->m_ecombine == ::draw2d::e_combine_add)
      {

         hr = pgeometry1->CombineWithGeometry(pgeometry2, D2D1_COMBINE_MODE_UNION, nullptr, 0.f, psink);

      }
      else if(pitem->m_ecombine == ::draw2d::e_combine_exclude)
      {

         hr = pgeometry1->CombineWithGeometry(pgeometry2, D2D1_COMBINE_MODE_EXCLUDE, nullptr, 0.f, psink);

      }
      else if(pitem->m_ecombine == ::draw2d::e_combine_intersect)
      {

         hr = pgeometry1->CombineWithGeometry(pgeometry2, D2D1_COMBINE_MODE_INTERSECT, nullptr, 0.f, psink);

      }
      else
      {

         hr = pgeometry1->CombineWithGeometry(pgeometry2, D2D1_COMBINE_MODE_UNION, nullptr, 0.f, psink);

      }

      if(FAILED(hr))
      {

         return nullptr;

      }


      hr = psink->Close();

      if(FAILED(hr))
      {

         return nullptr;

      }

      return ppathgeometry.detach();

   }


   void region::destroy()
   {

      destroy_os_data();

   }


   void region::destroy_os_data()
   {

      m_pgeometry = nullptr;

      object::destroy_os_data();

   }


} // namespace draw2d_direct2d




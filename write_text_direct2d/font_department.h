// Created by camilo 2021-02-10 <3TBS_!!
#pragma once


namespace write_text_direct2d
{


   class CLASS_DECL_WRITE_TEXT_DIRECT2D font_department :
      virtual public ::font_department
   {
   public:


    /*  __pointer(::write_text::font_enumeration)   m_pfontenumeration;*/

      //::write_text::font_pointer                m_fontCaption;
      //::write_text::font_pointer                m_fontMenu;
      //::write_text::font_pointer                m_fontStandard;
      //::write_text::font_pointer                m_font;
      //::write_text::font_pointer                m_fontListCtrl;
      //::write_text::font_pointer                m_fontMeshCtrl;
      //::write_text::font_pointer                m_fontGridCtrl;
      //__pointer(::write_text::font)                 m_lpSongLabelFont;
      //__pointer(::write_text::font)                 m_lpSongListFont;
      //__pointer(::write_text::font)                 m_pfontLyric;
      //__pointer(::write_text::font)                 m_pfontLyricCompact;
      //__pointer(::write_text::font)                 m_pxffontMidiTrackName;
      //draw2d_font_spa              m_pTitleFonts;
      //draw2d_font_spa              m_pSubTitleFonts;
      //draw2d_font_spa              m_pH3TitleFonts;
      //bool                             m_bInitialized;
      //bool                             m_bLyric;


      //::write_text::font * GetListCtrlFont();
      //::write_text::font * GetMeshCtrlFont();
      //::write_text::font * GetGridCtrlFont();
      //::write_text::font * GetCaptionFont();
      //::write_text::font * GetStandartFont();
      //::write_text::font * GetSongListFont();
      //::write_text::font * GetSongLabelFont();
      //::write_text::font * GetTitleFont();
      //::write_text::font * GetSubTitleFont();
      //::write_text::font * GetH3TitleFont();
      //::write_text::font * GetMidiTrackNameFont();
      //::write_text::font * GetMenuFont();
      //::write_text::font * GetLyricViewFont();
      //::write_text::font * GetLyricViewCompactFont();
      //draw2d_font_spa & GetTitleFonts();
      //draw2d_font_spa & GetSubTitleFonts();
      //draw2d_font_spa & GetH3TitleFonts();

      //void CreateLyricViewFonts();
      //void DestroyLyricViewFonts();


      font_department();
      virtual ~font_department();


      virtual ::e_status initialize(::object * pobject);
      virtual ::e_status finalize();


      //virtual bool is_initialized();
      //virtual void defer_create_font_enumeration(::subject::subject * psubject);


      virtual void enum_fonts(::write_text::font_enum_item_array & itema);
      //virtual void sorted_fonts(::write_text::font_enum_item_array & itema);


   };


} // namespace write_text_direct2d




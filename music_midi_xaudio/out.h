#pragma once


namespace music
{


   namespace midi
   {


      namespace xaudio
      {

         class CLASS_DECL_MUSIC_MIDI_XAUDIO out :
            virtual public ::music::midi::out
         {
         public:


            //HMIDIOUT       m_hmidiout;


            out();
            ~out() override;


            virtual void     open();
            virtual void     close();

            void     send_short_message(::music::midi::e_message emessage, int iChannel, int iData1, int iData2) override;


         };


      } // namespace xaudio


   } // namespace midi


} // namespace music





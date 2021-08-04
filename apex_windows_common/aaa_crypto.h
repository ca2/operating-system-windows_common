#pragma once


namespace windows
{


   class CLASS_DECL_APEX_WINDOWS_COMMON crypto :
      virtual public ::crypto::crypto
   {
   public:


      crypto();
      virtual ~crypto();

      ::file::path get_crypt_key_file_path();
      virtual bool encrypt(memory & storageEncrypt, const memory & storageDecrypt, const ::string & pszSalt);
      virtual bool decrypt(memory & storageDecrypt, const memory & storageEncrypt, const ::string & pszSalt);


   };


} // namespace axis




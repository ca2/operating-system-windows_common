#include "framework.h"
#include "acme/operating_system.h"


#include "_file_c.h"
#include "file.h"

#ifdef _UWP
CLASS_DECL_ACME::winrt::Windows::Storage::StorageFolder^ winrt_folder(string& strPath, string& strPrefix);
#endif


::u32 windows_get_file_attributes(const char* path)
{

   wstring wstr(path);

   u32 dwFileAttributes = ::GetFileAttributesW(wstr);

   if (dwFileAttributes == INVALID_FILE_ATTRIBUTES)
   {

      if (windows_get_alternate_path(wstr))
      {

         dwFileAttributes = GetFileAttributesW(wstr);

      }

   }

   return dwFileAttributes;

}


int_bool windows_create_directory(const char* path, LPSECURITY_ATTRIBUTES lpSecurityAttributes)
{

   wstring wstr(path);


   if (::CreateDirectoryW(wstr, lpSecurityAttributes))
   {

      return true;

   }

   if (windows_get_alternate_path(wstr))
   {

      if (::CreateDirectoryW(wstr, lpSecurityAttributes))
      {

         return true;

      }

   }

   return false;

}




//extern "C"
int_bool file_copy_dup(const char * pszNew, const char * pszSrc, int_bool bOverwrite)
{

   string strNew(pszNew);

   if (strNew.get_length() >= MAX_PATH)
   {

      strNew = "\\\\.\\" + strNew;

   }

   string strSrc(pszSrc);

   if (strSrc.get_length() >= MAX_PATH)
   {

      strSrc = "\\\\.\\" + strSrc;

   }

   wstring wstrNew(strNew);

   wstring wstrSrc(strSrc);

   //return ::CopyFileExW(wstrSrc,wstrNew, nullptr, nullptr, nullptr, COPY_FILE_NO_BUFFERING | (bOverwrite ? 0 : COPY_FILE_FAIL_IF_EXISTS)) ? true : false;
   auto bCopy = ::CopyFileExW(wstrSrc, wstrNew, nullptr, nullptr, nullptr, (bOverwrite ? 0 : COPY_FILE_FAIL_IF_EXISTS)) ? true : false;

   if (!bCopy)
   {

      TRACELASTERROR();

      return false;

   }

   return true;

}









::file::path get_final_path(const ::file::path & path)
{

   wstring wstr(path);

   HANDLE hfile = INVALID_HANDLE_VALUE;

   try
   {

      if (         auto psystem = m_psystem;

         auto pacmedir = psystem->m_pacmedir;

pacmedir->is(path))
      {

         hfile = hfile_create(path,
                             GENERIC_READ,          // open for reading
                             FILE_SHARE_READ,       // share for reading
                             nullptr,                  // default security
                             OPEN_EXISTING,         // existing file only
                             FILE_ATTRIBUTE_NORMAL, // normal file
                             nullptr);                 // no attr. template

      }
      else
      {

         hfile = hfile_create(path,
                             FILE_LIST_DIRECTORY,          // open for reading
                             FILE_SHARE_READ,       // share for reading
                             nullptr,                  // default security
                             OPEN_EXISTING,         // existing file only
                             FILE_ATTRIBUTE_NORMAL, // normal file
                             nullptr);                 // no attr. template

      }

      if (hfile == INVALID_HANDLE_VALUE)
      {

         return wstr;

      }

      DWORD dw = GetFinalPathNameByHandleW(hfile, nullptr, 0, VOLUME_NAME_DOS);

      if (dw > 0)
      {

         wstring wstr2;

         auto * pwsz = wstr2.get_string_buffer(dw + 1);

         dw = GetFinalPathNameByHandleW(hfile, pwsz, dw + 1, VOLUME_NAME_DOS);

         if (dw > 0)
         {

            wstr2.release_string_buffer();

            wstr = wstr2;

         }

      }

   }
   catch (...)
   {

   }

   ::CloseHandle(hfile);

   return wstr;

}


CLASS_DECL_ACME bool __node_further_file_is_equal(const ::file::path & path1, const ::file::path & path2)
{

   ::file::path pathFull1 = get_final_path(path1);

   ::file::path pathFull2 = get_final_path(path2);

   return pathFull1.has_char() && pathFull1.compare_ci(pathFull2) == 0;

}


int_bool m_psystem->m_pacmepath->is_file_or_dir(const char * path, ::file::enum_type * petype)
{

   auto dwFileAttributes = windows_get_file_attributes(path);

   if (dwFileAttributes == INVALID_FILE_ATTRIBUTES)
   {

#ifdef _UWP

      DWORD dwLastError = ::GetLastError();

      string strPrefix;

      string strRelative = path;

      auto folderBase = winrt_folder(strRelative, strPrefix);

      if (folderBase != nullptr)
      {

         strRelative.replace("/", "\\");

         strPrefix.replace("/", "\\");

         ::str::begins_eat_ci(strRelative, strPrefix);

         strRelative.trim("/\\");

         //strPrefix.trim_right("/\\");

         try
         {

            auto item = ::wait(folderBase->GetItemAsync(strRelative));

            if (item != nullptr)
            {

               if (item->IsOfType(::winrt::Windows::Storage::StorageItemTypes::Folder))
               {

                  if (is_set(petype))
                  {

                     *petype = ::file::e_type_folder;

                  }

                  return true;

               }
               else if (item->IsOfType(::winrt::Windows::Storage::StorageItemTypes::File))
               {

                  if (is_set(petype))
                  {

                     *petype = ::file::e_type_file;

                  }

                  return true;

               }

               return false;

            }

         }
         catch (...)
         {

         }

      }

#endif

      if (is_set(petype))
      {

         *petype = ::file::e_type_none;

      }

      return false;

   }

   if (is_set(petype))
   {

      if (dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
      {

         *petype = ::file::e_type_folder;

      }
      else
      {

         *petype = ::file::e_type_file;

      }

   }

   return true;

}






//bool context::_os_resolve_alias(::file::path & path, const char * psz, ::user::primitive * pinteraction, bool bNoUI, bool bNoMount)
//{
//
//   if (os_is_alias(psz))
//   {
//
//      return os().resolve_link(path, psz, nullptr, nullptr, pinteraction);
//
//   }
//
//   return false;
//
//}


CLASS_DECL_ACME bool _os_may_have_alias(const char * psz)
{

   string str(psz);

   return str.ends_ci(".lnk") || str.contains_ci(".lnk/") || str.contains_ci(".lnk\\");

}

//
//bool context::os_is_alias(const char * psz)
//{
//
//   return ::str::ends_ci(psz, ".lnk");
//
//}





CLASS_DECL_ACME memsize hfile_read(hfile hfile, void* p, memsize s)
{

   DWORD dwRead = 0;

   if (!::ReadFile(hfile, p, (DWORD) minimum(INT_MAX, s), &dwRead, nullptr))
   {

      return 0;

   }

   return dwRead;

}


CLASS_DECL_ACME::e_status hfile_write(hfile hfile, const void* p, memsize s)
{

   memsize dwWrittenTotal = 0;

   while (s > 0)
   {

      int iWrite = (int) minimum(INT_MAX, s);

      DWORD dwWritten = 0;

      if (!WriteFile(hfile, p, iWrite, &dwWritten, nullptr))
      {

         return ::error_failed;

      }

      if (dwWritten != iWrite)
      {

         return ::error_failed;

      }

      s -= iWrite;

   }

   return ::success;

}

namespace path
{


   ::file::path module(HMODULE hmodule)
   {


#if defined(_UWP)

      return "m_app.exe";

#else

      wstring wstr(get_buffer, MAX_PATH * 8);

      ::GetModuleFileNameW(hmodule, wstr, (::u32)wstr.length());

      return solve_relative(string(wstr.release_string_buffer()));

#endif


   }


} // namespace path 






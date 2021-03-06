#pragma once


class CLASS_DECL_ACME_WINDOWS_COMMON hresult_exception :
   public ::exception
{
public:


   hresult_exception(HRESULT hresult, const char * pszMessage = nullptr);
   virtual ~hresult_exception();


};


CLASS_DECL_ACME_WINDOWS_COMMON int trace_hr(const char * psz, HRESULT hr);


inline void throw_if_failed(HRESULT hr)
{


   if (FAILED(hr))
   {

      throw hresult_exception(hr);

   }


}


﻿#include "framework.h"
#include "acme/operating_system.h"



#include "acme/platform/static_start.h"


VOID
WINAPI
output_debug_string_a(
_In_opt_ const char * pOutputString

)
{

   ::output_debug_string(wstring(pOutputString));


}

VOID
WINAPI
output_debug_string_w(
_In_opt_ const widechar * pOutputString

)
{

   ::output_debug_string(pOutputString);


}

typedef WINBASEAPI
VOID
WINAPI
FN_OutputDebugStringA(
_In_opt_ const char * pOutputString

);

typedef WINBASEAPI
VOID
WINAPI
FN_OutputDebugStringW(
_In_opt_ const widechar * pOutputString

);

typedef FN_OutputDebugStringA * PFN_OutputDebugStringA;

typedef FN_OutputDebugStringW * PFN_OutputDebugStringW;

PFN_OutputDebugStringA g_pfnOutputDebugStringA = ::OutputDebugStringA;

PFN_OutputDebugStringW g_pfnOutputDebugStringW = ::OutputDebugStringW;


namespace acme
{


   CLASS_DECL_ACME void set_simple_output_debug_string_a()
   {
      g_pfnOutputDebugStringA = ::OutputDebugStringA;
   }

   CLASS_DECL_ACME void set_extended_output_debug_string_a()
   {
      g_pfnOutputDebugStringA = ::output_debug_string_a;
   }

   CLASS_DECL_ACME void set_simple_output_debug_string_w()
   {
      g_pfnOutputDebugStringW = ::OutputDebugStringW;
   }

   CLASS_DECL_ACME void set_extended_output_debug_string_w()
   {
      g_pfnOutputDebugStringW = ::OutputDebugStringW;
   }


} // namespace acme



void output_debug_string(const char * psz)
{

   g_pfnOutputDebugStringA(psz);

}


void w_output_debug_string(const unichar * pwsz)
{

   g_pfnOutputDebugStringW(pwsz);

}


void output_debug_string(const unichar * pwsz)
{

   g_pfnOutputDebugStringW(pwsz);

}



namespace process
{


   bool set_priority(::e_priority epriority)
   {

      return (::SetPriorityClass(::GetCurrentProcess(), get_os_priority_class(epriority)) != 0);


   }


} // namespace process







#ifdef WINDOWS

extern LARGE_INTEGER g_largeintegerFrequency;

#endif


CLASS_DECL_ACME ::i64 _get_nanos()
{

   LARGE_INTEGER largeinteger = {};

   QueryPerformanceCounter(&largeinteger);

   return muldiv64(largeinteger.QuadPart, 1'000'000'000, ::g_largeintegerFrequency.QuadPart);

}


string FormatMessageFromSystem(u32 dwError)
{


   return ::get_last_error_message(dwError);


}




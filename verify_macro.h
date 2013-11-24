//---------------------------------------------------------------------
// Copyleft olibre@Lmap.org
// This C header file is licenced under two licences, 
// choose the one you prefer:
// - MPL v2 or later
// - GPL v3 or later 
//---------------------------------------------------------------------

#ifndef __VERIFY_MACRO_H__
#define __VERIFY_MACRO_H__

//---------------------------------------------------------------------
// Motivation: debuger break-point may be more useful than crash-dump
//---------------------------------------------------------------------
// Function assert(condition) and macro ASSERT(condition) 
// are enabled when NDEBUG is not defined. When enabled,
// they crashes-dump the software when the condition is false.
//
// Macro VERIFY(condition) is similar but differs on:
// - never crashes-dump the program
// - insead print a warning on standard error
// - options to prevent verbose print (once per minute...) 
// - easy debuger break-point

//---------------------------------------------------------------------
// Usage based on break-point
//---------------------------------------------------------------------
// - remplace assert(condition) by VERIFY_ALWAYS(condition)
// - on your debuger, put a break-point on function breakpoint() call

//---------------------------------------------------------------------
// Usage based on standar error
//---------------------------------------------------------------------
// - remplace assert(condition) by VERIFY(condition)
// - add "#define VERIFY VERIFY_xxxxx" where VERIFY_xxxxx can be:
//   VERIFY_ALWAYS    if false, always print "file:line: condition in function f()"
//   VERIFY_ONCE      if false, print once (stop print after the first print) 
//   VERIFY_TENTIMES  if false, print ten times, after do not print any more
//   VERIFY_1pSECOND  if false, print once per second
//   VERIFY_1pMINUTE  if false, print once per minute
// - you can also use macro WARN(message) or variants WARN_xxxxx

//---------------------------------------------------------------------
// Use "if(likely(condition))" or "if(unlikely(condition))"
// depending on the condition probability 
//---------------------------------------------------------------------
#if (__GNUC__ >= 4) || (__GNUC__ == 3 && __GNUC_MINOR__ >= 2)
#  define   likely(cond) __builtin_expect((cond), 1)
#  define unlikely(cond) __builtin_expect((cond), 0)
#else
#  define   likely(cond) (cond)
#  define unlikely(cond) (cond)
#endif

//---------------------------------------------------------------------
// now() = seconds since epoch
// used by some macro VERIFY() below
#ifdef WIN32
#  include <sys/timeb.h>
   inline time_t now()
   {
     struct _timeb t;
     _ftime_s( &t );
     return t.time;
   }
#else
#  include <sys/time.h> //gettimeofday
   inline time_t now()
   {
     struct timeval tv; 
     gettimeofday (&tv, 0);
     return tv.tv_sec;
   }
#endif


//---------------------------------------------------------------------
#ifdef NDEBUG
#    define breakpoint()
#    define WARN(msg)
#    define WARN_ONCE(msg)    
#    define WARN_TENTIMES(msg)
#    define WARN_1pSECOND(msg) 
#    define WARN_1pMINUTE(msg)
#    define FILE_LINE(expr)
#else
     inline void breakpoint() {;}
//   ------------------------------------------------------------------
#    if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600))
#      define __func__   __PRETTY_FUNCTION__
#    elif defined(__DMC__) && (__DMC__ >= 0x810)
#      define __func__   __PRETTY_FUNCTION__
#    elif defined(__FUNCSIG__)
#      define __func__   __FUNCSIG__
#    elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
#      define __func__   __FUNCTION__
#    elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
#      define __func__   __FUNC__
#    elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
#      define __func__   __func__
#    elif ! defined(__func__)
#      define __func__   "(unknown)"
#    endif
//   ------------------------------------------------------------------
//   WARN          => always print
//   WARN_ONCE     => print ponce (stop print after the first print) 
//   WARN_TENTIMES => print ten times, after do not print any more
//   WARN_1pSECOND => print once per second
//   WARN_1pMINUTE => print once per minute
#    include <iostream> //cerr
#    define WARN(msg)          ({ std::cerr << msg << std::endl; breakpoint(); })
#    define WARN_ONCE(msg)     ({ static bool   first=1;             if(first) { first=0; WARN(msg); } })
#    define WARN_TENTIMES(msg) ({ static int    count=10;            if(count) { count--; WARN(msg); } })
#    define WARN_1pSECOND(msg) ({ static time_t t=0; time_t c=now(); if(c>t)   { t=c;     WARN(msg); } })
#    define WARN_1pMINUTE(msg) ({ static time_t t=0; time_t c=now(); if(c>t)   { t=c+59;  WARN(msg); } })
#    define FILE_LINE(expr)   __FILE__ ":"<< __LINE__ <<": VERIFY(" #expr ") in function "<< __func__
#endif


#define VERIFY_ALWAYS(expr)   ({ if(unlikely(!(expr))) WARN         (FILE_LINE(expr)); })
#define VERIFY_ONCE(expr)     ({ if(unlikely(!(expr))) WARN_ONCE    (FILE_LINE(expr)); })
#define VERIFY_TENTIMES(expr) ({ if(unlikely(!(expr))) WARN_TENTIMES(FILE_LINE(expr)); })
#define VERIFY_1pSECOND(expr) ({ if(unlikely(!(expr))) WARN_1pSECOND(FILE_LINE(expr)); })
#define VERIFY_1pMINUTE(expr) ({ if(unlikely(!(expr))) WARN_1pMINUTE(FILE_LINE(expr)); })

#undef  VERIFY
#define VERIFY VERIFY_TENTIMES

#endif

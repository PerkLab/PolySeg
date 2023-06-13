/*
 * Here is where system computed values get stored.
 * These values should only change when the target compile platform changes.
 */

/* #undef BUILD_SHARED_LIBS */
#ifndef BUILD_SHARED_LIBS
#define PolySeg_STATIC
#endif

#if defined(WIN32) && !defined(PolySeg_STATIC)
#pragma warning ( disable : 4275 )

#if defined(PolySeg_EXPORT)
#define PolySeg_EXPORT __declspec( dllexport )
#else
#define PolySeg_EXPORT __declspec( dllimport )
#endif
#else
#define PolySeg_EXPORT
#endif

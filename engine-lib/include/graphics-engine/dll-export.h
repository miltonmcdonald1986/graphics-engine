#ifndef ENGINE_LIB_DLL_EXPORT_H_
#define ENGINE_LIB_DLL_EXPORT_H_

#ifdef _WIN32
#ifdef ENGINE_LIB_EXPORTS
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT __declspec(dllimport)
#endif
#else
#define DLLEXPORT
#endif

#endif  // ENGINE_LIB_DLL_EXPORT_H_

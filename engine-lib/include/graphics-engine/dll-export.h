// Copyright (c) 2025 Milton McDonald
// This source code is licensed under the MIT License. See LICENSE file in the
// project root for details.

#ifndef ENGINE_LIB_DLL_EXPORT_H_
#define ENGINE_LIB_DLL_EXPORT_H_

#ifdef _WIN32
#ifdef BUILD_SHARED_LIBS
#ifdef ENGINE_LIB_EXPORTS
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT __declspec(dllimport)
#endif
#else
#define DLLEXPORT
#endif
#else
#define DLLEXPORT
#endif

#endif  // ENGINE_LIB_DLL_EXPORT_H_

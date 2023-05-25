#ifndef LOGICANALYZER_CONFIG_HPP
#define LOGICANALYZER_CONFIG_HPP

#if defined _WIN32 || defined __CYGWIN__
#define DLL_IMPORT __declspec(dllimport)
#define DLL_EXPORT __declspec(dllexport)
#define DLL_LOCAL
#else
#if __GNUC__ >= 4 || defined(__clang__)
#define DLL_IMPORT __attribute__((visibility("default")))
#define DLL_EXPORT __attribute__((visibility("default")))
#define DLL_LOCAL __attribute__((visibility("hidden")))
#else
#define DLL_IMPORT
#define DLL_EXPORT
#define DLL_LOCAL
#endif
#endif

#if defined(__cplusplus) && __cplusplus >= 201703L
#define MAYBE_UNUSED [[maybe_unused]]
#elif defined(__GNUC__) || defined(__clang__)
#define MAYBE_UNUSED __attribute__((unused))
#else
#define MAYBE_UNUSED
#endif

#endif // LOGICANALYZER_CONFIG_HPP

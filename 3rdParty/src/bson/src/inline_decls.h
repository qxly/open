#ifndef BSON_INLINE_DECLS_H
#define BSON_INLINE_DECLS_H

#if defined(__GNUC__)

#define NOINLINE_DECL __attribute__((noinline))

#elif defined(_MSC_VER)

#define NOINLINE_DECL __declspec(noinline)

#else

#define NOINLINE_DECL

#endif

#endif
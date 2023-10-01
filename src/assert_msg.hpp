//
// Created by james on 9/28/23.
//


#pragma once

#include <cstdio>
#include <cstdarg>
#include <cassert>
#include <cstdlib>



#ifdef NDEBUG
#define assert_msg(...)            (void)0;
#define nopath_msg(...)            (void)0;
#else
#define assert_msg(x_, ...)        do { __jassert_expression(x_, __LINE__, #x_, __func__, __FILE__, __VA_ARGS__); }while(0)
#define nopath_msg(...)            do { __jassert_impossible(__LINE__, __func__, __FILE__, __VA_ARGS__); }while(0)
#endif

#define nopath_impl       nopath_msg("UNIMPLEMENTED")
#define nopath_break      nopath_msg("BREAKPOINT")
#define nopath_null       assert_msg((x_) != nullptr, "NULL POINTER")

#define nopath_case(x_) do {                   \
    static_assert(std::is_enum_v< x_ >);        \
    nopath_msg("BAD ENUM CASE: %s", #x_);          \
}while(0)


[[noreturn]] static inline void __jassert(int line, char const* msg, char const* expr, char const* func, char const* file)
{
    bool m=msg != nullptr, e=expr!= nullptr;

    if (m && e)
    {
        fprintf(stderr, "\nASSERTION : %s"
                        "\nEXPR      : %s"
                        "\nFILE      : %s:%d"
                        "\nFUNC      : %s",
                msg, expr, file, line, func);

    }
    else if (!m && !e)
    {
        fprintf(stderr, "\nASSERTION"
                        "\nFILE      : %s:%d"
                        "\nFUNC      : %s",
                file, line, func);
    }
    else
    {
        fprintf(stderr, "\nASSERTION : %s"
                        "\nFILE      : %s:%d"
                        "\nFUNC      : %s",
                m ? msg : expr, file, line, func);
    }

    fflush(stderr);
    abort();
}

static inline void __jassert_expression(bool b, int line, char const* expr, char const* func, char const* file, char const* fmt, ...)
{
    if (!b)
    {
        size_t constexpr MAX_LEN = 512;
        static char buffer[MAX_LEN];

        va_list args;
        va_start(args, fmt);
        bool has_msg = vsnprintf(buffer, MAX_LEN, fmt, args) > 0;
        va_end(args);

        __jassert(line, has_msg ? buffer : nullptr, expr, func, file);
    }
}

static inline void __jassert_impossible(int line, char const* func, char const* file, char const* fmt, ...)
{
    size_t constexpr MAX_LEN = 512;
    static char buffer[MAX_LEN];

    va_list args;
    va_start(args, fmt);
    bool has_msg = vsnprintf(buffer, MAX_LEN, fmt, args) > 0;
    va_end(args);

    __jassert(line, has_msg ? buffer : nullptr, nullptr, func, file);
}
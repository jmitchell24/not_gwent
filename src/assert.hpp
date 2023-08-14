// Copyright (c) 2023. James Mitchell


#pragma once

#include <cassert>
#include <type_traits>

#ifndef NDEBUG
#define dbg__
#endif

#define assert_impl()       assert(!"unimplemented")
#define assert_reach()      assert(!"unreachable")
#define assert_null(x_)     assert((x_) != nullptr)

#define assert_range_open(x_, min_, max_)         assert(!"range check failed -> " && (x_) >= (min_) && (x_) <= (max_))
#define assert_range_closed(x_, min_, max_)       assert(!"range check failed -> " && (x_)  > (min_) && (x_) <  (max_))

#define assert_range_open_left(x_, min_, max_)    assert(!"range check failed -> " && (x_) >= (min_) && (x_) < (max_))
#define assert_range_closed_right(x_, min_, max_) assert_range_open_left(x_, min_, max_)

#define assert_range_open_right(x_, min_, max_)   assert(!"range check failed -> " && (x_) > (min_) && (x_) <=  (max_))
#define assert_range_closed_left(x_, min_, max_)  assert_range_open_right(x_, min_, max_)

#define assert_case(x_) do {                                            \
    static_assert(std::is_enum_v< x_ >, "'x_' should be an enum type"); \
    assert(!"invalid " #x_ " value");                                   \
}while(0)


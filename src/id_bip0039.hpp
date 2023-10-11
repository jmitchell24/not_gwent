//
// Created by james on 10/10/23.
//

#include <ut/string.hpp>
#include <array>


namespace bip0039
{
    using wordlist_t = std::array<ut::cstrview, 2048>;

    wordlist_t& wordlist();
    wordlist_t& wordlistStaticShuffle();
    wordlist_t& wordlistRuntimeShuffle();
}
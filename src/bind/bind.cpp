#include "bind.hpp"
using namespace bind;

using namespace ut;

#define SCALAR(n_, t_) \
    void Binder::rw (lbl_param lbl, opt_##n_ const& opt, t_             & x) { onRW1_##n_(lbl, opt, &x);     } \
    void Binder::rw (lbl_param lbl, opt_##n_ const& opt, ut::vec2x  <t_>& x) { onRW2_##n_(lbl, opt, x.pack); } \
    void Binder::rw (lbl_param lbl, opt_##n_ const& opt, ut::vec3x  <t_>& x) { onRW3_##n_(lbl, opt, x.pack); } \
    void Binder::rw (lbl_param lbl, opt_##n_ const& opt, ut::vec4x  <t_>& x) { onRW4_##n_(lbl, opt, x.pack); } \
    void Binder::rw (lbl_param lbl, opt_##n_ const& opt, ut::rectx  <t_>& x) { onRW4_##n_(lbl, opt, x.pack); }
        ENUM_BIND_SCALARS
#undef SCALAR

    void Binder::ro(lbl_param lbl, lbl_param x) { onRO1_str(lbl, x); }
    void Binder::ro(lbl_param lbl, bool x) { onRO1_bool(lbl, x); }


#define SCALAR(n_, t_) \
    void Binder::ro (lbl_param lbl, t_              const& x) { onRO1_##n_(lbl, &x);     } \
    void Binder::ro (lbl_param lbl, ut::vec2x  <t_> const& x) { onRO2_##n_(lbl, x.pack); } \
    void Binder::ro (lbl_param lbl, ut::vec3x  <t_> const& x) { onRO3_##n_(lbl, x.pack); } \
    void Binder::ro (lbl_param lbl, ut::vec4x  <t_> const& x) { onRO4_##n_(lbl, x.pack); } \
    void Binder::ro (lbl_param lbl, ut::rectx  <t_> const& x) { onRO4_##n_(lbl, x.pack); }
        ENUM_BIND_SCALARS
#undef SCALAR



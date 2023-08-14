#pragma once

#include "bind.hpp"

#include <unordered_set>
#include <ut/algo.hpp>

namespace bind
{
    class NodeHashList
    {
    public:
        struct Node { ut::cstrview s; ut::digest h; };

        using node_type     = ut::cstrview;
        using hash_type     = ut::digest_t;
        using hashes_type   = std::unordered_map<ut::digest_t, int>;
        using path_type     = std::vector<Node>;
        using string_type   = std::string;

        NodeHashList();

        void push(node_type leaf);
        void pop();

        void clear();
        void insert(node_type leaf);
        void remove(node_type leaf);
        void toggle(node_type leaf);
        bool contains(node_type leaf) const;
        bool empty() const;

        string_type pathString(node_type leaf) const;

    private:
        hashes_type         m_hashes;
        path_type           m_path;
    };

    class DebugWatchManager : public bind::Binder
    {
    public:
        template <typename T> void drawDebug(T& t, lbl_param lbl = T::BIND_LABEL_DEFAULT)
        {
            m_mode = MODE_ALL;

            if (begin())
            {
                t.bind(*this, lbl);
                end();
            }

            m_mode = MODE_FILTERED;

            if (begin())
            {
                t.bind(*this, lbl);
                end();
            }
        }

        static DebugWatchManager& instance()
        {
            static DebugWatchManager x;
            return x;
        }

    private:
        DebugWatchManager() {}

        bool begin();
        void end();

        bool push(lbl_param lbl, id_param id) override;
        void pop() override;

        void onRW1_color    (lbl_param lbl, ut::color* x) override;
        void onRW1_bool     (lbl_param lbl, bool* x) override;

#define SCALAR(n_, t_) \
        void onRW1_##n_ (lbl_param lbl, bind::opt_##n_ const& opt, t_* x) override; \
        void onRW2_##n_ (lbl_param lbl, bind::opt_##n_ const& opt, t_* x) override; \
        void onRW3_##n_ (lbl_param lbl, bind::opt_##n_ const& opt, t_* x) override; \
        void onRW4_##n_ (lbl_param lbl, bind::opt_##n_ const& opt, t_* x) override;
        ENUM_BIND_SCALARS
#undef SCALAR

        void onRO1_color    (lbl_param lbl, ut::color x) override;
        void onRO1_str      (lbl_param lbl, lbl_param x) override;
        void onRO1_bool     (lbl_param lbl, bool x) override;
        void onRO1_nullptr  (lbl_param lbl) override;

#define SCALAR(n_, t_) \
        void onRO1_##n_ (lbl_param lbl, t_ const* x) override; \
        void onRO2_##n_ (lbl_param lbl, t_ const* x) override; \
        void onRO3_##n_ (lbl_param lbl, t_ const* x) override; \
        void onRO4_##n_ (lbl_param lbl, t_ const* x) override;
        ENUM_BIND_SCALARS
#undef SCALAR

    private:
        enum Mode { MODE_ALL, MODE_FILTERED, MODE_COUNT };

        NodeHashList    m_node_hash_list;

        Mode            m_mode          = MODE_ALL;
        bool            m_begin_flag    = false;

        void rowRO(ut::color const& c, lbl_param lbl, lbl_param value);
    };

    [[maybe_unused]] static DebugWatchManager& DEBUGWATCH = DebugWatchManager::instance();

}

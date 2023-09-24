#include "imgui_bind.hpp"
using namespace bind;

#include "rlImGui/extras/IconsFontAwesome5.h"
#include "rlImGui/imgui/imgui.h"

#include <cassert>
using namespace std;

using namespace ut;

//
// NodeHash
//

#define LEAF_HASHER(l_) m_path.back().h.withRange(l_)

NodeHashList::NodeHashList()
    : m_hashes{}, m_path{1}
{ }

void NodeHashList::push(node_type leaf)
{
    assert(!leaf.empty());
    m_path.push_back({leaf, m_path.back().h.withRange(leaf)});
}

void NodeHashList::pop()
{
    assert(m_path.size() > 1);
    m_path.pop_back();
}

void NodeHashList::clear()
{
    m_hashes.clear();
}

void NodeHashList::insert(node_type leaf)
{
    for (auto&& x: m_path)
        ++m_hashes[x.h.value()];
    ++m_hashes[LEAF_HASHER(leaf).value()];
}

void NodeHashList::remove(node_type leaf)
{
    for (auto&& x: m_path)
    {
        auto d = x.h.value();
        --m_hashes[d];
        assert(m_hashes[d] >= 0);
    }

    auto d = LEAF_HASHER(leaf).value();
    --m_hashes[d];
    assert(m_hashes[d] >= 0);
}

void NodeHashList::toggle(node_type leaf)
{
    if (contains(leaf))
        remove(leaf);
    else
        insert(leaf);
}

bool NodeHashList::contains(node_type leaf) const
{
    auto d = LEAF_HASHER(leaf).value();
    if (auto it = m_hashes.find(d); it != m_hashes.end())
        return it->second > 0;
    return false;
}

bool NodeHashList::empty() const
{
    for (auto&& x: m_hashes)
        if (x.second > 0)
            return false;
    return true;
}

NodeHashList::string_type NodeHashList::pathString(node_type leaf) const
{
    constexpr char DELIM = '.';

    string_type s;
    for (auto&& x: m_path)
    {
        s.append(x.s.str());
        s.push_back(DELIM);
    }
    s.append(leaf.str());
    return s;
}

#undef LEAF_HASHER

//
// DebugWatchManager
//

#define DT_PICKER(t_, e_) template <> struct dt_picker<t_> { constexpr static ImGuiDataType value = e_; };
template <typename T> struct dt_picker;
    DT_PICKER(std::int8_t  , ImGuiDataType_S8       )
    DT_PICKER(std::int16_t , ImGuiDataType_S16      )
    DT_PICKER(std::int32_t , ImGuiDataType_S32      )
    DT_PICKER(std::int64_t , ImGuiDataType_S64      )
    DT_PICKER(std::uint8_t , ImGuiDataType_U8       )
    DT_PICKER(std::uint16_t, ImGuiDataType_U16      )
    DT_PICKER(std::uint32_t, ImGuiDataType_U32      )
    DT_PICKER(std::uint64_t, ImGuiDataType_U64      )
    DT_PICKER(float        , ImGuiDataType_Float    )
    DT_PICKER(double       , ImGuiDataType_Double   )
#undef DT_PICKER

#define SCALAR(n_, t_) \
    void DebugWatchManager::onRW1_##n_ (lbl_param lbl, bind::opt_##n_ const& opt, t_* x) { assert(m_begin_flag); ImGui::DragScalarN(lbl.c_str(), dt_picker<t_>::value, x, 1, opt.speed, &opt.min, &opt.max, "%d"); } \
    void DebugWatchManager::onRW2_##n_ (lbl_param lbl, bind::opt_##n_ const& opt, t_* x) { assert(m_begin_flag); ImGui::DragScalarN(lbl.c_str(), dt_picker<t_>::value, x, 2, opt.speed, &opt.min, &opt.max, "%d"); } \
    void DebugWatchManager::onRW3_##n_ (lbl_param lbl, bind::opt_##n_ const& opt, t_* x) { assert(m_begin_flag); ImGui::DragScalarN(lbl.c_str(), dt_picker<t_>::value, x, 3, opt.speed, &opt.min, &opt.max, "%d"); } \
    void DebugWatchManager::onRW4_##n_ (lbl_param lbl, bind::opt_##n_ const& opt, t_* x) { assert(m_begin_flag); ImGui::DragScalarN(lbl.c_str(), dt_picker<t_>::value, x, 4, opt.speed, &opt.min, &opt.max, "%d"); }
ENUM_BIND_SCALARS
#undef SCALAR

#define FMT_PICKER(t_, e_) template<> struct fmt_picker<t_> { constexpr static char const* fmt = e_; };
    template <typename T> struct fmt_picker;
    FMT_PICKER(std::int8_t  ,  "%f")
    FMT_PICKER(std::int16_t ,  "%f")
    FMT_PICKER(std::int32_t ,  "%d")
    FMT_PICKER(std::int64_t ,  "%d")
    FMT_PICKER(std::uint8_t ,  "%d")
    FMT_PICKER(std::uint16_t,  "%d")
    FMT_PICKER(std::uint32_t,  "%d")
    FMT_PICKER(std::uint64_t,  "%d")
    FMT_PICKER(float        ,  "%.1f")
    FMT_PICKER(double       ,  "%.1f")
#undef FMT_PICKER

template <typename X> auto value1(X const* x)
{
    return PRINTER(fmt_picker<X>::fmt, x[0]);
}

template <typename X> auto value2(X const* x)
{
    return PRINTER("%s %s",
        PRINTER(fmt_picker<X>::fmt, x[0]).c_str(),
        PRINTER(fmt_picker<X>::fmt, x[1]).c_str());
}

template <typename X> auto value3(X const* x)
{
    return PRINTER("%s %s %s",
        PRINTER(fmt_picker<X>::fmt, x[0]).c_str(),
        PRINTER(fmt_picker<X>::fmt, x[1]).c_str(),
        PRINTER(fmt_picker<X>::fmt, x[2]).c_str());
}

template <typename X> auto value4(X const* x)
{
    return PRINTER("%s %s %s %s",
        PRINTER(fmt_picker<X>::fmt, x[0]).c_str(),
        PRINTER(fmt_picker<X>::fmt, x[1]).c_str(),
        PRINTER(fmt_picker<X>::fmt, x[2]).c_str(),
        PRINTER(fmt_picker<X>::fmt, x[3]).c_str());
}

constexpr static auto COLOR_NUMERIC_LITERAL = colors::hotpink;//color{0xd69aa7ff};
constexpr static auto COLOR_STRING_LITERAL  = colors::sandybrown;
constexpr static auto COLOR_NULL            = colors::gray;

void DebugWatchManager::onRW1_color(lbl_param lbl, ut::color* x)
{
    assert(!"not implemented");
}

void DebugWatchManager::onRW1_bool(lbl_param lbl, bool* x)
{
    assert(!"not implemented");
}

#define SCALAR(n_, t_) \
    void DebugWatchManager::onRO1_##n_ (lbl_param lbl, t_ const* x) { assert(m_begin_flag); rowRO(COLOR_NUMERIC_LITERAL, lbl, value1(x)); } \
    void DebugWatchManager::onRO2_##n_ (lbl_param lbl, t_ const* x) { assert(m_begin_flag); rowRO(COLOR_NUMERIC_LITERAL, lbl, value2(x)); } \
    void DebugWatchManager::onRO3_##n_ (lbl_param lbl, t_ const* x) { assert(m_begin_flag); rowRO(COLOR_NUMERIC_LITERAL, lbl, value3(x)); } \
    void DebugWatchManager::onRO4_##n_ (lbl_param lbl, t_ const* x) { assert(m_begin_flag); rowRO(COLOR_NUMERIC_LITERAL, lbl, value4(x)); }
ENUM_BIND_SCALARS
#undef SCALAR

void DebugWatchManager::rowRO(color const& c, lbl_param lbl, lbl_param value)
{
    if (m_mode == MODE_FILTERED && !m_node_hash_list.contains(lbl))
        return;

    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);

    ImGui::PushID(lbl.c_str());

    switch (m_mode)
    {
        case MODE_ALL:
        {
            bool selected = m_mode == MODE_ALL && m_node_hash_list.contains(lbl);
            ImGuiSelectableFlags flags = ImGuiSelectableFlags_None;
            if (ImGui::Selectable(lbl.c_str(), selected, flags))
                m_node_hash_list.toggle(lbl);
            break;
        }

        case MODE_FILTERED:
        {
            if (ImGui::SmallButton(ICON_FA_TRASH))
                m_node_hash_list.remove(lbl);
            ImGui::SameLine();
            ImGui::TextUnformatted(lbl.begin(), lbl.end());
            break;
        }
        default: assert(!"invalid 'm_mode' value");
    }

    ImGui::PopID();

    if (ImGui::IsItemHovered())
    {
        auto full_path = m_node_hash_list.pathString(lbl);
        ImGui::SetTooltip("%.*s", (int)full_path.size(), full_path.c_str());
    }

    ImGui::TableSetColumnIndex(1);

    ImGui::PushStyleColor(ImGuiCol_Text, c);
    ImGui::TextUnformatted(value.begin(), value.end());
    ImGui::PopStyleColor();
}

void DebugWatchManager::onRO1_color(lbl_param lbl, ut::color x)
{
    assert(!"not implemented");
}

void DebugWatchManager::onRO1_str (lbl_param lbl, lbl_param x)
{
    rowRO(COLOR_STRING_LITERAL, lbl, PRINTER("\"%.*s\"", x.size(), x.c_str()));
}

void DebugWatchManager::onRO1_bool(lbl_param lbl, bool x)
{
    rowRO(COLOR_NUMERIC_LITERAL, lbl, x ? "true" : "false");
}

void DebugWatchManager::onRO1_nullptr(lbl_param lbl)
{
    if (m_mode == MODE_FILTERED)
        return;

    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);

    ImGui::TextUnformatted(lbl.begin(), lbl.end());

    if (ImGui::IsItemHovered())
    {
        auto full_path = m_node_hash_list.pathString(lbl);
        ImGui::SetTooltip("%.*s", (int)full_path.size(), full_path.c_str());
    }

    ImGui::TableSetColumnIndex(1);
    ImGui::TextDisabled("null");
}

bool DebugWatchManager::begin()
{
    assert(!m_begin_flag);

    float       text_base_width     = ImGui::CalcTextSize("A").x;
    float       text_base_height    = ImGui::GetTextLineHeightWithSpacing();
    char const* str_id              = "IMGUIBINDER_TABLE";

    switch (m_mode)
    {
        case MODE_ALL:
        {
            ImGui::Text("All");
            ImGuiTableFlags flags =
                ImGuiTableFlags_BordersV |
                ImGuiTableFlags_BordersOuterH |
                ImGuiTableFlags_Resizable |
                ImGuiTableFlags_RowBg;

            if (ImGui::BeginTable(str_id, 2, flags))
            {
                ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_NoHide);
                ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthFixed, text_base_width * 12.0f);
                ImGui::TableHeadersRow();
                m_begin_flag = true;
                return true;
            }

            break;
        }

        case MODE_FILTERED:
        {
            ImGui::Text("Selected");
            ImGui::SameLine();
            if (ImGui::SmallButton("Clear"))
                m_node_hash_list.clear();

            if (m_node_hash_list.empty())
            {
                m_begin_flag = true;
                return true;
            }

            ImGuiTableFlags flags =
                ImGuiTableFlags_BordersV |
                ImGuiTableFlags_BordersOuterH |
                ImGuiTableFlags_Resizable |
                ImGuiTableFlags_RowBg;
            if (ImGui::BeginTable(str_id, 2, flags))
            {
                m_begin_flag = true;
                return true;
            }

            break;
        }
        default: assert(!"invalid 'm_mode' value");
    }

    return false;
}

void DebugWatchManager::end()
{
    assert(m_begin_flag);

    m_begin_flag = false;

    if (m_mode == MODE_FILTERED && m_node_hash_list.empty())
        return;
    ImGui::EndTable();
}

bool DebugWatchManager::push(lbl_param lbl, id_param id)
{
    assert(m_begin_flag);

    if (m_mode == MODE_FILTERED && !m_node_hash_list.contains(lbl))
        return false;

    ImGui::TableNextRow();
    ImGui::TableNextColumn();

    if (m_mode == MODE_FILTERED)
    {
        m_node_hash_list.push(lbl);
        return true;
    }

    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanFullWidth;
    if (ImGui::TreeNodeEx(PRINTER("%s (%x)", lbl.c_str(), id).c_str(), flags))
    {
        m_node_hash_list.push(lbl);
        return true;
    }

    return false;
}

void DebugWatchManager::pop()
{
    assert(m_begin_flag);

    if (m_mode == MODE_ALL)
        ImGui::TreePop();
    m_node_hash_list.pop();
}



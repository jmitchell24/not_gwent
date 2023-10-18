//
// Created by james on 10/3/23.
//

#pragma once

#include "scene.hpp"
#include "ledit/ledit_box.hpp"
#include "gfx/gfx_virt2d.hpp"

#include <string>
#include <vector>
#include <variant>
#include <optional>





class SceneLayoutEditor : public Scene
{
public:
    char const* name() const override { return "Layout Editor"; }

    ut::rect bounds;

    void layout(ut::rect const& b) override;
    void update(float dt) override;
    void draw() override;
    void drawDebug() override;

private:
    using filelist_t = std::vector<std::string>;

    std::string m_active_filename;
    filelist_t  m_filenames;

    void refreshFiles();
};
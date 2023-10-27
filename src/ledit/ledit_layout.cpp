//
// Created by james on 10/16/23.
//
#include "ledit/ledit_layout.hpp"
#include "ledit/ledit_yaml.hpp"
using namespace ledit;

//
// ut
//
#include <ut/check.hpp>
#include <ut/algo.hpp>
using namespace ut;

bool Layout::loadYaml(cstrparam filename)
{
    return false;
//    if (filename.empty())
//        return false;
//
//    if (FILE* file = fopen(filename, "r"))
//    {
//        try
//        {
//            auto text = gulp::file_to_string(file);
//
//            box_ptr b = Box::createRoot();
//            fromYaml(YAML::Load(text), b);
//            root_box = b;
//            selected_box = root_box;
//            return true;
//        }
//        catch (YAML::Exception const& ex)
//        {
//            fprintf(stderr, "%s:%d:%d:\n    ", filename.c_str(), ex.mark.line, ex.mark.column);
//            fprintf(stderr, "YAML parse error: %s\n", ex.msg.c_str());
//        }
//
//        fclose(file);
//    }
//    else
//    {
//        fprintf(stderr, "error opening layout file (%s)", filename.c_str());
//    }
//    return false;
}

bool Layout::saveYaml(cstrparam filename)
{
    if (filename.empty())
        return false;

    if (FILE* file = fopen(filename, "w"))
    {
        YAML::Emitter emitter;

        emitYaml(emitter, root_box);

        fwrite(emitter.c_str(), sizeof(char), emitter.size(), file);
        fclose(file);
        return true;
    }
    else
    {
        fprintf(stderr, "error opening layout file (%s)", filename.c_str());
    }
    return false;
}
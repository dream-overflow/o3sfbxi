/**
 * @brief FBX import module object
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-01-14
 * @details
 */

#include "plugin.h"
#include "importer.h"
#include <o3d/core/base.h>

using namespace o3d::studio::fbxi;

extern "C"
{

O3S_PLUGIN_API o3d::studio::common::Module* o3dstudioPlugin(const o3d::String &name, o3d::DynamicLibrary *library)
{
    Plugin *module = new Plugin(name, library);
    return module;
}

} // extern "C"

Plugin::Plugin(const String &name, DynamicLibrary *library) :
    DynamicModule(name, library),
    m_importer(nullptr)
{
    m_properties["section"] = "o3s::plugin::importer";
    m_properties["verbose"] = "Autodesk FBX Importer";

    m_properties["vendor"] = "Dream Overflow";
    m_properties["group"] = "Official extensions";
    m_properties["url"] = "http://o3dstudio.dreamoverflow.org/fbxi";
    m_properties["plateforms"] = "all";

    m_properties["version"] = "1.0.0";
    m_properties["description"] = "An Autodesk FBX scene importer";
    m_properties["copyright"] = "(C) 2017 Dream Overflow";
    m_properties["license"] = "Proprietary";

    m_properties["dependencies"] = "";

    m_capacities.setBit(common::CAPACITY_IMPORTER, True);
}

Plugin::~Plugin()
{

}

o3d::Bool Plugin::start()
{
    m_started = True;
    return True;
}

o3d::Bool Plugin::stop()
{
    m_started = False;
    return False;
}

o3d::studio::common::Importer* Plugin::importer()
{
    if (!m_importer) {
        m_importer = new Importer();
    }

    return m_importer;
}

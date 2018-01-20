/**
 * @brief FBX importer adapter to O3S implementation
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-01-20
 * @details
 */

#include "o3sadapter.h"

#include <o3d/studio/common/objectref.h>

using namespace o3d::studio::fbxi;

O3SAdapter::O3SAdapter(
        o3d::studio::common::ImporterOption *options,
        o3d::studio::common::Entity *parent) :
    m_options(options),
    m_parent(parent)
{
    O3D_ASSERT(options);
    O3D_ASSERT(parent);
}

O3SAdapter::~O3SAdapter()
{

}

o3d::Bool O3SAdapter::processImport(std::list<FBXNode *> &rootNodes)
{
    common::Hub *topLevelHub = nullptr;

    if (m_parent->ref().light().baseTypeOf(common::TypeRef::asset())) {
        common::Asset *asset = static_cast<common::Asset*>(m_parent);
        setupAsset(asset);

        topLevelHub = new common::Hub("Asset root hub", m_parent);
    } else if (m_parent->ref().light().baseTypeOf(common::TypeRef::hub())) {
        topLevelHub = static_cast<common::Hub*>(m_parent);
    }

    if (topLevelHub) {
        setupHub(topLevelHub);
    } else {
        return False;
    }

    return True;
}

void O3SAdapter::setupAsset(common::Asset* asset)
{
    // @todo setup asset options
}

void O3SAdapter::setupHub(common::Hub* rootHub)
{
    // @todo all work here
}

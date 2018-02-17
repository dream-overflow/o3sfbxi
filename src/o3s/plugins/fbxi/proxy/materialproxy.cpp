/**
 * @brief FBX material data proxy
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-01-29
 * @details
 */

#include "materialproxy.h"
#include <o3d/core/debug.h>

#include "../property/propertystring.h"
#include "../property/propertyint32.h"
#include "../property/propertyfloat32.h"
#include "../property/propertyfloat64.h"

using namespace o3d::studio::fbxi;

MaterialProxy::MaterialProxy(FBXNode *node) :
    ObjectProxy(node),
    m_textures{0}
{
    if (!m_node || m_node->name() != "Material") {
        O3D_ERROR(E_InvalidParameter("Must be a Material node"));
    }

    FBXNode *version = m_node->child("Version");
    if (version) {
        m_version = version->directAsInt32();
    }

    if (m_version != 102) {
        O3D_ERROR(E_InvalidParameter("Must be a Model node version 102"));
    }

    m_objectType = OBJECT_MATERIAL;

    for (UInt32 i = 0; i < MAP_NORMAL+1; ++i) {
        m_textures[i] = nullptr;
    }
}

void MaterialProxy::setTexture(MaterialProxy::MapType map, TextureProxy *texture)
{
    m_textures[map] = texture;
}

TextureProxy *MaterialProxy::texture(MaterialProxy::MapType map)
{
    return m_textures[map];
}

o3d::Bool MaterialProxy::hasTexture(MaterialProxy::MapType map) const
{
    return m_textures[map] != nullptr;
}

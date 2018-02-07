/**
 * @brief FBX mesh data proxy
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-02-04
 * @details
 */

#include "meshproxy.h"
#include <o3d/core/debug.h>

#include "../property/propertystring.h"
#include "../property/propertyint32.h"
#include "../property/propertyfloat32.h"
#include "../property/propertyfloat64.h"

using namespace o3d::studio::fbxi;

MeshNodeProxy::MeshNodeProxy(FBXNode *node) :
    NodeAttributeProxy(node),
    m_geometry(nullptr)
{
    if (subClass() != "Mesh") {
        O3D_ERROR(E_InvalidParameter("Must be sub class mesh"));
    }

    // @todo check Version == 100
    m_objectType = OBJECT_MESH_NODE_ATTR;
}

void MeshNodeProxy::setGeometry(GeometryProxy *geometry)
{
    m_geometry = geometry;
}

GeometryProxy *MeshNodeProxy::geometry()
{
    return m_geometry;
}

MaterialProxy *MeshNodeProxy::material(o3d::UInt32 idx)
{
    if (idx > (UInt32)m_materials.size()) {
        O3D_ERROR(E_IndexOutOfRange("Material index"));
    }

    return m_materials[idx];
}

o3d::UInt32 MeshNodeProxy::numMaterials() const
{
    return (UInt32)m_materials.size();
}

MeshModelProxy::MeshModelProxy(FBXNode *node) :
    ModelProxy(node)
{
    if (subClass() != "Mesh") {
        O3D_ERROR(E_InvalidParameter("Must be sub class mesh"));
    }

    // @todo check Version == 100
    m_objectType = OBJECT_MESH_MODEL;
}

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
    NodeAttributeProxy(node)
{
    if (subClass() != "Mesh") {
        O3D_ERROR(E_InvalidParameter("Must be sub class mesh"));
    }

    // @todo check Version == 100
    m_objectType = OBJECT_MESH_NODE_ATTR;
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
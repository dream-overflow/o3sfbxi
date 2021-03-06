/**
 * @brief FBX light data proxy
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-01-29
 * @details
 */

#include "lightproxy.h"
#include <o3d/core/debug.h>

#include "../property/propertystring.h"
#include "../property/propertyint32.h"
#include "../property/propertyfloat32.h"
#include "../property/propertyfloat64.h"

using namespace o3d::studio::fbxi;

LightNodeProxy::LightNodeProxy(FBXNode *node) :
    NodeAttributeProxy(node)
{
    if (subClass() != "Light") {
        O3D_ERROR(E_InvalidParameter("Must be sub class Light"));
    }

    // @todo check Version == 100
    m_objectType = OBJECT_LIGHT_NODE_ATTR;
}

LightModelProxy::LightModelProxy(FBXNode *node) :
    ModelProxy(node)
{
    if (subClass() != "Light") {
        O3D_ERROR(E_InvalidParameter("Must be sub class Light"));
    }

    // @todo check Version == 100
    m_objectType = OBJECT_LIGHT_MODEL;
}

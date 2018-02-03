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

LightProxy::LightProxy(FBXNode *node) :
    ObjectProxy(node)
{
    if (!m_node || m_node->name() != "Light") {
        O3D_ERROR(E_InvalidParameter("Must be a Light node"));
    }

    // @todo check Version == 100
}

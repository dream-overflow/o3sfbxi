/**
 * @brief FBX bone data proxy
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-01-04
 * @details
 */

#include "boneproxy.h"
#include <o3d/core/debug.h>

#include "../property/propertystring.h"
#include "../property/propertyint32.h"
#include "../property/propertyfloat32.h"
#include "../property/propertyfloat64.h"

using namespace o3d::studio::fbxi;

BoneModelProxy::BoneModelProxy(FBXNode *node) :
    ModelProxy(node)
{
    if (subClass() != "LimbNode") {
        O3D_ERROR(E_InvalidParameter("Must be sub class LimbNode"));
    }

    // @todo check Version == 100
    m_objectType = OBJECT_LIMB_NODE_MODEL;
}

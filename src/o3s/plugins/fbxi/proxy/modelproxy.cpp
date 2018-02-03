/**
 * @brief FBX model data proxy
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-01-29
 * @details
 */

#include "modelproxy.h"
#include <o3d/core/debug.h>

#include "../property/propertystring.h"
#include "../property/propertyint32.h"
#include "../property/propertyfloat32.h"
#include "../property/propertyfloat64.h"

using namespace o3d::studio::fbxi;

ModelProxy::ModelProxy(FBXNode *node) :
    ObjectProxy(node)
{
    if (!m_node || m_node->name() != "Model") {
        O3D_ERROR(E_InvalidParameter("Must be a Model node"));
    }

    FBXNode *version = m_node->child("Version");
    if (version) {
        m_version = version->directAsInt32();
    }

    if (m_version != 232) {
        O3D_ERROR(E_InvalidParameter("Must be a Model node version 232"));
    }
}


o3d::Vector3 ModelProxy::position()
{
    FBXNode *p70 = m_node->child("Properties70");
    Vector3 pos;
//    if (p70) {
//        pos = p70->interpretAsVector3();
//        "Lcl Translation"
//    }

    return pos;
}

o3d::Vector3 ModelProxy::rotation()
{
    FBXNode *p70 = m_node->child("Properties70");
    Vector3 rot;
//    if (p70) {
//        pos = p70->interpretAsVector3();
//        "Lcl Rotation"
//    }

    return rot;
}

o3d::Vector3 ModelProxy::scale()
{
    FBXNode *p70 = m_node->child("Properties70");
    Vector3 scale;
//    if (p70) {
//        pos = p70->interpretAsVector3();
//        "Lcl Scaling"
//    }

    return scale;
}

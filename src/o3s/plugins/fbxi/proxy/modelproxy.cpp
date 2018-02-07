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

    m_objectType = OBJECT_MODEL;
}


o3d::Vector3 ModelProxy::position()
{
    Vector3 pos;
    FBXNode *p70 = m_node->child("Properties70");
    if (p70) {
        FBXNode *vNode = p70->child("Lcl Translation");
        if (vNode) {
            pos = vNode->interpretAsVector3();
        }
    }

    return pos;
}

o3d::Vector3 ModelProxy::rotation()
{
    Vector3 rot;
    FBXNode *p70 = m_node->child("Properties70");
    if (p70) {
        FBXNode *vNode = p70->child("Lcl Rotation");
        if (vNode) {
            rot = vNode->interpretAsVector3();
        }
    }

    return rot;
}

o3d::Vector3 ModelProxy::scale()
{
    Vector3 scale;  
    FBXNode *p70 = m_node->child("Properties70");
    if (p70) {
        FBXNode *vNode = p70->child("Lcl Scaling");
        if (vNode) {
            scale = vNode->interpretAsVector3();
        }
    }

    return scale;
}

RootModelProxy::RootModelProxy(FBXNode *node) :
    ModelProxy(node ? node : new FBXNode("Root"))
{
    m_objectType = OBJECT_ROOT_NODE;
}

NullModelProxy::NullModelProxy(FBXNode *node) :
    ModelProxy(node)
{
    m_objectType = OBJECT_NULL_NODE;
}

/**
 * @brief FBX objects data proxy
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-01-20
 * @details
 */

#include "objectsproxy.h"
#include <o3d/core/debug.h>

#include "modelproxy.h"
#include "materialproxy.h"
#include "cameraproxy.h"
#include "lightproxy.h"
#include "textureproxy.h"
#include "nodeattributeproxy.h"
#include "geometryproxy.h"

#include "../property/propertystring.h"
#include "../property/propertyint32.h"
#include "../property/propertyfloat64.h"

using namespace o3d::studio::fbxi;

ObjectsProxy::ObjectsProxy(FBXNode *node) :
    Proxy(node)
{
    if (!m_node || m_node->name() != "Objects") {
        O3D_ERROR(E_InvalidParameter("Must be an Objects node"));
    }
}

o3d::UInt32 ObjectsProxy::numObjects() const
{
    return m_node->numChildren();
}

ObjectsProxy::ObjectType ObjectsProxy::objectType(o3d::UInt32 i)
{
    if (i < m_node->numChildren()) {
        FBXNode *node = m_node->childAt(i);
        if (node->name() == "NodeAttribute") {
            return OBJECT_NODE_ATTRIBUTE;
        } else if (node->name() == "Camera") {
            return OBJECT_CAMERA;
        } else if (node->name() == "Light") {
            return OBJECT_LIGHT;
        } else if (node->name() == "Geometry") {
            return OBJECT_GEOMETRY;
        } else if (node->name() == "Material") {
            return OBJECT_MATERIAL;
        } else if (node->name() == "Model") {
            return OBJECT_MODEL;
        } else if (node->name() == "Texture") {
            return OBJECT_TEXTURE;
        }
    }

    return OBJECT_UNDEFINED;
}

ModelProxy* ObjectsProxy::model(o3d::UInt32 i)
{
    return nullptr;
}

MaterialProxy* ObjectsProxy::material(o3d::UInt32 i)
{
    return nullptr;
}

CameraProxy* ObjectsProxy::camera(o3d::UInt32 i)
{
    return nullptr;
}

LightProxy* ObjectsProxy::light(o3d::UInt32 i)
{
    return nullptr;
}

TextureProxy* ObjectsProxy::texture(o3d::UInt32 i)
{
    return nullptr;
}

NodeAttributeProxy* ObjectsProxy::nodeAttribute(o3d::UInt32 i)
{
    return nullptr;
}

GeometryProxy* ObjectsProxy::geometry(o3d::UInt32 i)
{
    return nullptr;
}

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
#include "boneproxy.h"
#include "meshproxy.h"

#include "../property/propertystring.h"
#include "../property/propertyint32.h"
#include "../property/propertyint64.h"
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

ObjectProxy::ObjectType ObjectsProxy::objectType(o3d::UInt32 i)
{
    if (i < m_node->numChildren()) {
        FBXNode *node = m_node->childAt(i);
        return nodeType(node);
    }

    return ObjectProxy::OBJECT_UNDEFINED;
}

ObjectProxy::ObjectType ObjectsProxy::objectTypeByUid(o3d::Int64 uid)
{
    for (auto it = m_node->begin(); it != m_node->end(); ++it) {
        FBXNode *node = *it;
        Int64 uid = -1;

        if (node->property(0) && node->property(0)->type() == Property::PROP_INT64) {
            if (static_cast<PropertyInt64*>(node->property(0))->value() == uid) {
                return nodeType(node);
            }
        }
    }

    return ObjectProxy::OBJECT_UNDEFINED;
}

//ObjectProxy *ObjectsProxy::objectByUid(o3d::Int64 uid)
//{
//     for (auto it = m_node->begin(); it != m_node->end(); ++it) {
//        ObjectProxy *objectProxy = static_cast<ObjectProxy*>(*it);
//        if (objectProxy->uid() == uid) {
//            return objectProxy;
//        }
//    }

//    return nullptr;
//}

ModelProxy* ObjectsProxy::model(o3d::UInt32 i)
{
    FBXNode *node = m_node->childAt(i);
    if (node) {
        return new ModelProxy(node);
    }
    return nullptr;
}

MaterialProxy* ObjectsProxy::material(o3d::UInt32 i)
{
    FBXNode *node = m_node->childAt(i);
    if (node) {
        return new MaterialProxy(node);
    }
    return nullptr;
}

CameraNodeProxy* ObjectsProxy::cameraNode(o3d::UInt32 i)
{
    FBXNode *node = m_node->childAt(i);
    if (node) {
        return new CameraNodeProxy(node);
    }
    return nullptr;
}

LightNodeProxy* ObjectsProxy::lightNode(o3d::UInt32 i)
{
    FBXNode *node = m_node->childAt(i);
    if (node) {
        return new LightNodeProxy(node);
    }
    return nullptr;
}

TextureProxy* ObjectsProxy::texture(o3d::UInt32 i)
{
    FBXNode *node = m_node->childAt(i);
    if (node) {
        return new TextureProxy(node);
    }
    return nullptr;
}

BoneModelProxy *ObjectsProxy::boneModel(o3d::UInt32 i)
{
    FBXNode *node = m_node->childAt(i);
    if (node) {
        return new BoneModelProxy(node);
    }
    return nullptr;
}

MeshModelProxy *ObjectsProxy::meshModel(o3d::UInt32 i)
{
    FBXNode *node = m_node->childAt(i);
    if (node) {
        return new MeshModelProxy(node);
    }
    return nullptr;
}

CameraModelProxy *ObjectsProxy::cameraModel(o3d::UInt32 i)
{
    FBXNode *node = m_node->childAt(i);
    if (node) {
        return new CameraModelProxy(node);
    }
    return nullptr;
}

LightModelProxy *ObjectsProxy::lightModel(o3d::UInt32 i)
{
    FBXNode *node = m_node->childAt(i);
    if (node) {
        return new LightModelProxy(node);
    }
    return nullptr;
}

RootModelProxy *ObjectsProxy::rootProxy(o3d::UInt32 i)
{
    FBXNode *node = m_node->childAt(i);
    if (node) {
        return new RootModelProxy(node);
    }
    return nullptr;
}

NullModelProxy *ObjectsProxy::nullProxy(o3d::UInt32 i)
{
    FBXNode *node = m_node->childAt(i);
    if (node) {
        return new NullModelProxy(node);
    }
    return nullptr;
}

NodeAttributeProxy* ObjectsProxy::nodeAttribute(o3d::UInt32 i)
{
    FBXNode *node = m_node->childAt(i);
    if (node) {
        return new NodeAttributeProxy(node);
    }
    return nullptr;
}

GeometryProxy* ObjectsProxy::geometry(o3d::UInt32 i)
{
    FBXNode *node = m_node->childAt(i);
    if (node) {
        return new GeometryProxy(node);
    }
    return nullptr;
}

ObjectProxy::ObjectType ObjectsProxy::nodeType(FBXNode *node)
{
    if (node) {
        if (node->name() == "NodeAttribute") {
            // sub classed
            String subClass;

            if (node->property(2) && node->property(2)->type() == Property::PROP_STRING) {
                subClass = static_cast<PropertyString*>(node->property(2))->value();
            }

            if (subClass == "Camera") {
                return ObjectProxy::OBJECT_CAMERA_NODE_ATTR;
            } else if (subClass == "Light") {
                return ObjectProxy::OBJECT_LIGHT_NODE_ATTR;
            } else if (subClass == "Mesh") {
                return ObjectProxy::OBJECT_MESH_NODE_ATTR;
            }

            // unspecific node
            return ObjectProxy::OBJECT_NODE_ATTRIBUTE;
        } else if (node->name() == "Geometry") {
            return ObjectProxy::OBJECT_GEOMETRY;
        } else if (node->name() == "Material") {
            return ObjectProxy::OBJECT_MATERIAL;
        } else if (node->name() == "Model") {
            // sub classed
            String subClass;

            if (node->property(2) && node->property(2)->type() == Property::PROP_STRING) {
                subClass = static_cast<PropertyString*>(node->property(2))->value();
            }

            if (subClass == "Camera") {
                return ObjectProxy::OBJECT_CAMERA_MODEL;
            } else if (subClass == "Light") {
                return ObjectProxy::OBJECT_LIGHT_MODEL;
            } else if (subClass == "Mesh") {
                return ObjectProxy::OBJECT_MESH_MODEL;
            } else if (subClass == "LimbNode") {
                return ObjectProxy::OBJECT_LIMB_NODE_MODEL;
            } else if (subClass == "Null") {
                return ObjectProxy::OBJECT_NULL_NODE;
            } else if (subClass == "Root") {
                return ObjectProxy::OBJECT_ROOT_NODE;
            }

            return ObjectProxy::OBJECT_MODEL;
        } else if (node->name() == "Texture") {
            return ObjectProxy::OBJECT_TEXTURE;
        } else if (node->name() == "AnimationStack") {
            return ObjectProxy::OBJECT_ANIMATION_STACK;
        } else if (node->name() == "AnimationLayer") {
            return ObjectProxy::OBJECT_ANIMATION_LAYER;
        } else if (node->name() == "AnimationCurve") {
            return ObjectProxy::OBJECT_ANIMATION_CURVE;
        } else if (node->name() == "AnimationCurveNode") {
            return ObjectProxy::OBJECT_ANIMATION_CURVE_NODE;
        } else if (node->name() == "Deformer") {
            // sub classed
            String subClass;

            if (node->property(2) && node->property(2)->type() == Property::PROP_STRING) {
                subClass = static_cast<PropertyString*>(node->property(2))->value();
            }

            if (subClass == "Cluster") {
                return ObjectProxy::OBJECT_CLUSTER;
            } else if (subClass == "Skin") {
                return ObjectProxy::OBJECT_SKIN;
            }

            return ObjectProxy::OBJECT_DEFORMER;
        }
    }

    return ObjectProxy::OBJECT_UNDEFINED;
}

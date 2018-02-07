/**
 * @brief FBX node base object data proxy
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-02-03
 * @details
 */

#ifndef _O3DS_FBXI_OBJECTPROXY_H
#define _O3DS_FBXI_OBJECTPROXY_H

#include "proxy.h"

namespace o3d {
namespace studio {
namespace fbxi {

class NodeAttributeProxy;

class ObjectProxy : public Proxy
{
public:

    enum ObjectType
    {
        OBJECT_UNDEFINED = 0,
        OBJECT_NODE_ATTRIBUTE,
        OBJECT_NULL_NODE,
        OBJECT_ROOT_NODE,
        OBJECT_CAMERA_NODE_ATTR,
        OBJECT_LIGHT_NODE_ATTR,
        OBJECT_MESH_NODE_ATTR,
        OBJECT_MODEL,
        OBJECT_LIMB_NODE_MODEL,
        OBJECT_CAMERA_MODEL,
        OBJECT_LIGHT_MODEL,
        OBJECT_MESH_MODEL,
        OBJECT_GEOMETRY,
        OBJECT_MATERIAL,
        OBJECT_TEXTURE,
        OBJECT_ANIMATION_STACK,
        OBJECT_ANIMATION_LAYER,
        OBJECT_ANIMATION_CURVE,
        OBJECT_ANIMATION_CURVE_NODE,
        OBJECT_DEFORMER,
        OBJECT_CLUSTER,
        OBJECT_SKIN
    };

    /**
     * @brief ObjectProxy
     * @param node Node specialized from object
     */
    ObjectProxy(FBXNode *node);

    String name();

    ObjectType objectType() const;

    Int64 uid();
    String className();
    String subClass();

    void setParent(ObjectProxy *parent);
    void addChild(ObjectProxy *proxy);

    void setNodeAttribute(NodeAttributeProxy *attr);
    NodeAttributeProxy* nodeAttribute();

    ObjectProxy* recursiveNext(std::list<size_t> &cursor);

protected:

    ObjectType m_objectType;

    ObjectProxy *m_parent;
    std::vector<ObjectProxy*> m_children;

    NodeAttributeProxy *m_nodeAttr;
};

} // namespace fbxi
} // namespace studio
} // namespace o3d

#endif // _O3DS_FBXI_OBJECTPROXY_H

/**
 * @brief FBX objects data proxy
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-01-20
 * @details
 */

#ifndef _O3DS_FBXI_OBJECTSPROXY_H
#define _O3DS_FBXI_OBJECTSPROXY_H

#include "objectproxy.h"
#include <o3d/core/vector3.h>

namespace o3d {
namespace studio {
namespace fbxi {

class ModelProxy;
class MaterialProxy;
class CameraNodeProxy;
class LightNodeProxy;
class TextureProxy;
class NodeAttributeProxy;
class GeometryProxy;
class BoneModelProxy;
class MeshModelProxy;
class CameraModelProxy;
class LightModelProxy;

class ObjectsProxy : public Proxy
{
public:

    /**
     * @brief ObjectsProxy
     * @param node Node named Objects
     */
    ObjectsProxy(FBXNode *node);

    UInt32 numObjects() const;

    ObjectProxy::ObjectType objectType(UInt32 i);
    ObjectProxy::ObjectType objectTypeByUid(Int64 uid);

//    ObjectProxy* objectByUid(Int64 uid);

    MaterialProxy* material(UInt32 i);
    TextureProxy* texture(UInt32 i);
    GeometryProxy* geometry(UInt32 i);

    NodeAttributeProxy* nodeAttribute(UInt32 i);

    CameraNodeProxy* cameraNode(UInt32 i);
    LightNodeProxy* lightNode(UInt32 i);

    ModelProxy* model(UInt32 i);
    BoneModelProxy* boneModel(UInt32 i);
    MeshModelProxy* meshModel(UInt32 i);
    CameraModelProxy* cameraModel(UInt32 i);
    LightModelProxy* lightModel(UInt32 i);

private:

    ObjectProxy::ObjectType nodeType(FBXNode *node);
};

} // namespace fbxi
} // namespace studio
} // namespace o3d

#endif // _O3DS_FBXI_OBJECTSPROXY_H

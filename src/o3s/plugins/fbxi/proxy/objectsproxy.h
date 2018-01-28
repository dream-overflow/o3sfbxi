/**
 * @brief FBX objects data proxy
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-01-20
 * @details
 */

#ifndef _O3DS_FBXI_OBJECTSPROXY_H
#define _O3DS_FBXI_OBJECTSPROXY_H

#include "proxy.h"
#include <o3d/core/vector3.h>

namespace o3d {
namespace studio {
namespace fbxi {

class ModelProxy;
class MaterialProxy;
class CameraProxy;
class LightProxy;
class TextureProxy;
class NodeAttributeProxy;
class GeometryProxy;

class ObjectsProxy : public Proxy
{
public:

    enum ObjectType
    {
        OBJECT_UNDEFINED = 0,
        OBJECT_NODE_ATTRIBUTE,
        OBJECT_CAMERA,
        OBJECT_LIGHT,
        OBJECT_GEOMETRY,
        OBJECT_MODEL,
        OBJECT_MATERIAL,
        OBJECT_TEXTURE
    };

    /**
     * @brief ObjectsProxy
     * @param node Node named Objects
     */
    ObjectsProxy(FBXNode *node);

    UInt32 numObjects() const;

    ObjectType objectType(UInt32 i);

    ModelProxy* model(UInt32 i);
    MaterialProxy* material(UInt32 i);
    CameraProxy* camera(UInt32 i);
    LightProxy* light(UInt32 i);
    TextureProxy* texture(UInt32 i);
    NodeAttributeProxy* nodeAttribute(UInt32 i);
    GeometryProxy* geometry(UInt32 i);
};

} // namespace fbxi
} // namespace studio
} // namespace o3d

#endif // _O3DS_FBXI_OBJECTSPROXY_H

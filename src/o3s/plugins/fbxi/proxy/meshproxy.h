/**
 * @brief FBX node mesh data proxy
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-02-04
 * @details
 */

#ifndef _O3DS_FBXI_MESHPROXY_H
#define _O3DS_FBXI_MESHPROXY_H

#include "nodeattributeproxy.h"
#include "modelproxy.h"

#include <vector>

namespace o3d {
namespace studio {
namespace fbxi {

class GeometryProxy;
class MaterialProxy;

class MeshNodeProxy : public NodeAttributeProxy
{
public:

    /**
     * @brief MeshNodeProxy
     * @param node Node subclassed Mesh
     */
    MeshNodeProxy(FBXNode *node);

    void setGeometry(GeometryProxy *geometry);
    GeometryProxy* geometry();

    void addMaterial(MaterialProxy *material);
    MaterialProxy* material(UInt32 idx);

    o3d::UInt32 numMaterials() const;

protected:

    GeometryProxy *m_geometry;
    std::vector<MaterialProxy*> m_materials;
};

class MeshModelProxy : public ModelProxy
{
public:

    /**
     * @brief MeshModelProxy
     * @param node Node subclassed Mesh
     */
    MeshModelProxy(FBXNode *node);
};

} // namespace fbxi
} // namespace studio
} // namespace o3d

#endif // _O3DS_FBXI_CAMERAPROXY_H

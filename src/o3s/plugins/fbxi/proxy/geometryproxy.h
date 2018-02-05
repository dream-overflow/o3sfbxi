/**
 * @brief FBX node geometry data proxy
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-01-29
 * @details
 */

#ifndef _O3DS_FBXI_GEOMETRYPROXY_H
#define _O3DS_FBXI_GEOMETRYPROXY_H

#include "objectproxy.h"
#include <o3d/core/templatearray.h>

namespace o3d {
namespace studio {
namespace fbxi {

class GeometryProxy : public ObjectProxy
{
public:

    enum VertexData
    {
        DATA_VERTICES = 0,
        DATA_COLORS = 1,
        DATA_UVS = 2,
        DATA_NORMALS = 3,
        DATA_TANGENTS = 4,
        DATA_BITANGENTS = 5
    };

    /**
     * @brief GeometryProxy
     * @param node Node named Camera
     */
    GeometryProxy(FBXNode *node);

    /**
     * @brief Process the geometry conversion and set array to
     * be accessed once processing is done.
     * @return True if success. Can raise exceptions.
     */
    Bool processGeometry();

    SmartArrayFloat vertexData(VertexData v);
    SmartArrayInt32 indices();

    SmartArrayInt32 edges();

private:

    enum MapType
    {
        BY_POLYGON_VERTEX,
        BY_POLYGON,
        BY_VERTEX
    };

    Bool readVertexData(FBXNode *node,
            const String& name,
            const String &indexName,
            SmartArrayDouble &data,
            SmartArrayInt32 &indices,
            MapType &mapType);

    SmartArrayFloat m_vertexData[DATA_BITANGENTS+1];
    SmartArrayInt32 m_indices;

    ArrayUInt32 m_toOldVertices;
    ArrayUInt32 m_toNewVertices;
};

} // namespace fbxi
} // namespace studio
} // namespace o3d

#endif // _O3DS_FBXI_GEOMETRYPROXY_H

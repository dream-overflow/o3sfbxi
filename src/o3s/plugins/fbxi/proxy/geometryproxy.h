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

namespace o3d {
namespace studio {
namespace fbxi {

class GeometryProxy : public ObjectProxy
{
public:

    /**
     * @brief GeometryProxy
     * @param node Node named Camera
     */
    GeometryProxy(FBXNode *node);

    SmartArrayDouble vertices();
    SmartArrayFloat normals();
    SmartArrayDouble uvs();
    SmartArrayInt32 indexes();
    SmartArrayInt32 edges();

    Bool processGeometry();

private:

    enum MapType
    {
        BY_POLYGON_VERTEX,
        BY_POLYGON,
        BY_VERTEX
    };

    Bool vertexData(FBXNode *node,
            const String& name,
            const String &indexName,
            SmartArrayDouble &data,
            SmartArrayInt32 &indices,
            MapType &mapType);
};

} // namespace fbxi
} // namespace studio
} // namespace o3d

#endif // _O3DS_FBXI_GEOMETRYPROXY_H

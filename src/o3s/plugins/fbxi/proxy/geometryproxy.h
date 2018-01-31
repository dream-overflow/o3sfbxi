/**
 * @brief FBX node geometry data proxy
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-01-29
 * @details
 */

#ifndef _O3DS_FBXI_GEOMETRYPROXY_H
#define _O3DS_FBXI_GEOMETRYPROXY_H

#include "proxy.h"

namespace o3d {
namespace studio {
namespace fbxi {

class GeometryProxy : public Proxy
{
public:

    /**
     * @brief GeometryProxy
     * @param node Node named Camera
     */
    GeometryProxy(FBXNode *node);

    SmartArrayDouble vertices();
    SmartArrayDouble normals();
    SmartArrayDouble uvs();
    SmartArrayInt32 indexes();
    SmartArrayInt32 edges();

private:
};

} // namespace fbxi
} // namespace studio
} // namespace o3d

#endif // _O3DS_FBXI_GEOMETRYPROXY_H

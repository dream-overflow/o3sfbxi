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

class ObjectsProxy : public Proxy
{
public:

    /**
     * @brief ObjectsProxy
     * @param node Node named Objects
     */
    ObjectsProxy(FBXNode *node);

    // mesh
    // material
    // camera
    // light
    // pose ??
};

} // namespace fbxi
} // namespace studio
} // namespace o3d

#endif // _O3DS_FBXI_OBJECTSPROXY_H

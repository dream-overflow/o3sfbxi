/**
 * @brief FBX node camera data proxy
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-01-29
 * @details
 */

#ifndef _O3DS_FBXI_CAMERAPROXY_H
#define _O3DS_FBXI_CAMERAPROXY_H

#include "nodeattributeproxy.h"
#include "modelproxy.h"

namespace o3d {
namespace studio {
namespace fbxi {

class CameraNodeProxy : public NodeAttributeProxy
{
public:

    /**
     * @brief CameraNodeProxy
     * @param node Node subclassed Camera
     */
    CameraNodeProxy(FBXNode *node);
};

class CameraModelProxy : public ModelProxy
{
public:

    /**
     * @brief CameraModelProxy
     * @param node Node subclassed Camera
     */
    CameraModelProxy(FBXNode *node);
};

} // namespace fbxi
} // namespace studio
} // namespace o3d

#endif // _O3DS_FBXI_CAMERAPROXY_H

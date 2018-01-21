/**
 * @brief FBX global settings data proxy
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-01-20
 * @details
 */

#ifndef _O3DS_FBXI_GLOBALSETTINGSPROXY_H
#define _O3DS_FBXI_GLOBALSETTINGSPROXY_H

#include "proxy.h"
#include <o3d/core/vector3.h>
#include <o3d/image/color.h>

namespace o3d {
namespace studio {
namespace fbxi {

class GlobalSettingsProxy : public Proxy
{
public:

    /**
     * @brief ObjectsProxy
     * @param node Node named GlobalSettings
     */
    GlobalSettingsProxy(FBXNode *node);

    Vector3 upAxis();
    Vector3 frontAxis();
    Vector3 coordAxis();
    Double unitScale();

    Color ambientColor();

    Int64 timeSpanStart();
    Int64 timeSpanEnd();

    // DefaultCamera
    // TimeMode
    // CustomFrameRate
};

} // namespace fbxi
} // namespace studio
} // namespace o3d

#endif // _O3DS_FBXI_GLOBALSETTINGSPROXY_H

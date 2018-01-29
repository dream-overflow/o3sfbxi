/**
 * @brief FBX node model data proxy
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-01-29
 * @details
 */

#ifndef _O3DS_FBXI_MODELPROXY_H
#define _O3DS_FBXI_MODELPROXY_H

#include "proxy.h"
#include <o3d/core/vector3.h>
#include <o3d/image/color.h>

namespace o3d {
namespace studio {
namespace fbxi {

class ModelProxy : public Proxy
{
public:

    /**
     * @brief ModelProxy
     * @param node Node named Model
     */
    ModelProxy(FBXNode *node);
};

} // namespace fbxi
} // namespace studio
} // namespace o3d

#endif // _O3DS_FBXI_MODELPROXY_H

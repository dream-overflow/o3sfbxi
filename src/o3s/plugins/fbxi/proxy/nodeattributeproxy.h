/**
 * @brief FBX node attribute data proxy
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-01-28
 * @details
 */

#ifndef _O3DS_FBXI_NODEATTRIBUTEPROXY_H
#define _O3DS_FBXI_NODEATTRIBUTEPROXY_H

#include "proxy.h"
#include <o3d/core/vector3.h>
#include <o3d/image/color.h>

namespace o3d {
namespace studio {
namespace fbxi {

class NodeAttributeProxy : public Proxy
{
public:

    /**
     * @brief NodeAttributeProxy
     * @param node Node named Definitions
     */
    NodeAttributeProxy(FBXNode *node);
};

} // namespace fbxi
} // namespace studio
} // namespace o3d

#endif // _O3DS_FBXI_NODEATTRIBUTEPROXY_H

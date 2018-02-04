/**
 * @brief FBX node light data proxy
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-01-29
 * @details
 */

#ifndef _O3DS_FBXI_LIGHTPROXY_H
#define _O3DS_FBXI_LIGHTPROXY_H

#include "modelproxy.h"
#include "nodeattributeproxy.h"

#include <o3d/image/color.h>

namespace o3d {
namespace studio {
namespace fbxi {

class LightNodeProxy : public NodeAttributeProxy
{
public:

    /**
     * @brief LightNodeProxy
     * @param node Node subclassed Light
     */
    LightNodeProxy(FBXNode *node);
};

class LightModelProxy : public ModelProxy
{
public:

    /**
     * @brief LightModelProxy
     * @param node Node subclassed Light
     */
    LightModelProxy(FBXNode *node);
};

} // namespace fbxi
} // namespace studio
} // namespace o3d

#endif // _O3DS_FBXI_LIGHTPROXY_H

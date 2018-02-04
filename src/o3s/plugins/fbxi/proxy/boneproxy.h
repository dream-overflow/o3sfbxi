/**
 * @brief FBX node bone data proxy
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-02-04
 * @details
 */

#ifndef _O3DS_FBXI_BONEPROXY_H
#define _O3DS_FBXI_BONEPROXY_H

#include "modelproxy.h"

namespace o3d {
namespace studio {
namespace fbxi {

class BoneModelProxy : public ModelProxy
{
public:

    /**
     * @brief BoneProxy
     * @param node Node named Model with LimbNode sub-node
     */
    BoneModelProxy(FBXNode *node);
};

} // namespace fbxi
} // namespace studio
} // namespace o3d

#endif // _O3DS_FBXI_BONEPROXY_H

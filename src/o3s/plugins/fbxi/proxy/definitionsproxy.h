/**
 * @brief FBX definitions data proxy
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-01-20
 * @details
 */

#ifndef _O3DS_FBXI_DEFINITIONSPROXY_H
#define _O3DS_FBXI_DEFINITIONSPROXY_H

#include "proxy.h"
#include <o3d/core/vector3.h>
#include <o3d/image/color.h>

namespace o3d {
namespace studio {
namespace fbxi {

class DefinitionsProxy : public Proxy
{
public:

    /**
     * @brief DefinitionsProxy
     * @param node Node named Definitions
     */
    DefinitionsProxy(FBXNode *node);

    Int32 count();
};

} // namespace fbxi
} // namespace studio
} // namespace o3d

#endif // _O3DS_FBXI_DEFINITIONSPROXY_H

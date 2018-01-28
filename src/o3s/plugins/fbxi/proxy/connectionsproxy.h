/**
 * @brief FBX connections data proxy
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-01-28
 * @details
 */

#ifndef _O3DS_FBXI_CONNECTIONSPROXY_H
#define _O3DS_FBXI_CONNECTIONSPROXY_H

#include "proxy.h"

namespace o3d {
namespace studio {
namespace fbxi {

class ConnectionsProxy : public Proxy
{
public:

    /**
     * @brief ConnectionsProxy
     * @param node Node named Definitions
     */
    ConnectionsProxy(FBXNode *node);
};

} // namespace fbxi
} // namespace studio
} // namespace o3d

#endif // _O3DS_FBXI_CONNECTIONSPROXY_H

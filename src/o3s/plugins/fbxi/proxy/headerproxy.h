/**
 * @brief FBX header data proxy
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-01-20
 * @details
 */

#ifndef _O3DS_FBXI_HEADERPROXY_H
#define _O3DS_FBXI_HEADERPROXY_H

#include "proxy.h"

#include <o3d/core/datetime.h>

namespace o3d {
namespace studio {
namespace fbxi {

class HeaderProxy : public Proxy
{
public:

    /**
     * @brief HeaderProxy
     * @param node Node named FBXHeaderExtension
     */
    HeaderProxy(FBXNode *node);

    DateTime creationTimeStamp() const;
    String creator() const;
};

} // namespace fbxi
} // namespace studio
} // namespace o3d

#endif // _O3DS_FBXI_HEADERPROXY_H

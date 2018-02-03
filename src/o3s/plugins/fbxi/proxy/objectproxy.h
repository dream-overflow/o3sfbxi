/**
 * @brief FBX node base object data proxy
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-02-03
 * @details
 */

#ifndef _O3DS_FBXI_OBJECTPROXY_H
#define _O3DS_FBXI_OBJECTPROXY_H

#include "proxy.h"

namespace o3d {
namespace studio {
namespace fbxi {

class ObjectProxy : public Proxy
{
public:

    /**
     * @brief ObjectProxy
     * @param node Node specialized from object
     */
    ObjectProxy(FBXNode *node);

    String name();

    Int64 uuid();
    String className();
    String subClass();
};

} // namespace fbxi
} // namespace studio
} // namespace o3d

#endif // _O3DS_FBXI_OBJECTPROXY_H

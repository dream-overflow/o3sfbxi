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

    enum ConnectionType
    {
        CONN_UNKNOWN = 0,
        CONN_OO,          //!< Object to object (eg: node relation...)
        CONN_OP,          //!< Object to property (eg: texture to material...)
    };

    /**
     * @brief ConnectionsProxy
     * @param node Node named Definitions
     */
    ConnectionsProxy(FBXNode *node);

    UInt32 numConnections();
    ConnectionType connectionType(UInt32 i);

    /**
     * @brief Object to object relation details.
     * @param i Index
     * @param parent Id
     * @param child Id
     */
    void objectRelation(UInt32 i, Int64 &parent, Int64 &child);

    /**
     * @brief Object to property details.
     * @param i Index
     * @param parent Id
     * @param child Id
     * @return Name of the property
     */
    String propertyRelation(UInt32 i, Int64 &parent, Int64 &child);
};

} // namespace fbxi
} // namespace studio
} // namespace o3d

#endif // _O3DS_FBXI_CONNECTIONSPROXY_H

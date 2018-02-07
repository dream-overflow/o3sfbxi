/**
 * @brief FBX node cluster of limbs data proxy
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-02-07
 * @details
 */

#ifndef _O3DS_FBXI_CLUSTERPROXY_H
#define _O3DS_FBXI_CLUSTERPROXY_H

#include "objectproxy.h"

#include <vector>

namespace o3d {
namespace studio {
namespace fbxi {

class ClusterProxy : public ObjectProxy
{
public:

    /**
     * @brief ClusterProxy
     * @param node Node subclassed Cluster
     */
    ClusterProxy(FBXNode *node);

    void setLink(ObjectProxy *link);
    ObjectProxy* link();

protected:

    ObjectProxy *m_link;
};

} // namespace fbxi
} // namespace studio
} // namespace o3d

#endif // _O3DS_FBXI_CAMERAPROXY_H

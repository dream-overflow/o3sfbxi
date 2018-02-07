/**
 * @brief FBX node cluster data proxy
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-02-07
 * @details
 */

#include "clusterproxy.h"
#include <o3d/core/debug.h>

#include "../property/propertystring.h"
#include "../property/propertyint32.h"
#include "../property/propertyfloat32.h"
#include "../property/propertyfloat64.h"

using namespace o3d::studio::fbxi;

ClusterProxy::ClusterProxy(FBXNode *node) :
    ObjectProxy(node),
    m_link(nullptr)
{
    if (subClass() != "Cluster") {
        O3D_ERROR(E_InvalidParameter("Must be sub class cluster"));
    }

    // @todo check Version == 100
    m_objectType = OBJECT_CLUSTER;
}

void ClusterProxy::setLink(ObjectProxy *link)
{
    m_link = link;
}

ObjectProxy *ClusterProxy::link()
{
    return m_link;
}

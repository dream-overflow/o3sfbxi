/**
 * @brief FBX definitions data proxy
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-01-20
 * @details
 */

#include "definitionsproxy.h"
#include <o3d/core/debug.h>

#include "../property/propertystring.h"
#include "../property/propertyint32.h"
#include "../property/propertyfloat32.h"
#include "../property/propertyfloat64.h"

using namespace o3d::studio::fbxi;

DefinitionsProxy::DefinitionsProxy(FBXNode *node) :
    Proxy(node)
{
    if (!m_node || m_node->name() != "Definitions") {
        O3D_ERROR(E_InvalidParameter("Must be an Definitions node"));
    }

    // @todo check Version == 100
}

o3d::Int32 DefinitionsProxy::count()
{
    Int64 c = 0;

    FBXNode *count = m_node->child("Count");
    if (count) {
        c = count->directAsInt32();
    }

    return c;
}

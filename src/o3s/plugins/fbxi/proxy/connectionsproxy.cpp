/**
 * @brief FBX connections data proxy
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-01-28
 * @details
 */

#include "connectionsproxy.h"
#include <o3d/core/debug.h>

#include "../property/propertystring.h"
#include "../property/propertyint32.h"
#include "../property/propertyint64.h"
#include "../property/propertyfloat32.h"
#include "../property/propertyfloat64.h"

using namespace o3d::studio::fbxi;

ConnectionsProxy::ConnectionsProxy(FBXNode *node) :
    Proxy(node)
{
    if (!m_node || m_node->name() != "Connections") {
        O3D_ERROR(E_InvalidParameter("Must be an Connections node"));
    }

    // @todo check Version ==
}

o3d::UInt32 ConnectionsProxy::numConnections()
{
    // assume any children are "C" else have to count it properly
    return m_node->numChildren();
}

ConnectionsProxy::ConnectionType ConnectionsProxy::connectionType(UInt32 i)
{
    if (i < m_node->numChildren()) {
        // assume any children are "C" else have to index properly
        FBXNode *node = m_node->childAt(i);
        if (node->name() == "C") {
            if (node->property(0)->type() == Property::PROP_STRING) {
                PropertyString *prop = static_cast<PropertyString*>(node->property(0));
                String v = prop->value();

                if (v == "OO") {
                    return CONN_OO;
                } else if (v == "OP") {
                    return CONN_OP;
                } else {
                    return CONN_UNKNOWN;
                }
            }
        }
    }

    return CONN_UNKNOWN;
}

void ConnectionsProxy::objectRelation(o3d::UInt32 i, o3d::Int64 &parent, o3d::Int64 &child)
{
     if (i < m_node->numChildren()) {
        FBXNode *node = m_node->childAt(i);
        if (node->name() == "C") {
            if (node->property(0)->type() == Property::PROP_STRING) {
                PropertyString *prop = static_cast<PropertyString*>(node->property(0));
                String v = prop->value();

                if (v == "OO") {
                    PropertyInt64 *cp = static_cast<PropertyInt64*>(node->property(1));
                    PropertyInt64 *pp = static_cast<PropertyInt64*>(node->property(2));

                    parent = pp->value();
                    child = cp->value();

                    return;
                }
            }
        }
    }
}

o3d::String ConnectionsProxy::propertyRelation(o3d::UInt32 i, o3d::Int64 &parent, o3d::Int64 &child)
{
     if (i < m_node->numChildren()) {
        FBXNode *node = m_node->childAt(i);
        if (node->name() == "C") {
            if (node->property(0)->type() == Property::PROP_STRING) {
                PropertyString *prop = static_cast<PropertyString*>(node->property(0));
                String v = prop->value();

                if (v == "OP") {
                    PropertyInt64 *cp = static_cast<PropertyInt64*>(node->property(1));
                    PropertyInt64 *pp = static_cast<PropertyInt64*>(node->property(2));
                    PropertyString *np = static_cast<PropertyString*>(node->property(3));

                    parent = pp->value();
                    child = cp->value();

                    return np->value();
                }
            }
        }
    }

    return String();
}

/**
 * @brief FBX node object data proxy
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-02-03
 * @details
 */

#include "objectproxy.h"
#include <o3d/core/debug.h>

#include "../property/propertystring.h"
#include "../property/propertyint32.h"
#include "../property/propertyint64.h"

using namespace o3d::studio::fbxi;

ObjectProxy::ObjectProxy(FBXNode *node) :
    Proxy(node)
{
}

o3d::String ObjectProxy::name()
{
    String name = className();
    Int32 pos = name.find(':');
    if (pos > 0) {
        return name.slice(0, pos-1);
    } else {
        return name;
    }
}

o3d::Int64 ObjectProxy::uuid()
{
    if (m_node->property(0) && m_node->property(0)->type() == Property::PROP_INT64) {
        return static_cast<PropertyInt64*>(m_node->property(0))->value();
    }
}

o3d::String ObjectProxy::className()
{
    if (m_node->property(0) && m_node->property(1)->type() == Property::PROP_STRING) {
        return static_cast<PropertyString*>(m_node->property(1))->value();
    }
}

o3d::String ObjectProxy::subClass()
{
    if (m_node->property(0) && m_node->property(2)->type() == Property::PROP_STRING) {
        return static_cast<PropertyString*>(m_node->property(2))->value();
    }
}

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
    Proxy(node),
    m_objectType(OBJECT_UNDEFINED),
    m_parent(nullptr),
    m_nodeAttr(nullptr)
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

ObjectProxy::ObjectType ObjectProxy::objectType() const
{
    return m_objectType;
}

o3d::Int64 ObjectProxy::uid()
{
    if (m_node->property(0) && m_node->property(0)->type() == Property::PROP_INT64) {
        return static_cast<PropertyInt64*>(m_node->property(0))->value();
    }

    return -1;
}

o3d::String ObjectProxy::className()
{
    if (m_node->property(1) && m_node->property(1)->type() == Property::PROP_STRING) {
        return static_cast<PropertyString*>(m_node->property(1))->value();
    }

    return String();
}

o3d::String ObjectProxy::subClass()
{
    if (m_node->property(2) && m_node->property(2)->type() == Property::PROP_STRING) {
        return static_cast<PropertyString*>(m_node->property(2))->value();
    }

    return String();
}

void ObjectProxy::setParent(ObjectProxy *parent)
{
    m_parent = parent;
}

void ObjectProxy::addChild(ObjectProxy *proxy)
{
    m_children.push_back(proxy);
}

ObjectProxy *ObjectProxy::parent()
{
    return m_parent;
}

void ObjectProxy::setNodeAttribute(NodeAttributeProxy *attr)
{
    m_nodeAttr = attr;
}

NodeAttributeProxy *ObjectProxy::nodeAttribute()
{
    return m_nodeAttr;
}

ObjectProxy* ObjectProxy::recursiveNext(std::list<size_t> &cursor)
{
    ObjectProxy *result = nullptr;
    ObjectProxy *node = this;

    while (cursor.back() >= node->m_children.size()) {
        cursor.pop_back();

        node = node->m_parent;

        if (cursor.empty() || node == nullptr) {
            return nullptr;
        }
    }

    if (!cursor.empty() && cursor.back() < node->m_children.size()) {
        result = node->m_children[cursor.back()];

        // next potential child at this level
        ++cursor.back();

        // and enter to one more depth
        cursor.push_back(0);
    }

    return result;
}

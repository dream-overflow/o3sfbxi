/**
 * @brief FBX importer node controller
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-01-14
 * @details
 */

#include "fbxnode.h"

using namespace o3d::studio::fbxi;

FBXNode::FBXNode(const String &name) :
    m_name(name)
{

}

FBXNode::~FBXNode()
{
    for (auto it = m_properties.begin(); it != m_properties.end(); ++it) {
        delete it->second;
    }

    for (FBXNode *node : m_nodes) {
        delete node;
    }
}

const o3d::String &FBXNode::name() const
{
    return m_name;
}

void FBXNode::addChild(FBXNode *child)
{
    m_nodes.push_back(child);
}

FBXNode *FBXNode::child(const o3d::String &name)
{
    for (FBXNode *node : m_nodes) {
        if (node->m_name == name) {
            return node;
        }
    }

    return nullptr;
}

void FBXNode::addProperty(Property *property)
{
    if (property) {
        m_properties[property->name()] = property;
    }
}

const Property *FBXNode::property(const o3d::String &name) const
{
    auto cit = m_properties.find(name);
    if (cit != m_properties.cend()) {
        return cit->second;
    } else {
        return nullptr;
    }
}

const std::list<const Property *> FBXNode::propertyList() const
{
    std::list<const Property*> res;

    for (auto cit = m_properties.cbegin(); cit != m_properties.cend(); ++cit) {
        res.push_back(cit->second);
    }

    return res;
}

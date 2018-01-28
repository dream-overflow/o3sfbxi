/**
 * @brief FBX importer node controller
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-01-14
 * @details
 */

#include "fbxnode.h"
#include "property/propertystring.h"
#include "property/propertyfloat32.h"
#include "property/propertyfloat64.h"
#include "property/propertyint16.h"
#include "property/propertyint32.h"
#include "property/propertyint64.h"

using namespace o3d::studio::fbxi;

FBXNode::FBXNode(const String &name) :
    m_name(name)
{

}

FBXNode::~FBXNode()
{
    for (Property *property : m_properties) {
        delete property;
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

FBXNode *FBXNode::childAt(o3d::UInt32 i)
{
    if (i < (UInt32)m_nodes.size()) {
        UInt32 c = 0;
        for (auto it = m_nodes.begin(); it != m_nodes.end(); ++it) {
            if (c == i) {
                return *it;
            }

            ++c;
        }
    }

    return nullptr;
}

void FBXNode::addProperty(Property *property)
{
    if (property) {
        m_properties.push_back(property);
    }
}

const Property *FBXNode::property(UInt32 idx) const
{
    if (idx < m_properties.size()) {
        return m_properties[idx];
    } else {
        return nullptr;
    }
}

Property *FBXNode::property(o3d::UInt32 idx)
{
    if (idx < m_properties.size()) {
        return m_properties[idx];
    } else {
        return nullptr;
    }
}

o3d::UInt32 FBXNode::numProperties() const
{
    return (UInt32)m_properties.size();
}

o3d::UInt32 FBXNode::numChildren() const
{
    return (UInt32)m_nodes.size();
}

FBXNode *FBXNode::searchPropertyNode(const o3d::String &name)
{
    for (FBXNode *node : m_nodes) {
        if (node->m_name == "P") {
            if (node->m_properties.size() > 1) {
                if (node->m_properties[0]->type() == Property::PROP_STRING) {
                    System::print(static_cast<PropertyString*>(node->m_properties[0])->value(), "");
                }
            }
        }
    }

    for (FBXNode *node : m_nodes) {
        if (node->m_name == "P") {
            if (node->m_properties.size() > 1) {
                if (node->m_properties[0]->type() == Property::PROP_STRING) {
                    if (static_cast<PropertyString*>(node->m_properties[0])->value() == name) {
                        return node;
                    }
                }
            }

        }
    }

    return nullptr;
}

const std::vector<const Property *> FBXNode::properties() const
{
    std::vector<const Property*> res;

    for (auto cit = m_properties.cbegin(); cit != m_properties.cend(); ++cit) {
        res.push_back(*cit);
    }

    return res;
}

o3d::Int32 FBXNode::directAsInt32() const
{
    Int32 i = 0;

    if (m_properties.size() == 1) {
        Property::Type typeDef = m_properties[0]->type();
        if (typeDef == Property::PROP_INT32) {
            PropertyInt32 *pV = static_cast<PropertyInt32*>(m_properties[0]);
            i = pV->value();
        }
    }

    return i;
}

o3d::Vector3 FBXNode::interpretAsVector3() const
{
    Vector3 v;

    if ((m_properties.size() == 7) && (m_name == "P" || m_name == "PS")) {
        String typeName = static_cast<PropertyString*>(m_properties[1])->value();
        Property::Type typeDef = m_properties[4]->type();

        if (typeName == "ColorRGB" && typeDef == Property::PROP_FLOAT64) {
            v.x() = static_cast<PropertyFloat64*>(m_properties[4])->value();
            v.y() = static_cast<PropertyFloat64*>(m_properties[5])->value();
            v.z() = static_cast<PropertyFloat64*>(m_properties[6])->value();
        } else if (typeName == "ColorRGB" && typeDef == Property::PROP_FLOAT32) {
            v.x() = static_cast<PropertyFloat32*>(m_properties[4])->value();
            v.y() = static_cast<PropertyFloat32*>(m_properties[5])->value();
            v.z() = static_cast<PropertyFloat32*>(m_properties[6])->value();
        }
    }

    return v;
}

o3d::Color FBXNode::interpretAsColor() const
{
    Color color;
    color.a(1.0);

    if ((m_properties.size() == 7) && (m_name == "P" || m_name == "PS")) {
        String typeName = static_cast<PropertyString*>(m_properties[1])->value();
        Property::Type typeDef = m_properties[4]->type();

        if (typeName == "ColorRGB" && typeDef == Property::PROP_FLOAT64) {
            color.r(static_cast<PropertyFloat64*>(m_properties[4])->value());
            color.g(static_cast<PropertyFloat64*>(m_properties[5])->value());
            color.b(static_cast<PropertyFloat64*>(m_properties[6])->value());
        } else if (typeName == "ColorRGB" && typeDef == Property::PROP_FLOAT32) {
            color.r(static_cast<PropertyFloat32*>(m_properties[4])->value());
            color.g(static_cast<PropertyFloat32*>(m_properties[5])->value());
            color.b(static_cast<PropertyFloat32*>(m_properties[6])->value());
        }
    }

    return color;
}

o3d::Float FBXNode::interpretAsFloat() const
{
    Float f = 0.0;

    if ((m_properties.size() == 5) && (m_name == "P" || m_name == "PS")) {
        String typeName = static_cast<PropertyString*>(m_properties[1])->value();
        Property::Type typeDef = m_properties[4]->type();

        if (typeName == "float" && typeDef == Property::PROP_FLOAT32) {
            f = static_cast<PropertyFloat64*>(m_properties[4])->value();
        }
    }

    return f;
}

o3d::Double FBXNode::interpretAsDouble() const
{
    Double f = 0.0;

    if (m_properties.size() == 5) {
        String typeName = static_cast<PropertyString*>(m_properties[1])->value();
        Property::Type typeDef = m_properties[4]->type();

        if (typeName == "double" && typeDef == Property::PROP_FLOAT64) {
            f = static_cast<PropertyFloat64*>(m_properties[4])->value();
        }
    }

    return f;
}

o3d::Int32 FBXNode::interpretAsInt32() const
{
    Int32 i = 0.0;

    if ((m_properties.size() == 5) && (m_name == "P" || m_name == "PS")) {
        String typeName = static_cast<PropertyString*>(m_properties[1])->value();
        Property::Type typeDef = m_properties[4]->type();

        if (typeName == "int" && typeDef == Property::PROP_INT32) {
            i = static_cast<PropertyInt32*>(m_properties[4])->value();
        }
    }

    return i;
}

o3d::Int64 FBXNode::interpretAsInt64() const
{
    Int64 i = 0.0;

    if ((m_properties.size() == 5) && (m_name == "P" || m_name == "PS")) {
        String typeName = static_cast<PropertyString*>(m_properties[1])->value();
        Property::Type typeDef = m_properties[4]->type();

        if (typeName == "int" && typeDef == Property::PROP_INT64) {
            i = static_cast<PropertyInt64*>(m_properties[4])->value();
        }
    }

    return i;
}

o3d::Int64 FBXNode::interpretAsTime() const
{
    Int64 i = 0.0;

    if ((m_properties.size() == 5) && (m_name == "P" || m_name == "PS")) {
        String typeName = static_cast<PropertyString*>(m_properties[1])->value();
        Property::Type typeDef = m_properties[4]->type();

        if (typeName == "KTime" && typeDef == Property::PROP_INT64) {
            i = static_cast<PropertyInt64*>(m_properties[4])->value();
        }
    }

    return i;
}

/**
 * @brief FBX global settings data proxy
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-01-20
 * @details
 */

#include "globalsettingsproxy.h"
#include <o3d/core/debug.h>

#include "../property/propertystring.h"
#include "../property/propertyint32.h"
#include "../property/propertyfloat32.h"
#include "../property/propertyfloat64.h"

using namespace o3d::studio::fbxi;

GlobalSettingsProxy::GlobalSettingsProxy(FBXNode *node) :
    Proxy(node)
{
    if (!m_node || m_node->name() != "GlobalSettings") {
        O3D_ERROR(E_InvalidParameter("Must be an GlobalSettings node"));
    }
}

o3d::Vector3 GlobalSettingsProxy::upAxis()
{
    Vector3 axis;
    Int32 a = 0;
    Int32 sign = 0;

    FBXNode *p70 = m_node->child("Properties70");
    if (p70) {
        FBXNode *P = p70->searchPropertyNode("UpAxis");
        if (P) {
            String type = static_cast<PropertyString*>(P->property(1))->value();
            a = static_cast<PropertyInt32*>(P->property(4))->value();
        }
        P = p70->searchPropertyNode("UpAxisSign");
        if (P) {
            String type = static_cast<PropertyString*>(P->property(1))->value();
            sign = static_cast<PropertyInt32*>(P->property(4))->value();
        }

        if (a == 0) {
            axis.set(sign ? 1 : -1, 0, 0);
        } else if (a == 1) {
            axis.set(0, sign ? 1 : -1, 0);
        } else if (a == 2) {
            axis.set(0, 0, sign ? 1 : -1);
        }
    }

    return axis;
}

o3d::Vector3 GlobalSettingsProxy::frontAxis()
{
    Vector3 axis;
    Int32 a = 0;
    Int32 sign = 0;

    FBXNode *p70 = m_node->child("Properties70");
    if (p70) {
        FBXNode *P = p70->searchPropertyNode("FrontAxis");
        if (P) {
            String type = static_cast<PropertyString*>(P->property(1))->value();
            Int32 a = static_cast<PropertyInt32*>(P->property(4))->value();
        }
        P = p70->searchPropertyNode("FrontAxisSign");
        if (P) {
            String type = static_cast<PropertyString*>(P->property(1))->value();
            sign = static_cast<PropertyInt32*>(P->property(4))->value();
        }

        if (a == 0) {
            axis.set(sign ? 1 : -1, 0, 0);
        } else if (a == 1) {
            axis.set(0, sign ? 1 : -1, 0);
        } else if (a == 2) {
            axis.set(0, 0, sign ? 1 : -1);
        }
    }

    return axis;
}

o3d::Vector3 GlobalSettingsProxy::coordAxis()
{
    Vector3 axis;
    Int32 a = 0;
    Int32 sign = 0;

    FBXNode *p70 = m_node->child("Properties70");
    if (p70) {
        FBXNode *P = p70->searchPropertyNode("CoordAxis");
        if (P) {
            String type = static_cast<PropertyString*>(P->property(1))->value();
            a = static_cast<PropertyInt32*>(P->property(4))->value();
        }
        P = p70->searchPropertyNode("CoordAxisSign");
        if (P) {
            String type = static_cast<PropertyString*>(P->property(1))->value();
            sign = static_cast<PropertyInt32*>(P->property(4))->value();
        }

        if (a == 0) {
            axis.set(sign ? 1 : -1, 0, 0);
        } else if (a == 1) {
            axis.set(0, sign ? 1 : -1, 0);
        } else if (a == 2) {
            axis.set(0, 0, sign ? 1 : -1);
        }
    }

    return axis;
}

o3d::Double GlobalSettingsProxy::unitScale()
{
    Double scale = 1.0;

    FBXNode *p70 = m_node->child("Properties70");
    if (p70) {
        FBXNode *P = p70->searchPropertyNode("UnitScaleFactor");
        if (P) {
            String type = static_cast<PropertyString*>(P->property(1))->value();
            Double value = static_cast<PropertyFloat64*>(P->property(4))->value();

            scale = value;
        }
    }

    return scale;
}

o3d::Color GlobalSettingsProxy::ambientColor()
{
    Color color;
    color.a(1.0);

    FBXNode *p70 = m_node->child("Properties70");
    if (p70) {
        FBXNode *P = p70->searchPropertyNode("AmbientColor");
        if (P) {
            String type = static_cast<PropertyString*>(P->property(1))->value();
            Property::Type ptype = P->property(4)->type();

            if (ptype == Property::PROP_FLOAT64) {
                color.r(static_cast<PropertyFloat64*>(P->property(4))->value());
                color.g(static_cast<PropertyFloat64*>(P->property(5))->value());
                color.b(static_cast<PropertyFloat64*>(P->property(6))->value());
            }
        }
    }

    return color;
}

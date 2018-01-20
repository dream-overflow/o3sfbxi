/**
 * @brief FBX header data proxy
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-01-20
 * @details
 */

#include "headerproxy.h"
#include <o3d/core/debug.h>

#include "../property/propertyint16.h"
#include "../property/propertyint32.h"
#include "../property/propertyfloat32.h"
#include "../property/propertyfloat64.h"
#include "../property/propertybool.h"
#include "../property/propertyboolarray.h"
#include "../property/propertyint32array.h"
#include "../property/propertyint64array.h"
#include "../property/propertyfloat32array.h"
#include "../property/propertyfloat64array.h"
#include "../property/propertystring.h"
#include "../property/propertyrawbytearray.h"

using namespace o3d::studio::fbxi;

Proxy::Proxy(o3d::studio::fbxi::FBXNode *node) :
    m_node(node)
{
    O3D_ASSERT(m_node != nullptr);
}

Proxy::~Proxy()
{

}

HeaderProxy::HeaderProxy(FBXNode *node) :
    Proxy(node)
{
    if (!m_node || m_node->name() != "FBXHeaderExtension") {
        O3D_ERROR(E_InvalidParameter("Must be a FBXHeaderExtension node"));
    }
}

o3d::DateTime HeaderProxy::creationTimeStamp() const
{
    o3d::DateTime dateTime;
    FBXNode *cts = m_node->child("CreationTimeStamp");
    if (cts) {
        FBXNode *year = cts->child("Year");
        PropertyInt32 *pYear = static_cast<PropertyInt32*>(year->property(0));
        dateTime.year = pYear->value();

        FBXNode *month = cts->child("Month");
        PropertyInt32 *pMonth = static_cast<PropertyInt32*>(month->property(0));
        dateTime.month = (o3d::Month)pMonth->value();

        FBXNode *day = cts->child("Day");
        PropertyInt32 *pDay = static_cast<PropertyInt32*>(day->property(0));
        dateTime.mday = pDay->value();

        FBXNode *hour = cts->child("Hour");
        PropertyInt32 *pHour = static_cast<PropertyInt32*>(hour->property(0));
        dateTime.hour = pHour->value();

        FBXNode *minute = cts->child("Minute");
        PropertyInt32 *pMinute = static_cast<PropertyInt32*>(minute->property(0));
        dateTime.minute = pMinute->value();

        FBXNode *second = cts->child("Second");
        PropertyInt32 *pSecond = static_cast<PropertyInt32*>(second->property(0));
        dateTime.second = pSecond->value();

        FBXNode *millisecond = cts->child("Millisecond");
        PropertyInt32 *pMillisecond = static_cast<PropertyInt32*>(millisecond->property(0));
        dateTime.millisecond = pMillisecond->value();
    }

    return dateTime;
}

o3d::String HeaderProxy::creator() const
{
    String creator;
    FBXNode *c = m_node->child("Creator");
    if (c) {
        PropertyString *pCreator = static_cast<PropertyString*>(c->property(0));
        creator = pCreator->value();
    }

    return creator;
}

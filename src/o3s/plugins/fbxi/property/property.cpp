/**
 * @brief FBX importer base property
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-01-16
 * @details
 */

#include "property.h"

using namespace o3d::studio::fbxi;

Property::Property(Property::Type type) :
    m_type(type)
{
}

Property::~Property()
{

}

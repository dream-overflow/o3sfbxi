/**
 * @brief FBX importer boolean property
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-01-16
 * @details
 */

#ifndef _O3DS_FBXI_PROPERTYINT16_H
#define _O3DS_FBXI_PROPERTYINT16_H

#include "property.h"

namespace o3d {
namespace studio {
namespace fbxi {

class PropertyInt16 : public Property
{
public:

    PropertyInt16(const String &name, Int16 value) :
        Property(name, PROP_INT16),
        m_value(value)
    {

    }

    virtual ~PropertyInt16() {}

    inline Int16 value() const { return m_value; }

protected:

    Int16 m_value;
};

} // namespace fbxi
} // namespace studio
} // namespace o3d

#endif // _O3DS_FBXI_PROPERTYINT16_H

/**
 * @brief FBX importer boolean array property
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-01-16
 * @details
 */

#ifndef _O3DS_FBXI_PROPERTYBOOLARRAY_H
#define _O3DS_FBXI_PROPERTYBOOLARRAY_H

#include "property.h"

namespace o3d {
namespace studio {
namespace fbxi {

class PropertyBoolArray : public Property
{
public:

    PropertyBoolArray(SmartArrayUInt8 &value) :
        Property(PROP_BOOL_ARRAY),
        m_value(value)
    {

    }

    virtual ~PropertyBoolArray() {}

    inline const SmartArrayUInt8& value() const { return m_value; }

protected:

    SmartArrayUInt8 m_value;
};

} // namespace fbxi
} // namespace studio
} // namespace o3d

#endif // _O3DS_FBXI_PROPERTYBOOLARRAY_H

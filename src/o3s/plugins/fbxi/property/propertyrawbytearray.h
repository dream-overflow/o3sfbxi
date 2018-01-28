/**
 * @brief FBX importer raw byte array property
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-01-16
 * @details
 */

#ifndef _O3DS_FBXI_PROPERTYRAWBYTEARRAY_H
#define _O3DS_FBXI_PROPERTYRAWBYTEARRAY_H

#include "property.h"

namespace o3d {
namespace studio {
namespace fbxi {

class PropertyRawByteArray : public Property
{
public:

    PropertyRawByteArray(SmartArrayUInt8 &value) :
        Property(PROP_RAW_BYTE_ARRAY),
        m_value(value)
    {

    }

    virtual ~PropertyRawByteArray() {}

    inline const SmartArrayUInt8& value() const { return m_value; }

protected:

    SmartArrayUInt8 m_value;
};

} // namespace fbxi
} // namespace studio
} // namespace o3d

#endif // _O3DS_FBXI_PROPERTYRAWBYTEARRAY_H

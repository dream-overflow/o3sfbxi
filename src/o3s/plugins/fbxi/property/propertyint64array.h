/**
 * @brief FBX importer int64 array property
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-01-16
 * @details
 */

#ifndef _O3DS_FBXI_PROPERTYINT64ARRAY_H
#define _O3DS_FBXI_PROPERTYINT64ARRAY_H

#include "property.h"

namespace o3d {
namespace studio {
namespace fbxi {

class PropertyInt64Array : public Property
{
public:

    PropertyInt64Array(SmartArrayInt64 &value) :
        Property(PROP_INT64_ARRAY),
        m_value(value)
    {

    }

    virtual ~PropertyInt64Array() {}

    inline const SmartArrayInt64& value() const { return m_value; }

protected:

    SmartArrayInt64 m_value;
};

} // namespace fbxi
} // namespace studio
} // namespace o3d

#endif // _O3DS_FBXI_PROPERTYINT64ARRAY_H

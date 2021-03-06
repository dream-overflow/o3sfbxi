/**
 * @brief FBX importer float64 array property
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-01-16
 * @details
 */

#ifndef _O3DS_FBXI_PROPERTYFLOAT64ARRAY_H
#define _O3DS_FBXI_PROPERTYFLOAT64ARRAY_H

#include "property.h"

namespace o3d {
namespace studio {
namespace fbxi {

class PropertyFloat64Array : public Property
{
public:

    PropertyFloat64Array(SmartArrayDouble &value) :
        Property(PROP_FLOAT64_ARRAY),
        m_value(value)
    {

    }

    virtual ~PropertyFloat64Array() {}

    inline const SmartArrayDouble& value() const { return m_value; }

protected:

    SmartArrayDouble m_value;
};

} // namespace fbxi
} // namespace studio
} // namespace o3d

#endif // _O3DS_FBXI_PROPERTYFLOAT64ARRAY_H

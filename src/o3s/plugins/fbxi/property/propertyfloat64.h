/**
 * @brief FBX importer float64 property
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-01-16
 * @details
 */

#ifndef _O3DS_FBXI_PROPERTYFLOAT64_H
#define _O3DS_FBXI_PROPERTYFLOAT64_H

#include "property.h"

namespace o3d {
namespace studio {
namespace fbxi {

class PropertyFloat64 : public Property
{
public:

    PropertyFloat64(Double value) :
        Property(PROP_FLOAT64),
        m_value(value)
    {

    }

    virtual ~PropertyFloat64() {}

    inline Double value() const { return m_value; }

protected:

    Double m_value;
};

} // namespace fbxi
} // namespace studio
} // namespace o3d

#endif // _O3DS_FBXI_PROPERTYFLOAT64_H

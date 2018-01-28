/**
 * @brief FBX importer int64 property
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-01-16
 * @details
 */

#ifndef _O3DS_FBXI_PROPERTYINT64_H
#define _O3DS_FBXI_PROPERTYINT64_H

#include "property.h"

namespace o3d {
namespace studio {
namespace fbxi {

class PropertyInt64 : public Property
{
public:

    PropertyInt64(Int64 value) :
        Property(PROP_INT64),
        m_value(value)
    {

    }

    virtual ~PropertyInt64() {}

    inline Int64 value() const { return m_value; }

protected:

    Int64 m_value;
};

} // namespace fbxi
} // namespace studio
} // namespace o3d

#endif // _O3DS_FBXI_PROPERTYINT64_H

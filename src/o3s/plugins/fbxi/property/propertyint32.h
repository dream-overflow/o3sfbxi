/**
 * @brief FBX importer int32 property
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-01-16
 * @details
 */

#ifndef _O3DS_FBXI_PROPERTYINT32_H
#define _O3DS_FBXI_PROPERTYINT32_H

#include "property.h"

namespace o3d {
namespace studio {
namespace fbxi {

class PropertyInt32 : public Property
{
public:

    PropertyInt32(Int32 value) :
        Property(PROP_INT32),
        m_value(value)
    {

    }

    virtual ~PropertyInt32() {}

    inline Int32 value() const { return m_value; }

protected:

    Int32 m_value;
};

} // namespace fbxi
} // namespace studio
} // namespace o3d

#endif // _O3DS_FBXI_PROPERTYINT32_H

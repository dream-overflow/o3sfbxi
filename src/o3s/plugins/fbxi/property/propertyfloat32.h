/**
 * @brief FBX importer float32 property
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-01-16
 * @details
 */

#ifndef _O3DS_FBXI_PROPERTYFLOAT32_H
#define _O3DS_FBXI_PROPERTYFLOAT32_H

#include "property.h"

namespace o3d {
namespace studio {
namespace fbxi {

class PropertyFloat32 : public Property
{
public:

    PropertyFloat32( Float value) :
        Property(PROP_FLOAT32),
        m_value(value)
    {

    }

    virtual ~PropertyFloat32() {}

    inline Float value() const { return m_value; }

protected:

    Float m_value;
};

} // namespace fbxi
} // namespace studio
} // namespace o3d

#endif // _O3DS_FBXI_PROPERTYFLOAT32_H

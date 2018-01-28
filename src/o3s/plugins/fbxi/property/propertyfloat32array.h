/**
 * @brief FBX importer float32 array property
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-01-16
 * @details
 */

#ifndef _O3DS_FBXI_PROPERTYFLOAT32ARRAY_H
#define _O3DS_FBXI_PROPERTYFLOAT32ARRAY_H

#include "property.h"

namespace o3d {
namespace studio {
namespace fbxi {

class PropertyFloat32Array : public Property
{
public:

    PropertyFloat32Array(SmartArrayFloat &value) :
        Property(PROP_FLOAT32_ARRAY),
        m_value(value)
    {

    }

    virtual ~PropertyFloat32Array() {}

    inline const SmartArrayFloat& value() const { return m_value; }

protected:

    SmartArrayFloat m_value;
};

} // namespace fbxi
} // namespace studio
} // namespace o3d

#endif // _O3DS_FBXI_PROPERTYFLOAT32ARRAY_H

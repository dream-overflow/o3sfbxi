/**
 * @brief FBX importer int32 array property
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-01-16
 * @details
 */

#ifndef _O3DS_FBXI_PROPERTYINT32ARRAY_H
#define _O3DS_FBXI_PROPERTYINT32ARRAY_H

#include "property.h"

namespace o3d {
namespace studio {
namespace fbxi {

class PropertyInt32Array : public Property
{
public:

    PropertyInt32Array(SmartArrayInt32 &value) :
        Property(PROP_INT32_ARRAY),
        m_value(value)
    {

    }

    virtual ~PropertyInt32Array() {}

    inline const SmartArrayInt32& value() const { return m_value; }

protected:

    SmartArrayInt32 m_value;
};

} // namespace fbxi
} // namespace studio
} // namespace o3d

#endif // _O3DS_FBXI_PROPERTYINT32ARRAY_H

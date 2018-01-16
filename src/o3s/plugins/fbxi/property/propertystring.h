/**
 * @brief FBX importer string property
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-01-16
 * @details
 */

#ifndef _O3DS_FBXI_PROPERTYSTRING_H
#define _O3DS_FBXI_PROPERTYSTRING_H

#include "property.h"

namespace o3d {
namespace studio {
namespace fbxi {

class PropertyString : public Property
{
public:

    PropertyString(const String &name, const String &value) :
        Property(name, PROP_STRING),
        m_value(value)
    {

    }

    virtual ~PropertyString() {}

    inline const String& value() const { return m_value; }

protected:

    String m_value;
};

} // namespace fbxi
} // namespace studio
} // namespace o3d

#endif // _O3DS_FBXI_PROPERTYSTRING_H

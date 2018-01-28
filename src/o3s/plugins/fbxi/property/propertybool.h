/**
 * @brief FBX importer boolean property
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-01-16
 * @details
 */

#ifndef _O3DS_FBXI_PROPERTYBOOL_H
#define _O3DS_FBXI_PROPERTYBOOL_H

#include "property.h"

namespace o3d {
namespace studio {
namespace fbxi {

class PropertyBool : public Property
{
public:

    PropertyBool(Bool value) :
        Property(PROP_BOOL),
        m_value(value)
    {

    }

    virtual ~PropertyBool() {}

    inline Bool value() const { return m_value; }

protected:

    Bool m_value;
};

} // namespace fbxi
} // namespace studio
} // namespace o3d

#endif // _O3DS_FBXI_PROPERTYBOOL_H

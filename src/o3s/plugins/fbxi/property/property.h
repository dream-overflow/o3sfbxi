/**
 * @brief FBX importer base property
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-01-16
 * @details
 */

#ifndef _O3DS_FBXI_PROPERTY_H
#define _O3DS_FBXI_PROPERTY_H

#include <o3d/core/string.h>
#include <o3d/studio/common/global.h>

namespace o3d {
namespace studio {
namespace fbxi {

class O3S_PLUGIN_API Property
{
public:

    enum Type
    {
        PROP_BOOL,
        PROP_INT16,
        PROP_INT32,
        PROP_INT64,
        PROP_FLOAT32,
        PROP_FLOAT64,
        PROP_STRING,
        PROP_BOOL_ARRAY,
        PROP_RAW_BYTE_ARRAY,
        PROP_INT32_ARRAY,
        PROP_INT64_ARRAY,
        PROP_FLOAT32_ARRAY,
        PROP_FLOAT64_ARRAY
    };

    Property(Type type);

    virtual ~Property();

    inline Type type() const { return m_type; }

protected:

    Type m_type;
};

} // namespace fbxi
} // namespace studio
} // namespace o3d

#endif // _O3DS_FBXI_PROPERTY_H

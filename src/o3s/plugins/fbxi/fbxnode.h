/**
 * @brief FBX importer node controller
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-01-14
 * @details
 */

#ifndef _O3DS_FBXI_FBXNODE_H
#define _O3DS_FBXI_FBXNODE_H

#include <o3d/core/instream.h>
#include <o3d/core/stringmap.h>

#include "property/property.h"

namespace o3d {
namespace studio {
namespace fbxi {

class FBXNode
{
public:

    FBXNode(const String &name);
    virtual ~FBXNode();

    const String& name() const;

    void addChild(FBXNode *child);
    FBXNode *child(const String &name);

    void addProperty(Property *property);
    const Property* property(const String &name) const;

    const std::list<const Property *> propertyList() const;

protected:

    String m_name;

    std::list<FBXNode*> m_nodes;
    StringMap<Property*> m_properties;
};

} // namespace fbxi
} // namespace studio
} // namespace o3d

#endif // _O3DS_FBXI_FBXNODE_H

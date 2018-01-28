/**
 * @brief FBX importer node controller
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-01-14
 * @details
 */

#ifndef _O3DS_FBXI_FBXNODE_H
#define _O3DS_FBXI_FBXNODE_H

#include <vector>
#include <o3d/core/instream.h>
#include <o3d/core/vector3.h>
#include <o3d/image/color.h>
#include "property/property.h"

namespace o3d {
namespace studio {
namespace fbxi {

class O3S_PLUGIN_API FBXNode
{
public:

    FBXNode(const String &name);
    virtual ~FBXNode();

    const String& name() const;

    void addChild(FBXNode *child);
    FBXNode *child(const String &name);
    FBXNode *childAt(UInt32 i);

    void addProperty(Property *property);
    const Property* property(UInt32 idx) const;
    Property* property(UInt32 idx);

    UInt32 numProperties() const;
    UInt32 numChildren() const;

    FBXNode* searchPropertyNode(const String &name);

    const std::vector<const Property *> properties() const;

    //
    // Direct property (with sub node)
    //

    Int32 directAsInt32() const;

    //
    // Property conversion helpers for named named "P" or "PS"
    //

    Vector3 interpretAsVector3() const;
    Color interpretAsColor() const;
    Float interpretAsFloat() const;
    Double interpretAsDouble() const;
    Int32 interpretAsInt32() const;
    Int64 interpretAsInt64() const;
    Int64 interpretAsTime() const;

protected:

    String m_name;

    std::list<FBXNode*> m_nodes;
    std::vector<Property*> m_properties;
};

} // namespace fbxi
} // namespace studio
} // namespace o3d

#endif // _O3DS_FBXI_FBXNODE_H

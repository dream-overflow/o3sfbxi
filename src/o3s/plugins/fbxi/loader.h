/**
 * @brief FBX importer data loader from parser.
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-01-20
 * @details
 */

#ifndef _O3DS_FBXI_LOADER_H
#define _O3DS_FBXI_LOADER_H

#include "parser.h"

#include "importer.h"

#include <o3d/core/vector3.h>
#include <map>

namespace o3d {
namespace studio {
namespace fbxi {

class ObjectProxy;

class O3S_PLUGIN_API Loader
{
public:

    Loader(Parser *parser, FbxImportDefinition *def);
    ~Loader();

    void load();

    ObjectProxy* objectProxy(Int64 uid);

private:

    Parser *m_parser;

    FBXNode *m_rootNode;
    std::map<Int64, ObjectProxy*> m_objects;

    FbxImportDefinition *m_def;

    Vector3 m_upAxis;
    Vector3 m_frontAxis;
    Float m_unitScale;

    Int64 m_startTime;   //!< unit of 1/46186158000 seconds
    Int64 m_endTime;
};

} // namespace fbxi
} // namespace studio
} // namespace o3d

#endif // _O3DS_FBXI_LOADER_H

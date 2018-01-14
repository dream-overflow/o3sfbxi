/**
 * @brief FBX parser object
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-01-14
 * @details
 */

#ifndef _O3DS_FBXI_PARSER_H
#define _O3DS_FBXI_PARSER_H

#include <o3d/core/instream.h>

namespace o3d {
namespace studio {
namespace fbxi {

class FBXNode;

class Parser
{
public:

    Parser(InStream *stream);
    virtual ~Parser();

    o3d::Bool parse();

private:

    InStream &m_stream;

    Bool parseBinary7300();
    Bool parseBinary7400();

    void parseNodeBinary7300(FBXNode *parent);
    void parseNodeBinary7400(FBXNode *parent);

    String readString();

    std::list<FBXNode*> m_nodes;
};

} // namespace fbxi
} // namespace studio
} // namespace o3d

#endif // _O3DS_FBXI_PARSER_H

/**
 * @brief FBX import parser object
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-01-14
 * @details
 */

#include "parser.h"
#include "fbxnode.h"

#include <o3d/core/debug.h>

using namespace o3d::studio::fbxi;

Parser::Parser(o3d::InStream *stream) :
    m_stream(*stream)
{
    O3D_ASSERT(stream != nullptr);
}

Parser::~Parser()
{
    for (FBXNode *node : m_nodes) {
        delete node;
    }
}

o3d::Bool Parser::parse()
{
    static const Char MAGIC[] = "Kaydara FBX Binary  \0";
    Char magic[21] = {0};

    m_stream.read(magic, 21);

    if (memcmp(MAGIC, magic, 21) != 0) {
        O3D_ERROR(E_InvalidFormat("Unrecognized MAGIC"));
    }

    UInt8 res1, res2;
    m_stream >> res1 >> res2;   // 0x1A or 0x00 mostly

    if (res1 != 0x1a || res2 != 0x00) {
        O3D_ERROR(E_InvalidFormat("Unknown FBX format"));
    }

    UInt32 version;
    m_stream >> version;

    Bool result = False;

    if (version == 7300) {
        result = parseBinary7300();
    } else if (version == 7400) {
        result = parseBinary7400();
    } else {
        O3D_ERROR(E_InvalidFormat(String("Unsupported FBX version ").arg(version)));
    }

    return result;
}

o3d::Bool Parser::parseBinary7300()
{
    // parse the root node (present in binary mode, not in ascii)
    parseNodeBinary7300(nullptr);

    return True;
}

o3d::Bool Parser::parseBinary7400()
{
    // parse the root node (present in binary mode, not in ascii)
    parseNodeBinary7400(nullptr);

    return True;
}

void Parser::parseNodeBinary7300(FBXNode *parent)
{
    // @todo
}

void Parser::parseNodeBinary7400(FBXNode *parent)
{
    UInt32 endOfs, numProps, propListLen;
    String name;

    m_stream >> endOfs
             >> numProps
             >> propListLen;

    name = readString();

    FBXNode *node = new FBXNode(name);
    m_nodes.push_back(node);

    if (parent) {
        parent->addChild(node);
    }

    // node->setProperty
    // FBXHeaderExtension

    for (UInt32 i = 0; i < numProps; ++i) {
        // @todo
    }

    // nested properties
    if (m_stream.getPosition() < endOfs) {
        // @todo

        // present when nested properties only
        UInt8 emptyBytes[13] = {0};
        m_stream.read(emptyBytes, 13);
    }
}

o3d::String Parser::readString()
{
    UInt8 len = 0;
    Char data[256];
    m_stream >> len;

    m_stream.read(data, len);
    data[len] = 0;

    return data;
}

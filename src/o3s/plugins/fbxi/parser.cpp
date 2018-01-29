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

#include "property/propertybool.h"
#include "property/propertyint16.h"
#include "property/propertyint32.h"
#include "property/propertyint64.h"
#include "property/propertyfloat32.h"
#include "property/propertyfloat64.h"
#include "property/propertystring.h"
#include "property/propertyrawbytearray.h"
#include "property/propertyint32array.h"
#include "property/propertyint64array.h"
#include "property/propertyfloat32array.h"
#include "property/propertyfloat64array.h"
#include "property/propertyboolarray.h"

#include <zlib.h>

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
    } else if (version >= 7400) {
        result = parseBinary7400();
    } else {
        O3D_ERROR(E_InvalidFormat(String("Unsupported FBX version ").arg(version)));
    }

    return result;
}

FBXNode *Parser::child(const o3d::String &name)
{
    for (FBXNode *node : m_nodes) {
        if (node->name() == name) {
            return node;
        }
    }

    return nullptr;
}

const FBXNode *Parser::child(const o3d::String &name) const
{
    for (const FBXNode *node : m_nodes) {
        if (node->name() == name) {
            return node;
        }
    }

    return nullptr;
}

o3d::Bool Parser::parseBinary7300()
{
    // parse the root node (present in binary mode, not in ascii)
    while (!m_stream.isEnd()) {
        if (!parseNodeBinary7300(nullptr)) {
            break;
        }
    }

    return True;
}

o3d::Bool Parser::parseBinary7400()
{
    // parse the root node (present in binary mode, not in ascii)
    while (!m_stream.isEnd()) {
        if (!parseNodeBinary7400(nullptr)) {
            break;
        }
    }

    return True;
}

o3d::Bool Parser::parseNodeBinary7300(FBXNode *parent)
{
    // @todo if necessary
    return False;
}

o3d::Bool Parser::parseNodeBinary7400(FBXNode *parent)
{
    UInt32 endOfs, numProps, propListLen;
    String name;

    m_stream >> endOfs
             >> numProps
             >> propListLen;

    name = readString();

    // end of document reached
    if (!endOfs && !numProps && !propListLen) {
        return False;
    }

    FBXNode *node = new FBXNode(name);
    if (parent) {
        parent->addChild(node);
        System::print(name, String("subnode ") + parent->name());
    } else {
        m_nodes.push_back(node);
        System::print(name, "new node");
    }

    Int8 propType;
    Int8 b;
    Int16 si16;
    Int32 si32;
    Int64 si64;
    Float float32;
    Double float64;

    for (UInt32 i = 0; i < numProps; ++i) {
        m_stream >> propType;

        switch (propType) {
            case 'Y':
                m_stream >> si16;
                node->addProperty(new PropertyInt16(si16));
                break;
            case 'C':
                m_stream >> b;
                node->addProperty(new PropertyBool(b > 0));
                break;
            case 'I':
                m_stream >> si32;
                node->addProperty(new PropertyInt32(si32));
                break;
            case 'F':
                m_stream >> float32;
                node->addProperty(new PropertyFloat32(float32));
                break;
            case 'D':
                m_stream >> float64;
                node->addProperty(new PropertyFloat64(float64));
                break;
            case 'L':
                m_stream >> si64;
                node->addProperty(new PropertyInt64(si64));
                break;

            case 'f':
            {
                SmartArrayFloat arrF = readFloatArray();
                node->addProperty(new PropertyFloat32Array(arrF));
            }
                break;
            case 'd':
            {
                SmartArrayDouble arrD = readDoubleArray();
                node->addProperty(new PropertyFloat64Array(arrD));
            }
                break;
            case 'l':
            {
                SmartArrayInt64 arrI64 = readInt64Array();
                node->addProperty(new PropertyInt64Array(arrI64));
            }
                break;
            case 'i':
            {
                SmartArrayInt32 arrI32 = readInt32Array();
                node->addProperty(new PropertyInt32Array(arrI32));
            }
                break;
            case 'b':
            {
                SmartArrayUInt8 arrB = readBoolArray();
                node->addProperty(new PropertyBoolArray(arrB));
            }
                break;

            case 'S':
            {
                String str = readStringProp();
                node->addProperty(new PropertyString(str));
            }
                break;
            case 'R':
            {
                SmartArrayUInt8 raw = readUInt8RawProp();
                node->addProperty(new PropertyRawByteArray(raw));
            }
                break;

            default:
                O3D_ERROR(E_InvalidFormat("Unknown property type"));
        }
    }

    // nested properties
    if (m_stream.getPosition() < endOfs) {
        while (m_stream.getPosition() + 13 < endOfs) {
            parseNodeBinary7400(node);
        }

        // node terminating 13 null bytes
        UInt8 emptyBytes[13] = {0};
        m_stream.read(emptyBytes, 13);
    }

    return True;
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

template<class T>
void deflateArray(o3d::SmartArrayUInt8 &data , o3d::SmartArray<T> &out)
{
    // Decompression en utilisant Zlib
    z_stream stream;
    o3d::Int32 err;

    stream.next_in = (Bytef*)data.getData();
    stream.avail_in = (uInt)data.getSizeInBytes();
    stream.next_out = (Bytef*)out.getData();
    stream.avail_out = (uInt)out.getSizeInBytes();
    stream.zalloc = (alloc_func)nullptr;
    stream.zfree = (free_func)nullptr;
    stream.opaque = nullptr;

    // deflate data. wbits < 0 if no header data
    // err = inflateInit2(&stream,-MAX_WBITS);
    err = inflateInit(&stream);
    if (err == Z_OK) {
        err = inflate(&stream,Z_FINISH);
        inflateEnd(&stream);
        if (err == Z_STREAM_END) {
            err = Z_OK;
        }

        err = Z_OK;
        inflateEnd(&stream);
    }
}

o3d::SmartArrayFloat Parser::readFloatArray()
{
    UInt32 arrayLen;
    UInt32 encoding;
    UInt32 compressedLength;

    m_stream >> arrayLen
             >> encoding
             >> compressedLength;

    if (encoding == 0) {
        SmartArrayFloat arrF(arrayLen);
        m_stream.read(arrF.getData(), arrayLen);

        return arrF;
    } else if (encoding == 1) {
        SmartArrayUInt8 compressedData(compressedLength);
        m_stream.read(compressedData.getData(), compressedLength);

        SmartArrayFloat arrF(arrayLen);
        deflateArray(compressedData, arrF);

        return arrF;
    } else {
        O3D_ERROR(E_InvalidFormat("Unknown array encoding format"));
    }
}

o3d::SmartArrayDouble Parser::readDoubleArray()
{
    UInt32 arrayLen;
    UInt32 encoding;
    UInt32 compressedLength;

    m_stream >> arrayLen
             >> encoding
             >> compressedLength;

    if (encoding == 0) {
        SmartArrayDouble arrD(arrayLen);
        m_stream.read(arrD.getData(), arrayLen);

        return arrD;
    } else if (encoding == 1) {
        SmartArrayUInt8 compressedData(compressedLength);
        m_stream.read(compressedData.getData(), compressedLength);

        SmartArrayDouble arrD(arrayLen);
        deflateArray(compressedData, arrD);

        return arrD;
    } else {
        O3D_ERROR(E_InvalidFormat("Unknown array encoding format"));
    }
}

o3d::SmartArrayInt32 Parser::readInt32Array()
{
    UInt32 arrayLen;
    UInt32 encoding;
    UInt32 compressedLength;

    m_stream >> arrayLen
             >> encoding
             >> compressedLength;

    if (encoding == 0) {
        SmartArrayInt32 arrI32(arrayLen);
        m_stream.read(arrI32.getData(), arrayLen);

        return arrI32;
    } else if (encoding == 1) {
        SmartArrayUInt8 compressedData(compressedLength);
        m_stream.read(compressedData.getData(), compressedLength);

        SmartArrayInt32 arrI32(arrayLen);
        deflateArray(compressedData, arrI32);

        return arrI32;
    } else {
        O3D_ERROR(E_InvalidFormat("Unknown array encoding format"));
    }
}

o3d::SmartArrayInt64 Parser::readInt64Array()
{
    UInt32 arrayLen;
    UInt32 encoding;
    UInt32 compressedLength;

    m_stream >> arrayLen
             >> encoding
             >> compressedLength;

    if (encoding == 0) {
        SmartArrayInt64 arrI64(arrayLen);
        m_stream.read(arrI64.getData(), arrayLen);

        return arrI64;
    } else if (encoding == 1) {
        SmartArrayUInt8 compressedData(compressedLength);
        m_stream.read(compressedData.getData(), compressedLength);

        SmartArrayInt64 arrI64(arrayLen);
        deflateArray(compressedData, arrI64);

        return arrI64;
    } else {
        O3D_ERROR(E_InvalidFormat("Unknown array encoding format"));
    }
}

o3d::SmartArrayUInt8 Parser::readBoolArray()
{
    UInt32 arrayLen;
    UInt32 encoding;
    UInt32 compressedLength;

    m_stream >> arrayLen
             >> encoding
             >> compressedLength;

    if (encoding == 0) {
        SmartArrayUInt8 arrB(arrayLen);
        m_stream.read(arrB.getData(), arrayLen);

        return arrB;
    } else if (encoding == 1) {
        SmartArrayUInt8 compressedData(compressedLength);
        m_stream.read(compressedData.getData(), compressedLength);

        SmartArrayUInt8 arrB(arrayLen);
        deflateArray(compressedData, arrB);

        return arrB;
    } else {
        O3D_ERROR(E_InvalidFormat("Unknown array encoding format"));
    }
}

o3d::String Parser::readStringProp()
{
    // same format
    UInt32 strLen;
    m_stream >> strLen;

    if (strLen > 0) {
        SmartArrayChar cstr(strLen+1);
        m_stream.read(cstr.getData(), strLen);
        cstr[strLen] = 0;

        String str(cstr.getData(), cstr.getSize());
        return str;
    } else {
        return String();
    }
}

o3d::SmartArrayUInt8 Parser::readUInt8RawProp()
{
    // same format
    SmartArrayUInt8 raw;
    m_stream >> raw;

    return raw;
}

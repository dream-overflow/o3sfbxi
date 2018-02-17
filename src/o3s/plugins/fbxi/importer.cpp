/**
 * @brief FBX importer controller object
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-01-14
 * @details
 */

#include "importer.h"
#include "parser.h"
#include "o3sadapter.h"

#include <o3d/core/filemanager.h>

using namespace o3d::studio::fbxi;


Importer::Importer() :
    common::Importer()
{
    m_name = "o3s::plugin::importer::fbxi";
}

Importer::~Importer()
{

}

o3d::studio::common::ImporterOption *Importer::buildOptions()
{
    return nullptr;
}

o3d::studio::common::ImportDefinition *Importer::introspect(const o3d::String &filename)
{
    InStream *inStream = o3d::FileManager::instance()->openInStream(filename);
    FbxImportDefinition *def = new FbxImportDefinition(File(filename).getFilePath());

    Parser *parser = new Parser(inStream);
    Bool result = parser->parse();

    // keep global structure except data
    // @todo

    delete parser;
    delete inStream;

    if (result) {
        return def;
    } else {
        delete def;
        return nullptr;
    }
}

o3d::studio::common::ImportDefinition *Importer::import(const o3d::String &filename,
        common::ImporterOption *options,
        common::Hub *parent)
{
    InStream *inStream = o3d::FileManager::instance()->openInStream(filename);
    FbxImportDefinition *def = new FbxImportDefinition(File(filename).getFilePath());

    Parser *parser = new Parser(inStream);
    Bool result = parser->parse();

    if (result) {
        // instanciate into parent entity
        O3SAdapter *adapter = new O3SAdapter(parser, options, parent, def);
        result = adapter->processImport();

        delete adapter;
    }

    delete parser;
    delete inStream;   // keep until the end if lazy mode

    if (result) {
        return def;
    } else {
        delete def;
        return nullptr;
    }
}

FbxImportDefinition::FbxImportDefinition(const o3d::String &basePath) :
    m_basePath(basePath)
{

}

FbxImportDefinition::~FbxImportDefinition()
{

}

o3d::String FbxImportDefinition::basePath() const
{
    return m_basePath;
}

o3d::String FbxImportDefinition::creator() const
{
    return m_creator;
}

o3d::DateTime FbxImportDefinition::creationDateTime() const
{
    return m_creationTimestamp;
}

o3d::Float FbxImportDefinition::unit() const
{
    return m_unit;
}

o3d::Color FbxImportDefinition::ambientColor() const
{
    return m_ambientColor;
}

o3d::UInt32 FbxImportDefinition::numModel() const
{
    return m_numModel;
}

o3d::UInt32 FbxImportDefinition::numGeometry() const
{
    return m_numGeometry;
}

o3d::UInt32 FbxImportDefinition::numMaterial() const
{
    return m_numMaterial;
}

o3d::UInt32 FbxImportDefinition::numCamera() const
{
    return m_numCamera;
}

o3d::UInt32 FbxImportDefinition::numLight() const
{
    return m_numLight;
}

/**
 * @brief FBX importer controller object
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-01-14
 * @details
 */

#include "importer.h"
#include "parser.h"

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

o3d::Bool Importer::introspect(const o3d::String &filename)
{
    InStream *inStream = o3d::FileManager::instance()->openInStream(filename);

    Parser *parser = new Parser(inStream);
    Bool result = parser->parse();

    // keep global structure except data
    // @todo

    delete parser;
    delete inStream;

    return result;
}

o3d::Bool Importer::import(const o3d::String &filename, o3d::studio::common::ImporterOption *options, o3d::studio::common::Entity *parent)
{
    InStream *inStream = o3d::FileManager::instance()->openInStream(filename);

    Parser *parser = new Parser(inStream);
    Bool result = parser->parse();

    // instanciate into parent entity
    // @todo

    delete parser;
    delete inStream;

    return result;
}

/**
 * @brief FBX importer adapter to O3S implementation
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-01-20
 * @details
 */

#include "o3sadapter.h"
#include "proxy/headerproxy.h"
#include "proxy/definitionproxy.h"
#include "proxy/objectsproxy.h"
#include "proxy/meshproxy.h"
#include "proxy/relationsproxy.h"
#include "proxy/globalsettingsproxy.h"
// #include "proxy/takesproxy.h"
// #include "proxy/takeproxy.h"

#include <o3d/studio/common/objectref.h>
#include <o3d/studio/common/component/dummyhub.h>

using namespace o3d::studio::fbxi;

O3SAdapter::O3SAdapter(Parser *parser,
        o3d::studio::common::ImporterOption *options,
        o3d::studio::common::Entity *parent,
        FbxImportDefinition* def) :
    m_parser(parser),
    m_options(options),
    m_parent(parent),
    m_def(def),
    m_upAxis(0, 1, 0),
    m_frontAxis(0, 0, 1),
    m_unitScale(1.0)
{
    O3D_ASSERT(parser);
    O3D_ASSERT(options);
    O3D_ASSERT(parent);
    O3D_ASSERT(def);
}

O3SAdapter::~O3SAdapter()
{

}

o3d::Bool O3SAdapter::processImport()
{
    common::Hub *topLevelHub = nullptr;

    // read interesting definitions
    setupDef();

    if (m_parent->ref().light().baseTypeOf(common::TypeRef::asset())) {
        common::Asset *asset = static_cast<common::Asset*>(m_parent);
        setupAsset(asset);

        topLevelHub = new common::DummyHub("Asset root hub", m_parent);
    } else if (m_parent->ref().light().baseTypeOf(common::TypeRef::hub())) {
        topLevelHub = static_cast<common::Hub*>(m_parent);
    }

    if (topLevelHub) {
        setupHub(topLevelHub);
    } else {
        return False;
    }

    return True;
}

o3d::Bool O3SAdapter::processImportLazy()
{
    // @todo
    return False;
}

void O3SAdapter::setupDef()
{
    FBXNode *node = m_parser->child("FBXHeaderExtension");
    if (node) {
        HeaderProxy *hp = new HeaderProxy(node);
        m_def->m_creator = hp->creator();
        m_def->m_creationTimestamp = hp->creationTimeStamp();

        delete hp;
    }

    node = m_parser->child("GlobalSettings");
    if (node) {
        GlobalSettingsProxy *gs = new GlobalSettingsProxy(node);
        m_unitScale = gs->unitScale();
        m_upAxis = gs->upAxis();
        m_frontAxis = gs->frontAxis();

        m_def->m_ambientColor = gs->ambientColor();

        delete gs;
    }


    node = m_parser->child("Objects");
    if (node) {
        ObjectsProxy *objects = new ObjectsProxy(node);
        // @todo

        delete objects;
    }

    m_def->m_unit = m_unitScale;
}

void O3SAdapter::setupAsset(common::Asset* asset)
{
    FBXNode *node = m_parser->child("FBXHeaderExtension");
    if (node) {
        HeaderProxy *hp = new HeaderProxy(node);

        asset->setCreator(hp->creator());
        asset->setCreationTimeStamp(hp->creationTimeStamp());

        delete hp;
    }
}

void O3SAdapter::setupHub(common::Hub* rootHub)
{
    // @todo all work here
}

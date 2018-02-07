/**
 * @brief FBX importer data loader from parser.
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-01-20
 * @details
 */

#include "loader.h"

#include "proxy/headerproxy.h"
#include "proxy/definitionsproxy.h"
#include "proxy/objectsproxy.h"
#include "proxy/modelproxy.h"
#include "proxy/relationsproxy.h"
#include "proxy/globalsettingsproxy.h"
#include "proxy/connectionsproxy.h"
// #include "proxy/takesproxy.h"
// #include "proxy/takeproxy.h"

#include "proxy/modelproxy.h"
#include "proxy/clusterproxy.h"
#include "proxy/materialproxy.h"
#include "proxy/textureproxy.h"
#include "proxy/cameraproxy.h"
#include "proxy/lightproxy.h"
#include "proxy/textureproxy.h"
#include "proxy/nodeattributeproxy.h"
#include "proxy/geometryproxy.h"
#include "proxy/boneproxy.h"
#include "proxy/meshproxy.h"

#include "property/propertyint32.h"
#include "property/propertyint64.h"
#include "property/propertystring.h"

using namespace o3d::studio::fbxi;

Loader::Loader(Parser *parser, FbxImportDefinition *def) :
    m_parser(parser),
    m_rootNode(nullptr),
    m_def(def),
    m_upAxis(0, 1, 0),
    m_frontAxis(0, 0, 1),
    m_unitScale(1.0)
{
    O3D_ASSERT(parser);
    O3D_ASSERT(def);
}

Loader::~Loader()
{
    // delete object proxies including the root proxy
    for (auto it = m_objects.begin(); it != m_objects.end(); ++it) {
        delete it->second;
    }

    // and the fake root node (manually created)
    deletePtr(m_rootNode);
}

ObjectProxy *Loader::objectProxy(o3d::Int64 uid)
{
    auto it = m_objects.find(uid);
    if (it != m_objects.end()) {
        return it->second;
    }

    return nullptr;
}

void Loader::load()
{
    // global informations about this document
    FBXNode *node = m_parser->child("FBXHeaderExtension");
    if (node) {
        HeaderProxy *hp = new HeaderProxy(node);
        m_def->m_creator = hp->creator();
        m_def->m_creationTimestamp = hp->creationTimeStamp();

        delete hp;
    }

    // global settings of the data format (for transform, and details)
    node = m_parser->child("GlobalSettings");
    if (node) {
        GlobalSettingsProxy *gs = new GlobalSettingsProxy(node);
        m_unitScale = gs->unitScale();
        m_upAxis = gs->upAxis();
        m_frontAxis = gs->frontAxis();
        m_startTime = gs->timeSpanStart();
        m_endTime = gs->timeSpanEnd();

        m_def->m_ambientColor = gs->ambientColor();

        delete gs;
    }

    // definitions of the templates
    node = m_parser->child("Definitions");
    if (node) {
        DefinitionsProxy *definitions = new DefinitionsProxy(node);

        // @todo TemplateProxy ... apply template to objects after ...
        Int32 count = definitions->count();

        delete definitions;
    }

    node = m_parser->child("Objects");
    if (node) {
        ObjectsProxy *objects = new ObjectsProxy(node);

        for (UInt32 i = 0; i < objects->numObjects(); ++i) {
            switch (objects->objectType(i)) {
                case ObjectProxy::OBJECT_CAMERA_MODEL:
                {
                    CameraModelProxy *cp = objects->cameraModel(i);
                    m_objects[cp->uid()] = cp;
                }
                    break;
                case ObjectProxy::OBJECT_CAMERA_NODE_ATTR:
                {
                    CameraNodeProxy *cp = objects->cameraNode(i);
                    m_objects[cp->uid()] = cp;
                }
                    break;
                case ObjectProxy::OBJECT_GEOMETRY:
                {
                    GeometryProxy *gp = objects->geometry(i);
                    m_objects[gp->uid()] = gp;

                    gp->processGeometry();
                }
                    break;
                case ObjectProxy::OBJECT_LIGHT_NODE_ATTR:
                {
                    LightNodeProxy *lp = objects->lightNode(i);
                    m_objects[lp->uid()] = lp;

                    // @todo params
                }
                    break;
                case ObjectProxy::OBJECT_MATERIAL:
                {
                    MaterialProxy *mp = objects->material(i);
                    m_objects[mp->uid()] = mp;

                    // @todo color
                }
                    break;
                case ObjectProxy::OBJECT_MODEL:
                {
                    // unspecialized are "Null" and "Root" or unsupported
                    ModelProxy *mp = objects->model(i);
                    m_objects[mp->uid()] = mp;
                }
                    break;
                case ObjectProxy::OBJECT_NODE_ATTRIBUTE:
                {
                    // unspecialized are unsupported
                    NodeAttributeProxy *np = objects->nodeAttribute(i);
                    m_objects[np->uid()] = np;
                }
                    break;
                case ObjectProxy::OBJECT_LIMB_NODE_MODEL:
                {
                    BoneModelProxy *lp = objects->boneModel(i);
                    m_objects[lp->uid()] = lp;
                }
                    break;
                case ObjectProxy::OBJECT_MESH_MODEL:
                {
                    MeshModelProxy *mp = objects->meshModel(i);
                    m_objects[mp->uid()] = mp;
                }
                    break;
                case ObjectProxy::OBJECT_TEXTURE:
                {
                    TextureProxy *tp = objects->texture(i);
                    m_objects[tp->uid()] = tp;
                }
                    break;
                default:
                    break;
            }
        }

        delete objects;
    }

    // add a root proxy at uid 0
    FBXNode *m_rootNode = new FBXNode("Model");
    m_rootNode->addProperty(new PropertyInt64(0));
    m_rootNode->addProperty(new PropertyString("Root::Root"));
    m_rootNode->addProperty(new PropertyString("Root"));

    FBXNode *rootNodeVersion = new FBXNode("Version");
    rootNodeVersion->addProperty(new PropertyInt32(232));
    m_rootNode->addChild(rootNodeVersion);

    ModelProxy *rootProxy = new ModelProxy(m_rootNode);
    m_objects[0] = rootProxy;

    m_def->m_unit = m_unitScale;

    node = m_parser->child("Connections");
    if (node) {
        ConnectionsProxy *connections = new ConnectionsProxy(node);

        // setup connections between objects
        for (UInt32 i = 0; i < connections->numConnections(); ++i) {
            if (connections->connectionType(i) == ConnectionsProxy::CONN_OO) {
                Int64 parentUid, childUid;
                connections->objectRelation(i, parentUid, childUid);

                ObjectProxy* parentProxy = objectProxy(parentUid);
                ObjectProxy* childProxy = objectProxy(childUid);

                // parent proxy not found
                if (!parentProxy) {
                    continue;
                }

                // not imported children ignore the connection
                if (!childProxy) {
                    continue;
                }

                ObjectProxy::ObjectType parentType = parentProxy->objectType();
                ObjectProxy::ObjectType childType = childProxy->objectType();

                // @todo subclasses
                if (childType == ObjectProxy::OBJECT_NODE_ATTRIBUTE) {
                    // must be assigned to only one parent model
                    if (parentProxy->nodeAttribute() != nullptr) {
                        O3D_ERROR(E_InvalidPrecondition("A node attribute is already assigned to this parent"));
                    } else {
                        parentProxy->setNodeAttribute(static_cast<NodeAttributeProxy*>(childProxy));
                    }
                } else if (childType == ObjectProxy::OBJECT_MODEL ||
                           childType == ObjectProxy::OBJECT_MESH_MODEL ||
                           childType == ObjectProxy::OBJECT_CAMERA_MODEL ||
                           childType == ObjectProxy::OBJECT_LIGHT_MODEL) {
                    // parent the child to root or another parent node
                    childProxy->setParent(parentProxy);
                    parentProxy->addChild(childProxy);
                } else if (childType == ObjectProxy::OBJECT_LIMB_NODE_MODEL) {
                    if (parentType == ObjectProxy::OBJECT_LIMB_NODE_MODEL) {
                        // parent the child if the parent is itself a limb node
                        childProxy->setParent(parentProxy);
                        parentProxy->addChild(childProxy);
                    }
                }

                if (parentType == ObjectProxy::OBJECT_MESH_MODEL) {
                    if (childType == ObjectProxy::OBJECT_GEOMETRY) {
                        GeometryProxy* geometryProxy = static_cast<GeometryProxy*>(parentProxy);
                        MeshModelProxy* meshProxy = static_cast<MeshModelProxy*>(parentProxy);

                        if (meshProxy->geometry() != nullptr) {
                            O3D_ERROR(E_InvalidPrecondition("Geometry is already assigned to this mesh"));
                        } else {
                            meshProxy->setGeometry(geometryProxy);
                        }
                    } else if (childType == ObjectProxy::OBJECT_MATERIAL) {
                        MaterialProxy* materialProxy = static_cast<MaterialProxy*>(materialProxy);
                        MeshModelProxy* meshProxy = static_cast<MeshModelProxy*>(parentProxy);

                        meshProxy->addMaterial(materialProxy);
                    }
                } else if (parentType == ObjectProxy::OBJECT_SKIN) {
                    if (childType == ObjectProxy::OBJECT_CLUSTER) {
                    // @todo SkinProxy
//                        SkinProxy* skinProxy = static_cast<SkinProxy*>(parentProxy);
//                        ClusterProxy* clusterProxy = static_cast<ClusterProxy*>(childProxy);

//                        skinProxy.addCluster(clusterProxy);

//                        if (clusterProxy->skin()) {
//                            O3D_ERROR(E_InvalidPrecondition("Cluster is already assigned to a skin"));
//                        } else {
//                            clusterProxy->setSkin(skinProxy);
//                        }
                    }
                } else if (parentType == ObjectProxy::OBJECT_GEOMETRY) {
                    if (childType == ObjectProxy::OBJECT_SKIN) {
//                        GeometryProxy* geometryProxy = static_cast<GeometryProxy*>(parentProxy);
//                        SkinProxy* skinProxy = static_cast<SkinProxy*>(childType);

//                        geometryProxy->setSkin(skinProxy);
                    }
                } else if (parentType == ObjectProxy::OBJECT_CLUSTER) {
                    if (childType == ObjectProxy::OBJECT_LIMB_NODE_MODEL ||
                        childType == ObjectProxy::OBJECT_MESH_MODEL ||
                        childType == ObjectProxy::OBJECT_NULL_NODE) {

                        ClusterProxy* clusterProxy = static_cast<ClusterProxy*>(parentProxy);
                        if (clusterProxy->link()) {
                            O3D_ERROR(E_InvalidPrecondition("Link is already assigned to this cluster"));
                        } else {
                            clusterProxy->setLink(childProxy);
                        }
                    }
                } else if (parentType == ObjectProxy::OBJECT_ANIMATION_LAYER) {
                    if (childType == ObjectProxy::OBJECT_ANIMATION_CURVE_NODE) {
                        // @todo
                    }
                } else if (parentType == ObjectProxy::OBJECT_ANIMATION_CURVE_NODE) {
                    if (childType == ObjectProxy::OBJECT_ANIMATION_CURVE) {
                        // @todo
                    }
                }
            } else if (connections->connectionType(i) == ConnectionsProxy::CONN_OP) {
                Int64 parentUid, childUid;
                String propertyName = connections->propertyRelation(i, parentUid, childUid);

                ObjectProxy* parentProxy = objectProxy(parentUid);
                ObjectProxy* childProxy = objectProxy(childUid);

                // parent proxy not found
                if (parentUid != 0 && !parentProxy) {
                    continue;
                }

                // not imported children ignore the connection
                if (!childProxy) {
                    continue;
                }

                ObjectProxy::ObjectType childType = childProxy->objectType();
                if (childType == ObjectProxy::OBJECT_ANIMATION_CURVE_NODE) {
                    // @todo the parent is a reference on the bone
                    // and the property is linked to this reference
                }

                ObjectProxy::ObjectType parentType = parentProxy->objectType();
                if (parentType == ObjectProxy::OBJECT_MATERIAL) {
                    MaterialProxy *materialProxy = static_cast<MaterialProxy*>(parentProxy);
                    TextureProxy *textureProxy = static_cast<TextureProxy*>(childProxy);

                    if (childType == ObjectProxy::OBJECT_TEXTURE) {
                        if (propertyName == "AmbientMap") {
                            materialProxy->setTexture(MaterialProxy::MAP_AMBIENT, textureProxy);
                        } else if (propertyName == "DiffuseColor") {
                            materialProxy->setTexture(MaterialProxy::MAP_DIFFUSE, textureProxy);
                        } else if (propertyName == "EmissiveColor") {
                            materialProxy->setTexture(MaterialProxy::MAP_EMISSIVE, textureProxy);
                        } else if (propertyName == "SpecularMap") {
                            materialProxy->setTexture(MaterialProxy::MAP_SPECULAR, textureProxy);
                        } else if (propertyName == "NormalColor") {
                            materialProxy->setTexture(MaterialProxy::MAP_NORMAL, textureProxy);
                        }
                    }
                }
            }
        }

        delete connections;
    }

    // processGeometry for mesh


    // post process each OBJECT_CLUSTER
    for (auto it = m_objects.begin(); it != m_objects.end(); ++it) {
        ObjectProxy *object = it->second;
        if (object->objectType() == ObjectProxy::OBJECT_CLUSTER) {
            //static_cast<ClusterProxy*>(object)->postProcess();
            // @todo
        }
    }
}

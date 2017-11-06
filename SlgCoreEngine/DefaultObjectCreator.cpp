#include "DefaultObjectCreator.h"

#include "NameManager.h"
#include "RenderSceneManager.h"

#include "IMesh.h"
#include "CubeMesh.h"
#include "CloudMesh.h"

#include "Vertex.h"
#include "Material.h"

using namespace Slg3DScanner;

IMeshRef DefaultObjectCreator::createDefaultCubeMesh()
{
    CubeMeshInitializer meshInitializer;
    meshInitializer.name = NameManager::instance().createUniqueName("Cube");
    
    IMeshRef cube = RenderSceneManager::instance().createMeshOfType<CubeMesh>(meshInitializer);

    MaterialInitializer materialInitializer;
    materialInitializer.m_VSFileName = L"Shader/MiniPhong.hlsl";
    materialInitializer.m_PSFileName = materialInitializer.m_VSFileName;

    materialInitializer.m_VSFuncName = "mainVS";
    materialInitializer.m_PSFuncName = "mainPS";

    materialInitializer.m_Desc = Slg3DScanner::SimpleVertex::s_Layout;
    materialInitializer.m_ElemCount = ARRAYSIZE(Slg3DScanner::SimpleVertex::s_Layout);

    cube->addMaterial(materialInitializer);

    return std::move(cube);
}

IMeshRef DefaultObjectCreator::createDefaultPointCloud(const std::string &pathFileName, int cloudVersion)
{
    CloudMeshInitializer meshInitializer;
    meshInitializer.name = NameManager::instance().createUniqueName("CloudMesh");
    meshInitializer.m_cloudFileName = pathFileName;
    meshInitializer.m_version = cloudVersion;

    IMeshRef cloud = RenderSceneManager::instance().createMeshOfType<CloudMesh>(meshInitializer);

    MaterialInitializer materialInitializer;
    materialInitializer.m_VSFileName = L"Shader/MiniPhong.hlsl";
    materialInitializer.m_PSFileName = materialInitializer.m_VSFileName;

    materialInitializer.m_VSFuncName = "mainVS";
    materialInitializer.m_PSFuncName = "mainPS";

    materialInitializer.m_Desc = Slg3DScanner::SimpleVertex::s_Layout;
    materialInitializer.m_ElemCount = ARRAYSIZE(Slg3DScanner::SimpleVertex::s_Layout);

    cloud->addMaterial(materialInitializer);

    return std::move(cloud);
}

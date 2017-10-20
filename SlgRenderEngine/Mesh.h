#pragma once

#include "IMesh.h"
#include "Material.h"

namespace Slg3DScanner
{
    struct MeshInitializer : public IMeshInitializer{};

    class Mesh : public IMesh
    {
    protected:
        std::vector<Material> m_materialArray;


    public:
        Mesh(const MeshInitializer& meshInitializer);
        virtual ~Mesh();

        virtual void addMaterial(const MaterialInitializer& materialInit) override;
        virtual void addMaterial(MaterialInitializer&& materialInit) override;
    };
}

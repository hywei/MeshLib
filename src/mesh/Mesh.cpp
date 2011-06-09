#include "Mesh.h"
#include "MeshKernel.h"
#include "MeshIO.h"
#include "MeshBasicOp.h"
#include "MeshInfo.h"

namespace MeshLib{
    
    Mesh::Mesh(){}
    Mesh::~Mesh(){}

    // Input/Output functions
    bool Mesh::AttachModel(const std::string& filename)
    {

        p_Kernel.reset(); p_IO.reset(); p_BasicOP.reset();
    
        p_Kernel = boost::shared_ptr<MeshKernel> (new MeshKernel(*this));
        p_IO = boost::shared_ptr<MeshIO> (new MeshIO(*this));
        p_BasicOP = boost::shared_ptr<MeshBasicOP> (new MeshBasicOP(*this));
        p_ModelInfo = boost::shared_ptr<ModelInfo> (new ModelInfo(*this));
    
        if(!p_IO->LoadModel(filename))
            return false;

        p_BasicOP->InitModel();
        return true;
    }

    bool Mesh::StoreModel(const std::string& filename) const
    {
        return p_IO->StoreModel(filename);
    }

    size_t Mesh::GetVertexNumber() const {  return p_ModelInfo->GetVertNum() ;}
    
    size_t Mesh::GetFaceNumber() const{ return p_ModelInfo->GetFaceNum(); }
    
    const Coord3D& Mesh::GetVertexCoord(VertHandle vh) const
    {
        return p_Kernel->GetVertArray()[vh].coord;
    }
    
    const Coord3D& Mesh::GetVertexNorm(VertHandle vh) const
    {
        return p_Kernel->GetVertArray()[vh].normal;
    }
    
    const Coord3D& Mesh::GetFaceNorm(FaceHandle fh) const
    {
        return p_Kernel->GetFaceArray()[fh].normal;
    }
    
    std::vector<VertHandle> Mesh::GetAdjVertics(VertHandle vh) const
    {
        return p_BasicOP->GetAdjVertArray(vh);
    }
    
    std::vector<FaceHandle> Mesh::GetAdjFaces(VertHandle vh) const
    {
        return p_BasicOP->GetAdjFaceArray(vh);
    }

    std::vector<VertHandle> Mesh::GetFaceVertics(FaceHandle fh) const
    {
        return p_Kernel->GetFaceArray()[fh].vert_handle_vec;
    }

}

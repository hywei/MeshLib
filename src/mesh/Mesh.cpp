#include "Mesh.h"
#include "MeshKernel.h"
#include "MeshIO.h"
#include "MeshBasicOP.h"

namespace MeshLib{
    
    Mesh::Mesh(){}
    Mesh::~Mesh(){}

    // Input/Output functions
    bool Mesh::AttachModel(const std::string& filename)
    {

        p_Kernel->reset(); p_IO->reset(); p_BasicOP->reset();
    
        p_Kernel = boost::shared_ptr<MeshKernel> (new MeshKernel(*this));
        p_IO = boost::shared_ptr<MeshIO> (new MeshIO(*this));
        p_BasicOP = boost::shared_ptr<MeshBasicOP> (new MeshBasicOP(*this));
    
        if(!p_IO->LoadModel(filename))
            return false;

        p_BasicOP->InitModel();
        return true;
    }

    bool Mesh::StoreModel(const std::string& filename) const
    {
        return p_IO->StoreModel(filename);
    }

}

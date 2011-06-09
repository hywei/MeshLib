#ifndef MESHLIB_MESH_H_
#define MESHLIB_MESH_H_

#include <boost/shared_ptr.hpp>
#include <string>
#include "../common/types.h"
#include "MeshElement.h"

namespace MeshLib{

    class MeshKernel;
    class MeshIO;
    class MeshBasicOP;
    class ModelInfo;
    
    class Mesh
    {
    public:
        // Constructor/Destructor
        Mesh();
        ~Mesh();
        
        // Input/Output functions
        bool AttachModel(const std::string& filename);
        bool StoreModel(const std::string& filename) const;

        size_t GetVertexNumber() const;
        size_t GetFaceNumber() const;
        const Coord3D& GetVertexCoord(VertHandle vh) const;
        const Coord3D& GetVertexNorm(VertHandle vh) const;
        const Coord3D& GetFaceNorm(FaceHandle fh) const;
        std::vector<VertHandle> GetAdjVertics(VertHandle vh) const;
        std::vector<FaceHandle> GetAdjFaces(VertHandle vh) const;
        std::vector<VertHandle> GetFaceVertics(FaceHandle fh) const;

        bool IsBoundaryVertex(VertHandle vh);
        bool IsBoundaryFace(FaceHandle fh);
        bool IsBoundaryEdge(EdgeHandle eh);        
        
    private:            
        boost::shared_ptr<MeshKernel> p_Kernel;
        boost::shared_ptr<MeshIO> p_IO;
        boost::shared_ptr<MeshBasicOP> p_BasicOP;
        boost::shared_ptr<ModelInfo> p_ModelInfo;

        friend class MeshKernel;
        friend class MeshIO;
        friend class MeshBasicOP;
        friend class ModelInfo;
    };


} // 
#endif

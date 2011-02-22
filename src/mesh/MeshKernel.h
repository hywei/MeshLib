#ifndef MESHLIB_MESHKERNEL_H_
#define MESHLIB_MESHKERNEL_H_

#include "MeshItem.h"
#include "MeshInfo.h"

namespace MeshLib{

    class Mesh;
    
    /* class MeshKernel */
    /* { */
    /* private: */
    /*     VertexInfo  m_VertexInfo; */
    /*     FaceInfo    m_FaceInfo; */
    /*     EdgeInfo    m_EdgeInfo; */
    /*     ModelInfo   m_ModelInfo; */

    /*     Mesh& m_mesh; */

    /* public: */
    /*     MeshKernel(Mesh& mesh) : m_mesh(mesh){} */
    /*     ~MeshKernel(); */
    
    /*     VertexInfo& GetVertexInfo() { return m_VertexInfo; } */
    /*     EdgeInfo&   GetEdgeInfo()   { return m_EdgeInfo; } */
    /*     FaceInfo&   GetFaceInfo()   { return m_FaceInfo; } */
    /*     ModelInfo&  GetModelInfo()  { return m_ModelInfo; } */
    /* }; */

    class MeshKernel
    {
    public:
        MeshKernel(Mesh& _mesh);
        ~MeshKernel();

        //! analysis model info
        void AnalysisModel();
        //! create halfedge data structure        
        bool CreateHalfEdgeDS();
        
    private:
        std::vector<Vert> vert_vec;
        std::vector<Face> face_vec;
        std::vector<Edge> edge_vec;
        std::vector<HalfEdge> he_vec;

        MeshInfo& mesh_info;
        
        Mesh& mesh;
    }
}
#endif

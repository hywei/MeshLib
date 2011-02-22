#ifndef MESHLIB_MESHBASICOP_H_
#defein MESHLIB_MESHBASICOP_H_

#include <string>
#include "../common/types"

namespace MeshLib{

    class Mesh;
    
    class MeshBasicOP
    {   
    public:
        MeshBasicOP(Mesh& mesh);
        ~MeshBasicOP();

        void InitModel(); // Analyzing and initializing the topological structure

    private:
        
        void CalAdjacentInfo();
      
        void CalVertexNormal(); // Calculate normal vector of all vertices      
        void CalFaceNormal();   // Calculate normal vector of all faces
        
        void CreateHalfEdge();  // Create halfedge, optional function
        
        void CalBoundingBox();      // Bounding box calculation
        void CalComponentInfo();    // Component calculation
        
        void TopologyAnalysis();
        
        double GetAvgEdgeLength();
        
    private:
        Mesh& m_mesh;
    };
}
#endif

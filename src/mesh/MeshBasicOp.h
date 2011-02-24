#ifndef MESHLIB_MESHBASICOP_H_
#defein MESHLIB_MESHBASICOP_H_

#include <string>
#include "../common/types"
#include "MeshElement.h"

namespace MeshLib{

    class Mesh;
    
    class MeshBasicOP
    {   
    public:
        MeshBasicOP(Mesh& mesh);
        ~MeshBasicOP();

        void InitModel();

        std::vector<VertHanlde> GetAdjVertArray(const VertHanlde&) const;
        std::vector<FaceHanlde> GetAdjFaceArray(const VertHanlde&) const;
        
    private:
        
        void CalAdjacentInfo();
      
        void CalVertNormal(); // Calculate normal vector of all vertices      
        void CalFaceNormal();   // Calculate normal vector of all faces
        void CalBoundingBox(Coord3D& box_min, Coord3D& box_max, Coord3D& box_dim) const;
        void CalBoundingSphere(Coord3D& sphere_center, double radius) const;

        size_t CountComponentNum() const; 
        double CalAvgEdgeLength() const;
        
    private:
        Mesh& m_mesh;
    };
}
#endif

#ifndef MESHLIB_MESHITEM_H_
#define MESHLIB_MESHITEM_H_

#include "../common/types.h"

namespace MeshLib{

    class VertexInfo
    {        
    public:
        VertexInfo(){}
        ~VertexInfo(){}
        
        CoordArray& GetCoord() { return m_Coord; }
        NormalArray& GetNormal() { return m_Normal; }
        ColorArray& GetColor() { return m_Color; }
        TexCoordArray& GetTexCoord() { return m_TexCoord; }

        int GetVertexNum() const { return m_nVertices; }

    private:
        CoordArray      m_Coord;    // Vertex coordinate array
        NormalArray     m_Normal;   // Vertex normal array
        ColorArray      m_Color;    // Vertex color array
        TexCoordArray   m_TexCoord; // Vertex texture coordinate array
    
        int m_nVertices;

        friend class MeshKernel;
        
    };

    class FaceInfo
    {
    public:
        FaceInfo(){}
        ~FaceInfo(){}
        
        PolyIndexArray& GetIndex() { return m_Index; }
        NormalArray& GetNormal() { return m_Normal; }
        ColorArray& GetColor() { return m_Color; }
        PolyIndexArray& GetTexIndex() { return m_TexIndex; }

        int GetFaceNum() const { return m_nFaces; }

    private:
        PolyIndexArray     m_Index;    // Face vertex-index array
        NormalArray        m_Normal;   // Face normal array
        ColorArray         m_Color;    // Face color array
        PolyIndexArray     m_TexIndex; // Face vertex-texture-index array
     
        int m_nFaces;

        friend class MeshKernel;
        
    };

    class EdgeInfo
    {
    
    public:
        EdgeInfo(){}
        ~EdgeInfo(){}
        
        PolyIndexArray& GetVertexIndex() { return m_VtxIndex; }
        ColorArray& GetColor() { return m_Color; }
        PolyIndexArray& GetFaceIndex() {return m_FaceIndex;}

    private:
        PolyIndexArray  m_VtxIndex;    // Edge vertex-index array
        ColorArray      m_Color;       // Edge color array
        PolyIndexArray  m_FaceIndex;   // Edge face-index array
        int m_nHalfEdges;

        friend class MeshKernel;
    };

}
#endif

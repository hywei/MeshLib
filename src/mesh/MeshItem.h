#ifndef MESHLIB_MESHITEM_H_
#define MESHLIB_MESHITEM_H_

#include "../common/types.h"

namespace MeshLib{

    class VertexInfo
    {
    public:
        enum VertFlag{
            ISOLATED = 0x00000100, // isolated vertex flag
            MANIFOLD = 0x00000200, // manifold vertex flag
            BOUNDARY = 0x00000300  // boundary vertex flag
        };
    public:
        VertexInfo(){}
        ~VertexInfo(){}
        
        CoordArray& GetCoord() { return m_Coord; }
        NormalArray& GetNormal() { return m_Normal; }
        ColorArray& GetColor() { return m_Color; }
        TexCoordArray& GetTexCoord() { return m_TexCoord; }

        PolyIndexArray& GetAdjFaces() { return m_AdjFaces; }
        PolyIndexArray& GetAdjVertices() { return m_AdjVerts; }
        PolyIndexArray& GetAdjEdges() { return m_AdjEdges; }

        
        int GetVertexNum() const { return m_nVertices; }

    private:
        CoordArray      m_Coord;    // Vertex coordinate array
        NormalArray     m_Normal;   // Vertex normal array
        ColorArray      m_Color;    // Vertex color array
        TexCoordArray   m_TexCoord; // Vertex texture coordinate array

        
        PolyIndexArray m_AdjFaces; // Vertex adjacent face array
        PolyIndexArray m_AdjVerts; // Vertex adjacent vert array
        PolyIndexArray m_AdjEdges; // Vertex adjacent edge array
        
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


    /// The following haven't implement currently
    class HalfEdge
    {
    public:
        HalfEdge();
        ~HalfEdge();

    public:
        int id;
        int nxt_he_id;
        int pre_he_id;
        int opp_he_id;

        int vert_id;
        int edge_id;
        int face_id;
    };

    
    class HEInfo
    {
    public:
        HEInfo(){}
        ~HEInfo(){}


        std::vector<HalfEdge>& GetHalfEdgeArray() { return m_he_vec; }
        
    private:
        std::vector<HalfEdge> m_he_vec;

        friend class MeshKernel;
        
    }
    
    HalfEdge::HalfEdge() {
        id = nxt_he_id = pre_he_id = opp_he_id = vert_id = edge_id = face_id = -1;
    }
    HalfEdge::~HalfEdge() {}

    
}
#endif

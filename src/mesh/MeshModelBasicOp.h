#ifndef MESHLIB_MESHBASICOP_H_
#define MESHLIB_MESHBASICOP_H_

class MeshModelBasicOp
{   
public:
    MeshModelBasicOp();
    ~MeshModelBasicOp();

    // Initializer
    void AttachKernel(MeshModelKernel* pKernel);
    void AttachAuxData(MeshModelAuxData* pAuxData);

    // Vertex information calculation
    void CalAdjacentInfo(); // Calculate the adjacent information for each vertex
    void CalVertexNormal(); // Calculate normal vector of all vertices
    void CalVertexNormal(IntArray& arrIndex);   // Calculate normal vector of selected vertices

    // Face information calculation
    void CalFaceNormal();   // Calculate normal vector of all faces
    void CalFaceNormal(IntArray& arrIndex);     // Calculate normal vector of selected faces

    // Edge information calculation, optional functions
    void CreateHalfEdge();  // Create halfedge, optional function
    void CalHalfEdgeInfo(); // Calculate the halfedge information, optional function
	void CalFaceEdgeInfo(); // Calculate the face index information, optional function
    void CalVertexEdgeInfo(); // Calculate the edge information, optional function
    
	// Model information calculation
    void CalBoundingBox();      // Bounding box calculation
    void CalBoundingBox(IntArray& arrIndex);   // Update bounding box according to updated vertex coords
    void CalComponentInfo();    // Component calculation

    // Manifold functions
    void SortAdjacentInfo();    // Make sure the 1-ring neighbors are CCW order
    void CalBoundaryInfo();     // Boundary calculation

    // Model analysis functions
    int AdjFaceNum(VertexID vID, VertexID vID2);
    void TopologyAnalysis();

public:
    // Topology analisis and initialization
    void InitModel();   // Analyzing and initializing the topological structure of the mesh model

    // Shortest path between two given vertices
    void GetShortestPath(VertexID vStart, VertexID vEnd, IndexArray& Path);

    // Flood fill from a seed to marked boundary
    void SurfaceFloodFillVertex(VertexID vID, IndexArray& FillVtx, IndexArray& FillFace, IndexArray& SelectedVtx);
    void SurfaceFloodFillFace(FaceID fID, IndexArray& FillVtx, IndexArray& FillFace, IndexArray& SelectedFace);

    // Get the neighbors of a given face
    void GetFaceNeighborhood(FaceID fID, IndexArray& NeiFace);

    // Get the neighbors of a given vertex inside a circle with a given radius
    void GetNeighborhood(VertexID vID, IndexArray& NeiVtx, IndexArray& NeiFace, double radius);

    // Get the nearest vertex in a given face for a give position
    void GetNearestVertex(FaceID fID, Coord pos, VertexID& vID);
    void GetNearestVertex(Coord pos, VertexID& vID);

    //
    inline bool IsValidVertexIndex(VertexID vID);
    inline bool IsValidFaceIndex(FaceID fID);
    inline bool IsBoundaryVertex(VertexID vID);
    inline bool IsBoundaryFace(FaceID fID);

    // Get the average edge length
    double GetAvgEdgeLength();

    // Get the boundary index and the loop index
    bool GetBdyIndex(VertexID vID, BdyID& bdyID, Index& idx);

    // Get the adjacent face(s) of the edge (vID1, vID2)
    void GetAdjacentFace(VertexID vID1, VertexID vID2, FaceID& fID1, FaceID& fID2);
	void GetAdjacentFace(VertexID vID1, VertexID vID2, vector<FaceID>& face_vec);
	void GetAdjacentFace(FaceID fID, vector<FaceID>& face_vec);
	void GetEdgeOppositeFace(VertexID vID1, VertexID vID2, FaceID& fID, FaceID& oppfID);

    // Get the adjacent vertices of the edge (vID1, vID2)
    void GetAdjacentVertex(VertexID vID1, VertexID vID2, vector<VertexID>& oppVID);

	void CalBaryCentricCoord(FaceID fID, Coord& pos, Coord& bCenter);
	void CalTrglGradient(vector<Coord>& trgP, vector<Coord>& trgG);
	double GetBaryAdjFaceArea(VertexID vID);
	double GetEdgeAdjFaceArea(VertexID vID_1, VertexID vID_2);

	// Get the neighbors of a given vertex inside a circle with a given radius
	void GetNeighborhoodVertex(int vID, size_t neighRingSize, bool onlyRing, vector<int>& neighVIDs);
};

//////////////////////////////////////////////////////////////////////////
// Inline functions
//////////////////////////////////////////////////////////////////////////

bool MeshModelBasicOp::IsValidVertexIndex(VertexID vID)
{
    if(vID < 0  || vID >= (int) kernel->GetVertexInfo().GetCoord().size())
        return false;
    
    return (!util.IsSetFlag(kernel->GetVertexInfo().GetFlag()[vID], FLAG_INVALID));
}

inline bool MeshModelBasicOp::IsValidFaceIndex(FaceID fID)
{
    if(fID < 0  || fID >= (int) kernel->GetFaceInfo().GetIndex().size())
        return false;
    
    return (!util.IsSetFlag(kernel->GetFaceInfo().GetFlag()[fID], FLAG_INVALID));
}

inline bool MeshModelBasicOp::IsBoundaryVertex(VertexID vID)
{
    return (util.IsSetFlag(kernel->GetVertexInfo().GetFlag()[vID], VERTEX_FLAG_BOUNDARY));
}

inline bool MeshModelBasicOp::IsBoundaryFace(FaceID fID)
{
    return (util.IsSetFlag(kernel->GetFaceInfo().GetFlag()[fID], FACE_FLAG_BOUNDARY));
}

#endif

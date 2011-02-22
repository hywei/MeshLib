#include "MeshBasicOP.h"
#include "Mesh"
#include <stack>
#include <algorithm>
#include <cmath>
#include <cassert>
#include <fstream>

namespace MeshLib{

    MeshBasicOP::MeshBasicOP(Mesh& mesh) : m_mesh(mesh){}
    MeshBasicOP::~MeshBasicOP() {}
    void MeshBasicOP::CalAdjacentInfo()
    {
        VertexInfo& vInfo = m_mesh.p_Kernel->GetVertexInfo();
        FaceInfo& fInfo = m_mesh.p_Kernel->GetFaceInfo();

        PolyIndexArray& vAdjVerts = vInfo.GetAdjVertices();
        PolyIndexArray& vAdjFaces = vInfo.GetAdjFaces();

        vAdjVerts.clear();
        vAdjFaces.clear();
    
        // Calculate the adjacent faces for each vertex
        size_t i, j, n;
        size_t nVertex = vInfo.GetCoord().size();
        vAdjFaces.resize(nVertex);
        vAdjVerts.resize(nVertex);
    
        PolyIndexArray& fIndex = fInfo.GetIndex();
        size_t nFace = fIndex.size();
        for(i = 0; i < nFace; ++ i)
            {
                IndexArray& f = fIndex[i];
                n = f.size();
                for(j = 0; j < n; ++ j)
                    {
                        VertexID vID = f[j];
                        vAdjFaces[vID].push_back((int) i);

                        VertexID prev_vID = f[(j+n-1)%n];
                        VertexID next_vID = f[(j+1)%n];

                        if(find(vAdjVerts.begin(), vAdjVerts.end(), prev_vID) == vAdjVerts.end()) vAdjVerts.push_back(prev_vID);
                        if(find(vAdjVerts.begin(), vAdjVerts.end(), next_vID) == vAdjVerts.end()) vAdjVerts.push_back(next_vID); 
                    }
            }
    }

    // Bounding box calculation
    void MeshBasicOP::CalBoundingBox()
    {
        CoordArray& vCoord = m_mesh.p_Kernel->GetVertexInfo().GetCoord();
        size_t nVertex = vCoord.size();
    
        Coord BoxMin(vCoord[0]), BoxMax(vCoord[0]), SphereCenter;
        double SphereRadius;
    
        // Calculate the bounding box and the center of the bounding sphere
        for(size_t i = 0; i < nVertex; ++ i)
            {
                Coord& v = vCoord[i];
                for(int j = 0; j < 3; ++ j)
                    {
                        if(v[j] < BoxMin[j]) BoxMin[j] = v[j];
                        if(v[j] > BoxMax[j]) BoxMax[j] = v[j];
                        SphereCenter[j] += v[j];
                    }
            }
        SphereCenter /= (double)nVertex;

        // Calculate the radius of the bounding sphere
        SphereRadius = (vCoord[0]-SphereCenter).abs();
        for(size_t i = 1; i < nVertex; ++ i)
            {
                double r = (vCoord[i]-SphereCenter).abs();
                if(r > SphereRadius) SphereRadius = r;
            }

        // Update corresponding model information
        ModelInfo& mInfo = m_mesh.p_Kernel->GetModelInfo();
        mInfo.SetBoundingBox(BoxMin, BoxMax, BoxMax-BoxMin);
        mInfo.SetBoundingSphere(SphereCenter, SphereRadius);

        printf("Model BoundingSphere Center : (%lf %lf %lf), Radius : %lf\n",
               SphereCenter[0], SphereCenter[1], SphereCenter[2], SphereRadius);
    }



    void MeshBasicOP::CalFaceNormal()
    {
        CoordArray& vCoord = m_mesh.p_Kernel->GetVertexInfo().GetCoord();
        PolyIndexArray& fIndex = m_mesh.p_Kernel->GetFaceInfo().GetIndex();
        NormalArray& fNormal = m_mesh.p_Kernel->GetFaceInfo().GetNormal();

        fNormal.resize(fIndex.size());
        Coord v[3];
        for(size_t i = 0; i < fIndex.size(); ++ i)
            {
                IndexArray& f = fIndex[i];
                for(size_t j = 0; j < 3; ++ j) v[j] = vCoord[f[j]];
                Normal& fn = fNormal[i];
                fn = cross(v[1]-v[0], v[2]-v[0]);
                if(!fn.normalize()) fn = COORD_AXIS_Z;
            }
    }
    void MeshBasicOP::CalVertexNormal()
    {
        CoordArray& vCoord = m_mesh.p_Kernel->GetVertexInfo().GetCoord();
        NormalArray& vNormal = m_mesh.p_Kernel->GetVertexInfo().GetNormal();
        NormalArray& fNormal = m_mesh.p_Kernel->GetFaceInfo().GetNormal();
        PolyIndexArray& vAdjFaces = m_mesh.p_Kernel->GetVertexInfo().GetAdjFaces();
    
        vNormal.resize(nVertex);
        for(size_t i = 0; i < vCoord.size(); ++i)
            {
                Normal& vn = vNormal[i];
                vn.setCoords(0.0, 0.0, 0.0);
                for(size_t j = 0; j < vAdj.size(); ++ j)
                    {
                        vn += fNormal[vAdjFaces[i][j]];
                    }
                if(!vn.normalize()) vn = COORD_AXIS_Z;
            }
    }


    void MeshBasicOP::CalComponentInfo()
    {
        PolyIndexArray& vAdjVerts = m_mesh.p_Kernel->GetVertexInfo().GetAdjVertices();

        size_t nVertex = vAdjVerts.size();
        std::vector<bool> Visited;
        Visited.resize(nVertex);
        std::fill(Visited.begin(), Visited.end(), false);

        int nComponent = 0;
        for(size_t i = 0; i < nVertex; ++ i)
            {
                if(Visited[i]) continue;
        
                std::stack<VertexID> Stack;
                Stack.push((int) i);
                Visited[i] = true;
                while(Stack.size()){
                    VertexID vID = Stack.top(); Stack.pop();

                    IndexArray& adjVertices = vAdjVerts[vID];
                    for(size_t j = 0; j < adjVertices.size(); ++ j)
                        {
                            if(!Visited[adjVertices[j]])
                                {
                                    Stack.push(adjVertices[j]);
                                    Visited[adjVertices[j]] = true;
                                }
                        }
                }
                ++ nComponent;
            }
    
        m_mesh.p_Kernel->GetModelInfo().SetComponentNum(nComponent);
        printf("Number of Components = %4d\n", nComponent);
    }

    void MeshBasicOP::TopologyAnalysis()
    {
        //TODO
        CoordArray& vCoord = kernel->GetVertexInfo().GetCoord();
        FlagArray& vFlag = kernel->GetVertexInfo().GetFlag();
        PolyIndexArray& vAdjFaces = kernel->GetVertexInfo().GetAdjFaces();
        PolyIndexArray& vAdjVertices = kernel->GetVertexInfo().GetAdjVertices();
        PolyIndexArray& fIndex = kernel->GetFaceInfo().GetIndex();
        FlagArray& fFlag = kernel->GetFaceInfo().GetFlag();

        size_t nVertex = vCoord.size();
        size_t nFace = fIndex.size();

        size_t i, j, n;
        vFlag.resize(nVertex);
        fFlag.resize(nFace);
        fill(vFlag.begin(),vFlag.end(), 0);
        fill(fFlag.begin(),fFlag.end(), 0);

        ofstream fout("bad_topogloy.txt");

        bool bManifoldModel = true;
        for(i = 0; i < nVertex; ++ i)
            {
                IndexArray& adjFaces = vAdjFaces[i];
                Flag& flag = vFlag[i];

                n = adjFaces.size();
                if(!n)  // Isolated vertex
                    {
                        util.SetFlag(flag, VERTEX_FLAG_ISOLATED);
                        continue;
                    }

                // Check topology for the 1-ring neighborhood of vertex i
                IndexArray& adjVertices = vAdjVertices[i];
                n = adjVertices.size();
                int nBdyFace = 0, nNonManifoldFace = 0;
                for(j = 0; j < n; ++ j)
                    {
                        switch(AdjFaceNum((int) i, adjVertices[j]))
                            {
                            case 1:     // Boundary
                                ++ nBdyFace;
                                //		auxdata->AddLine(vCoord[i], vCoord[adjVertices[j]], DARK_GREEN);
                                fout << i+1 <<' '<<adjVertices[j]+1 <<endl;
                                break;
                            case 2:     // 2-Manifold
                                break;
                            default:    // Non-Manifold
                                ++ nNonManifoldFace;
                                auxdata->AddLine(vCoord[i], vCoord[adjVertices[j]], DARK_GREEN);
                                fout << i+1 <<' '<<adjVertices[j]+1 <<endl;
                            }
			
                    }
                if(nNonManifoldFace || nBdyFace > 2)    // Non-manifold vertex
                    {
                        Coord v = vCoord[i];
                        auxdata->AddPoint(v, DARK_RED);
                        fout<< i+1 << endl;
                        bManifoldModel = false;
                        continue;
                    }

                // 		auxdata->AddPoint(vCoord[0], DARK_RED);
                // 		auxdata->AddPoint(vCoord[15], DARK_GREEN);
                // 		auxdata->AddPoint(vCoord[240], DARK_BLUE);
                // 		auxdata->AddPoint(vCoord[255], DARK_GREY);
        
                // Manifold vertex
                util.SetFlag(flag, VERTEX_FLAG_MANIFOLD);
                if(nBdyFace == 2)   // Boundary vertex
                    util.SetFlag(flag, VERTEX_FLAG_BOUNDARY);
            }
        fout.close();

        // Set face flag
        bool bTriMesh = true;
        bool bQuadMesh = true;
        for(i = 0; i < nFace; ++ i)
            {
                IndexArray& f = fIndex[i];
                n = f.size();

                if(n != 3)
                    bTriMesh = false;
                else if(n != 4)
                    bQuadMesh = false;

                int nBdyVtx = 0;
                bool bManifoldFace = true;
                for(j = 0; j < n; ++ j)
                    {
                        if(!util.IsSetFlag(vFlag[f[j]], VERTEX_FLAG_MANIFOLD))
                            {
                                bManifoldFace = false;
                                break;
                            }
                        else if(util.IsSetFlag(vFlag[f[j]], VERTEX_FLAG_BOUNDARY))
                            nBdyVtx ++;
                    }
                Flag& flag = fFlag[i];
                if(bManifoldFace)
                    {
                        util.SetFlag(flag, FACE_FLAG_MANIFOLD);
                        if(nBdyVtx > 1)
                            util.SetFlag(flag, FACE_FLAG_BOUNDARY);
                    }
            }

        // Set model flag
        Flag& mFlag = kernel->GetModelInfo().GetFlag();
        mFlag = 0;
        if(bManifoldModel)  // Manifold model
            {
                util.SetFlag(mFlag, MODEL_FLAG_MANIFOLD);
                printf("Manifold Model\n");
            }
        if(bTriMesh)
            {
                util.SetFlag(mFlag, MODEL_FLAG_TRIMESH);
                printf("Triangle Mesh\n");
            }
        else if(bQuadMesh)
            {
                util.SetFlag(mFlag, MODEL_FLAG_QUADMESH);
                printf("Quadangle Mesh\n");
            }
        else
            {
                util.SetFlag(mFlag, MODEL_FLAG_GENERALMESH);
                printf("General Mesh\n");
            }

    }

    // Make sure the 1-ring neighbors are CCW order
    void MeshBasicOP::SortAdjacentInfo()
    {
        CoordArray& vCoord = m_mesh.p_Kernel->GetVertexInfo().GetCoord();
        PolyIndexArray& vAdjFaces = m_mesh.p_Kernel->GetVertexInfo().GetAdjFaces();
        PolyIndexArray& vAdjVerts = m_mesh.p_Kernel->GetVertexInfo().GetAdjVertices();
        PolyIndexArray& fIndex = m_mesh.p_Kernel->GetFaceInfo().GetIndex();

        size_t nVertex = vCoord.size();
        size_t nFace = fIndex.size();
    
        for(size_t i = 0; i < nVertex; ++ i)
            {
                // Only for manifold vertex
                IndexArray& adjFaces = vAdjFaces[i];
                n = adjFaces.size();
                assert(n > 0);

                // Find the start face
                FaceID start_fID = adjFaces[0];
     
                {
                    start_fID = -1;
                    for(j = 0; j < n; ++ j)
                        {
                            FaceID fID = adjFaces[j];
                            if(!util.IsSetFlag(fFlag[fID], FACE_FLAG_BOUNDARY))
                                continue;
                            IndexArray& face = fIndex[fID];
                            size_t idx = distance(face.begin(), find(face.begin(), face.end(), i));
                            VertexID next_vID = face[(idx+1)%face.size()];
                            if(util.IsSetFlag(vFlag[next_vID], VERTEX_FLAG_BOUNDARY))   // Find it
                                {
                                    if(AdjFaceNum((int) i, next_vID) == 1)    // Make sure it is a boundary edge (i, next_vID)
                                        {
                                            start_fID = fID;
                                            break;
                                        }
                                }
                        }
                    assert(start_fID != -1);
                }

                // Iteratively find the next neighboring face
                IndexArray Sorted;
                Sorted.reserve(n);
        
                IndexArray Idx;     // Record the index of vertex i in each sorted adjacent face
                Idx.reserve(n);
                for(j = 0; j < n; ++ j)
                    {
                        IndexArray& face = fIndex[start_fID];
                        m = face.size();
                        size_t idx = distance(face.begin(), find(face.begin(), face.end(), i));
                        Idx.push_back((int) idx);
            
                        // Vertex i may have only 1 adjacent face
                        if(j == n-1)    // Last one, Not need to find the next neighboring face
                            continue;

                        Sorted.push_back(start_fID);
                        adjFaces.erase(remove(adjFaces.begin(), adjFaces.end(), start_fID), adjFaces.end());
        
                        VertexID prev_vID = face[(idx+m-1)%m];
                        for(k = 0; k < adjFaces.size(); ++ k)
                            {
                                FaceID fID = adjFaces[k];
                                IndexArray& f = fIndex[fID];
                                size_t idx = distance(f.begin(), find(f.begin(), f.end(), i));
                                VertexID next_vID = f[(idx+1)%f.size()];
                                if(next_vID == prev_vID)    // Next neighboring face
                                    {
                                        start_fID = fID;
                                        break;
                                    }
                            }
                    }
                assert(adjFaces.size() >= 1);
                Sorted.push_back(adjFaces[0]);
                adjFaces.clear();
                adjFaces = Sorted;

                // Set sorted adjacent vertices for vertex i
                IndexArray& adjVertices = vAdjVerts[i];
                adjVertices.clear();
                n = adjFaces.size();
                for(j = 0; j < n; ++ j)
                    {
                        IndexArray& face = fIndex[adjFaces[j]];
                        int idx = Idx[j];
                        m = face.size();
                        VertexID vID = face[(idx+1)%m];
                        adjVertices.push_back(vID);
                    }
                if(util.IsSetFlag(vFlag[i], VERTEX_FLAG_BOUNDARY))  // Boundary vertex, add one more adjacent vertex
                    {
                        IndexArray& face = fIndex[adjFaces[n-1]];
                        int idx = Idx[n-1];
                        m = face.size();
                        VertexID vID = face[(idx+m-1)%m];
                        adjVertices.push_back(vID);
                    }
            }
    }

    void MeshBasicOP::InitModel()
    {   
        printf("Analyze the mesh model...\n");

        CalAdjacentInfo();
        CalBoundingBox();

        // Normal calculations
        CalFaceNormal();
        CalVertexNormal();

        // Calculating number of connected components
        CalComponentInfo();

        // Analyzing the topology of the model, manifold or not
        TopologyAnalysis();
    
        if(m_mesh.p_Kernel->GetModelInfo().IsManifold())
            {
                SortAdjacentInfo(); // Sorting the adjacent face/vertex information for each vertex
                CalBoundaryInfo();  // Extracting the boundaries of the model
            }

        // cal avg edge length here.
        m_mesh.p_Kernel->GetModelInfo().SetAvgEdgeLength(GetAvgEdgeLength());

        // cal the dihedral angle here.
        CalVertexEdgeInfo();
        CalFaceEdgeInfo();
    
        int vert_num = (int)m_mesh.p_Kernel->GetVertexInfo().GetCoord().size();
        int face_num = (int)m_mesh.p_Kernel->GetFaceInfo().GetIndex().size();

        m_mesh.p_Kernel->GetModelInfo().SetVertexNum(vert_num);
        m_mesh.p_Kernel->GetModelInfo().SetFaceNum(face_num);
    }


    // Get the average edge length
    double MeshBasicOP::GetAvgEdgeLength()
    {
        CoordArray& vCoord = m_mesh.p_Kernel->GetVertexInfo().GetCoord();
        PolyIndexArray& vAdjVerts = m_mesh.p_Kernel->GetVertexInfo().GetAdjVertices();
        size_t nVertex = vCoord.size();
    
        time_t tt1;
        time(&tt1);
        srand((unsigned)tt1);

        double avg_edge = 0.0;
        int nEdge = 0;
        // Calculate by full sampling
        for(size_t i = 0; i < nVertex; ++ i)
            {
                IndexArray& adjVertices = vAdjVerts[i];
                size_t j, m = adjVertices.size();
                for(j = 0; j < m; ++ j)
                    {
                        VertexID vID = adjVertices[j];
                        if(vID > (int) i)
                            continue;
                        avg_edge += (vCoord[i]-vCoord[vID]).abs();
                        ++ nEdge;
                    }
            }
        return avg_edge/(double)nEdge;
    }

}

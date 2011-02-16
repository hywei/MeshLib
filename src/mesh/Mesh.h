#ifndef MESHLIB_MESH_H_
#define MESHLIB_MESH_H_

#include <boost/shared_ptr.hpp>
#include <string>

namespace MeshLib{

    class MeshKernel;
    class MeshIO;
    class MeshBasicOP;
    
    class Mesh
    {
    public:
        // Constructor/Destructor
        Mesh();
        ~Mesh();
        
        // Input/Output functions
        bool AttachModel(const std::string& filename);
        bool StoreModel(const std::string& filename) const;

    private:
        boost::shared_ptr<MeshKernel> p_Kernel;
        boost::shared_ptr<MeshIO> p_IO;
        boost::shared_ptr<MeshBasicOP> p_BasicOP;

        friend class MeshKernel;        
        friend class MeshIO;
        friend class MeshBasicOP;
        
    };

} // 
#endif

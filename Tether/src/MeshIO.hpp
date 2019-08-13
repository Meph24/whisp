#ifndef MESHIO_HPP
#     define MESHIO_HPP


#include <string>

#include "Mesh.hpp"
#include "IFileIO.hpp"

using std::string;

class MeshIO : public IFileIO<Mesh>
{
	string filename;
public:
	MeshIO(string filename);
	Mesh get() const;
	void save(const Mesh& mesh) const;
};

#endif /* MESHIO_HPP */

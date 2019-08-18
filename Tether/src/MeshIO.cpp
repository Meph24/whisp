#include "MeshIO.hpp"

#include <fstream>

using std::ifstream;
using std::ofstream;

MeshIO::MeshIO(string filename)
	:
	filename(filename)
{}

Mesh MeshIO::get() const
{
	ifstream is ( filename );
	if(is.fail())
	{
		std::cerr << "MeshIO: File <" << filename << "> could no be opened!\n";
	}

	Mesh mesh;
	is >> mesh;
	is.close();
	return mesh;
}

void MeshIO::save(const Mesh& mesh) const
{
	ofstream os(filename);
	os << mesh;
	os.close();
}

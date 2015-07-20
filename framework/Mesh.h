#ifndef __MESH_H__
#define __MESH_H__
#include"main.h"
#include "GLM.h"
#include "Math_Header.h"

class Mesh
{
public:
	Mesh(){};
	bool LoadModel(char* path,char* texfilename,int texid);
	void RenderFP();
	~Mesh(){};
private:
	GLMmodel* obj;
    unsigned int m_vertices_number; // m
    unsigned int m_system_dimension; // 3m

 //   // vertices positions/previous positions/mass
    VectorX m_current_positions; // 1x3m
    VectorX m_current_velocities; // 1x3m
    SparseMatrix m_mass_matrix; // 3mx3m
    SparseMatrix m_inv_mass_matrix; // 3mx3m
    SparseMatrix m_identity_matrix; // 3mx3m

	vector<VertexPoint> m_pointCloud;
	vector<VertexPoint> m_meshData;

	vector<glm_vector3> m_vertex;
	vector<glm_vector3> m_vertexNormal;
	vector<glm_vector2> m_vertexTexcoord;
	vector<unsigned int> m_vertexIndex;
	vector<unsigned int> m_normalIndex;
	vector<unsigned int> m_texcoordIndex;
};

#endif 
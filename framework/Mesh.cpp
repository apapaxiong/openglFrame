#include "Mesh.h"
extern bool g_show_wireframe;
extern bool g_show_edgelist;
extern float g_linewidth;
extern bool g_show_render;
extern float g_pointsize;
extern bool g_show_fixedvertex;
bool Mesh::LoadModel(char* path,char *texfilename,int texid)
{
	if (!obj)
	{
		obj = glmReadOBJ(path);

		if (!obj)
		{
			printf("OBJ file does not exist \n");
			return false;
		}

		// glmUnitize(objmodel);
		glmFacetNormals(obj);
		glmVertexNormals(obj, 90.0);
	}
	m_vertices_number=obj->numvertices;
	m_system_dimension=3*obj->numvertices;

	m_vertex.resize(obj->numvertices);
	memcpy(&m_vertex[0],obj->vertices+3,(obj->numvertices)*sizeof(float)*3);
	//cout<<vertexCache[id][objmodel->numvertices].x<<endl;
	m_vertexNormal.resize(obj->numnormals);
	memcpy(&m_vertexNormal[0],obj->normals+3,obj->numnormals*sizeof(float)*3);
	m_vertexTexcoord.resize(obj->numtexcoords);
	memcpy(&m_vertexTexcoord[0],obj->texcoords+2,obj->numtexcoords*sizeof(float)*2);

	for(int i=0;i<obj->numtriangles;i++)
	{
		m_vertexIndex.push_back(obj->triangles[i].vindices[0]);
		m_vertexIndex.push_back(obj->triangles[i].vindices[1]);
		m_vertexIndex.push_back(obj->triangles[i].vindices[2]);

		m_normalIndex.push_back(obj->triangles[i].nindices[0]);
		m_normalIndex.push_back(obj->triangles[i].nindices[1]);
		m_normalIndex.push_back(obj->triangles[i].nindices[2]);

		m_texcoordIndex.push_back(obj->triangles[i].tindices[0]);
		m_texcoordIndex.push_back(obj->triangles[i].tindices[1]);
		m_texcoordIndex.push_back(obj->triangles[i].tindices[2]);
	}
	int numtri=obj->numtriangles;
	m_pointCloud.resize(obj->numvertices);
	m_total_mass=10;
	for(int i=0;i<3*numtri-2;i+=3)
	{
		//cout<<i<<endl;
		int id=m_vertexIndex[i];
		int id1=m_vertexIndex[i+1];
		int id2=m_vertexIndex[i+2];
		int nid=m_normalIndex[i];
		int nid1=m_normalIndex[i+1];
		int nid2=m_normalIndex[i+2];
		int tid=m_texcoordIndex[i];
		if(tid<0)
			tid=1;
		int tid1=m_texcoordIndex[i+1];
		if(tid1<0)
			tid1=1;
		int tid2=m_texcoordIndex[i+2];
		if(tid2<0)
			tid2=1;
		VertexPoint temp;
		temp.v=m_vertex[id-1];
		temp.nor=m_vertexNormal[nid-1];
		temp.tex=m_vertexTexcoord[tid-1];

		m_pointCloud[id-1].v=m_vertex[id-1];
		m_pointCloud[id-1].nor=m_vertexNormal[nid-1];
		m_pointCloud[id-1].tex=m_vertexTexcoord[tid-1];

		m_meshData.push_back(temp);
		temp.v=m_vertex[id1-1];
		temp.nor=m_vertexNormal[nid1-1];
		temp.tex=m_vertexTexcoord[tid1-1];

		m_pointCloud[id1-1].v=m_vertex[id1-1];
		m_pointCloud[id1-1].nor=m_vertexNormal[nid1-1];
		m_pointCloud[id1-1].tex=m_vertexTexcoord[tid1-1];

		m_meshData.push_back(temp);
		temp.v=m_vertex[id2-1];
		temp.nor=m_vertexNormal[nid2-1];
		temp.tex=m_vertexTexcoord[tid2-1];

		m_pointCloud[id2-1].v=m_vertex[id2-1];
		m_pointCloud[id2-1].nor=m_vertexNormal[nid2-1];
		m_pointCloud[id2-1].tex=m_vertexTexcoord[tid2-1];

		m_meshData.push_back(temp);	
	}
	m_pointCloud_ori.resize(m_pointCloud.size());
	m_pointFixed.resize(m_pointCloud.size());
	for(int i=0;i<m_pointCloud.size();i++)
	{
		m_pointCloud_ori[i]=m_pointCloud[i];
	}
	//------------------------Eigen Setup----------------------//
	m_current_positions.resize(m_system_dimension);
    m_current_velocities.resize(m_system_dimension);
	m_mass_matrix.resize(m_system_dimension, m_system_dimension);
    m_inv_mass_matrix.resize(m_system_dimension, m_system_dimension);
    m_identity_matrix.resize(m_system_dimension, m_system_dimension);

	m_current_positions.setZero();
	for(int i=0;i<m_pointCloud.size();i++)
	{
		m_current_positions.block_vector(i)=EigenVector3(m_pointCloud[i].v.x, m_pointCloud[i].v.y, m_pointCloud[i].v.z);
	}
	m_current_velocities.setZero();
	std::vector<SparseMatrixTriplet> i_triplets;
    std::vector<SparseMatrixTriplet> m_triplets;
    std::vector<SparseMatrixTriplet> m_inv_triplets;
	i_triplets.clear();
    m_triplets.clear();
	float unit_mass=m_total_mass/m_pointCloud.size();
    ScalarType inv_unit_mass = 1.0 / unit_mass;
	for (int index = 0; index < m_system_dimension; index++)
    {
        i_triplets.push_back(SparseMatrixTriplet(index, index, 1));
        m_triplets.push_back(SparseMatrixTriplet(index, index, unit_mass));
        m_inv_triplets.push_back(SparseMatrixTriplet(index, index, inv_unit_mass));
    }
	m_mass_matrix.setFromTriplets(m_triplets.begin(), m_triplets.end());
    m_inv_mass_matrix.setFromTriplets(m_inv_triplets.begin(), m_inv_triplets.end());
    m_identity_matrix.setFromTriplets(i_triplets.begin(), i_triplets.end());

	cout<<"Mesh Loading Finished"<<endl;
	cout<<"Number of Vertices: "<<m_pointCloud.size()<<endl;
	cout<<"Number of Faces: "<<m_vertexIndex.size()/3<<endl;
	GenerateEdgeList();
	GenerateFixedConstraint();
//	glActiveTexture(GL_TEXTURE1);
//	LoadTexture(skinnormal);
//	glActiveTexture(GL_TEXTURE5);
//	LoadTexture(bumpnormal);
//	if(texid==0)
//	{
//	   glActiveTexture(GL_TEXTURE0);
//	   LoadTexture(texfilename);
//	}
//	if(texid==1)
//	{
//	   glActiveTexture(GL_TEXTURE4);
//	   LoadTexture(texfilename);
//	}
//	CreateNeighborFaceNew();
//	return true;
}
void Mesh::GenerateFixedConstraint()
{
	for(int i=0;i<m_pointCloud.size();i++)
	{
		m_pointFixed[i]=false;
	}
	for(int i=0;i<m_vertexIndex.size()-2;i+=3)
	{
		int id1=m_vertexIndex[i]-1;
		int id2=m_vertexIndex[i+1]-1;
		int id3=m_vertexIndex[i+2]-1;
		int count=0;
		for(int j=0;j<m_vertexIndex.size()-2;j+=3)
		{
			if(i!=j)
			{
				if((id1==m_vertexIndex[j]-1&&id2==m_vertexIndex[j+1]-1)||(id1==m_vertexIndex[j+1]-1&&id2==m_vertexIndex[j]-1)||
					(id1==m_vertexIndex[j+1]-1&&id2==m_vertexIndex[j+2]-1)||(id1==m_vertexIndex[j+2]-1&&id2==m_vertexIndex[j+1]-1)||
					(id1==m_vertexIndex[j]-1&&id2==m_vertexIndex[j+2]-1)||(id1==m_vertexIndex[j+2]-1&&id2==m_vertexIndex[j]-1)
					)
					count++;
			}
		}
		if(0==count)
		{
			m_pointFixed[id1]=1;
			m_pointFixed[id2]=1;
		}
		count=0;
		for(int j=0;j<m_vertexIndex.size()-2;j+=3)
		{
			if(i!=j)
			{
				if((id1==m_vertexIndex[j]-1&&id3==m_vertexIndex[j+1]-1)||(id1==m_vertexIndex[j+1]-1&&id3==m_vertexIndex[j]-1)||
					(id1==m_vertexIndex[j+1]-1&&id3==m_vertexIndex[j+2]-1)||(id1==m_vertexIndex[j+2]-1&&id3==m_vertexIndex[j+1]-1)||
					(id1==m_vertexIndex[j]-1&&id3==m_vertexIndex[j+2]-1)||(id1==m_vertexIndex[j+2]-1&&id3==m_vertexIndex[j]-1)
					)
					count++;
			}
		}
		if(0==count)
		{
			m_pointFixed[id1]=1;
			m_pointFixed[id3]=1;
		}
		count=0;
		for(int j=0;j<m_vertexIndex.size()-2;j+=3)
		{
			if(i!=j)
			{
				if((id2==m_vertexIndex[j]-1&&id3==m_vertexIndex[j+1]-1)||(id2==m_vertexIndex[j+1]-1&&id3==m_vertexIndex[j]-1)||
					(id2==m_vertexIndex[j+1]-1&&id3==m_vertexIndex[j+2]-1)||(id2==m_vertexIndex[j+2]-1&&id3==m_vertexIndex[j+1]-1)||
					(id2==m_vertexIndex[j]-1&&id3==m_vertexIndex[j+2]-1)||(id2==m_vertexIndex[j+2]-1&&id3==m_vertexIndex[j]-1)
					)
					count++;
			}
		}
		if(0==count)
		{
			m_pointFixed[id2]=1;
			m_pointFixed[id3]=1;
		}

	}
}
void Mesh::Render()
{
	for(int i=0;i<m_pointCloud.size();i++)
	{
		m_pointCloud[i].v= glm_vector3(m_current_positions[3*i+0], m_current_positions[3*i+1], m_current_positions[3*i+2]);
	}
	if(g_show_render)
		RenderFP();

	Degbug_Render();
}
void Mesh::RenderFP()
{
	glBegin(GL_TRIANGLES);
	for(int i=0;i<m_meshData.size()-2;i+=3)
	{
		glNormal3f(m_meshData[i].nor.x,m_meshData[i].nor.y,m_meshData[i].nor.z);
		glVertex3f(m_meshData[i].v.x, m_meshData[i].v.y, m_meshData[i].v.z);
		
		glNormal3f(m_meshData[i+1].nor.x,m_meshData[i+1].nor.y,m_meshData[i+1].nor.z);
		glVertex3f(m_meshData[i+1].v.x, m_meshData[i+1].v.y, m_meshData[i+1].v.z);

		glNormal3f(m_meshData[i+2].nor.x,m_meshData[i+2].nor.y,m_meshData[i+2].nor.z);
		glVertex3f(m_meshData[i+2].v.x, m_meshData[i+2].v.y, m_meshData[i+2].v.z);
	}
	glEnd();
}
void Mesh::GenerateEdgeList()
{
	for(int i=0;i<m_vertexIndex.size()-2;i+=3)
		{
			int idx1=m_vertexIndex[i]-1;
			int idx2=m_vertexIndex[i+1]-1;
			int idx3=m_vertexIndex[i+2]-1;
			glm::vec3 avernor=(m_pointCloud[idx1].nor+m_pointCloud[idx2].nor+m_pointCloud[idx3].nor)/3.0f;
			glm::vec3 averver=(m_pointCloud[idx1].v+m_pointCloud[idx2].v+m_pointCloud[idx3].v)/3.0f;
			
			bool exist=false;
			for(int j=0;j<m_edgelist.size();j++)
			{
				if(m_edgelist[j].pa==idx1&&m_edgelist[j].pb==idx2||m_edgelist[j].pa==idx2&&m_edgelist[j].pb==idx1)
				{
					exist=true;
					break;
				}
			}
			if(exist==false)
			{
				EdgeList tempedge;
				tempedge.pa=idx1;
				tempedge.pb=idx2;
				m_edgelist.push_back(tempedge);
			}
		    exist=false;
			for(int j=0;j<m_edgelist.size();j++)
			{
				if(m_edgelist[j].pa==idx1&&m_edgelist[j].pb==idx3||m_edgelist[j].pa==idx3&&m_edgelist[j].pb==idx1)
				{
					exist=true;
					break;
				}
			}
			if(exist==false)
			{
				EdgeList tempedge;
				tempedge.pa=idx1;
				tempedge.pb=idx3;
				m_edgelist.push_back(tempedge);
			}
			 exist=false;
			for(int j=0;j<m_edgelist.size();j++)
			{
				if(m_edgelist[j].pa==idx2&&m_edgelist[j].pb==idx3||m_edgelist[j].pa==idx3&&m_edgelist[j].pb==idx2)
				{
					exist=true;
					break;
				}
			}
			if(exist==false)
			{
				EdgeList tempedge;
				tempedge.pa=idx2;
				tempedge.pb=idx3;
				m_edgelist.push_back(tempedge);
			}
		}
	cout<<"Edge List Generation Finished: "<<m_edgelist.size()<<endl;
}

void Mesh::Debug_Render_EdgeList()
{
	glBegin(GL_LINES);
	for(int i=0;i<m_edgelist.size();i++)
	{
		int idx1=m_edgelist[i].pa;
		int idx2=m_edgelist[i].pb;
		glm::vec3 v1=m_pointCloud[idx1].v;
		glm::vec3 v2=m_pointCloud[idx2].v;

		glVertex3f(v1.x,v1.y,v1.z);
		glVertex3f(v2.x,v2.y,v2.z);
	}
	glEnd();
}
void Mesh::Degbug_Render()
{
	glLineWidth(g_linewidth);
	if(g_show_edgelist)
	{
	    Debug_Render_EdgeList();
	}
	if(g_show_fixedvertex)
	{
		Debug_Render_FixedVertex();
	}
}
void Mesh::Debug_Render_FixedVertex()
{
	glPointSize(g_pointsize);
	glBegin(GL_POINTS);
	for(int i=0;i<m_pointFixed.size();i++)
	{
		if(m_pointFixed[i]==true)
		{
			glVertex3f(m_pointCloud[i].v.x, m_pointCloud[i].v.y, m_pointCloud[i].v.z);
		}
	}
	glEnd();
}
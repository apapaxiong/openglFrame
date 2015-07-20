#include "Mesh.h"

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
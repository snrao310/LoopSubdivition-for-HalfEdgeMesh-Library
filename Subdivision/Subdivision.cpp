#include <fstream>
#include <vector>
#include <stdio.h>
#include <cmath>


#include "OBJFileReader.h"
#include "Solid.h"
#include "iterators.h"
# define PI           3.14159265358979323846

using namespace MeshLib;

int main(int argc, char *argv[])
{
	// Read in the obj file
	Solid mesh;
	OBJFileReader of;
	std::ifstream in(argv[1]);
	of.readToSolid(&mesh, in);
	
	
	
	

	//answer part
	Solid meshCopy;
	mesh.copy(meshCopy);
	
	//storing all vertices
	int i=0;
	SolidVertexIterator veriter(&mesh);
	Vertex *allvert[mesh.numVertices()];
	for(i=0;!veriter.end();i++,++veriter)
	{
		allvert[i]=*veriter;
	}
	
	//storing all edges
	SolidEdgeIterator eiter(&mesh);
	Edge *alledges[mesh.numEdges()];
	for(i=0;!eiter.end();i++,++eiter)
	{
		alledges[i]=*eiter;
	}
	
	
	Edge *swapList[mesh.numFaces()];
	Edge *splitSwap[2]={NULL,NULL};
	
	//getting max face
	SolidFaceIterator fciter(&mesh);
	int maxfcid=-1;
	for(; !fciter.end(); ++fciter)
	{
		if((*fciter)->id()>maxfcid)
			maxfcid=(*fciter)->id();
	}
	
	//splitting and swapping by iterating through edges
	SolidEdgeIterator eitercpy(&meshCopy);
	int j;
	double x,y,z;
	for(i=0,j=0;i<(sizeof(alledges)/sizeof(*alledges));i++,++eitercpy)
	{
		splitSwap[0]=splitSwap[1]=NULL;
		Vertex *evert1= mesh.edgeVertex1(alledges[i]);
		Vertex *evert2= mesh.edgeVertex2(alledges[i]);
		Vertex *ver1=mesh.edgeSplit(alledges[i],splitSwap,maxfcid);
		if(splitSwap[0]!=NULL)
			swapList[j++]=splitSwap[0];
		if(splitSwap[1]!=NULL)
			swapList[j++]=splitSwap[1];
		
		//position of new vertex determined here
		if(mesh.isBoundary(*eitercpy))
		{
			//printf("Check");
			x=(evert1->point()(0)/2)+(evert2->point()(0)/2);
			y=(evert1->point()(1)/2)+(evert2->point()(1)/2);
			z=(evert1->point()(2)/2)+(evert2->point()(2)/2);
			//printf("\n%f %f %f\n",x,y,z);
		}
		else
		{
			x=((evert1->point()(0))+(evert2->point()(0)))*(3.0/8.0);
			y=((evert1->point()(1))+(evert2->point()(1)))*(3.0/8.0);
			z=((evert1->point()(2))+(evert2->point()(2)))*(3.0/8.0);
			x+=(((((*eitercpy)->halfedge(0))->he_next())->target())->point()(0)/8.0)+(((((*eitercpy)->halfedge(1))->he_next())->target())->point()(0)/8.0);
			y+=(((((*eitercpy)->halfedge(0))->he_next())->target())->point()(1)/8.0)+(((((*eitercpy)->halfedge(1))->he_next())->target())->point()(1)/8.0);
			z+=(((((*eitercpy)->halfedge(0))->he_next())->target())->point()(2)/8.0)+(((((*eitercpy)->halfedge(1))->he_next())->target())->point()(2)/8.0);
		}
		
		ver1->point()=Point(x,y,z);
	}
	
	
	for(i=0;i<(sizeof(swapList)/sizeof(*swapList));i++)
	{
		Edge *e=swapList[i];
		mesh.edgeSwap(e,((e->halfedge(0))->he_next())->target(),((e->halfedge(1))->he_next())->target());
	}
	
	
	
	
	//new position of old vertices determined here
	SolidVertexIterator veritercpy(&meshCopy);
	double tem1,tem2,tem3,alpha;
	for(i=0;i<(sizeof(allvert)/sizeof(*allvert));i++,++veritercpy)
	{
		Vertex *oldv=allvert[i];
		//printf("\n%f %f %f -> ",oldv->point()(0),oldv->point()(1),oldv->point()(2));
		VertexVertexIterator vviter(*veritercpy);
		//printf("Im here\n");
		tem1=tem2=tem3=0;
		j=0;
		if(mesh.isBoundary(*veritercpy))
		{
			//printf("Hi");
			for(j=0;!vviter.end();++vviter)
			{
				if(mesh.isBoundary(*vviter))
				{
					tem1+=(*vviter)->point()(0);
					tem2+=(*vviter)->point()(1);
					tem3+=(*vviter)->point()(2);
					j++;
				}
			}
			x=tem1/8.0 + oldv->point()(0)*(3.0/4.0);
			y=tem2/8.0 + oldv->point()(1)*(3.0/4.0);
			z=tem3/8.0 + oldv->point()(2)*(3.0/4.0);
		}
		else
		{
			tem1=tem2=tem3=0;
			for(j=0;!vviter.end();++vviter)
			{
				tem1+=(*vviter)->point()(0);
				tem2+=(*vviter)->point()(1);
				tem3+=(*vviter)->point()(2);
				j++;
			}
			//printf("%d\n",j);
			if(j>3)
				alpha=(1.0/j)*((5.0/8.0)-pow(((3.0/8.0)+(1.0/4.0)*(cos(2*PI/j))),2));
			else if(j==3)
				alpha=3/16;

			x=((1-j*alpha)*oldv->point()(0)) + (alpha*(tem1));
			y=((1-j*alpha)*oldv->point()(1)) + (alpha*(tem2));;
			z=((1-j*alpha)*oldv->point()(2)) + (alpha*(tem3));;
		}
		oldv->point()=Point(x,y,z);
		//printf("\n%f %f %f %f-> ",x,y,z, (1.0/4.0)*cos(2*PI/j));
		//printf("\n%f %f %f -> ",oldv->point()(0),oldv->point()(1),oldv->point()(2));
	}
	
	
	
	


	// Write out the resultant obj file
	int vObjID = 1;
	std::map<int, int> vidToObjID;

	std::ofstream os(argv[2]);

	SolidVertexIterator iter(&mesh);

	for(; !iter.end(); ++iter)
	{
		Vertex *v = *iter;
		Point p = v->point();
		os << "v " << p[0] << " " << p[1] << " " << p[2] << std::endl;
		vidToObjID[v->id()] = vObjID++;
	}
	os << "# " << (unsigned int)mesh.numVertices() << " vertices" << std::endl;

	float u = 0.0, v = 0.0;
	for(iter.reset(); !iter.end(); ++iter)
	{
		Vertex *vv = *iter;
		std::string key( "uv" );
		std::string s = Trait::getTraitValue (vv->string(), key );
		if( s.length() > 0 )
		{
			sscanf( s.c_str (), "%f %f", &u, &v );
		}
		os << "vt " << u << " " << v << std::endl;
	}
	os << "# " << (unsigned int)mesh.numVertices() << " texture coordinates" << std::endl;

	SolidFaceIterator fiter(&mesh);
	for(; !fiter.end(); ++fiter)
	{
		Face *f = *fiter;
		FaceVertexIterator viter(f);
		os << "f " ;
		for(; !viter.end(); ++viter)
		{
			Vertex *v = *viter;
			os << vidToObjID[v->id()] << "/" << vidToObjID[v->id()] << " ";
		}
		os << std::endl;
	}
	os.close();
	return 0;
}

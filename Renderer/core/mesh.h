#ifndef MESH_H
#define MESH_H

#include <vector>
#include "geometry.h"

using std::vector;

class Vertex
{
public:
	Point3d position_;
	Vector2d texCoord_;
	Vector3d normal_;


};

class Edge
{
public:

private:
	int vertex_indics[2];
};

//default as triangle face
class Face
{
public:
	Face(int index1, int index2, int index3);
	~Face();

	int* indics() { return &vertex_indics[0]; }

private:
	int vertex_indics[3];
	Vector3d normal_;
};

class Mesh
{
public:
	Mesh();
	~Mesh();

	int vertex_num() const;
	int face_num() const;

private:
	std::vector<Vertex> vertics_;
	//std::vector<Edge> edges_;
	std::vector<Face> faces_;
};

#endif

#ifndef SCENE_H
#define SCENE_H

#include <vector>

class Model;

using std::vector;

class Scene
{
public:
	Scene();
	~Scene();

private:
	vector<Model* > models_;
	//Color bgColor_;
	//Model* skybox_;
	//vector<Light* > lights_;
	//Camera* camera_;
};

#endif

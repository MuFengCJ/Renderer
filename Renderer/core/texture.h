#ifndef TEXTURE_H
#define TEXTURE_H

#include <vector>

class Color;

using std::vector;

class Texture
{
public:

private:
	int width_;
	int height_;
	vector<vector<Color>> texData_;
};

#endif

#include "color.h"

Color::Color(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}


Color Color::White = Color(1, 1, 1, 1);
Color Color::Black = Color(0, 0, 0, 1);
Color Color::Red = Color(1, 0, 0, 1);
Color Color::Cyan = Color(0, 1, 1, 1);
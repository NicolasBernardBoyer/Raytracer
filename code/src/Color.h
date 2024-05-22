#ifndef COLOR_H
#define COLOR_H

#include "Vec3.h"

#include <iostream>

using color = Vec3;

inline double linearToGamma(double linearComponent){
	return sqrt(linearComponent);
}

/*
void writeColor(std::ostream& out, color pixelColor)
{
	// Write the translated [0,255] value of each color component
	out << static_cast<int>(255.999 * pixelColor.x()) << ' '
		<< static_cast<int>(255.999 * pixelColor.y()) << ' '
		<< static_cast<int>(255.999 * pixelColor.z()) << '\n';
}
*/

#endif


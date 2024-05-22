#ifndef LIGHTING_H
#define LIGHTING_H

#include "HelperFunctions.h"

struct Lighting
{
    Point3 diffuse;
    Point3 specular;
    Point3 ambient;
};

struct AreaLight
{
    Rectangle area;
    Point3 center;
    Point3 diffuseLight;
    Point3 specularLight;
    Point3 ambientIntensity;
};

struct PointLight
{
    Point3 pos;
    Point3 diffuseCol;
    Point3 specularCol;
    Point3 ambientCol;
    double diffusePower;
    double specularPower;
    double ambientPower;
    Point3 diffuseLight;
    Point3 specularLight;
    Point3 ambientIntensity;
    double phongCoefficient;
};


inline Lighting GetPointLight(PointLight light, Point3 pos3D, Vec3 viewDir, Vec3 normal, const Output& output)
{
    Lighting OUT;
    if (light.diffusePower > 0)
    {
        Point3 lightDir = light.pos - pos3D; //3D pos in space of surface
        double distance = lightDir.length();
        lightDir = unitVector(lightDir);
        distance = distance * distance;

        // Intensity of diffuse light.
        double NdotL = dot(normal, lightDir);

        // Calculate diffuse light factoring in light color, power and attenuation
        OUT.diffuse = light.diffuseLight * light.diffuseCol * light.diffusePower * NdotL;

        //Calculate the half vector between the light vector and the view vector.
		//This is typically slower than calculating the actual reflection vector
		// due to the normalize function's reciprocal square root
        Point3 V = unitVector(lightDir + viewDir);

        // Specular light intensity
        double NdotV =  pow(dot(normal, V), light.phongCoefficient);
        //double powerNdotV = pow(NdotV,light.phongCoefficient);

        // if (output.twosiderenderer) {
        //     if (NdotV < 0) {
        //         powerNdotV = -powerNdotV;
        //     }
        // }

        OUT.specular = light.specularLight * light.specularCol * light.specularPower * NdotV;
        OUT.ambient  = light.ambientIntensity * light.ambientCol * light.ambientPower;
    }
    return OUT;
}

// inline std::vector<Lighting> GetAreaLight(AreaLight light, Point3 pos3D, Vec3 viewDir, Vec3 normal)
// {

//     Lighting OUT;
//     if (light.diffusePower > 0)
//     {
//         Point3 lightDir = light.pos - pos3D; //3D pos in space of surface
//         double distance = lightDir.length();
//         lightDir = unitVector(lightDir);
//         distance = distance * distance;

//         // Intensity of diffuse light.
//         double NdotL = dot(normal, lightDir);

//         // Calculate diffuse light factoring in light color, power and attenuation
//         OUT.diffuse = light.diffuseLight * light.diffuseCol * light.diffusePower * NdotL;

//         //Calculate the half vector between the light vector and the view vector.
// 		//This is typically slower than calculating the actual reflection vector
// 		// due to the normalize function's reciprocal square root
//         Point3 V = unitVector(lightDir + viewDir);

//         // Specular light intensity
//         double NdotV = pow(dot(normal, V), light.phongCoefficient);

//         OUT.specular = light.specularLight * light.specularCol * light.specularPower * NdotV;
//         OUT.ambient  = light.ambientIntensity * light.ambientCol * light.ambientPower;
//     }
//     return OUT;
// }

#endif
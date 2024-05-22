#ifndef CAMERA_H
#define CAMERA_H

#include "HelperFunctions.h"

#include "Color.h"
#include "Hittable.h"
#include "Light.h"
#include "Output.h"
#include "Lighting.h"

#include <vector>
#include <math.h>

#include "../external/simpleppm.h"

class Camera {
    public:
        int dimy = 100;
        int dimx = 100;
        double aspectRatio = 1.0; // Ratio of image width over hiehgt
        int samplesPerPixel = 100;
        int maxBounces = 10; // max bounces
        color background;

        double fov = 90; // Vertical view angle
        Point3 lookAt = Point3(0,0,0);
        Point3 lookFrom = Point3(0,0,0);
        Vec3 vUp = Vec3(0,1,0);

    void renderScene(const HittableList& world, const std::vector<Output>& out, const std::vector<Light>& light) {
        fprintf(stderr, "\nStarting render...");
        for (const auto& output : out) {
            initialise(world, output, light);
            std::string filename = output.filename;

            std::vector<double> buffer(3 * dimx * dimy);
            for (int j = 0; j < dimy; ++j) {
                std::clog << "\rScanlines remaining: " << (dimy - j) << ' ' << std::flush;
                for (int i = 0; i < dimx; ++i) {
                    if (output.antialiasing == true || output.globalillum == true) {
                            // TODO: find a way to disable the antialiasing
                        // antialiasing begin
                        color pixelColor(0,0,0);
                        for (int sample = 0; sample < samplesPerPixel; ++sample) {
                            Ray newRay = getRay(i , j);
                            pixelColor += rayColor(newRay, maxBounces, world, output, light);
                        }
                        // Color gets divided by a fraction of pixels
                        auto r = pixelColor.x();
                        auto g = pixelColor.y();
                        auto b = pixelColor.z();

                        auto scale = 1.0 / samplesPerPixel;
                        r *= scale;
                        g *= scale;
                        b *= scale;

                        // Linear to Gamma tf
                        //r = linearToGamma(r);
                        //g = linearToGamma(g);
                        //b = linearToGamma(b);

                        // Translated values are written
                        buffer[3 * j * dimx + 3 * i + 0] = r;
                        buffer[3 * j * dimx + 3 * i + 1] = g;
                        buffer[3 * j * dimx + 3 * i + 2] = b;
                        // antialiasing end
                    } else {
                        // render without antialiasing
                        auto pixelCenter = pixel100Loc + (i * pixelDeltaU) + (j * pixelDeltaV);
                        auto rayDir = pixelCenter - cameraCenter;
                        Ray newRay(cameraCenter, rayDir);

                        color pixelColor = rayColor(newRay, maxBounces, world, output, light);

                        auto r = pixelColor.x();
                        auto g = pixelColor.y();
                        auto b = pixelColor.z();

                        buffer[3 * j * dimx + 3 * i + 0] = r;
                        buffer[3 * j * dimx + 3 * i + 1] = g;
                        buffer[3 * j * dimx + 3 * i + 2] = b;
                    }
                }
            }
            fprintf(stderr, "\nRender finished, saving ppm...");
            save_ppm(filename, buffer, dimx, dimy);
        }
    }

    private:
        Point3 cameraCenter;
        Point3 pixel100Loc;  // Location of 0,0
        Vec3    pixelDeltaU; // Right pixel offset
        Vec3    pixelDeltaV; // Below pixel offset
        Vec3    u, v, w;    // Camera frame basis vectors

        void initialise(const HittableList& world, const Output& output, const std::vector<Light>& light) {
            fprintf(stderr, "\nStarting initialisation...");

            aspectRatio = 1.0;
            dimx = output.size[0];
            dimy = output.size[1];
            samplesPerPixel = 100;

            fov = output.fov;
            lookFrom = output.center;
            lookAt = output.lookat;
            vUp = output.up;

            maxBounces = output.maxbounces;

            background = output.bkc;

            cameraCenter = lookFrom;

            if (output.raysperpixel.size() != 0){
                samplesPerPixel = output.raysperpixel[0];
            }

            // viewport dimensions
            // Camera
            auto focalDistance = lookAt.length();
            auto theta = degreesToRadians(fov);
            auto h = tan(theta/2);
            auto viewportHeight = 2 * h * focalDistance;
            auto viewportWidth = viewportHeight * (static_cast<double>(dimx)/dimy);

            // Camera coordinate frame
            w = unitVector(-lookAt);
            u = unitVector(cross(vUp, w));
            v = cross(w, u);

            // vector horizontal and vertical viewport edges
            Vec3 viewportU = viewportWidth * u;
            Vec3 viewportV = viewportHeight * -v;

            // horizontal and vertical delta vectors
            pixelDeltaU = viewportU / dimx;
            pixelDeltaV = viewportV / dimy;

            // Work our way from top left pixel to bottom right pixel
            auto viewportUpperLeft = cameraCenter - (focalDistance*w) - viewportU/2 - viewportV/2;
            pixel100Loc = viewportUpperLeft + 0.5*(pixelDeltaU + pixelDeltaV);
        }

        Ray getRay(int i, int j) const {
            // Gives random sampled camera ray for pixel located at i,j
            auto pixelCenter = pixel100Loc + (i * pixelDeltaU) + (j * pixelDeltaV);
            auto pixelSample = pixelCenter + pixelSampleSquare();

            auto rayOrigin = cameraCenter;
            auto rayDir = pixelSample - rayOrigin;

            return Ray(rayOrigin, rayDir);
        }

        Vec3 pixelSampleSquare() const {
            // Gives random point in square surrounding a pixel at origin
            auto px = -0.5 + randomDouble();
            auto py = -0.5 + randomDouble();
            return (px * pixelDeltaU) + (py * pixelDeltaV);
        }
        
    color rayColor(const Ray& r, int bounces, const HittableList& world, const Output& output, const std::vector<Light>& light) const {
        if (output.globalillum) {
            return renderGlobalIllum(r, bounces, world, output, light);
        } else {
            return renderLocalIllum(r, world, output, light);
        }
    }

    color renderLocalIllum(const Ray& r, const HittableList& world, const Output& output, const std::vector<Light>& light) const {
        // Compiler will detect stack smashing if world is not a HittableList
        HitRecord rec;
        HitRecord lightRec;

        if (output.twosiderenderer){
            rec.twoSideHit = true;
        } else { rec.twoSideHit = false; }

        color finalCol = color(0,0,0);
        bool hitAnything = false;
        double curKa = 0;
        color curAc = color(0,0,0);

        if (output.globalillum == false) {
            if (world.hit(r, Interval(0.001, infinity), rec)) {
                for (const auto& curLight : light) {
                    if (curLight.type=="point") {
                        // point light
                        PointLight ptLight;
                        Ray lightHit = Ray(rec.p, curLight.center - rec.p);

                        shared_ptr<Hittable> objPtr = rec.currentHit;
                        if (!objPtr) {
                            std::cout << "Pointer is null" << std::endl;
                            return background;
                        }
                        Hittable& obj = *objPtr;
                        if (obj.visible == true) {
                            if (!world.hit(lightHit, Interval(0.01, 0.999), lightRec)) {

                                if (light.empty()) {
                                    std::cout << "Light is empty" << std::endl;
                                    return background;
                                }

                                // only add light color if we are using the current light
                                if (curLight.use == true){
                                    ptLight.pos              = curLight.center;
                                    ptLight.diffuseCol       = obj.dc;
                                    ptLight.specularCol      = obj.sc;
                                    ptLight.ambientCol       = obj.ac;
                                    ptLight.diffusePower     = obj.kd;
                                    ptLight.specularPower    = obj.ks;
                                    ptLight.ambientPower     = obj.ka;
                                    ptLight.diffuseLight     = curLight.id;
                                    ptLight.specularLight    = curLight.is;
                                    ptLight.ambientIntensity = output.ai;
                                    ptLight.phongCoefficient = obj.pc;

                                    Lighting theLight = GetPointLight(ptLight, rec.p, -r.direction(), rec.normal, output);
                                    finalCol += theLight.diffuse + theLight.specular;
                                    hitAnything = true;
                                }

                                curAc = obj.ac;
                                curKa = obj.ka;
                            }
                        }
                    } else {
                        //area light
                        //fprintf(stderr, "area light");

                        shared_ptr<Hittable> objPtr = rec.currentHit;
                        if (!objPtr) {
                            std::cout << "Pointer is null" << std::endl;
                            return background;
                        }
                        Hittable& obj = *objPtr;

                        AreaLight area;

                        Rectangle newRect(curLight.p1, curLight.p2, curLight.p3, curLight.p4, obj.ac, obj.dc, obj.sc,
                                            obj.ka, obj.kd, obj.ks, obj.pc);

                        area.center = newRect.getCenter();
                        area.diffuseLight = curLight.id;
                        area.specularLight = curLight.is;
                        area.ambientIntensity = output.ai;

                        if (curLight.usecenter){
                            // do the center like you'd do a pointlight
                            PointLight ptLight;
                            Ray lightHit = Ray(rec.p, area.center - rec.p);

                            if (obj.visible == true) {
                                if (!world.hit(lightHit, Interval(0.01, 0.999), lightRec)) {
                                   
                                    if (light.empty()) {
                                        std::cout << "Light is empty" << std::endl;
                                        return background;
                                    }

                                    // only add light color if we are using the current light
                                    if (curLight.use == true){
                                        ptLight.pos              = area.center;
                                        ptLight.diffuseCol       = obj.dc;
                                        ptLight.specularCol      = obj.sc;
                                        ptLight.ambientCol       = obj.ac;
                                        ptLight.diffusePower     = obj.kd;
                                        ptLight.specularPower    = obj.ks;
                                        ptLight.ambientPower     = obj.ka;
                                        ptLight.diffuseLight     = curLight.id;
                                        ptLight.specularLight    = curLight.is;
                                        ptLight.ambientIntensity = output.ai;
                                        ptLight.phongCoefficient = obj.pc;
                                        
                                        Lighting theLight = GetPointLight(ptLight, rec.p, -r.direction(), rec.normal, output);
                                        finalCol += theLight.diffuse + theLight.specular;
                                        hitAnything = true;
                                    }

                                    curAc = obj.ac;
                                    curKa = obj.ka;
                                }
                            }
                        } else {
                            // Dont use center, render a proper area light
                            std::vector<Point3> samples;

                            for (int i = 0; i < curLight.n; i++) {
                                for (int j = 0; j < curLight.n; j++) {
                                double sampleU = (i + randomDouble()) / curLight.n;
                                double sampleV = (j + randomDouble()) / curLight.n;

                                Point3 curSample(0,0,0);
                                curSample[0] = newRect.p1.x() * (1 - sampleU) * (1 - sampleV) + newRect.p2.x() * sampleU * (1 - sampleV) + newRect.p3.x() * sampleU * sampleV + newRect.p4.x() * (1 - sampleU) * sampleV;
                                curSample[1] = newRect.p1.y() * (1 - sampleU) * (1 - sampleV) + newRect.p2.y() * sampleU * (1 - sampleV) + newRect.p3.y() * sampleU * sampleV + newRect.p4.y() * (1 - sampleU) * sampleV;
                                curSample[2] = newRect.p1.z() * (1 - sampleU) * (1 - sampleV) + newRect.p2.z() * sampleU * (1 - sampleV) + newRect.p3.z() * sampleU * sampleV + newRect.p4.z() * (1 - sampleU) * sampleV;

                                samples.push_back(curSample);
                                }
                            }
                            for (const auto& thisSample : samples){
                                // point light
                                PointLight ptLight;
                                Ray lightHit = Ray(rec.p, thisSample - rec.p);

                                if (obj.visible == true) {
                                    if (!world.hit(lightHit, Interval(0.01, 0.999), lightRec)) {

                                        if (light.empty()) {
                                            std::cout << "Light is empty" << std::endl;
                                            return background;
                                        }

                                        // only add light color if we are using the current light
                                        if (curLight.use == true){
                                            ptLight.pos              = thisSample;
                                            ptLight.diffuseCol       = newRect.dc;
                                            ptLight.specularCol      = newRect.sc;
                                            ptLight.ambientCol       = newRect.ac;
                                            ptLight.diffusePower     = newRect.kd/curLight.n;
                                            ptLight.specularPower    = newRect.ks/curLight.n;
                                            ptLight.ambientPower     = newRect.ka;
                                            ptLight.diffuseLight     = curLight.id/curLight.n;
                                            ptLight.specularLight    = curLight.is/curLight.n;
                                            ptLight.ambientIntensity = output.ai;
                                            ptLight.phongCoefficient = newRect.pc;

                                            Lighting theLight = GetPointLight(ptLight, rec.p, -r.direction(), rec.normal, output);
                                            finalCol += theLight.diffuse + theLight.specular;
                                            hitAnything = true;
                                        }

                                        curAc = newRect.ac;
                                        curKa = newRect.ka;
                                    }
                                }
                            }                        
                        }
                    }
                }
            }
        }

        finalCol += (output.ai * curAc * curKa);

        Interval toClamp(0, 1);
        double newX = toClamp.clamp(finalCol.x());
        double newY = toClamp.clamp(finalCol.y());
        double newZ = toClamp.clamp(finalCol.z());

        finalCol = color(newX, newY, newZ);

        if (hitAnything || world.hit(r, Interval(0.001, infinity), rec))
            return finalCol;

        return background;
    }

    color renderGlobalIllum(const Ray& r, int bounces, const HittableList& world, const Output& output, const std::vector<Light>& light) const {
        /*
        - path tracing
        - light goes from light in a straight line, when it reaches an object it bounces
        - one of the directions end at the eye
        - need to find all the lights that make it to the eye
        - start at the eye, with a random ray shooting through a specific pixel
        - after bouncing is done we need to send one last ray to the light
        - every bounce goes through the BRDF tells the reflective property of the material
        - how many bounces, and how do I know how to stop?
        Russian roulette technique:
            probability of a ray ending here (probTerminate)
            stop path = second last step before sending ray to the light
        - path starts at intensity, which is given.
        - BRDF is the function tells me if a ray is coming and hits something, how much energy (reflection is in the thing im using to reflect?)
        */
        HitRecord rec;
        HitRecord lightRec;
        double curKa = 0;
        color curAc = color(0,0,0);

        if (bounces <= 0)
            return color(0,0,0);


        if (world.hit(r, Interval(0.001, infinity), rec)) {
            Vec3 dir = rec.normal + randomUnitVector();

            shared_ptr<Hittable> objPtr = rec.currentHit;
            if (!objPtr) {
                std::cout << "Pointer is null" << std::endl;
                return background;
            }
            Hittable& obj = *objPtr;

            color emittance;

            for (const auto& curLight : light) {
                Point3 lightCenter;
                if (curLight.type=="point") {
                    lightCenter = curLight.center;
                } else {
                    Rectangle newRect(curLight.p1, curLight.p2, curLight.p3, curLight.p4, obj.ac, obj.dc, obj.sc,
                                            obj.ka, obj.kd, obj.ks, obj.pc);

                    lightCenter = newRect.getCenter();
                }
                // point light
                PointLight ptLight;
                Ray lightHit = Ray(rec.p, curLight.center - rec.p);
                
                if (obj.visible == true) {
                    if (!world.hit(lightHit, Interval(0.01, 0.999), lightRec)) {

                        if (light.empty()) {
                            std::cout << "Light is empty" << std::endl;
                            return background;
                        }

                        // only add light color if we are using the current light
                        if (curLight.use == true){
                            ptLight.pos              = curLight.center;
                            ptLight.diffuseCol       = obj.dc;
                            ptLight.specularCol      = obj.sc;
                            ptLight.ambientCol       = obj.ac;
                            ptLight.diffusePower     = obj.kd;
                            ptLight.specularPower    = obj.ks;
                            ptLight.ambientPower     = obj.ka;
                            ptLight.diffuseLight     = curLight.id;
                            ptLight.specularLight    = curLight.is;
                            ptLight.ambientIntensity = output.ai;
                            ptLight.phongCoefficient = obj.pc;

                            Lighting theLight = GetPointLight(ptLight, rec.p, -r.direction(), rec.normal, output);
                            emittance += theLight.diffuse + theLight.specular;
                        }

                        curAc = obj.ac;
                        curKa = obj.ka;   
                    }
                }
            }
            Ray scattered;

            color attenuation;
            color objCol = curAc;
            if (scatter(r, rec, attenuation, scattered, objCol))
                return attenuation * renderGlobalIllum(scattered, bounces-1, world, output, light);   
            return color(0,0,0);    
        }
        Vec3 unitDir = unitVector(r.direction());
        auto a = 0.5*(unitDir.y() + 1.0);


        return (1.0-a)*(light[0].center) + a*output.bkc;
    }

    bool scatter(const Ray& rIn, const HitRecord& record, color& attenuation, Ray& scattered, color& objCol) const {
        auto scatterDir = randCosDirection(record.normal);

        // catch scatter going into void
        if (scatterDir.nearZero())
            scatterDir = record.normal;

        scattered = Ray(record.p, scatterDir);
        attenuation = objCol;
        return true;
    }


};

#endif
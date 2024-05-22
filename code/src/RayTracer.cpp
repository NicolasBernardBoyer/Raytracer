#include "RayTracer.h"
#include <iostream>
#include <Eigen/Core>
#include <Eigen/Dense>

#include "../external/json.hpp"
#include "../external/simpleppm.h"

#include <fstream>

using namespace nlohmann;
using namespace std;

int test_json(nlohmann::json& j);

void RayTracer::parseWorld(nlohmann::json& j) {
    fprintf(stderr, "\nStarting World Parsing...");
    
    // use iterators to read-in array types
    for (auto itr = j["geometry"].begin(); itr!= j["geometry"].end(); itr++){
        std::string type;
        std::vector<std::vector<int>> transform = {{1,0,0,0},
                                                {0,1,0,0},
                                                {0,0,1,0},
                                                {0,0,0,1}};

        if(itr->contains("type")){
           // type = static_cast<std::string>((*itr)["type"]);
            type = (*itr)["type"].get<std::string>();
        } else {
            cout<<"Fatal error: geometry should always contain a type!!!"<<endl;
        }
        int i = 0;
        int j = 0;
        if(itr->contains("transform")){
           for (auto itr2 =(*itr)["transform"].begin(); itr2!= (*itr)["transform"].end(); itr2++){
                for (auto itr3 =(*itr2)["transform"].begin(); itr3!= (*itr2)["transform"].end(); itr3++){
                    if(i<3){
                        transform[i++][j] = (*itr3).get<double>();
                    } else {
                        cout<<"Warning: Too many entries in transform"<<endl;
                    }
                }
                j++;
           }
        } 

        if(type=="sphere"){
            Point3 centre(0,0,0);
            Point3 ac(0,0,0);
            Point3 dc(0,0,0);
            Point3 sc(0,0,0);
            double ka = 0.0;
            double kd = 0.0;
            double ks = 0.0;
            double pc = 0.0;
            double radius = 0.0;
            int i = 0;
            for (auto itr2 =(*itr)["centre"].begin(); itr2!= (*itr)["centre"].end(); itr2++){
                if(i<3){
                    centre[i++] = (*itr2).get<double>();
                } else {
                    cout<<"Warning: Too many entries in centre"<<endl;
                }
            }
            i = 0;
            for (auto itr2 =(*itr)["ac"].begin(); itr2!= (*itr)["ac"].end(); itr2++){
                if(i<3){
                    ac[i++] = (*itr2).get<double>();
                } else {
                    cout<<"Warning: Too many entries in ac"<<endl;
                }
            }
            i = 0;
            for (auto itr2 =(*itr)["dc"].begin(); itr2!= (*itr)["dc"].end(); itr2++){
                if(i<3){
                    dc[i++] = (*itr2).get<double>();
                } else {
                    cout<<"Warning: Too many entries in dc"<<endl;
                }
            }
            i = 0;
            for (auto itr2 =(*itr)["sc"].begin(); itr2!= (*itr)["sc"].end(); itr2++){
                if(i<3){
                    sc[i++] = (*itr2).get<double>();
                } else {
                    cout<<"Warning: Too many entries in sc"<<endl;
                }
            }
            ka = (*itr)["ka"].get<double>();
            kd = (*itr)["kd"].get<double>();
            ks = (*itr)["ks"].get<double>();
            pc = (*itr)["pc"].get<double>();
            radius = (*itr)["radius"].get<double>();

            // Sphere newSphere(centre, radius, ac, dc, sc, ka, kd, ks, pc);
            // spheres.push_back(newSphere);
            world.add(make_shared<Sphere>(centre, radius, ac, dc, sc, ka, kd, ks, pc));
        }
        if (type=="rectangle"){
            Point3 p1(0,0,0);
            Point3 p2(0,0,0);
            Point3 p3(0,0,0);
            Point3 p4(0,0,0);

            Point3 ac(0,0,0);
            Point3 dc(0,0,0);
            Point3 sc(0,0,0);

            double ka = 0.0;
            double kd = 0.0;
            double ks = 0.0;
            double pc = 0.0;
            
            int i = 0;
            for (auto itr3 =(*itr)["p1"].begin(); itr3!= (*itr)["p1"].end(); itr3++){
                if(i<3){
                    p1[i++] = (*itr3).get<double>();
                } else {
                    cout<<"Warning: Too many entries in p1"<<endl;
                }
            }
            i = 0;
            for (auto itr3 =(*itr)["p2"].begin(); itr3!= (*itr)["p2"].end(); itr3++){
                if(i<3){
                    p2[i++] = (*itr3).get<double>();
                } else {
                    cout<<"Warning: Too many entries in p2"<<endl;
                }
            }
            i = 0;
            for (auto itr3 =(*itr)["p3"].begin(); itr3!= (*itr)["p3"].end(); itr3++){
                if(i<3){
                    p3[i++] = (*itr3).get<double>();
                } else {
                    cout<<"Warning: Too many entries in p3"<<endl;
                }
            }
            i = 0;
            for (auto itr3 =(*itr)["p4"].begin(); itr3!= (*itr)["p4"].end(); itr3++){
                if(i<3){
                    p4[i++] = (*itr3).get<double>();
                } else {
                    cout<<"Warning: Too many entries in p4"<<endl;
                }
            }
            i = 0;
            for (auto itr3 =(*itr)["ac"].begin(); itr3!= (*itr)["ac"].end(); itr3++){
                if(i<3){
                    ac[i++] = (*itr3).get<double>();
                } else {
                    cout<<"Warning: Too many entries in ac"<<endl;
                }
            }
            i = 0;
            for (auto itr3 =(*itr)["dc"].begin(); itr3!= (*itr)["dc"].end(); itr3++){
                if(i<3){
                    dc[i++] = (*itr3).get<double>();
                } else {
                    cout<<"Warning: Too many entries in dc"<<endl;
                }
            }
            i = 0;
            for (auto itr3 =(*itr)["sc"].begin(); itr3!= (*itr)["sc"].end(); itr3++){
                if(i<3){
                    sc[i++] = (*itr3).get<double>();
                } else {
                    cout<<"Warning: Too many entries in sc"<<endl;
                }
            }
            ka = (*itr)["ka"].get<double>();
            kd = (*itr)["kd"].get<double>();
            ks = (*itr)["ks"].get<double>();
            pc = (*itr)["pc"].get<double>();

            //auto rectMaterial = make_shared<Diffuse>(color(0.8, 0.8, 0.0));
            // Rectangle newRect(p1, p2, p3, p4, ac, dc, sc, ka, kd, ks, pc);
            // rectangles.push_back(newRect);
            world.add(make_shared<Rectangle>(p1, p2, p3, p4, ac, dc, sc, ka, kd, ks, pc));
        }
        
    } 
    fprintf(stderr, "\nWorld Parsed.");
}

void RayTracer::parseLight(nlohmann::json& j) {   
    fprintf(stderr, "\nStarting Light Parsing...");
    Point3 id(0,0,0);
    Point3 is(0,0,0);
    Point3 center(0,0,0);

    Point3 p1(0,0,0);
    Point3 p2(0,0,0);
    Point3 p3(0,0,0);
    Point3 p4(0,0,0);

    unsigned int n = 0;
    bool use = true;
    bool usecenter = false;

    std::vector<std::vector<int>> transform = {{1,0,0,0},
                                                {0,1,0,0},
                                                {0,0,1,0},
                                                {0,0,0,1}};

    for (auto itr = j["light"].begin(); itr!= j["light"].end(); itr++){
        std::string type;
        if(itr->contains("type")){
            type = (*itr)["type"].get<std::string>();
        } else {
            cout<<"Fatal error: light should always contain a type!!!"<<endl;
        }
        // optional members
        if(itr->contains("n")){
            n = (*itr)["n"].get<int>();
        }
        if(itr->contains("use")){
            use = (*itr)["use"].get<bool>();
        }
        int i = 0;
        int j = 0;
        if(itr->contains("transform")){
           for (auto itr2 =(*itr)["transform"].begin(); itr2!= (*itr)["transform"].end(); itr2++){
                for (auto itr3 =(*itr2)["transform"].begin(); itr3!= (*itr2)["transform"].end(); itr3++){
                    if(i<3){
                        transform[i++][j] = (*itr3).get<double>();
                    } else {
                        cout<<"Warning: Too many entries in transform"<<endl;
                    }
                }
                j++;
           }
        } 
        // Get required variables for point
        if (type == "point") {
            i = 0;
            for (auto itr2 =(*itr)["centre"].begin(); itr2!= (*itr)["centre"].end(); itr2++){
                if(i<3){
                    center[i++] = (*itr2).get<double>();
                } else {
                    cout<<"Warning: Too many entries in id"<<endl;
                }
            }
        // Get required variables for area
        } else if (type == "area") {
            if(itr->contains("usecenter")){
                usecenter = (*itr)["usecenter"].get<bool>();
            }
            int i = 0;
            for (auto itr3 =(*itr)["p1"].begin(); itr3!= (*itr)["p1"].end(); itr3++){
                if(i<3){
                    p1[i++] = (*itr3).get<double>();
                } else {
                    cout<<"Warning: Too many entries in p1"<<endl;
                }
            }
            i = 0;
            for (auto itr3 =(*itr)["p2"].begin(); itr3!= (*itr)["p2"].end(); itr3++){
                if(i<3){
                    p2[i++] = (*itr3).get<double>();
                } else {
                    cout<<"Warning: Too many entries in p2"<<endl;
                }
            }
            i = 0;
            for (auto itr3 =(*itr)["p3"].begin(); itr3!= (*itr)["p3"].end(); itr3++){
                if(i<3){
                    p3[i++] = (*itr3).get<double>();
                } else {
                    cout<<"Warning: Too many entries in p3"<<endl;
                }
            }
            i = 0;
            for (auto itr3 =(*itr)["p4"].begin(); itr3!= (*itr)["p4"].end(); itr3++){
                if(i<3){
                    p4[i++] = (*itr3).get<double>();
                } else {
                    cout<<"Warning: Too many entries in p4"<<endl;
                }
            }
        }
        // rest of variables are generic
        i = 0;
        for (auto itr2 =(*itr)["id"].begin(); itr2!= (*itr)["id"].end(); itr2++){
            if(i<3){
                id[i++] = (*itr2).get<double>();
            } else {
                cout<<"Warning: Too many entries in id"<<endl;
            }
        }
        i = 0;
        for (auto itr2 =(*itr)["is"].begin(); itr2!= (*itr)["is"].end(); itr2++){
            if(i<3){
                is[i++] = (*itr2).get<double>();
            } else {
                cout<<"Warning: Too many entries in is"<<endl;
            }
        }
        Light newLight(type, id, is, center, p1, p2, p3, p4, transform, n, usecenter);
        light.push_back(newLight);
    }
    fprintf(stderr, "\nLight Parsed.");
}


void RayTracer::parseOutput(nlohmann::json& j) {
    fprintf(stderr, "\nStarting Output Parsing...");

    // Output params
    std::string filename = "";
    vector<int> size = {0,0};
    Point3 lookat(0,0,0), up(0,0,0), centre(0,0,0);
    Point3 ai(0,0,0), bkc(0,0,0);
    double fov = 0.0;

    // Optional members
    bool globalillum = false;
    std::vector<unsigned int> raysperpixel = {0,0,0};
    bool twosiderender = false;
    bool antialiasing = false;

    // other
    double probterminate = 0;
    int maxbounces = 10;

    // use iterators to read-in arRay types
    for (auto itr = j["output"].begin(); itr!= j["output"].end(); itr++){
        if(itr->contains("filename")){
          //  filename = static_cast<std::string>((*itr)["filename"]);
            filename = (*itr)["filename"].get<std::string>();
        } else {
            cout<<"Fatal error: output shoudl always contain a filename!!!"<<endl;
        }
        int i = 0;
        for (auto itr2 =(*itr)["size"].begin(); itr2!= (*itr)["size"].end(); itr2++){
            if(i<2){
                size[i++] = (*itr2).get<int>();
            } else {
                cout<<"Warning: Too many entries in size"<<endl;
            }
        }
        
        i = 0;
        for (auto itr2 =(*itr)["centre"].begin(); itr2!= (*itr)["centre"].end(); itr2++){
            if(i<3){
                centre[i++] = (*itr2).get<double>();
            } else {
                cout<<"Warning: Too many entries in centre"<<endl;
            }
        }
        i = 0;
        for (auto itr2 =(*itr)["up"].begin(); itr2!= (*itr)["up"].end(); itr2++){
            if(i<3){
                up[i++] = (*itr2).get<double>();
            } else {
                cout<<"Warning: Too many entries in up"<<endl;
            }
        }
        i = 0;
        for (auto itr2 =(*itr)["lookat"].begin(); itr2!= (*itr)["lookat"].end(); itr2++){
            if(i<3){
                lookat[i++] = (*itr2).get<double>();
            } else {
                cout<<"Warning: Too many entries in lookat"<<endl;
            }
        }

        i = 0;
        for (auto itr2 =(*itr)["ai"].begin(); itr2!= (*itr)["ai"].end(); itr2++){
            if(i<3){
                ai[i++] = (*itr2).get<double>();
            } else {
                cout<<"Warning: Too many entries in ai"<<endl;
            }
        }
        i = 0;
        for (auto itr2 =(*itr)["bkc"].begin(); itr2!= (*itr)["bkc"].end(); itr2++){
            if(i<3){
                bkc[i++] = (*itr2).get<double>();
            } else {
                cout<<"Warning: Too many entries in bkc"<<endl;
            }
        }
        fov = (*itr)["fov"].get<int>();

        //Optional members
        i = 0;
        if(itr->contains("raysperpixel")){
            for (auto itr2 =(*itr)["raysperpixel"].begin(); itr2!= (*itr)["raysperpixel"].end(); itr2++){
                if(i<3){
                    raysperpixel[i++] = (*itr2).get<int>();
                } else {
                    cout<<"Warning: Too many entries in raysperpixel"<<endl;
                }
            }
        }
        if(itr->contains("globalillum")){
            globalillum = (*itr)["globalillum"].get<bool>();
        }
        if(itr->contains("twosiderender")){
            twosiderender = (*itr)["twosiderender"].get<bool>();
        }
        if(itr->contains("antialiasing")){
            antialiasing = (*itr)["antialiasing"].get<bool>();
        }
        if(itr->contains("maxbounces")){
            antialiasing = (*itr)["maxbounces"].get<int>();
        }
        if(itr->contains("probterminate")){
            antialiasing = (*itr)["probterminate"].get<double>();
        }

        Output output(filename, size, lookat, up, fov, centre, ai, bkc, raysperpixel, globalillum, twosiderender, 
        antialiasing, maxbounces, probterminate);
        out.push_back(output);
    }
    
    fprintf(stderr, "\nOutput Parsed.");

}

RayTracer::RayTracer(nlohmann::json& j) {
    // Image
    parseOutput(j);


    // World
    parseWorld(j);

    // Light
    parseLight(j);
    
    //world = HittableList(make_shared<bvhNode>(world));
}

void RayTracer::run() {
    fprintf(stderr, "\nStarting Raytracer::run()");
    Camera cam;

    cam.renderScene(world, out, light);
    fprintf(stderr, "\nSolution complete.\n");
}
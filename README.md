**RAYTRACER PROJECT**

This was a project created in my fourth semester as part of a Computer Graphics Project. It needs to be run on a specific docker image to function correctly.

Use the docker image found in the following link, and follow the instructions: 
https://github.com/tiperiu/COMP371_dockerimage

**HOW TO SET UP**

Here's what you have to do:
- Clone the this repository
- Clone the docker image
- Start up docker
- go in a command line, navigate to the folder for the docker image and type ./windows/interactive_run.ps1 "*Path to raytracer here*"
- Now that you are in the docker container, navigate to the build folder of the raytracer (./code/build)
- run cmake with the following command: cmake ../
- then run the following command: make
- This creates the build files for the raytracer
- You can now parse a json file by running the raytracer as such: ./raytracer *insert path of the json file name here*
- The json files are normally located in the assets folder.

**PPM FILE LOGIC**
![image](https://github.com/NicolassBoyer/Raytracer/assets/77691659/343790f4-6dff-4305-8281-ad258448d387)
Mandatory to have at least one geometry, one light and one output

![image](https://github.com/NicolassBoyer/Raytracer/assets/77691659/78ca748d-7a21-4fc0-a5e8-1c638a322c66)
![image](https://github.com/NicolassBoyer/Raytracer/assets/77691659/76d6f995-9575-45ef-a277-d776f2d2a71c)
![image](https://github.com/NicolassBoyer/Raytracer/assets/77691659/d45a5e2c-5dff-4dcf-84ea-195d0c947cb0)
![image](https://github.com/NicolassBoyer/Raytracer/assets/77691659/33393b05-75c0-4be9-81b4-a2c0b116edb2)

The Raytracer supports Direct Illumination, Path tracing (without lights), anti-aliasing, two-sided rendering, and sphere and rectangles primitives

**PROFESSOR'S INSTRUCTIONS**
These are the instructions in order to run the raytracer provided by my professor (credits to Tiberiu Popa) (you can also find this in the code folder):
"
This code is used for three purposes:

1) Build the official course solution
2) Build the student solution 
3) Build a dummy application that showcases some of the codebase.

Each build is controlled by two defines:
COURSE_SOLUTION and STUDENT_SOLUTION.

If none of these variables are defined the dummy code is built. This is the default behaviour.
If COURSE_SOLUTION is defined, the code expects the course solution (this should never be defined)
IF STUDENT_SOLUTION is defined the code expects a student solution is implemented in the src folder. Have a look at the main.cpp file for the expected interface.

You can switch on and off these defines in the CMakeLists.txt file. 


Building the dummy test

1) Create build folder: mkdir build
2) cd build
3) cmake ../
4) make
5) ./raytracer

Building the solution:
1) Uncomment the #add_compile_options(-DSTUDENT_SOLUTION) line in the CMakeLists.txt 
2) Create build folder: mkdir build
3) Create the src folder: mkdir src
3) cd build
3) cmake ../
4) make
5) ./raytracer <filename.json>


Note that some test scenes are provided in the assets folder. You can do a soft link to the assets folder in the build folder for your convenience.
"



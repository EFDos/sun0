# SUN-0

Sun-0 is the latest Open Source iteration of in-house engines developed for Sinz, to support the creation of 2D graphics creative code, and of course, 3D graphics in the near future.
The engine is built only upon open source technologies to target as many platforms as possible - including Haiku-OS - and is distributed as Free Software under the GPL License.

![alt text](res/screenshots/physics-demo-elementary.png?raw=true "Physics Demo on Linux Elementary OS")
![alt text](res/screenshots/lighting-demo-elementary.png?raw=true "Lighting Demo on Linux Elementary OS")
![alt text](res/screenshots/hello-demo-haiku.png?raw=true "Hello Demo on Haiku OS!")

## What is this repository for?

* Main Development of the Sun-0 Engine by its' authors
* Development of new features by the community, under the authors supervision
[Version 1.2.0]

## How do I get set up?

You'll need Meson, a C++17 compliant compiler and the development files for the following libraries:
OpenGL, GLEW, SDL2, FreeImage, Freetype, and LuaJIT.
Box2D will be provided by this repository, as we use a specific version tweaked to handle one-way-collisions.
Configuration
For now our Meson files are only configured to build on Linux.
To configure, create a build directory and then run:
`$ meson build`
If all dependencies are found you'll be ready to build.
There are optional build parameters, for instance:

`$ meson -Dbuild-demos=true build`
for building the demo projects.

Then you only need to get into the new build/ directory and run
`$ ninja`

## Dependencies

### Windows
You'll need to download the libraries through the official sites. [Specific steps will be written in the future]

### Linux
You can download almost all you need to get running through repositories.
Debian based:
`$ sudo apt-get install libsdl2-dev libglew-dev libfreetype6-dev libfreeimage-dev libluajit5.1-dev`

### Haiku
You can also download all dependencies through pkgman.

## Contribution guidelines
To be written.

## Who do I talk to?
E-mails: douglas.muratore@gmail.com or cassio.raposa@gmail.com
Site: http://sinz.com.br/

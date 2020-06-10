============
Installation
============

As one of the main pourposes of UFJF-MLTK is the easy of use, the compilation and installation
can't be different, for that the project was made using ``cmake`` as most of the known open source
projects.

Requirements
------------

* CMake
* C++ compiler with support to C++17
* Gnuplot >= 5 (Optional, but needed for the Visualization module)

Command to install all necessary CLI dependencies on Arch linux:

.. code-block:: 

    sudo pacman -Sy gnuplot


Requirements for the graphical interface (Optional)
---------------------------------------------------

* Qt5
* Qt5::Core
* Qt5::Widgets
* Qt5::Gui
* Qt5::DataVisualization
* Qt5::Svg
* Qt5::Charts

Command to install all necessary GUI dependencies on Arch linux:

.. code-block:: 

    sudo pacman -Sy qt5-base qt5-charts qt5-datavis3d qt5-svg


Build on any system
-------------------

The project can be compiled using the same commands in any system, the only difference is that on
``Windows`` you'll need to make sure that the folder containing UFJF-MLTK is in your include path, so
you can use include statements as ``#include <UFJF-MLTK/Core/Data.hpp>``. For the standart instalation
you only need to execute the following commands:

.. code-block:: 

    mkdir build
    cd build
    cmake ..
    cmake --build . --target install

``CMake`` can produce build systems for several compilers, if you have more than one C++ compiler in your 
operational system you can especify which one you want to use by adding the flag -G to CMake, for example,
if you want to configure the project for Visual Studio, you could execute the commanda as ``cmake .. -G "Visual Studio 16 2019"``
and then open the generated project on it.

UFJF-MLTK was projected to be as modular as possible, so if you don't want to compile some module, you could just
turn off it's configuration on cmake, keeping in mid that it would be compiled in the same way if it's a dependency
for another module to be compiled. The available options to be set on cmake are listed below:

+-----------------------+---------------+-----------------------------------------------------+
|      CMake option     | Default value |                     Description                     |
+-----------------------+---------------+-----------------------------------------------------+
| -DBUILD_LIBVISUALIZE  |       ON      |   Tells if the visualization module must be built   |
+-----------------------+---------------+-----------------------------------------------------+
| -DBUILD_LIBCLASSIFIER |       ON      |     Tells if the classifier module must be built    |
+-----------------------+---------------+-----------------------------------------------------+
| -DBUILD_LIBREGRESSOR  |       ON      |     Tells if the regressor module must be built     |
+-----------------------+---------------+-----------------------------------------------------+
| -DBUILD_LIBCLUSTERER  |       ON      |     Tells if the clusterer module must be built     |
+-----------------------+---------------+-----------------------------------------------------+
| -DBUILD_LIBFEATSELECT |       ON      | Tells if the feature selection module must be built |
+-----------------------+---------------+-----------------------------------------------------+
| -DBUILD_LIBVALIDATION |       ON      |     Tells if the validation module must be built    |
+-----------------------+---------------+-----------------------------------------------------+
| -DBUILD_CLI           |       ON      |    Tells if the console application must be built   |
+-----------------------+---------------+-----------------------------------------------------+
| -DBUILD_GUI           |      OFF      |    Tells if the Qt GUI application must be built    |
+-----------------------+---------------+-----------------------------------------------------+


With the libraries compiled and installed on the system you only need to add the UFJF-MLTK flag to
the compiler to link the libraries to your program. Supose that we want to compile a source called
``foo.cpp`` containing a main function, to compile it on the command line, you just need to add the
flag ``-lufjfmltk``, as follows:

.. code-block:: 

    g++ foo.cpp -o foo -lufjfmltk

With these steps complete, everything is set up and ready to use! 

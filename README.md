## TNM094 Labs - Particle System

![Contributors](https://img.shields.io/badge/contributors-3-green)

## Authors: Marcus Skoglund & Anton Martic

A minimal particle system acting as a foundation for the course labs.
A small graphical interface is provided for visually editing parameters.

#### UML diagrams
[Click here and them open in draw.io for diagrams](https://drive.google.com/file/d/1VYjRpuxV53BEmxqkvz2iq4STdPoqGKrt/view?usp=sharing)

#### Folder structure

- /include: Header files
- /src: Cpp files
- /unittest: Examples of

#### Setup instructions

Dependencies:

- [CMake](https://cmake.org/download/) For cross-platform compiler project generation.
- [Vcpkg](https://github.com/microsoft/vcpkg) For dependency management
- C++20 Required, e.g. [Visual Studio](https://visualstudio.microsoft.com/downloads/)

1.  Clone the project

2.  In the main project folder **_ParticleSystem/_**:

    _Open up the console and run:_

    ```bash
    git clone https://github.com/microsoft/vcpkg
    ```

3.  Open CMake (we recommend using the GUI here), enter the source path to the particlesystem git repo
    and select one of the "presets" ("MSVC 2022", "Xcode", "Ninja", "Unix Makefiles")
    and hit configure. This will build and the dependencies using vcpkg and make them available to
    the project. Then the will be configured

4.  Hit Generate and then Open Project to open the project in your IDE.

5.  Build and run the 'application' executable.

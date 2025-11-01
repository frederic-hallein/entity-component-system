# ECS

This project is a minimal implementation of the Entity-Component-System (ECS) architectural pattern in C++. It is created primarily for learning and experimentation, providing a simple and clear structure for understanding how ECS works.


## Requirements

- CMake >= 3.28.3
- C++20 compatible compiler (GCC, Clang, MSVC)
- OpenGL development libraries
- Linux (X11) recommended; other platforms may require changes

All other dependencies (GLFW, GLAD, yaml-cpp, ImGui, GoogleTest) are included in the repository.

## Running the Project

This section explains how to build and run the ECS application after setting up the repository and its dependencies. Follow the steps below to compile the code and launch the executable.

### Installation / Setup


To install the project, follow these steps:

1. **Clone the repository**

   Open a terminal and run:
   ```sh
   git clone https://github.com/frederic-hallein/entity-component-system.git
   ```

2. **Navigate to the project directory**

    ```sh
    cd ecs
    ```

### Build

To build the project, follow these steps:

1. **Create a build directory**

    This keeps build files separate from your source files.
    ```sh
    mkdir -p build
    ```

2. **Configure the project with CMake**

    Generate the build system files for your platform.

    ```sh
    cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
    ```

3. **Build the project**

    Compile the source code and link the executable.

    ```sh
    cmake --build build
    ```

The main executable will be located at ```build/ecs```.

### Run

To execute the binary:

```sh
./build/ecs
```

## Testing

To run all tests, use the command:

```sh
./build/tests/ecs_tests
```

# Setting Up the Backend
*If you don't already have it installed, install CMake by following the instructions [here](https://cmake.org/download/)*
1. The first step to building the backend is to get **OpenCV** installed as a dynamic library on your system. This process is OS and architecture specific so follow one of the following tutorials to complete the process on your system. This [article](https://gist.github.com/nucliweb/b2a234c673221af5ec24508da7d8b854) and this [video](https://www.youtube.com/watch?v=KaTA-yK7dWA) shows how to install OpenCV on Mac Arm architecture; consult the [OpenCV Documentation](https://docs.opencv.org/3.4/d7/d9f/tutorial_linux_install.html) for more general instructions.
2. Next follow the instructions [here](https://github.com/drogonframework/drogon/wiki/ENG-02-Installation) to get **Drogon** installed on your system. Drogon is a C++ library that is used in this project for the REST API.
3. Once OpenCV and Drogon are installed, we want to clone and compile the [Quest Image Sequence Library](https://github.com/noahtq/quest_image_seq_lib/blob/main/README.md). This can be placed anywhere you wish other than the project directory.
```
$ cd <dir to place image sequence repo> && git clone https://github.com/noahtq/quest_image_seq_lib.git && cd quest_image_seq_lib
```
*Note: You may no need to compile the library from scratch. Check the downloads area of the Quest Image Sequence Library README to see if there are already precompiled binaries for your system.*

2. Next configure the CMake project
```
$ cmake .
```
*Note: If you are getting errors related to Google Benchmark open the projects main CMakeLists.txt file and comment out the line that adds benchmark as a subdirectory by appending a # sign to the beginning. `#add_subdirectory(Google_benchmark`. Google Benchmark, although useful, is only used to test performance for the image sequence library and as such is not essential. Follow [these](https://github.com/google/benchmark?tab=readme-ov-file#installation) instructions to setup Google Benchmark on your system.*

3. Create a build directory and navigate into it.
```
$ mkdir build && cd build
```
4. Compile the library
```
$ cmake --build .
```
5. Create a directory for the lib file in the Quest Keyer repo and copy the now compiled static library into that directory
```
$ mkdir <path to quest_keyer repo>/quest_keyer_backend/quest_keyer/lib && <path to quest_keyer repo>/quest_keyer_backend/quest_keyer/lib
```
6. Navigate to the Quest Keyer repo backend
```
$ cd <path to quest_keyer repo>/quest_keyer_backend
```
7. Configure the CMake Project
```
$ cmake .
```
8. Create a build directory and navigate into it
```
mkdir build && cd build
```
9. Compile the backend executable
```
cmake --build .
```
10. Run the executable
```
$ ./quest_keyer_backend
```
Congrats! The backend has now been successfully configured and is ready for development.
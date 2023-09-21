# Lane detection using sliding window

## Index
  - [About](#About) 
  - [Prerequisites](#Prerequisites) 
  - [Getting Started](#getting-started)
  - [License](#license)


## About
This project's purpose is to create Lane detection algorithm using sliding window method

![](./data/output_video.gif)

## Prerequisites

### CMake

### C++11

### OpenCV
```bash
wget -O opencv.zip https://github.com/opencv/opencv/archive/refs/tags/4.4.0.zip
unzip opencv.zip

root_dir=$(pwd)
echo ${root_dir}

mkdir -p ${root_dir}/install/release
mkdir -p ${root_dir}/install/debug

mkdir -p ${root_dir}/build/release
mkdir -p ${root_dir}/build/debug

cd ${root_dir}/build/release 
cmake -GNinja \
-DCMAKE_BUILD_TYPE=Release \
-DCMAKE_INSTALL_PREFIX=../../install/release/ \
../../opencv-4.4.0/

ninja
sudo ninja install
```

### Eigen3
```bash
wget -O eigen.zip https://gitlab.com/libeigen/eigen/-/archive/3.1/eigen-3.1.zip
unzip eigen.zip

root_dir=$(pwd)
echo ${root_dir}

mkdir -p ${root_dir}/install/release
mkdir -p ${root_dir}/install/debug

mkdir -p ${root_dir}/build/release
mkdir -p ${root_dir}/build/debug

cd ${root_dir}/build/release 
cmake -GNinja \
-DCMAKE_BUILD_TYPE=Release \
-DCMAKE_INSTALL_PREFIX=../../install/release/ \
../../eigen/

ninja
sudo ninja install
```

## Profiling
### CPU
![](./data/cpu.png)
### GPU
![](./data/gpu.png)


## License

```
MIT License

Copyright (c) 2020 always0ne

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```
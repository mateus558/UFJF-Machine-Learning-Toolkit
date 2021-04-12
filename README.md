# UFJF - Machine Learning Toolkit

[![Open Source Love](https://badges.frapsoft.com/os/v1/open-source.png?v=103)](https://github.com/ellerbrock/open-source-badges/) 
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Project Status: WIP – Initial development is in progress, but there has not yet been a stable, usable release suitable for the public.](https://www.repostatus.org/badges/latest/wip.svg)](https://www.repostatus.org/#wip)
![GitHub last commit](https://img.shields.io/github/last-commit/mateus558/UFJF-MLTK)
[![Build Status](https://travis-ci.org/mateus558/UFJF-Machine-Learning-Toolkit.svg?branch=main)](https://travis-ci.org/mateus558/UFJF-Machine-Learning-Toolkit)
[![Documentation Status](https://readthedocs.org/projects/ufjf-mltk/badge/?version=documentation)](https://ufjf-mltk.readthedocs.io/en/documentation/?badge=documentation)


## Objective
This project aims to provide researchers and developers basic tools for manipulation of datasets, implementation and test of ML algorithms
and some already implemented methods.<br />
It's not intended to be just a collection of algorithms, but also to auxiliate and create a pattern in future ML algorithms implementations
through a set of interconected modules that can be used in most ML projects.<br />

## Documentation
You can find the documentation at the project page: [UFJF-MLTK](https://mateus558.github.io/UFJF-MLTK/index.html). <br />
And for examples and other information you can access the [wiki](https://github.com/mateus558/Machine-Learning-Toolkit/wiki).

## Installation
In order to make the project available for the majority of users and to be cross-platform, the project was adapted to CMake and Meson,the most wide used build systems. Therefore, there are two install methods for the project that can be seen below.

**Requirements**
* meson or cmake
* g++ >= 7
* gnuplot >= 5 (only for visualization module)

**CMake**
```
mkdir build
cd build
cmake ..
make
sudo make install
```

**Meson**
```
meson build
meson compile -C build
meson install -C build
```
After that, the library will be available system wide and it can be used as any library.

## Usage Example

The framework is intended to make easier the usage of machine learning algorithms in C++, in the following example we output the 10-fold cross validation accuracy of the kNN algorithm with 3 neighbors, as we can see, we can do it with few lines of code.  

**main.cpp**
```cpp
#include <ufjfmltk/Core.hpp>
#include <ufjfmltk/Validation.hpp>
#include <ufjfmltk/Classifier.hpp>

int main(){
  mltk::Data<double> data("iris.data");
  mltk::classifier::KNNClassifier<double> knn(data, 3);

  std::cout << "Dataset size: " << data.size() << std::endl;
  std::cout << "Dataset dimension: " << data.dim() << std::endl;

  std::cout << "KNN accuracy: ";
  std::cout << 100.0 - mltk::validation::kfold(data, knn, 10, 42, 0) 
            << "%" << std::endl;
}
```
Compiling:
```
g++ main.cpp -o main -lufjfmltk
```

This program outputs the following:

```
Dataset size: 150
Dataset dimension: 4
KNN accuracy: 100%
```


## Modules status
* Data manipulation [![Generic badge](https://img.shields.io/badge/status-Ready-green.svg)](https://shields.io/)

* Data visualization [![Generic badge](https://img.shields.io/badge/status-Ready-green.svg)](https://shields.io/)
* Classifiers (Primal and Dual) [![Generic badge](https://img.shields.io/badge/status-Ready-green.svg)](https://shields.io/)
* Ensemble [![Generic badge](https://img.shields.io/badge/status-WIP-yellow.svg)](https://shields.io/)
* Regression [![Generic badge](https://img.shields.io/badge/status-WIP-yellow.svg)](https://shields.io/)
* Validation (K-Fold Cross-Validation) [![Generic badge](https://img.shields.io/badge/status-Ready-green.svg)](https://shields.io/)
* Feature Selection [![Generic badge](https://img.shields.io/badge/status-Ready-green.svg)](https://shields.io/)
* Documentation [![Generic badge](https://img.shields.io/badge/status-WIP-yellow.svg)](https://shields.io/)


## Authors
Mateus Coutinho Marim (mateus.marim@ice.ufjf.br) <br />
Saulo Moraes Villela (saulo.moraes@ufjf.edu.br)<br />
Alessandreia Marta de Oliveira Julio (alessandreia.oliveira@ice.ufjf.br)<br />

<p align="center">
  Universidade Federal de Juiz de Fora <br />
  Departamento de Ciência da Computação
</p>

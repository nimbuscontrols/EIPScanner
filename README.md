# EIPScanner

[![Build Status](https://travis-ci.com/nimbuscontrols/EIPScanner.svg?branch=master)](https://travis-ci.com/nimbuscontrols/EIPScanner)
![GitHub release (latest by date)](https://img.shields.io/github/v/release/nimbuscontrols/EIPScanner)


Free implementation of Ethernet/IP scanner in C++


## Features

* Explicit messaging
* Implicit messaging (only point-to-point)

CIP Standard objects:

* File Object (upload only)
* Parameter Object (read only)
* Identity Object

Vendor specific objects:

* RA DPI Fault Object 

## Requirements

* CMake 3.5 and higher
* C++14 compiler
* Linux or MacOS

## Installing

```shell
$ mkdir build && cd build
$ cmake ..
$ cmake --build . --target install
```

## Usage

To see how to work with the library, look into `examples` directory 

## Authors

Aleksey Timin

Adam Roth

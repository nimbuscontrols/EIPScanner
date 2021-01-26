# EIPScanner

[![Build Status](https://travis-ci.com/nimbuscontrols/EIPScanner.svg?branch=master)](https://travis-ci.com/nimbuscontrols/EIPScanner)
![GitHub release (latest by date)](https://img.shields.io/github/v/release/nimbuscontrols/EIPScanner)


Free implementation of Ethernet/IP scanner in C++.


## Features

* Explicit messaging
* Implicit messaging (only point-to-point)
* Discovery

CIP Standard objects:

* File Object (upload only)
* Parameter Object (read only)
* Identity Object

Vendor specific objects:

* RA DPI Fault Object 

## Requirements

* CMake 3.5 and higher
* C++14 compiler (tested with GCC and MinGW)
* Linux, MacOS, and Windows

## Installing

```shell
$ mkdir build && cd build
$ cmake ..
$ cmake --build . --target install
```

## Usage

To see how to work with the library, look into `examples` directory 

## Contribution

We welcome outside contributions for any improvements, features, or new devices (see below) to the library. Please create a pull request with unit/mock tests

## Vendor specific objects & devices

Our goal is to maintain an active device catalog of all EtherNet/IP devices and their objects that we encounter. We encourage others to contribute to this communal device catalog in /src/vendor/.

## Authors

Aleksey Timin

Adam Roth

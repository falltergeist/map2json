map2json [![Build Status](https://travis-ci.org/falltergeist/map2json.svg?branch=develop)](https://travis-ci.org/falltergeist/map2json)         
========
*.map to JSON converter for Fallout 1/2 data files

Installation:
=============

Ubuntu
------

```bash
sudo add-apt-repository ppa:falltergeist/falltergeist
sudo apt-get update
sudo apt-get install map2json
```

Compilation from source:
============

Dependencies:
-------------

- CMake >= 2.8

Build:

```bash
mkdir build && cd build && cmake .. && make
```

Usage:
======

```bash
./map2json PATH_TO_FILE.map
```

Converted file will be written to STDOUT

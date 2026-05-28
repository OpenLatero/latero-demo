# latero-demo

A program that makes use of the latero-graphics library and showcases the Latero device's functionality. See [OpenLatero.org](http://OpenLatero.org) for more information.

## Installation

### macOS

Tested on macOS Tahoe 26.3.1.

Install the latero and latero-graphics libraries, then:

```sh
# Build
cmake --preset default
cmake --build build

# Run
build/src/latero-demo
```

Or all together:

```sh
cmake --preset default && cmake --build build && build/src/latero-demo
```

**Create an app bundle** (currently broken):
```sh
../create_app_bundle.sh
```
You should then be able to launch the program by clicking on the latero-demo app bundle.

---

## Authors

Vincent Levesque <vleves@cim.mcgill.ca>

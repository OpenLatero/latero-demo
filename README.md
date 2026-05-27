# latero-demo

A program that makes use of the latero-graphics library and showcases the Latero device's functionality.

The Latero device is distributed by the not-for-profit organization Tactile Labs Inc. (http://tactilelabs.com).
It was previously named STReSS and was originally designed at the McGill Haptics Laboratory (http://www.cim.mcgill.ca/~haptic/).
Videos of the device in action, as well as a list of related publications, can be found [here](http://www.cim.mcgill.ca/~haptic/laterotactile/dev/stress/).
Please refer to Vincent Levesque's 2009 Ph.D. thesis "Virtual Display of Tactile Graphics and Braille by Lateral Skin Deformation" for an in-depth report on the design, implementation and experimental evaluation of multiple tactile renders developed specifically for the STReSS device: http://vlevesque.com/go/phd.html

This software is part of a set of libraries and programs, all under the terms of the GNU General Public License or the GNU Lesser General Public License. A partial list of related libraries and programs:

- **latero** — the low level driver for the Latero device
- **latero-gui** — a GUI that allows the individual testing of the actuators
- **latero-graphics** — a library for tactile rendering
- **latero-examples** — a set of example programs using the latero or latero-graphics libraries
- **latero-demo** — a program that makes use of the latero-graphics library and showcases the Latero device's functionality
- **latero-graphics-demo** — a program that makes use of the latero-graphics library and showcases the Latero device's functionality when mounted on a 2D slider (Tactograph)
- **latero-graphics-studio** — a program that allows editing of 2D tactile graphics rendered using the latero-graphics library

See [OpenLatero.org](http://OpenLatero.org) for more information.

---

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

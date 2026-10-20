# latero-demo

A program that makes use of the latero-graphics library and showcases the Latero device's functionality. See [OpenLatero.org](http://OpenLatero.org) for more information.

## Installation

Expand the instructions for your platform.

<details>
<summary><b>macOS</b></summary>

_Last tested on macOS 27.0._

#### Install OpenLatero libraries

Install the [latero](https://github.com/OpenLatero/latero) and [latero-graphics](https://github.com/OpenLatero/latero-graphics) libraries.

#### Install dependencies

Install the dependencies through [Homebrew](https://brew.sh/):

```
brew install boost
```

or through [MacPorts](https://www.macports.org):

```
sudo port install boost
```

#### Build and run the app

The app can be compiled with the following commands:

```
cmake --preset default
cmake --build build
```

and ran with this command:

```
build/src/latero-demo
```

or in a single command:

```
cmake --preset default && cmake --build build && build/src/latero-demo
```

#### Create an app bundle (optional)

An app bundle can be created with the following command:

```
cd build
../create_app_bundle.sh
```

The LateroDemo app bundle can then be found in the build folder. It can be clicked on to start the app or copied to the Applications folder like any other app.

</details>

<details>
<summary><b>Linux</b></summary>

_Last tested on Ubuntu 26.04._

#### Install OpenLatero libraries

Install the [latero](https://github.com/OpenLatero/latero) and [latero-graphics](https://github.com/OpenLatero/latero-graphics) libraries.

#### Install dependencies

Install the dependencies with:

```
sudo apt install libboost-program-options-dev
```

#### Build and run the app

The app can be compiled with the following commands:

```
cmake --preset default
cmake --build build
```

and ran with this command:

```
build/src/latero-demo
```

or in a single command:

```
cmake --preset default && cmake --build build && build/src/latero-demo
```

</details>

<details>
<summary><b>Windows</b></summary>

_Last tested on Windows 11._

On Windows, the app is built natively with the GCC toolchain of [MSYS2](https://www.msys2.org).

#### Install OpenLatero libraries

Install the [latero](https://github.com/OpenLatero/latero) and [latero-graphics](https://github.com/OpenLatero/latero-graphics)
libraries, following their Windows instructions. This also sets up MSYS2 and the build tools.

#### Install dependencies

Open the **MSYS2 UCRT64** shell and install the dependencies with the following command:

```
pacman -S --needed mingw-w64-ucrt-x86_64-boost
```

Run all the commands below from the same UCRT64 shell.

#### Build and run the app

The app can be compiled with the following commands:

```
cmake --preset default
cmake --build build
```

and ran with this command, from this folder so that the app finds its `Resources` folder:

```
build/src/latero-demo
```

or in a single command:

```
cmake --preset default && cmake --build build && build/src/latero-demo
```

</details>

## Authors

OpenLatero is maintained by [Vincent Levesque](https://vlevesque.com) and his Haptic User Experience research group at [École de technologie supérieure](https://etsmtl.ca). It was originally developped as part of his PhD thesis at [McGill University](https://mcgill.ca) and prepared for release as as open source project by Jerome Pasquero (<jerome.pasquero@gmail.com>). Please see the git history for a full list of contributors.

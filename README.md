## to set up

```sh
git clone git@github.com:cubicvoid/erachnid.git
cd erachnid
git submodule init
git submodule update
```

## to build

```sh
cmake -Bbuild -DCMAKE_BUILD_TYPE=Debug -DCOPY_AFTER_BUILD=TRUE

cmake --build build --config Debug
```

## to edit the gui

(apple only for now)

```sh
mkdir xcode
cmake -G Xcode -B build-xcode .
open build-xcode/erachnid.xcodeproj
```

this will (possibly after building in xcode) let you open the xib files for the UI and link them to internal objective c owner classes.
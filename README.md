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


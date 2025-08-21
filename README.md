# StorPool Tasks

## Repository layout:

- `analysis`              -- Analysis task solution
- `CMakeLists.txt`        -- Root cmake file
- `counting`              -- Counting task solution
- `doc`                   -- doc directory
- `Dockerfile`            -- Dockerfile
- `fizzbuzz`              -- FizzBuzz task solution
- `LICENSE`               -- license file
- `project_configure`     -- Configuration script
- `README.md`             -- Readme file
- `reverse_engineering`   -- Reverse engineering task
- `scripts`               -- Helper utilities

## Build

**Prerequisites**

* build-essential
* cmake

This is a project with a standard `cmake` build system anyway for convinience
to execute subsequently after the build script utilities have been added.

**Build & Run Tests**

```console
$ source project_configure
$ sp_project_build
$ sp_project_test
```

Alternativelly build & run the tests using the command:

```
docker build -t storpool:latest .
```

## Build Artefacts

The build artefacts are stored in the `build` directory under their coresponding names.

**build** directory:

```
build/
├── analysis
│    └── analysis
│
├── counting
│    └── counting
│
└── fizzbuzz
     └── fizzbuzz
```

More information regarding the solutions can be found [here](https://ilieviliya92.github.io/StorPool/)

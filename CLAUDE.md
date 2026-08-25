# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Overview

KMS-Framework is a cross-platform (Windows + Linux) C++ framework and toolset by KMS - Martin Dubois, P. Eng. It provides a layered set of static libraries (`KMS-A`, `KMS-B`, `KMS-C`) plus a collection of executables built on top of them (build tooling, file servers, Modbus utilities, a scope/plotting tool, an installer/sync tool, etc). There are also two JavaScript sub-projects (`front-end`, a React app; `kms-react`, a component library) and an `STM32` embedded sub-tree that reuses the same `Includes/KMS` headers under `_KMS_EMBEDDED_`.

Two build systems exist side by side and must be kept consistent:
- **Windows**: Visual Studio 2022 (`Solution.sln`, per-project `.vcxproj`), toolset v143.
- **Linux**: GNU `make` using per-project `makefile` + shared `Common.mk`.

## Library layering

`KMS-A` → `KMS-B` → `KMS-C`, each a static library with a matching `*-Test` executable. Higher libraries depend on lower ones; never introduce a reverse dependency.

- **KMS-A** — no OS networking/device dependency. Core utilities: `DI` (dynamic/introspectable value objects: `DI::Object`, `Array`, `Dictionary`, `String`, etc., used for config and serialization), `DI2` (newer/parallel DI variant), `Cfg` (`Configurator`/`MetaData` — declarative config binding), `CLI` (command line parsing, macros, tools), `Dbg` (`Log`, `Stats`), `File`, `Text`, `Thread`, `Modbus` (protocol core), `WOP` (a framebuffer/object protocol), `Build` (the `KMS-Build`/`KMS-Make` engine lives here), `Script`, `Test` (the unit test framework), `Embedded` (headers shared with STM32 firmware), `DAQ`, `Scope`, `Graph`/`WGDI` (Windows GDI bitmap/plotting).
- **KMS-B** — adds networking and process/browser control: `Net` (`Socket`, TLS client, `AddressRange`), `HTTP` (client, server, file server, React-app server), `Proc::Browser`, `Build::Import` (front-end import tooling).
- **KMS-C** — adds physical device / serial / driver access: `Com::Port`, `Dev::Device`/`Device2`, `Install::DriverInstaller`, Modbus over COM/USART, `Stream::Stream_Cfg`.

Executables (`KMS-Build`, `KMS-Import`, `KMS-Make`, `KMS-Sync`, `KMS-FileServer`, `KMS-Bitmap`, `KMS-Editor`, and the Tools-product binaries `KMS-ComTool`/`KMS-ModbusSim`/`KMS-ModbusTool`/`KMS-WOP`/`KMS-ByteTool`) link against these libraries per `Solution.sln`.

## Source file conventions

- File names mirror their primary class: `Namespace_ClassName.cpp` (e.g. `DI_Dictionary.cpp` → `KMS::DI::Dictionary`, `Modbus_Master_Cfg.cpp` → `KMS::Modbus::Master_Cfg`). Corresponding headers live under `Includes/KMS/<Namespace>/<ClassName>.h`.
- OS-specific implementation files carry a suffix: `_L` = Linux/POSIX, `_W` = Windows. A class with divergent OS behavior typically has `Foo.cpp` (shared) plus `Foo_L.cpp`/`Foo_W.cpp`, or an OS file may be the entire implementation (e.g. `Environment_L.cpp`/`Environment_W.cpp`).
- `_DI2` suffix files are the newer DI2 reflection bindings for a class (e.g. `Version_DI2.cpp`, `Dbg_Log_DI2.cpp`).
- Every project has a local `Component.h` that is the mandatory first include (`#include "Component.h"`) in every `.cpp`; it pulls in `KMS/Base.h` and the OS/embedded-specific system headers for that library.
- Platform detection macros come from `Includes/KMS/Base.h`: `_KMS_WINDOWS_`, `_KMS_LINUX_`, `_KMS_DARWIN_`; embedded builds define `_KMS_EMBEDDED_`.
- Error handling uses `KMS::Exception` + `KMS::Result` codes, thrown via `KMS_EXCEPTION(code, msg, info)` / `KMS_EXCEPTION_ASSERT(cond, code, msg, info)`, caught via `KMS_CATCH` / `KMS_CATCH_RESULT(var)` (see `Includes/KMS/Exception.h`).
- `NO_COPY(ClassName)` (from `Base.h`) disables copy ctor/assignment — used throughout instead of `= delete`.
- Executables use the `KMS_MAIN_BEGIN` / `KMS_MAIN_PARSE_ARGS` / `KMS_MAIN_VALIDATE` / `KMS_MAIN_END` / `KMS_MAIN_RETURN` macro sequence from `Includes/KMS/Main.h`, which wires a `Cfg::Configurator`, `Install::Installer`, and execution-time stats around `main()`.

## Building

### Windows
Open `Solution.sln` in Visual Studio 2022, or build from the command line with MSBuild against configurations `Debug`, `Release`, `Release_Static` and platforms `x64`/`x86`, e.g.:
```
MSBuild.exe Solution.sln /target:rebuild /Property:Configuration=Debug /property:Platform=x64
```
`KMS-Build.cmd` shows the full documented release sequence (rebuild all 6 config/platform combinations, then run `*-Test.exe Groups+=Auto`, then Doxygen/Inno Setup packaging).

### Linux
Each library/binary directory has its own `makefile` including the shared `Common.mk`. Build with:
```
make CONFIG=Debug PROCESSOR=x86_64      # or CONFIG=Release
```
run from inside the target directory (e.g. `KMS-A/`, `KMS-Build/`). `FirstMake.sh` bootstraps a fresh clone (creates `Binaries/`/`Libraries/` output dirs, builds `KMS-A` then `KMS-Make`). `KMS-Build.sh` shows the full ordered build (`KMS-A` → `KMS-B` → `KMS-C` → the `*-Test` binaries → `KMS-Build` → `KMS-Import` → `KMS-Make`) for both `Debug` and `Release`, followed by running the test binaries and generating Doxygen docs.
Static libraries land in `Libraries/<Config>_<Processor>/KMS-{A,B,C}.a`; binaries land in `Binaries/<Config>_<Processor>/`.

`KMS-Build.cfg` (and each product's own `KMS-Build.cfg`, e.g. `STM32/KMS-Build.cfg`, `front-end/KMS-Build.cfg`) is the manifest the `KMS-Build` tool itself reads to know which libraries/binaries/tests belong to a release and how to bump `_DocUser/Documentation.html`/`KMS.ReadMe.txt` version strings on build — this is a self-hosting build tool (`KMS-Build`/`KMS-Make` are part of this repo and are themselves used to build releases of this repo and of other KMS products).

### STM32 / embedded
`STM32/` is a parallel tree with its own `Common.mk`, `Makefiles`, `LinkerScripts`, and `Startup` per chip family (F3, H7). It reuses headers from the root `Includes/KMS` (compiled with `_KMS_EMBEDDED_`) alongside STM32-specific ones (`Includes/KMS/STM`).

### Front-end / kms-react (JavaScript)
Standard CRA-based projects:
```
cd front-end && npm start | npm run build | npm test
cd kms-react  && npm run build | npm start (watch) | npm test
```
`kms-react`'s `test` script runs unit tests, eslint, and a build (`test:unit`, `test:lint`, `test:build` via `npm-run-all`).

## Testing (C++)

Tests live in `KMS-A-Test`, `KMS-B-Test`, `KMS-C-Test`, each producing a `*-Test` executable built from the corresponding library plus `Test::TestManager::Main` (see `KMS-A-Test/KMS-A-Test.cpp`). Test source files mirror the library's own `Namespace_ClassName.cpp` naming and typically test the file of the same name.

Write tests with the `KMS_TEST` macro family from `Includes/KMS/Test/Test.h`:
```cpp
KMS_TEST(Convert_Base, "Auto", sTest_Base)
{
    KMS_TEST_ASSERT(condition);
    KMS_TEST_COMPARE(actualValue, expectedValue);
}
```
- First macro arg is a unique test class name, second is the group (the `"Auto"` group is what CI/release scripts run), third is a unique static instance name.
- Use `KMS_TEST_CATCH(RESULT_CODE)` right after a `try { ...; KMS_TEST_ASSERT(false); }` block to assert a specific `Result` code was thrown.
- `_BREAK`/`_RETURN` variants (`KMS_TEST_ASSERT_BREAK`, `KMS_TEST_COMPARE_RETURN`, ...) stop the current loop/test early on failure instead of just recording it.

Run all "Auto"-group tests after building:
```
KMS-A-Test.exe Groups+=Auto
KMS-B-Test.exe Groups+=Auto
KMS-C-Test.exe Groups+=Auto
```
(Linux: run the built `Binaries/<Config>_x86_64/KMS-*-Test` binaries the same way.) Some tests reference fixture files under each test project's `Tests/` folder (e.g. `KMS-A-Test/Tests/Test1.cfg`) — keep paths relative to the repo root / working directory used by these scripts.

## Version / release bookkeeping

`Common/Version.h` (`VERSION_STR`, `VERSION_RC`, `KMS_VERSION(...)`) is the single source of truth for the product version and is edited manually as part of each release per `_DocDev/Build.txt`. `_DocUser/KMS.ReadMe.txt` keeps a changelog-like history under "EDIT ON BUILD" that the `KMS-Build` tool also rewrites automatically (see `EditOperations` in `KMS-Build.cfg`).

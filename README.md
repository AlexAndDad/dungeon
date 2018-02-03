Compiling for Windows:
======================


What has worked so far:

1. Install Visual Studio 2017

2. Install git for windows. Tell it you want the bash terminal and to add git to the path.

3. Install cmake for windows

4. Run Git Bash

```bash
mkdir -p ~/alex_dad
cd ~/alex_dad
git clone ssh://git@github.com/AlexAndDad/dungeon.git
cd dungeon
git submodule init
git submodule update
mkdir cmake-build-debug

```

now switch to the Visual Studio Command prompt

```bash
cd ${HOME}/alex_dad/dungeon
cmake -H. -Bcmake-build-debug -DCMAKE_BUILD_TYPE=Debug -G"CodeBlocks - NMake Makefiles"
```

This will take a while as windows is a shockingly bad operating system.

Now you need to configure clion to use the experimental Visual Studio Toolchain:

See here (about halfway down): https://www.jetbrains.com/help/clion/quick-tutorial-on-configuring-clion-on-windows.html

Then, with luck, you can open the dungeon project in clion and compile.

If that fails, leave off the -G"CodeBlocks - NMake Makefiles" part and open the resuling solution in visual studio :(

)
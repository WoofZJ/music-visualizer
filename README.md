# Music Visualizer

![demo.gif](/assets/image/demo.gif)

## Introduction
As the demo.gif shows, this program consists of three parts:
1. green part: visualization of the audio's raw wave.
2. orange part: the "strength" of current frame's audio. Its value is determined by the max value we get from FFT.
3. blue part: the spectrum of the frequency we get from FFT. The x-axis is logarithmic coordinates. Each bar's value is the sum of a specific span of the value we get from FFT. For example, the 10th-bar's x is 40Hz and the 11th-bar's x is 43Hz, then the value of 11th-bar is the sum of 41Hz, 42Hz, 43Hz FFT result. Since the x-axis is logarithmic, it will add more items when x grows.

NOTICE: I do not know much about music and audio process, and this project is a toy project trying to visualize a piece of audio from my own understanding. There may be somewhere I misunderstand or implement wrong, and I'll update the project as soon as I figure it out. Feel free to open any issues :)
## Build
This project uses [raylib](https://github.com/raysan5/raylib) to do the GUI job. No more libraries needed. I've made raylib as a submodule, so all you need is to clone this repository and build.

I write and test the program on [archwsl](https://github.com/yuk7/ArchWSL) (a distribution of windows subsystem for linux). Any linux distribution should be able to build this project using following commands, which is the content of [run.sh](/run.sh):
```shell
cmake -B build
make -C build
./build/music-visualizerbash
```
This also builds raylib, so please refer to [this page](https://github.com/raysan5/raylib/wiki/Working-on-GNU-Linux) to see the dependencies raylib needs.

<del>At first I wanted to write and build the program on Windows, but I sadly found that MSVC has its own implementation of complex numbers which differs a lot from C standard. So I switched to linux and gcc. But I'll rewrite related part someday, adding support for Windows build.</del>

Now This project can be built with MSVC on Windows using following commandline, which is saved at [run.ps1](/run.ps1):
```powershell
cmake --build build --target ALL_BUILD --config Release
msbuild build/music-visualizer.sln
./build/Release/music-visualizer.exe
```
## Others
I've been wanting to write a music visualizer like [this](https://www.youtube.com/watch?v=bQ7c9Vlhyp0) since I bought an esp32 board and a led panel, but I did not understand FFT then. Great thanks to [@tsoding](https://www.youtube.com/@TsodingDaily)'s high quality [video](https://www.youtube.com/watch?v=Xdbk1Pr5WXU), which clearly explains its principle.

This project uses [raylib](https://github.com/raysan5/raylib) to parse audio file and render the result. It's a simple and easy-to-use C library for videogame programming. If you have some C experience and want to write some simple games or applications, I recommend you to try it.
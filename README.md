# nvmake
Reverse engineered implementation of NVIDIA's internal nvmake tool.

Based on the nvmake-mac binary. 

# How to compile *you know what*

First of all, Gather necessary tools in your NV_TOOLS directory (could be a bit challenging):
- gcc 4.7.0
- llvm 3.8.0
- perl 5.10.0
- cuda-10.2-nightly-27005658 (Although you can build this from the source tree but I personally couldn't)
- python 2.7.11
- nvvmc (Found in -/drivers/compiler/tools/nvvm)

Run `nvmake b drivers` to build all compatible drivers.
Run `nvmake b cuda` to build CUDA

Run `nvmake depend` to install (EXPECT CRASHES IF YOU USE THE `dev` BRANCH!!)

# FAQ

#### 1) Can you compile *you know what* on macOS? 
-   Yes, you can. But it only works on macOS High Sierra though you can't load the webdriver as it segfaults. If you can get a kernel debugger and iron out the segfaults you *could* run this on macOS.

#### 2) Can you compile *you know what* on Windows?
-   Yes. Though the dependencies may be a bit different. Please note that this project is only tested to work with mingw-w64. msvc is not tested yet.

#### 3) What is *you know what*?
-   Spend 10 seconds researching this project and you'll figure it out yourself.

#### 4) Where do I get *you know what*?
-   This is from an Internal Leak. You probably can't find it on the internet.

#### 5) nvmake Segfaults on launch!
-   Make sure NV_SOURCE and NV_TOOLS are set correctly .

### Tested driver version:
-   450.56.11-dev (Thanks to the NVIDIA Developer who decided to kindly break their NDA and share this)

### Tested GPU:
- NVIDIA GeForce RTX 3060 LHR

**What works?**

- X can launch and GNOME does function with Hardware Acceleration enabled
- OpenGL apps seem to work fine (except for Minecraft oddly)
- CUDA works flawlessly
- OBS picked up nvenc and it worked without a hitch.

**What doesn't?**

- Attempting to use the Vulkan driver results in driver segfault.
- Wayland is fully broken
- vdpau seems to be broken as well.

### Any Questions? Unfortunately I'm unable to answer them. This project will get taken down at some point.

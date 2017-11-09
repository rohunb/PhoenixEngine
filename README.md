# PhoenixEngine
|Builds|  |
|---|---|
|Windows|[![Build status](https://ci.appveyor.com/api/projects/status/0wk764mmne7vcyut?svg=true)](https://ci.appveyor.com/project/wobbier/phoenixengine)|
|OS X|[![Build Status](https://travis-ci.org/PhoenixOrg/PhoenixEngine.svg?branch=master)](https://travis-ci.org/PhoenixOrg/PhoenixEngine)|
|Linux| [![Build Status](https://app.wercker.com/status/afda5dd03e9518a4d8f7966642e8d8a0/s/master "wercker status")](https://app.wercker.com/project/bykey/afda5dd03e9518a4d8f7966642e8d8a0) |
|Code Analysis|[![Static Code Analysis Status](https://scan.coverity.com/projects/6392/badge.svg)](https://scan.coverity.com/projects/phoenixorg-phoenixengine)  |

Main Features
-------------
   * Windows, OS X, and Linux support
   * Component System
   * Bullet Physics
   * OpenGL Rendering
   * Irrklang Audio
   * Language: C++
   * Open Source Commercial Friendly(MIT)

Build Requirements
------------------
* Windows 7+, Visual Studio 2015
* OS X: Mavericks+, XCode 6.4+, Brew Package Manager
* Linux: GCC 5+

Building PhoenixEngine
----------------------
__PLATFORM__ = _Windows_, _Mac_, or _Linux_
__TYPE__ = _VS2015_, _XCode_, _GMake_

1. Run __Phoenix/Tools/PLATFORM_PremakeAll__.
2. Run __Phoenix/Tools/PLATFORM_PremakeCopyAssets__ each time you modify the Phoenix/Solution/Assets folder and it will output the files to where they need to go.
3. Run __Phoenix/Tools/PLATFORM_PremakeTYPE__ if there are project changes (new files, deleted files, etc).  Note that some platforms may have x64 listed, but it may not be fully supported or tested yet.
4. Find the Phoenix/Solution/Libraries/Lib/Debug.x32/FBX folder and unzip the .7z file in place.  Currently, the FBX lib file is not uploaded due to its size.  This will also have to be done in Phoenix/Solution/Libraries/Lib/Release.x32/FBX as well if you plan on running in Release.
5. Open the solution in Phoenix/Solution.
6. Make sure to run PhoenixBuild prior to running the project.
7. And finally, if there are any problems, [please let us know][1].

_**NOTE**_: You may be missing some assets that we have.  This is because these assets may have been purchased and cannot be uploaded.  When the engine and game are further along, free assets will be uploaded.

Contributing to the Project
--------------------------------

Did you find a bug? Have a feature request?

  * [Contribute to the engine][1]

[1]: https://github.com/PhoenixOrg/PhoenixEngine/issues "GitHub Issues"

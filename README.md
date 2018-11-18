
# HairGroomer

Hair Groomer is a stand-alone software to generate guide hairs for NVIDIA HairWorks.

## Controls

### General Controls

| Key | Functionality |
|---|---|
| D | Toggle the UV overlay visibility |

### Brush Controls

| Key | Functionality |
|---|---|
| M | Reduce the brush intensity |
| P | Increase the brush intensity |
| - | Reduce the brush size |
| + | Increase the brush size |
| L | Set the brushmode to length (Red) |
| C | Set the brushmode to curl (Green) |
| T | Set the brushmode to twist (Blue) |
| R | Reset the drawing window content (hair) |

### Camera Controls

| Key | Functionality |
|---|---|
| Q | Reset the camera in the 3D viewport |
| Alt + Drag Left-click | Rotate the camera around the pivot point |
| Alt + Drag Middle-click | Move the pivot point of the camera up and down realitive to the current camera position and rotation |
| Mouse Wheel | Zoom camera in/out |

## Getting Started (Developers)

The following instructions will help you get the project up and running for development.

The current version of the project can only be built under Windows using [Microsoft Visual Studio 2017](https://visualstudio.microsoft.com/de/) and the MSCV compiler.

### Prerequisites

#### Visual Studio 2017

Download and install the [Visual Studio 2017](https://visualstudio.microsoft.com/)

#### Qt 5.11.2

**Note:**  If you already have a different version of Qt 5 installed you can skip step 1, run the MaintenanceTool located at your Qt install directory  (e.g. C:/Qt/MaintenanceTool.exe) and select `add components`

1. Download and install [the Open-Source version of the Qt](https://www.qt.io/download)
2. When asked to select components, click the `Qt` dropdown and look for version `5.11.2`
3. Open the `5.11.2` dropdown and select `MSVC 2017 64-bit`
4. Finish the installation

#### FBX SDK

Download and install [FBX SDK 2019.0 installer for VS2015](https://www.autodesk.com/developer-network/platform-technologies/fbx-sdk-2019-0)

#### Qt VS Tools

1. In Visual Studio 2017, select `Tools -> Extensions and Updates` in the menu bar
2. Click on `Online`
3. Type in `Qt Visual Studio Tools` in the search field on the top right
4. Select the first entry and click on `Download`
5. Restart the IDE, the menu bar should now show a new entry named `Qt VS Tools`
6. Select `Qt VS Tools -> Options`
7. In the `Qt Versions` tab click `Add`
8. Choose a version name (e.g. `5.11.2 msvc`)
9. Select the path to the `Qt msvc 2017_64` folder you installed previously (e.g. `C:\Qt\5.11.2\msvc2017_64`)
10. Click on `OK` and  in the Qt Options window, click on `OK` again to close the window

### Setup

After cloning the repository a few things need to be done to get the build working.

**TODO:** either copy or run fbxsdk.py

1. Obtain the required git submodules by using `git submodule update --recursive --remote` in the repository
2. FBX SDK
   - Execute the `libs/fbxsdk.py` Python script to automatically setup FBX SDK
   - Alternatively copy the "lib" and "include" directories from your FBX SDK installation directory (e.g. `C:\Program Files\Autodesk\FBX\FBX SDK\2019.0`) to `HairGroomer/libs/fbxsdk`
3. Open the HairGroomer solution in Visual Studio and select `Qt VS Tools -> Qt Project Settings` in the menu bar
4. In the `Properties` tab, select the Qt msvc2017_64 Version you installed earlier and click on `OK`
5. You should now be able to build and run the project

## Attribution

This software contains Autodesk® FBX® code developed by Autodesk, Inc. Copyright 2018 Autodesk, Inc. All rights, reserved. Such code is provided "as is" and Autodesk, Inc. disclaims any and all warranties, whether express or implied, including without limitation the implied warranties of merchantability, fitness for a particular purpose or non-infringement of third party rights. In no event shall Autodesk, Inc. be liable for any direct, indirect, incidental, special, exemplary, or consequential damages (including, but not limited to, procurement of substitute goods or services; loss of use, data, or profits; or business interruption) however caused and on any theory of liability, whether in contract, strict liability, or tort (including negligence or otherwise) arising in any way out of such code.

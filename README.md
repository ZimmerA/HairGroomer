# HairGroomer
Hair Groomer is a standalone software to generate guide hairs to be used by nvidia hairworks.
## Getting Started
The following instructions will help you get the project up and running for development.
### Prerequisites
#### Visual Studio 2017
1. Download and run the Visual Studio 2017 installer found at https://visualstudio.microsoft.com/de/vs/
2. Install the software
#### QT 5.11.2
**Note:**  If you already have a different version of Qt 5 installed you can skip step 1, run the MaintenanceTool located at your Qt install directory  (e.g. C:/QT/MaintenanceTool.exe) and select `add components`
1. Download and run the open source version of the Qt installer found at https://www.qt.io/download
2. When asked to select components, click the `Qt` dropdown and look for version `5.11.2`
3. Open the `5.11.2` dropdown and select `MSVC 2017 64-bit`
4. Finish the installation
#### FBX SDK
1. Download the `FBX SDK 2019.0 installer for VS2015`  at https://www.autodesk.com/developer-network/platform-technologies/fbx-sdk-2019-0
2. Install the software
#### Qt VS Tools
1. In Visual Studio 2017, select `Tools->Extensions and Updates` in the menu bar
2. Click on `Online`
3. Type in `Qt Visual Studio Tools` in the search field on the top right
4. Select the first entry and click on `Download`
5. Restart the IDE, the menu bar should now show a new entry named `Qt VS Tools`
6. Select `Qt VS Tools -> Options`
7. In the `Qt Versions` tab click `Add`
8. Choose a version name (e.g. `5.11.2 msvc`)
9. Select the path to the `QT msvc 2017_64` folder you installed previously (e.g. `C:\Qt\5.11.2\msvc2017_64`)
10. Click on `OK` and  in the Qt Options window, click on `OK` again to close the window

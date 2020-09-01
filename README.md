![iw4x](iw4x/resource/iw4x.jpg?raw=true)
=
[![license](https://img.shields.io/github/license/IW4x/iw4x2-client.svg)](LICENSE)
[![discord](https://img.shields.io/endpoint?url=https://momo5502.com/iw4x/members-badge.php)](https://discord.gg/sKeVmR3)

IW4x² is a work-in-progress open-source re-implementation of Call of Duty® Modern Warfare® 2.

Getting started
=
We use an easy set of rules, as defined by the conventional commits specification, for creating an explicit commit history; https://www.conventionalcommits.org/en/v1.0.0/. To get started, recursively clone IW4x² repository and either build the individual requirements, OR, download the prebuilt binaries they provide. Visual Studio uses MSBuild to build each project in a solution, and each project contains an MSBuild project file. To generate the project files for Visual Studio, use the Premake5 simple build configuration.

```bash
git clone --recursive https://github.com/IW4x/iw4x2-client
cd .\iw4x2-client\requirements\
.\premake5.exe vs2019
```

Licensing
=====
This program is neither developed by, nor endorsed by ‎Infinity Ward or its parent company Activision. You must own a legally purchased copy of Call of Duty®: Modern Warfare® 2. All other trademarks are property of their respective owners.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You can find a copy of the [GNU General Public License](LICENSE)
along with this program.  If not, see <https://www.gnu.org/licenses/>.

##Introduction
The aim of this project is to add the ability to change the position of the sidebar (left / right)

# Qt Creator Sidebar plugin

Sidebar on the right side
![Screen](demo/sidebar_right_side.png?raw=true)

Sidebar on the left side
![Screen](demo/sidebar_left_side.png?raw=true)

##Features
To change the sidebar position, you can use a hot key Ctrl+Shift+P, or activate the menu Window->Change sidebar position.
Sidebar changes position in EDIT mode and in DEBUG mode.

##Installation

###From source
1. Change sidebar.pro:

 - set `IDE_SOURCE_TREE` = path to Qt Creator source dir (with qtcreator.pro)
 - set `IDE_BUILD_TREE` = path to compiled Qt Creator dir (with bin,lib,libexec,...)

2. Compile plugin.

###From binaries
1. Extract/copy files from archive into Qt Creator's dir (archive already contains proper paths).
2. Enable plugin in Help->Modules menu.

## Copyright / License

Copyright (C) 2016 Stepan Kochetov

This software is available under the terms of the GNU Lesser General Public License version 3 (LGPLv3).

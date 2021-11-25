<p align="center">
<img src="contents/ui/app_icon128x128.png" alt="SafeHaven"/>
</p>

## SafeHaven
==============================

SafeHaven is a diary application built with the Qt5 library.

## Features
==============================

- Write about any of your days, be it the past, present, or future.
- Express yourself by making a simple drawing.
- Choose one color that you like from the six app's color option.
- Two background music to accompany you while writing.

## Requirements
==============================

- qt >= 5.11.3 (to install and configure, follow the guide [here](https://zetcode.com/gui/qt5/introduction/)
- fontconfig for displaying app's font
- gstreamer and libqt5multimedia5-plugins for playing audio
- libqt5svg5 for displaying app's window icon

## Installing
==============================
Download the appimage from the [Releases](https://github.com/adotrdot/SafeHaven/releases) page and set its permission by running `chmod +x` on it.

## Building from source
==============================

1. Clone this repo
1. Run `qmake`
1. Run `make`
1. Run `./safehaven` to launch the app after building had finished.

## Credits
==============================

This application is made possible thanks to:
- The Qt5 Developer Team
- stockio.com for the left and right arrow icons
- Joe Prince for the Varela Round font (obtained from Google Fonts)

# temporal_median

Temoral Median Smoothing Filter for movie file.

## Description

temporal_median is a simple program that imports movie file, add pixel to pixel median filter effect to temporal direction (not x-y direction).
This filer can remove moving small objects (such as floating dusts, bug flies, snow fall, so on).
At the same time this filter can add "long exposure effect" to the movie by smoothing the frames toward temporal direction.

## Features

- Imports a movie file, pixel to pixel median filter effect to temporal direction.
- The number of frames to be processed can be specified by [frames] argument.
- When finished, result movie file will be exported.

## Requirement

- OpenCV 3.0.0 or above is preferable.
- Movie codec such as openh264-1.4.0-win64msvc.dll.
- I tested with win7 32bit and 64bit + msys2 + gcc.

## Usage

1. Prepare a movie file to be filtered.
2. In the working directory, type following command in your command window.

 temporal_median.exe input_movie output_movie [frames]

- input_movie: input movie file
- output_movie: output move file
- frames: number of frames to median

3. The larger "frames" parameter, the smoother the result movie. Default "frames" parameter = 5.

## Installation

1. Compile with OpenCV includes and libraries.

## Movie example

[youtube](https://youtu.be/I4TPCfIyhsE)

## Author

delphinus1024

## License

[MIT](https://raw.githubusercontent.com/delphinus1024/temporal_median/master/LICENSE.txt)


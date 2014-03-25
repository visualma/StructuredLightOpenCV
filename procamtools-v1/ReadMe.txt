----------------------------------------------------------------------
Self-Calibration of Projector Camera Systems
ProCamTools ver.1.0
Copyright (c) 2009-2011
Shuntaro Yamazaki <shun-yamazaki@aist.go.jp>
National Institute of Advanced Industrial Science and Technology
----------------------------------------------------------------------

1. Overview of software package
----------------------------------------------------------------------

This software package is for geometric calibration of an active vision
system, composed of a projector and a camera, using structured light
projection. We estimate the intrinsic parameters of both the projector
and the camera as well as extrinsic parameters except a global scale
without any calibration apparatus such as a checker-pattern board. The
method is based on the decomposition of a radial fundamental matrix
into intrinsic and extrinsic parameters. Dense and accurate
correspondences are obtained utilizing structured light patterns
consisting of Gray code and phase-shifting sinusoidal code. Please
refer to the following paper for the technical details.

Shuntaro Yamazaki, Masaaki Mochimaru, and Takeo Kanade, "Simultaneous
self-calibration of a projector and a camera using structured light",
In Proc. Projector Camera Systems 2011, pp. 67-74, June 2011

The software is composed of four separate programs as follows.

1.1. encode:

This program generates the patterns for structured light projection
in BMP format. Specify the resolution of your projector in an
options.ini file (see below).

Usage: encode.exe <options>
Input: options.ini
Output: "bitmap images"

1.2. decode:

This program estimates the correspondence between a projector and a camera,
given the images acquired under structured light illumination. The output
'h.map' and 'v.map' are respectively horizontal and vertical correspondences,
and mask.bmp is the mask for illuminated pixels in the camera image.
Sample images are available online.

Usage: decode.exe <options.ini> <acquired images...>
Input: options.ini, "bitmap images"
Output: h.map, v.map, mask.bmp

1.3. calibrate:

This program estimates the calibration parameters of the projector and camera,
given the horizontal and vertical correspondences estimated in the above.

Usage: calibrate.exe <options> <h.map> <v.map> <mask.bmp>
Input: option file, h.map, v.map, mask.bmp
Output: cam-intrinsic, cam-distortion,
        pro-intrinsic, pro-distortion, pro-extrinsic

1.4. triangulate: reconstruct 3D mesh

This program reconstruct the 3D shape as a triangular mesh in PLY format.
Both horizontal and vertical, or only horizontal correspondences are used
for triangulation.

Usage: triangulate.exe [-o <file.ply>] [-t <val>] [-v <v.map>]
             <h.map> <mask.bmp> <options>
             <cam-intrinsic> <cam-distortion>
             <pro-intrinsic> <pro-distortion> <pro-extrinsic>
Input: v.map, h.map, mask.bmp, options.ini,
       cam-intrinsic, cam-distortion,
       pro-intrinsic, pro-distortion, pro-extrinsic
Output: mesh.ply

2. Build the programs
----------------------------------------------------------------------

We have developed and tested the software packages using
Visual Studio 2008 on Windows XP and 7.

3. Format of options.ini
----------------------------------------------------------------------

[projector]
width=1024 		; (int) width of projector images
height=768 		; (int) height of projector images
vertical_center = 0.83 ; (float) vertical center (0 = top, 1 = bottom)

[phase]
nfringes=8		; (int) number of phase-shifting images
interval=1		; (int) offset of phase-shifting images

[pattern]
horizontal=1	; (bool) horizontal code (should always be 1)
vertical=1		; (bool) vertical code (should be 1 for calibration)
complementary=1	; (bool) obsolete: complementary images instead of thresholding
debug=0			; (bool) debug

[reconstruction]
threshold=0.1	; (float) intensity mask
nsamples=1000	; (int) number of subsamples (0 = disable)

4. License

 This software package is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

This packages includes the following external packages.

levmar; Levenberg-Marquardt non-linear minimization algorithm is also
distributed under GNU General Public License version 2 or later.

mkl; Intel(R) Math Kernel Library is distributed under End User
License Agreement for the Intel(R) Software Development Products.

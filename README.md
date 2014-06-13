# Alpha Channel Bits Calculator

[![Build Status](https://travis-ci.org/xpol/acbc.png)](https://travis-ci.org/xpol/acbc)

Calculates minimal alpha channel bits for image.

This is useful when you want compress image by reduce bits per pixel (BPP).

## Build

```
cmake -H. -Bbuild
cmake --build build --target acbc --config Release
```

## Usage

```
acbc [-c channels] <IMAGEFILE>
```


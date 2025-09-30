<h1 align="center">
  <img src="./docs/assets/sparkles.webp" alt="✨" width="33"/>
  SAWC
  <img src="./docs/assets/sparkles.webp" alt="✨" width="33"/>
</h1>

[![Build](https://github.com/iqsnider/SAWC/actions/workflows/ci.yml/badge.svg)](https://github.com/iqsnider/SAWC/actions/workflows/ci.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

Self-Avoiding Walk in a Circle

## Sitting bored in class
"_If I make a circle (or an ellipse) and I start drawing a line at a random point within that circle and control the direction by randomly change the angle of the direction the line is drawn (with saturation and smoothing constraints obviously) and constrain the drawn line to never cross itself or the circle, where will the end of the line converge to? Or how long does it take to converge (to within some tolerance) given velocity of the line drawn and the constraint boundary conditions. What if we allow 1 cross? What if we allow N crosses?_"

## Self-avoidance
![Alt test](docs/sawc.png)

## A non-self-avoiding Petri dish
![Alt text](docs/nonSAWC.png)

## Building (macOS)

### Install dependencies
```bash
brew install meson ninja sdl2
```

### Build
```bash
meson setup build
ninja -C build
```

### Run
```bash
./build/walk
```

# image

## Overview

Host-side image containers and pixel types, used to prepare input for and verify
output from image read/write and sampling tests. This is purely host storage; the
device-side image object is `cassian::Image` from [runtime](../runtime).

## HostImage

`HostImage<Pixel, ImageType>` — a 1D, 2D or 3D buffer of pixels:

```cpp
cassian::HostImage<cassian::Unnormalized<uint8_t, ImageChannelOrder::rgba>,
                   ImageType::t_2d> image({width, height});
image(x, y) = pixel;
runtime->write_image(device_image, image.data());
```

* `operator()(x)`, `operator()(x, y)`, `operator()(x, y, z)` — coordinate access; only
  the arity matching the image type compiles.
* `dimensions()`, `data()`, `storage()`.
* Full iterator set (`begin`/`end`, `cbegin`/`cend`, and the reverse forms), so the
  container works with standard algorithms.
* `is_1d` / `is_2d` / `is_3d` compile-time predicates.

## Pixel types

A pixel is a `Storage<T, ImageChannelOrder>` subclass, where the channel order
determines which channels exist and at what index:

* `Unnormalized<T, Order>` — channel values used as stored.
* `Normalized<T, Order>` — integer storage interpreted as a normalized `[0,1]` or
  `[-1,1]` float, matching the `unorm`/`snorm` image formats.

`Storage` exposes `red_raw()`, `green_raw()`, `blue_raw()`, `alpha_raw()` — each only
present when the channel order actually has that channel — plus equality operators.
`PixelTraits<T>` maps a host type to its image format and channel information;
`pixel/simple.hpp` specializes it for `int8_t`, `int16_t`, `int32_t`, `uint8_t`,
`uint16_t`, `uint32_t`, `half` and `float`.

`generate_value<T>(seed)` in `pixel/common.hpp` fills every present channel of a
pixel with random data.

## Nv12Image

`Nv12Image` models the two-plane NV12 layout: a full-resolution `uint8_t` Y plane and
a half-resolution `uint16_t` interleaved UV plane, each a `HostImage`. Width and
height must be even — the constructor throws `std::logic_error` otherwise. Use it with
`Runtime::get_image_plane()` to bind an individual plane as a device image.

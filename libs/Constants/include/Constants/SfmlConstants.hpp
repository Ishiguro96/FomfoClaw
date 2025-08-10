#pragma once

#include <cstdlib>

namespace ish::constants {

// This is a constant to convert from Box2D's units (meters)
// to SFML pixels (pixels per meter).
constexpr float PPM = 30.f;

constexpr size_t WINDOW_WIDTH = 1920;
constexpr size_t WINDOW_HEIGHT = 1080;

} // namespace

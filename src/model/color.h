#ifndef COLOR_H_
#define COLOR_H_

#include <cstdio>
#include <string>

struct Color {
  Color(int r, int g, int b, int a = 255) : r(r), g(g), b(b), a(a) {}
  explicit Color(const std::string& hex) {
    sscanf(hex.c_str(), "%02hhx%02hhx%02hhx", &r, &g, &b);
  }

  int r, g, b, a;
};

#endif  // COLOR_H_

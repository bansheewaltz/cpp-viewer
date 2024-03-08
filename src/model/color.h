#ifndef MODEL_COLOR_H
#define MODEL_COLOR_H

#include <string>

class Color {
 public:
  Color(int red, int green, int blue, int alpha = 255)
      : r(red), g(green), b(blue), a(alpha) {}
  explicit Color(const std::string& hex) {
    sscanf(hex.c_str(), "%02x%02x%02x", &r, &g, &b);
  }
  explicit Color(unsigned int hex) {
    r = ((hex >> 16) & 0xFF);
    g = ((hex >> 8) & 0xFF);
    b = ((hex) & 0xFF);
  }

  float red_f() { return float(r) / 255; }
  float green_f() { return float(g) / 255; }
  float blue_f() { return float(b) / 255; }
  float alpha_f() { return float(a) / 255; }

  // QColor to_QColor() { return QColor(r, g, b, a); }
 private:
  int r, g, b, a;
};

#endif  // MODEL_COLOR_H

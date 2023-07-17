#include "Vector.h"

class CubicBezier {
  public:
    int startX = 0;
    int startY = 0;
    int endX = 0;
    int endY = 0;

  private:
    int x1 = 0;
    int y1 = 0;
    int x2 = 0;
    int y2 = 0;

  public:
    CubicBezier() {
    }

    CubicBezier(int startX, int startY) {
      this->startX = startX;
      this->startY = startY;
    }

    CubicBezier(int startX, int startY, int endX, int endY) {
      setPoints(startX, startY, endX, endY);
    }

    void setFactors(int x1, int y1, int x2, int y2) {
      this->x1 = x1;
      this->y1 = y1;
      this->x2 = x2;
      this->y2 = y2;
    }

    void setPoints(int startX, int startY, int endX, int endY) {
      this->startX = startX;
      this->startY = startY;
      this->endX = endX;
      this->endY = endY;
    }

    Vector getPoint(float t) {
      float pow3 = pow((1.0 - t), 3);
      float pow2 = pow((1.0 - t), 2);
      int Xc = (int)(pow3 * startX + pow2 * 3 * t * x1 + (1.0 - t) * 3 * pow(t, 2) * x2 + pow(t, 3) * endX);
      int Yc = (int)(pow((1.0 - t), 3) * startY + pow((1.0 - t), 2) * 3 * t * y1 + (1.0 - t) * 3 * pow(t, 2) * y2 + pow(t, 3) * endY);
      return Vector(Xc, Yc);
    }
};

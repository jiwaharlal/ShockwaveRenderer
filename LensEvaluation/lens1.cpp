#include <iostream>
#include <math.h>
#include <stdio.h>

#define PI 3.14159265358

float f(float x) {
    return 15.0 * x * pow(2.7183, - x / 30.0);
}

int main() {
    float xPrev = 0.0;
    float yPrev = 0.0;
    for (float x = 1.0; x < 60.0; x += 1.0) {
        float y = f(x);
        float dx = x - xPrev;
        float dy = y - yPrev;
        float sign = 1.0;
        if (dy < 0)
        {
            dy = -dy;
            sign = -1.0;
        }
        
        float alpha = atan(dx / dy);
        float beta = asin(cos(alpha) / 1.33);
        float theta = alpha + beta;
        
        float target = x + y / tan(theta) * sign;
        theta = theta * 180 / PI;
        // float target = (y - yPrev) * y / (x - xPrev);
        printf("x = %f, alpha = %f, y = %f, target = %f\n\r", x, alpha, y, target);
        xPrev = x;
        yPrev = y;
    }
    
    return 0;
}
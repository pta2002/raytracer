#pragma once

class vec2 {
    
    public:
        float x;
        float y;

        vec2() = default;
        vec2(float newX, float newY);
        void applyVector(vec2 vector);

};
#pragma once

class vec3 {
    
    public:
        float x;
        float y;
        float z;

        vec3() = default;
        vec3(float newX, float newY, float newZ);
        vec3(float newX, float newz);
        void applyVector(vec3 vector);
        static vec3 fromSpherical(float alpha, float beta, float radius);

};
#include <SDL2/SDL.h>
#include <vector>

using namespace std;

const int t_min = 1, t_max = 2 << 5;
extern const int width, height, bg;

class Camera {
    public:
        vector<double> cameraPos;
        vector<double> viewportPos;

        int vw, vh; // size of viewport
        double d;   // distance from camera to viewport

    Camera() {
        cameraPos = {0, 0, 0};
        d = 1; 

        viewportPos.push_back(cameraPos[0]);
        viewportPos.push_back(cameraPos[1]);
        viewportPos.push_back(d);

        vw = 1, vh = vw;
    }

    vector<double> canvasToViewport(int cx, int cy) {
        vector<double> D{(double)cx * (double)vw / (double)width, (double)cy * (double)vh / (double)height, viewportPos[2]};

        return D;
    }

};

class Object {
    public:
        vector<double> pos;
        vector<int> color;
};

class Sphere : public Object {
    public:
        double radius;

    Sphere(double x, double y, double z, int radius, int r, int g, int b) {
        pos.push_back(x);
        pos.push_back(y);
        pos.push_back(z);

        color.push_back(r);
        color.push_back(g);
        color.push_back(b);

        this->radius = radius;
    }
};

enum light_types { ambient, point, directional }; 
class Light {
    public:
        light_types type;
        vector<double> pos;
        vector<double> direction;
        double intensity; 
};

class AmbientLight : public Light {
    public:

    AmbientLight(double intensity) {
        this->type = light_types::ambient;
        this->intensity = intensity;
    }
};

class PointLight : public Light {
    public:

    PointLight(double intensity, double x, double y, double z) {
        this->type = light_types::point;
        this->intensity = intensity;

        pos.push_back(x);
        pos.push_back(y);
        pos.push_back(z);
    }
};

class DirectionalLight : public Light {
    public:

    DirectionalLight(double intensity, double x, double y, double z) {
        this->type = light_types::directional;
        this->intensity = intensity;

        direction.push_back(x);
        direction.push_back(y);
        direction.push_back(z);
    }
};

class Scene {
    public:
        Camera* camera;

        vector<Sphere> spheres; 
        vector<Light> lights;
};

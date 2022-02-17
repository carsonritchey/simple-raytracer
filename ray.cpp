// replace lower line with this if on windows
//#include <SDL.h>
#include <SDL2/SDL.h>

#include <cmath>
#include <vector>
#include <climits>

#include "obj.h"
#include "vect.h"

using namespace std;

const int width = 256, height = width;
const int bg = 0; // background color

bool running;

Scene* scene = NULL;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

void init() {
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("unable to initialize\nexiting...\n");
        return;
    }
	window = SDL_CreateWindow("simple ray tracer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	running = true;
}

// finds the light value at a point
// P is the point and N is the light's normal vector 
double computeLighting(vector<double> P, vector<double> N) {
    double light = 0.0; // the light level (on a scale of 1) at the given point 

    for(int i = 0; i < scene->lights.size(); i++) {
        // ambient light
        if(scene->lights[i].type == light_types::ambient) {
            light += scene->lights[i].intensity;
        } 

        // point and directional light
        else {
            vector<double> L; // the direction of the light source
            // point
            if(scene->lights[i].type == light_types::point) {
                L = v_sub(scene->lights[i].pos, P);
            }
            // directional 
            else {
                L = scene->lights[i].direction;
            }

            double n_dot_l = v_dot(N, L);
            // if light isn't coming from the back 
            if(n_dot_l > 0) light += scene->lights[i].intensity * n_dot_l / (v_len(N) * v_len(L));
        }
    }

    return light;
}

// returns the value(s) of t where D intersects the given sphere
// basically solves quadratic equation for sphere and finds where the ray actually hits it 
vector<double> raySphereIntersect(Camera* camera, vector<double> D, Sphere* sphere) {
    vector<double> CO = v_sub(camera->cameraPos, sphere->pos); // vector from sphere's center to the camera

    double a = v_dot(D, D);
    double b = 2 * v_dot(CO, D);
    double c = v_dot(CO, CO) - sphere->radius*sphere->radius;
    double discriminant = b*b - 4*a*c;

    vector<double> Ts{-1, -1};
    if(discriminant >= 0) {
        Ts[0] = (-b + sqrt(discriminant)) / (2*a);
        Ts[1] = (-b - sqrt(discriminant)) / (2*a);
    }

    return Ts;
}

// converts from 0, 0 being the center of the window to 0, 0 being the top left 
void drawPoint(SDL_Renderer* renderer, int x, int y) {
    SDL_RenderDrawPoint(renderer, width / 2 + x, height / 2 + y);
}

// finds color of ray
vector<int> traceRay(vector<double> D, vector<Sphere> spheres) {
    double closest_t = t_max + 1;
    Sphere* closest_sphere = NULL;

    for(int i = 0; i < spheres.size(); i++) {
        vector<double> t_values = raySphereIntersect(scene->camera, D, &spheres[i]);

        // finds nearest t value for all possible solutions 
        for(int j = 0; j < 2; j++) {
            if(t_values[j] >= t_min && t_values[j] < closest_t && t_values[j] <= t_max) {
                closest_t = t_values[j];
                closest_sphere = &spheres[i];
            }
        }
    }

    vector<int> color{bg, bg, bg};
    if(closest_sphere != NULL) {
        color = closest_sphere->color;

        vector<double> P = v_scalar_mult(D, closest_t); // where the light hits the sphere 
        vector<double> N  = v_sub(P, closest_sphere->pos); // the normal vector at the intersection
        N = v_scalar_div(N, v_len(N)); // actually making it a normal vector 

        double lighting = computeLighting(P, N); // getting the light value of the ray 

        for(int j = 0; j < color.size(); j++) {
            color[j] *= lighting; //adjusting color of ray according to light value 
        }
    }
    return color;
}

void draw() {
    // clears screen
	SDL_RenderClear(renderer);

    for(int i = -width / 2; i < width / 2; i++) {
        for(int j = -height / 2; j < height / 2; j++) {
            // converts from canvas to viewport to get trajectory of ray
            vector<double> D = scene->camera->canvasToViewport(i, j);

            // finds what color the ray hits
            vector<int> color = traceRay(D, scene->spheres);

            // draws the corresponding pixel that color
            SDL_SetRenderDrawColor(renderer, color[0], color[1], color[2], 255);
            drawPoint(renderer, i, j);
        }
    }

    // sends drawing to screen
	SDL_RenderPresent(renderer);
}

void clean() {
    delete scene->camera;
    delete scene;

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
}

int main() {
	init();

    vector<Sphere> spheres;
    spheres.push_back( Sphere(-3.5, -1, 7, 1, 100, 100, 200));

    spheres.push_back( Sphere(0, 1, 3, 1, 255, 0, 0) );
    spheres.push_back( Sphere(2, 0, 4, 1, 0, 0, 255) );
    spheres.push_back( Sphere(-2, 0, 4, 1, 0, 255, 0) );
    spheres.push_back( Sphere(0, 5001, 0, 5000, 255, 255, 00) );


    vector<Light> lights; 
    lights.push_back( AmbientLight(0.2) );
    lights.push_back( PointLight(0.6, 2, -1, 0) );
    lights.push_back( DirectionalLight(0.2, 1, 4, 4) );
    
    scene = new Scene();
    scene->camera = new Camera();
    scene->spheres = spheres;
    scene->lights = lights;

    int frames = 0;
	while(running) {
		SDL_Event e;
		while(SDL_PollEvent(&e)) {
			switch(e.type) {
				case SDL_QUIT:
					running = false;
					break;
			}
		}

		draw();

        scene->spheres[0].pos[0] += .05;
        if(scene->spheres[0].pos[0] >= 3.5) scene->spheres[0].pos[0] = -3.5;
        //printf("%d\n", ++frames);
	}

	clean();
	return 0;
}

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <math.h>

using namespace std;

SDL_Window* window;
SDL_Renderer* renderer;

double rads(double degrees) {
	return degrees * (M_PI / 180);
}

class vec2 {
public:
	double x,y;
	vec2 operator+(vec2 const &obj) {
		return {this->x + obj.x,this->y + obj.y};
	}
	vec2 operator-(vec2 const& obj) {
		return {this->x - obj.x,this->y - obj.y};
	}
};

class vec2p {
public:
	vec2 p;
	SDL_Color color;
};

vector<vec2p> objects;

double length(vec2 p1,vec2 p2) {
	return sqrt(pow((p2.x-p1.x),2) + pow((p2.y-p1.y),2));
}

double tfraction(double value,double maxofvalue,double maxreturn) {
	return (value / maxofvalue) * maxreturn;
}

class camera {
public:
	vec2 center;
	double fov = 45;
	double angle = 0;
	double renderdistance = 0;
	double rays;
	SDL_Color nothing_color = {0,0,0,255};
	vector<SDL_Color> lines = {};
	camera(vec2 _center,double _fov,double _renderdistance) {
		center = _center;
		fov = _fov;
		rays = fov;
		renderdistance = _renderdistance;
	}
	void update() {
		lines = {};
		for(int i = angle - (fov / 2); i < (fov / 2) + angle; i += (fov/rays)) {
			//cout << i << endl;
			bool stopNext = false;
			for(double a = 0; a < renderdistance; a += 1)
			{
				vec2 rp = {center.x + ((sin(rads(i)) * a)),center.y + ((cos(rads(i)) * a))};
				if(objects.size() != 0)
				{
					for(int i = 0; i < objects.size(); i++) {
						if((length(rp,objects[i].p)) < 1) {
							SDL_Color c = objects[i].color;
							cout << "alpha: " << tfraction(length(objects[i].p,center),renderdistance,255) << endl;
							lines.push_back({c.r,c.g,c.b,(Uint8)(tfraction(length(objects[i].p,center),renderdistance,255))});
							goto rayBlocked;
						}
						else {
							lines.push_back(nothing_color);
							//SDL_RenderDrawPoint(renderer,rp.x,rp.y);
						}
					}
				}
				else {
					break;
				}
			}
		rayBlocked: continue;
		}
	}
	void draw() {
		for(int i = 0; i < lines.size(); i++) {
			//cout << "line: (r: " << (int)lines[i].a << endl;
		}
	}
};

void createWall(vec2 center,int len,double angle) {
	for(int i = center.x-len/2; i < center.x + len / 2; i++) {
		objects.push_back({(double)i,center.y,{0,0,0,0}});
	}
}

#define MOTION_SPEED 3

int main() {
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_CreateWindowAndRenderer(1920,1080,NULL,&window,&renderer);
	SDL_SetWindowTitle(window,"Ray Casting 3D");
	SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND);
	camera main_camera({1920 / 2,1080 / 2},50,70);
	main_camera.rays = 50 / 2;
	SDL_Event e = {};
	createWall({1920 / 2,1080 / 2},100,0);
	//createWall({1920/2,1},1920,0);
	//createWall({1920/2,1079},1920,0);
	while(1) {
		SDL_PollEvent(&e);
		SDL_SetRenderDrawColor(renderer,0,0,0,255);
		SDL_RenderClear(renderer);
		//SDL_SetRenderDrawColor(renderer,255,0,0,255);
		/*for(int i = 0; i < objects.size(); i++) {
			SDL_RenderDrawPoint(renderer,objects[i].p.x,objects[i].p.y);
		}*/
		if(e.key.keysym.scancode == SDL_SCANCODE_D) {
			main_camera.angle -= 2;
		}
		else if(e.key.keysym.scancode == SDL_SCANCODE_A) {
			main_camera.angle += 2;
		}
		else if(e.key.keysym.scancode == SDL_SCANCODE_W) {
			main_camera.center.x += sin(rads(main_camera.angle))*MOTION_SPEED;
			main_camera.center.y += cos(rads(main_camera.angle)) * MOTION_SPEED;
		}
		else if(e.key.keysym.scancode == SDL_SCANCODE_S) {
			main_camera.center.x -= sin(rads(main_camera.angle)) * MOTION_SPEED;
			main_camera.center.y -= cos(rads(main_camera.angle)) * MOTION_SPEED;
		}
		//SDL_SetRenderDrawColor(renderer,255,255,255,255);
		main_camera.update();
		main_camera.draw();
		//SDL_RenderPresent(renderer);
	}
}
#include <iostream>
#include <cstring>
#include <thread>
using namespace std;

struct Point {
	int x, y;

	Point(){}
	Point(unsigned int x_, unsigned int y_)
		: x(x_), y(y_) {}
};

struct Pixel {
	Point point;
	unsigned char r;
	unsigned char g;
	unsigned char b;

	Pixel(Point p_, 
			unsigned char r_,
			unsigned char g_, 
			unsigned char b_)
		: point(p_), r(r_), g(g_), b(b_){}
};

class Rasterizer {
	public:
		Rasterizer(){
			memset(data, 0, 1920*1080*3);
		}

		void Write() {
			cout << "P6\n1920 1080 255\n";
			for(int y = 0; y < 1080; ++y){
				for(int x = 0; x < 1920; ++x){
					cout << data[x][y][0]; // R
					cout << data[x][y][1]; // G
					cout << data[x][y][2]; // B
				}
			}
		}

		void DrawNaiveLine(Point p, Point q,
				unsigned char r,
				unsigned char g, 
				unsigned char b) {
			if(p.x > q.x) swap(p, q);

			int dx = q.x - p.x;
			int dy = q.y - p.y;

			if(abs(dx) >= abs(dy)){
				if(dx == 0) return; // Or dx = 1
				for(int i = p.x; i < q.x; ++i){
					int j = p.y + (dy * (i - p.x)) / dx;
					SetPixel(Pixel(Point(i, j), r, g, b));
				}
			} else {
				if(dy == 0) return; // Or dy = 1
				if(p.y > q.y) swap(p, q);

				for(int i = p.y; i < q.y; ++i){
					int j = p.x + (dx * (j - p.y)) / dy;
					SetPixel(Pixel(Point(j, i), r, g, b));
				}
			}
		}

		void DrawDDALine(Point p, Point q,
				unsigned char r,
				unsigned char g, 
				unsigned char b) {
			if(p.x > q.x) swap(p, q);

			double dx = q.x - p.x;
		 	double dy = q.y - p.y;

			double increment = max(abs(dx), abs(dy));

			dx /= increment;
			dy /= increment;

			double x = p.x, y = p.y;
			for(int i = 0; i < increment; ++i) {
				SetPixel(Pixel(Point(x, y), r, g, b));
				x += dx;
				y += dy;
			}
		}

		void DrawBresenhamLine(Point p, Point q,
				unsigned char r,
				unsigned char g, 
				unsigned char b) {
			if(p.x > q.x) swap(p, q);

			int dx = q.x - p.x;
		 	int dy = q.y - p.y;

		 	int fdx = abs(dx);
		 	int fdy = abs(dy);

		 	int px = 2 * fdy - fdx;
		 	int py = 2 * fdx - fdy;

		 	int x, y, xe, ye;
		 	if(fdy <= fdx){ 
		 		if(dx >= 0){
		 			x = p.x;
		 			y = p.y;
		 			xe = q.x;
		 		} else {
		 			x = q.x;
		 			y = q.y;
		 			xe = p.x;
		 		}

		 		SetPixel(Pixel(Point(x, y), r, g, b));

		 		for(int i = 0; x < xe; ++i){
		 			x = x + 1;
		 			if(px < 0){
						px = px + 2 * fdy;
					} else {
						y += ((dx < 0 && dy < 0) || (dx > 0 && dy > 0))? 1 : -1;
						px = px + 2 * (fdy - fdx);
					}
					SetPixel(Pixel(Point(x, y), r, g, b));
		 		}
		 	} else {
		 		if(dy >= 0){
		 			x = p.x;
		 			y = p.y;
		 			ye = q.y;
		 		} else {
		 			x = q.x;
		 			y = q.y;
		 			ye = p.y;
		 		}

		 		SetPixel(Pixel(Point(x, y), r, g, b));

		 		for(int i = 0; y < ye; ++i){
		 			y = y + 1;
		 			if(py <= 0){
						py = py + 2 * fdx;
					} else {
						x += ((dx < 0 && dy < 0) || (dx > 0 && dy > 0))? 1 : -1;
						py = py + 2 * (fdx - fdy);
					}
					SetPixel(Pixel(Point(x, y), r, g, b));
		 		}
		 	}
		}

	private: 
		unsigned char data[1920][1080][3];

		void SetPixel(Pixel pixel) {
			int x = pixel.point.x;
			int y = pixel.point.y;
			if(x >= 1920 || y >= 1080) {
				return;
			}

			data[x][y][0] = pixel.r;
			data[x][y][1] = pixel.g;
			data[x][y][2] = pixel.b;
		}
};

int main(){
	Rasterizer* rasterizer = new Rasterizer();
	rasterizer->DrawBresenhamLine(Point(500, 500), Point(490, 980), 255, 0, 0);
	rasterizer->DrawDDALine(Point(500, 500), Point(490, 0), 0, 255, 0);
	rasterizer->DrawNaiveLine(Point(500, 500), Point(0, 480), 0, 0, 255);
	rasterizer->Write();
	return 0;
}

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <iostream>

#include "Voronoi.h"

using namespace std;
using namespace cv;

// settings
const unsigned int SCR_WIDTH = 500;
const unsigned int SCR_HEIGHT = 500;

const int MAX_COORDS = SCR_HEIGHT-1;
const int MIN_COORDS = 1;

const double EPS = 1e-6;

Mat src(SCR_WIDTH, SCR_HEIGHT, CV_8UC3, Scalar(100, 100, 100));
RNG rng(time(0));
vector<Vec3b> color;

VNI::Voronoi voronoi(MAX_COORDS, MIN_COORDS, MAX_COORDS, MIN_COORDS);

void showDiagram(Mat& image, VNI::Voronoi& v) {

	int kind_num = v.cell_num;

	for (int i = 0; i < kind_num; i++) {
		cout << "got divided cell" << endl;
		int point_num = v.all_cell[i].points.size();
		
		for (int j = 0; j < point_num; j++) {
			int row = v.all_cell[i].points[j].x;
			int col = v.all_cell[i].points[j].y;
			image.at<Vec3b>(row, col) = color[i];
		}
		
	}
	for (int i = 0; i < kind_num; i++)
	{
		Point center = v.all_cell[i].cell_center; 
		for (int j = -5; j < 5; j++) {
			if (center.x + j >= MIN_COORDS && center.x + j < +MAX_COORDS) {
				image.at<Vec3b>(center.x + j, center.y) = Vec3b(0, 0, 0);
			}
			if (center.x + j >= MIN_COORDS && center.x + j < +MAX_COORDS) {
				image.at<Vec3b>(center.x, center.y + j) = Vec3b(0, 0, 0);
			}
		}
		image.at<Vec3b>(center.x, center.y) = Vec3b(255, 255, 255);
	}
	imshow("Voronoi demo", image);
	waitKey(100);
}

void addPoint(Point p, VNI::Voronoi& v, Mat& image) {
	v.addCell(p);
	color.push_back(Vec3b(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255)));
	cout << "addCell succ!" << endl;
	int cnt = 0;
	double old_energy = DBL_MAX;
	double eps = 0;
	do {
		double new_energy = v.getVoronoiByPixel();
		eps = old_energy - new_energy;
		old_energy = new_energy;
		cout << "divide succ!" << endl;
		showDiagram(image, v);
		cout << "the " << cnt++ << "th iteration: " << eps << endl;
		v.replaceCenter();
	} while (abs(eps) > EPS);
}

void onMouse(int event, int y, int x, int flags, void* param)
{
	switch (event)
	{
	case 1:     //鼠标左键按下响应：返回坐标
		cout << "[" << x << "," << y << "]:" << endl;
		addPoint(Point(x, y), voronoi, src);
		break;
	case 2:    //鼠标右键按下响应：输入坐标
		cout << "input(x,y),x、y 范围为[" << MIN_COORDS << "," << MAX_COORDS << "]:" << endl;
		cout << "x =";
		cin >> x;
		cout << endl << "y =";
		cin >>y;
		addPoint(Point(x, y), voronoi, src);
		break;
	}

}

int main()
{
	Mat* img = &src;
	namedWindow("Voronoi demo", WINDOW_AUTOSIZE);

	setMouseCallback("Voronoi demo", onMouse, reinterpret_cast<void*> (img));

	waitKey();  

	return 0;
}

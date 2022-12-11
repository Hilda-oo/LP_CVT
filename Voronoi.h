#pragma once

#ifndef Voronoi_h
#define Voronoi_h

#include <opencv2/highgui/highgui.hpp>


namespace VNI {

	using namespace cv;

	class Cell {
	public:
		Point cell_center;
		Point cell_centroid;
		std::vector<Point> points;
		double cell_energy;

		Cell(Point center) {
			cell_center = center;
		}

		void clearCell() {
			points.clear();
			cell_centroid.x = 0;
			cell_centroid.y = 0;
			cell_energy = 0.;
		}

		void setNewCenter() {
			int point_num = points.size();
			if (point_num != 0) {
				cell_centroid /= point_num;
				cell_center = cell_centroid;
			}
		}
	};

	class Voronoi {
	public:
		int cell_num;
		std::vector<Cell> all_cell;
		double all_energy;

		int x_max;
		int x_min;

		int y_max;
		int y_min;

		Voronoi(int xmax, int xmin, int ymax, int ymin, int num = 0) {
			cell_num = num;
			all_cell.clear();
			x_max = xmax; x_min = xmin;
			y_max = ymax; y_min = ymin;
		}

		void addCell(Point p)
		{
			Cell cell(p);
			cell_num++;
			all_cell.push_back(cell);
		}

		void clearVoronoi() {
			for (int i = 0; i < cell_num; i++) {
				all_cell[i].clearCell();
			}
			all_energy = 0.;
		}

		double computeDistance(Point p1, Point p2) {
			Point tmp = p1 - p2;
			double dis = tmp.ddot(tmp);
			return dis;
		}

		void replaceCenter() {
			for (int i = 0; i < cell_num; i++) {
				all_cell[i].setNewCenter();
			}
		}

		double getVoronoiByPixel() {

			clearVoronoi();

			for (int i = x_min; i <= x_max; i++) {
				for (int j = y_min; j <= y_max; j++) {

					double min_distance = DBL_MAX;
					Point pixel(i, j);
					int cell_index = -1;

					for (int k = 0; k < cell_num; k++) {
						double distance = computeDistance(pixel, all_cell[k].cell_center);
						if (distance < min_distance)
						{
							min_distance = distance;
							cell_index = k;
						}
					}

					all_cell[cell_index].points.push_back(pixel); 
					all_cell[cell_index].cell_centroid += pixel;
					all_cell[cell_index].cell_energy += min_distance;

				}
			}

			for (int k = 0; k < cell_num; k++) {
				all_energy += all_cell[k].cell_energy;
			}
			return all_energy;
		}
	};

}
#endif // !Voronoi_h
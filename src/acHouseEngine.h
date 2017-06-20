#ifndef ACHOUSEENGINE_H
#define ACHOUSEENGINE_H

#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/IO/print_wavefront.h>

#include "Constraints.h"

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef Kernel::Point_3 Point;
typedef CGAL::Polyhedron_3<Kernel> Polyhedron;
typedef Polyhedron::Halfedge_handle Halfedge_handle;

class acHouseEngine
{
	public:
		acHouseEngine();
		virtual ~acHouseEngine();

		void genHouse(Constraints c);
		void genBuild(Constraints c);

	private:
		Constraints *c;

		void saveHouse(string path, Polyhedron house);
		vector<Point> basePoints(double width, double length, double height);
		vector<Point> roofPoints(vector<Point> basepoints);
		vector<Point> protPoints(vector<Point> basepoints);
		vector<Point> bPoints(double height, int steps);
		double baseWidth();
		double baseLength();
		double baseHeight();
		double randomD(double min, double max);
		Halfedge_handle make_house(Polyhedron& P, vector<Point> points);
};

#endif

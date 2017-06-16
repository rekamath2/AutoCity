#ifndef ACHOUSEENGINE_H
#define ACHOUSEENGINE_H

#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/IO/print_wavefront.h>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef CGAL::Polyhedron_3<Kernel> Polyhedron;

class acHouseEngine
{
	public:
		acHouseEngine();
		virtual ~acHouseEngine();

		void genHouse(string path);

	private:
		void saveHouse(string path, Polyhedron house);
};

#endif

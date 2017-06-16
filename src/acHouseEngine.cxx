#include "acHouseEngine.h"

using namespace std;

template <class Poly>
typename Poly::Halfedge_handle make_house(Poly& P){
	CGAL_precondition(P.is_valid());
	typedef typename Poly::Point_3 Point;
	typedef typename Poly::Halfedge_handle Halfedge_handle;
	Halfedge_handle h = P.make_tetrahedron(
			Point(1.0,-1.0,-1.0),
			Point(-1.0,-1.0,1.0),
			Point(-1.0,-1.0,-1.0),
			Point(-1.0,1.0,-1.0));
	Halfedge_handle g = h->next()->opposite()->next();
	P.split_edge(h->next());
	P.split_edge(g->next());
	P.split_edge(g);
	h->next()->vertex()->point() = Point(1,-1,1);
	g->next()->vertex()->point() = Point(-1,1,1);
	g->opposite()->vertex()->point() = Point(1,1,-1);
	Halfedge_handle f = P.split_facet(g->next(),
			                  g->next()->next()->next());
	Halfedge_handle e = P.split_edge(f);
	e->vertex()->point() = Point(1,1,1);
	P.split_facet(e, f->next()->next());
	CGAL_postcondition(P.is_valid());
	return h;
}

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef CGAL::Polyhedron_3<Kernel> Polyhedron;
typedef Polyhedron::Halfedge_handle Halfedge_handle;

acHouseEngine::acHouseEngine()
{

}

acHouseEngine::~acHouseEngine()
{

}

void acHouseEngine::genHouse(string path)
{
	//TODO: randomly generate house in polyhedron 'house'
	Polyhedron house1;
	Halfedge_handle h = make_house(house1);
	saveHouse(path, house1);
}

void acHouseEngine::saveHouse(string path, Polyhedron house)
{
	ofstream ofs(path);
	ofs << "mtllib Cube.mtl" << endl;
	CGAL::print_polyhedron_wavefront(ofs, house);
}

#include "acHouseEngine.h"

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef Kernel::Point_3 Point;
typedef CGAL::Polyhedron_3<Kernel> Polyhedron;
typedef Polyhedron::Halfedge_handle Halfedge_handle;

Halfedge_handle acHouseEngine::make_house(Polyhedron& P, vector<Point> points){
	CGAL_precondition(P.is_valid());
	Halfedge_handle h = P.make_tetrahedron(
			points.at(0),
			points.at(1),
			points.at(2),
			points.at(3));
	Halfedge_handle g = h->next()->opposite()->next();
	P.split_edge(h->next());
	P.split_edge(g->next());
	P.split_edge(g);
	h->next()->vertex()->point() = points.at(4);
	g->next()->vertex()->point() = points.at(5);
	g->opposite()->vertex()->point() = points.at(6);
	Halfedge_handle f = P.split_facet(g->next(),
			                  g->next()->next()->next());
	Halfedge_handle e = P.split_edge(f);
	e->vertex()->point() = points.at(7);
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

double acHouseEngine::randomD(double min, double max)
{
	double f = (double)rand() / RAND_MAX;
	return min + f * (max - min);
}

vector<Point> acHouseEngine::basePoints(double width, double length, double height)
{
	vector<Point> points;
	points.push_back(Point(width/2.0,-length/2.0,0));//bottom right front
	points.push_back(Point(-width/2.0,-length/2.0,height));//top left front
	points.push_back(Point(-width/2.0,-length/2.0,0));//bottom left front
	points.push_back(Point(-width/2.0,length/2.0,0));//bottom left back
	points.push_back(Point(width/2.0,-length/2.0,height));//top right front
	points.push_back(Point(-width/2.0,length/2.0,height));//top left back
	points.push_back(Point(width/2.0,length/2.0,0));//bottom right back
	points.push_back(Point(width/2.0,length/2.0,height));//top right back
	return points;
}

vector<Point> acHouseEngine::roofPoints(vector<Point> basepoints)
{
	vector<Point> roofpoints;
	double h = basepoints.at(7).hz();
	double w = basepoints.at(7).hx() * 2.0;
	double l = basepoints.at(7).hy() * 2.0;
	double rh = c->getRHeight();
	double ro = c->getROverh();
	roofpoints.push_back(Point(w/2.0 + ro,-l/2.0,h));//bottom right front
	roofpoints.push_back(Point(0.0,-l/2.0,rh + h));//top left front
	roofpoints.push_back(Point(-w/2.0 - ro,-l/2.0,h));//bottom left front
	roofpoints.push_back(Point(-w/2.0 - ro,l/2.0,h));//bottom left back
	roofpoints.push_back(Point(0.0,-l/2.0,rh + h));//top right front
	roofpoints.push_back(Point(0.0,l/2.0,rh + h));//top left back
	roofpoints.push_back(Point(w/2.0 + ro,l/2.0,h));//bottom right back
	roofpoints.push_back(Point(0.0,l/2.0,rh + h));//top right back
	return roofpoints;
}

vector<Point> acHouseEngine::protPoints(vector<Point> basepoints)
{
	vector<Point> protpoints;
	double h = basepoints.at(7).hz();
	double w = basepoints.at(7).hx();
	double l = basepoints.at(7).hy();
	double x = randomD(0.0, l) - l/2.0;
	double pw = randomD(c->getPMinWidth(),c->getPMaxWidth());
	double pl = randomD(c->getPMinLength(),c->getPMaxLength());
	double rh = c->getRHeight();
	double ro = c->getROverh();

	protpoints.push_back(Point(0.0,x - pl,0.0));//bottom right front
	protpoints.push_back(Point(-pw,x - pl,h));//top left front
	protpoints.push_back(Point(-pw,x - pl,0.0));//bottom left front
	protpoints.push_back(Point(-pw,x + pl,0.0));//bottom left back
	protpoints.push_back(Point(0.0,x - pl,h));//top right front
	protpoints.push_back(Point(-pw,x + pl,h));// top left back
	protpoints.push_back(Point(0.0,x + pl,0.0));//bottom right back
	protpoints.push_back(Point(0.0,x + pl,h));//top right back

	protpoints.push_back(Point(0.0,x - pl - ro,h));//bottom right front
	protpoints.push_back(Point(-pw,x     ,h + rh));// top left front
	protpoints.push_back(Point(-pw,x - pl - ro,h));// bottom left front
	protpoints.push_back(Point(-pw,x + pl + ro,h));//bottom left back
	protpoints.push_back(Point(0.0,x     ,h + rh));// top right front
	protpoints.push_back(Point(-pw,x     ,h + rh));// top left back
	protpoints.push_back(Point(0.0,x + pl + ro,h));//bottom right back
	protpoints.push_back(Point(0.0,x     ,h + rh));//top right back

	return protpoints;
}

vector<Point> acHouseEngine::bPoints(double height, int step)
{
	vector<Point> bpoints;
	double xl = -randomD(c->bgetMinSide()/2.0, c->bgetMaxSide()/2.0/step);//left x boundary
	double xr =  randomD(c->bgetMinSide()/2.0, c->bgetMaxSide()/2.0/step);//right x boundary
	double yf = -randomD(c->bgetMinSide()/2.0, c->bgetMaxSide()/2.0/step);//front y boundary
	double yb =  randomD(c->bgetMinSide()/2.0, c->bgetMaxSide()/2.0/step);//back y boundary

	bpoints.push_back(Point(xr,yf,0.0));//bottom right front
	bpoints.push_back(Point(xl,yf,height));//top left front
	bpoints.push_back(Point(xl,yf,0.0));//bottom left front
	bpoints.push_back(Point(xl,yb,0.0));//bottom left back
	bpoints.push_back(Point(xr,yf,height));//top right front
	bpoints.push_back(Point(xl,yb,height));//top left back
	bpoints.push_back(Point(xr,yb,0.0));//bottom right back
	bpoints.push_back(Point(xr,yb,height));//top right back

	return bpoints;
}

double acHouseEngine::baseWidth()
{
	return randomD(c->getMinWidth(), c->getMaxWidth());
}

double acHouseEngine::baseLength()
{
	return randomD(c->getMinLength(), c->getMaxLength());
}

double acHouseEngine::baseHeight()
{
	return randomD(c->getMinHeight(), c->getMaxHeight());
}

void acHouseEngine::genHouse(Constraints c)
{
	this->c = &c;
	Polyhedron house1;
	vector<Point> basepoints = basePoints(baseWidth(), baseLength(), baseHeight());
	make_house(house1, basepoints);
	make_house(house1, roofPoints(basepoints));
	vector<Point> prot = protPoints(basepoints);
	make_house(house1, prot);
	prot.erase(prot.begin(), prot.begin() + 8);
	make_house(house1, prot);
	saveHouse("./tmp/house.obj", house1);
}

void acHouseEngine::genBuild(Constraints c)
{
	this->c = &c;
	Polyhedron build;
	vector<Point> bpoints;

	int steps = this->c->bgetSteps();
	double height = randomD(this->c->bgetMinHeight(), this->c->bgetMaxHeight());
	height /= (double)steps;

	for(int i = 1; i <= steps; i++){
		make_house(build, bPoints((double)height * i,i));
	}

	saveHouse("./tmp/building.obj", build);
}

void acHouseEngine::saveHouse(string path, Polyhedron house)
{
	ofstream ofs(path);
	ofs << "mtllib Cube.mtl" << endl;
	CGAL::print_polyhedron_wavefront(ofs, house);
}

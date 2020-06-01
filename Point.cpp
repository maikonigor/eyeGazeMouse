#include "Point.h"

Point::Point():
	x(0.0),
	y(0.0)
{
}

Point::Point(double x, double y):
	x(x),
	y(y)
{
}

Point::Point(CvPoint2D32f const &point):
	x(point.x),
	y(point.y)
{
}

Point::Point(CvPoint const &point):
	x(point.x),
	y(point.y)
{
}

double Point::distance(Point other) const {
    return fabs(other.x - x) + fabs(other.y - y);
}

double Point::distance2f(cv::Point2f other) const {
    return fabs(other.x - x) + fabs(other.y - y);
}

int Point::closestPoint(const std::vector<Point> &points) const {
    if (points.empty()) {
		return -1;
	}
	
	double minDistance = 10000;
	int minIndex = 0;
	
	for(int i=0; i<points.size(); i++) {
		double tempDistance = this->distance(points[i]);
		
		if(tempDistance < minDistance) {
			minDistance = tempDistance;
			minIndex = 0;
		}
	}
	
    return minIndex;
}

void Point::save(CvFileStorage *out, const char *name) const {
    cvStartWriteStruct(out, name, CV_NODE_MAP);
    cvWriteReal(out, "x", x);
    cvWriteReal(out, "y", y);
    cvEndWriteStruct(out);
}

void Point::load(CvFileStorage *in, CvFileNode *node) {
    x = cvReadRealByName(in, node, "x");
    y = cvReadRealByName(in, node, "y");
}

Point Point::operator+(const Point &other) const {
    return Point(x + other.x, y + other.y);
}

Point Point::operator-(const Point &other) const {
    return Point(x - other.x, y - other.y);
}

void Point::operator=(CvPoint2D32f const &point) {
    x = point.x;
    y = point.y;
}

void Point::operator=(CvPoint const &point) {
    x = point.x;
    y = point.y;
}

void Point::operator=(cv::Point2f const &point) {
    x = point.x;
    y = point.y;
}

std::ostream &operator<< (std::ostream &out, const Point &p) {
    out << p.x << " " << p.y << std::endl;
    return out;
}

std::istream &operator>> (std::istream &in, Point &p) {
    in >> p.x >> p.y;
	return in;
}

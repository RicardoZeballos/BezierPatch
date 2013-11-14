#if !defined(BEZIER_H)
#define BEZIER_H

#include "Curve.h"

// The Bezier class.  It just adds the draw method to Curve.  
// This is a basic Bezier, can be implemented with deCasteljau.

class Bezier : public Curve  
{
public:
	int detail;
	void draw(int levelOfDetail);
	void setDetail(int x);
	Pvector evaluatePoints(Pvector::iterator p1, Pvector::iterator p2 , Pvector::iterator p3, Pvector::iterator p4, int levelOfDetail);
	Pvector createPlane(Pvector startingPoints, int levelOfDetail);
	Bezier() {};
	virtual ~Bezier() {};

};

#endif

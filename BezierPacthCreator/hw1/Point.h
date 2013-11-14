#if !defined(POINT_H)
#define POINT_H

// The basic Point Class.  
// ACTIVE_RADIUS is just how big we draw the points when active 
//    which is set to 0.15 in Point.cpp
// x and y are simply spatial locations 
// active says if the point is currently active.
// draw draws the point. 
// There's also a constructor that instantiates it.  

class Point  
{

public:
	static const float ACTIVE_RADIUS;

	float x;
	float y;
	float z;
	bool active;
	
	void draw();
	void drawXYZ();
	Point(float xpos, float ypos);
	Point(float xpos, float ypos, float zpos);
	virtual ~Point();
};

#endif


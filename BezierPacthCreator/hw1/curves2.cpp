#include "stdafx.h"
//#include "WorkingScene.h"
#include "Point.h"
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std ; 
// This file includes the basic functions that your program must fill in.  
// Your assignment consists of filling in parts that say /* YOUR CODE HERE */

// What happens when you drag the mouse to x and y?  
// In essence, you are dragging control points on the curve.
/*
void WorkingScene::drag(int x, int y) {

	
	//you must figure out how to transform x and y so they make sense
	//update oldx, and oldy
	//make sure scene gets redrawn
	float oldnx = oldx/(float)width;
	float oldny = ((float)height-oldy)/(float)height;
	float nx = x/(float)width;
	float ny = ((float)height-y)/(float)height;
	float diffx = nx - oldnx;
	float diffy = ny - oldny;
 	theOnlyCurve->updateActivePoint(oldnx,oldny);
	theOnlyCurve->moveActivePoint(diffx, diffy);
	theOnlyCurve->draw(levelOfDetail);
	oldx = x;
	oldy = y;
	glutPostRedisplay();
}

// Mouse motion.  You need to respond to left clicks (to add points on curve) 
// and right clicks (to delete points on curve) 
void WorkingScene::mouse(int button, int state, int x, int y) {
	if (theOnlyCurve && state == GLUT_DOWN) {
		if (button == GLUT_LEFT_BUTTON) {
			float nx = x/(float)width;
			float ny = ((float)height-y)/(float)height;
			theOnlyCurve->addPoint(nx,ny);
			theOnlyCurve->draw(levelOfDetail);

			
		}
		if (button == GLUT_RIGHT_BUTTON) {	
			float nx = x/(float)width;
			float ny = ((float)height-y)/(float)height;
			theOnlyCurve->updateActivePoint(nx,ny);
			theOnlyCurve->deleteActivePoint() ;
			theOnlyCurve->draw(levelOfDetail);
			
		}
	}
	oldx = x;
	oldy = y;
	
	//update oldx, and oldy
	//make sure scene gets redrawn
	glutPostRedisplay();
}

*/

#include "Bezier.h"

void Bezier::setDetail(int x){
	detail = x;
}

// Bezier drawing function.  This is by deCasteljau or equivalent algorithm. 
// It should support Bezier curves of arbitrary degree/order.
void Bezier::draw(int levelOfDetail) {

	connectTheDots();
	int i,j,k;
	//Pvector::iterator p;
	float Bx, By, Nx, Ny;
	Pvector pointsCopy;// = new Pvector();
	if((int)points.size() >= 2){
		Bx = (points.begin())->x; //Sets the Point we start to be the first control point if it exists
		By = (points.begin())->y;
		
		float u = 1/(float)levelOfDetail;
		for(float h = 0 ; h <= 1; h= h+u){ //Controls how many points to evaluate u at
			for (Pvector::iterator it = points.begin(); it != points.end(); it++){
				pointsCopy.push_back(Point(it->x,it->y));
			}


			for(k = 0; k < (int)points.size() ; k++){ // The degree of the polynomial will tell us the level
				for (Pvector::iterator p = pointsCopy.begin(); p != (pointsCopy.end()-k-1); p++){
					p->x = (1-h)*((float)p->x) + (h*(float)((p+1)->x));
					p->y = (1-h)*((float)p->y) + (h*(float)((p+1)->y));
				}
			}
			Nx = (pointsCopy.begin())->x;
			Ny = (pointsCopy.begin())->y;
			cout << "Eval Count" << endl;
			drawLine(Bx, By, Nx, Ny);
			Bx = Nx;
			By = Ny;
			pointsCopy.clear();
		}
	}
	/* YOUR CODE HERE */
}


Pvector Bezier::evaluatePoints(Pvector::iterator p1, Pvector::iterator p2 , Pvector::iterator p3, Pvector::iterator p4, int levelOfDetail) {
	//cout << "Bezier evaluationPoints called" << endl;
	//Implement the code for hw6 here
	//Use the planePoints Pvector to do the algoritm 
	// the plane Points should have 16 points each with xyz coordinates

	Pvector points;
	points.push_back(Point::Point(p1->x, p1->y, p1->z));
	points.push_back(Point::Point(p2->x, p2->y, p2->z));
	points.push_back(Point::Point(p3->x, p3->y, p3->z));
	points.push_back(Point::Point(p4->x, p4->y, p4->z));
	int i,j,k;
	int detail = levelOfDetail;
	//Pvector::iterator p;
	float Bx, By, Bz, Nx, Ny, Nz;
	Pvector pointsCopy;// = new Pvector();
	Pvector EvaluatedPoints; //Will store the evaluated points
		Bx = (points.begin())->x; //Sets the Point we start to be the first control point if it exists
		By = (points.begin())->y;
		Bz = (points.begin())->z;
		//We dont need this kuss first point gets evaluated at u = 0
		//EvaluatedPoints.push_back(Point::Point(Bx ,By, Bz)); //We add the first control point 

		float u = 1/(float)detail;
		for(float h = 0 ; h <= 1; h= h+u){ //Controls how many points to evaluate u at
			for (Pvector::iterator it = points.begin(); it != points.end(); it++){
				pointsCopy.push_back(Point::Point(it->x,it->y,it->z));
			}


			for(k = 0; k < (int)points.size() ; k++){ // The degree of the polynomial will tell us the level
				for (Pvector::iterator p = pointsCopy.begin(); p != (pointsCopy.end()-k-1); p++){
					p->x = (1-h)*((float)p->x) + (h*(float)((p+1)->x));
					p->y = (1-h)*((float)p->y) + (h*(float)((p+1)->y));
					p->z = (1-h)*((float)p->z) + (h*(float)((p+1)->z));
				}
			}
			Nx = (pointsCopy.begin())->x;
			Ny = (pointsCopy.begin())->y;
			Nz = (pointsCopy.begin())->z;
			//drawLine(Bx, By, Nx, Ny);
			//Must create the new points and add them to the new Pvector
			EvaluatedPoints.push_back(Point::Point(Nx ,Ny, Nz));
			pointsCopy.clear();
		}
	return EvaluatedPoints;
}

Pvector Bezier::createPlane(Pvector startingPoints, int levelOfDetail){
	//cout << "createPlane called" << endl;
	//This will call the evaluate the control points and make the evalated points plane
	//planePoints contains all 16 input points
	Pvector tmpVector;
	Pvector tmpVectorEval;
	Pvector rowiseEvalPoints;
	Pvector rowEvaluatedPoints;
	Pvector returnVector;
	//cout << "StartingPoints vector is of size" << startingPoints.size() << endl;

	for(Pvector::iterator it = startingPoints.begin(); it != startingPoints.end(); it = it+4){ //Gets the 16 points and uses every 4 to evaluate
		tmpVectorEval.clear();
		tmpVectorEval = Bezier::evaluatePoints(it, it+1, it+2, it+3, levelOfDetail);
		//cout << "TMPvectorEval vector is of size" << tmpVectorEval.size() << endl;
	
		for (Pvector::iterator newIt = tmpVectorEval.begin(); newIt != tmpVectorEval.end(); newIt++){ //Depends on the number of evaluated points should be 6 if we use level of detail of 5
			rowiseEvalPoints.push_back(Point::Point(newIt->x, newIt->y, newIt->z)); //Created the final plane made up of the evaluated points evaluated using the row evaluations
		}
		tmpVectorEval.clear();
	}

	//for (Pvector::iterator itz = rowiseEvalPoints.begin(); itz != rowiseEvalPoints.end(); itz++){
	//		printf ("Point: %f %f %f \n", itz->x, itz->y, itz->z);
	//}
	//cout<< "Gets here" << endl;

	
	int rowSize = (rowiseEvalPoints.size()/4);
	for (Pvector::iterator itw = rowiseEvalPoints.begin(); itw != (rowiseEvalPoints.begin()+rowSize); itw++){ 
		Pvector tmpFinal;
		tmpFinal.clear();
		tmpFinal = Bezier::evaluatePoints(itw, itw + rowSize, itw + 2*rowSize, itw + 3*rowSize, levelOfDetail );
		for (Pvector::iterator ity = tmpFinal.begin(); ity != tmpFinal.end(); ity++){ //Depends on the number of evaluated points should again be 6 if we use levelOfDetail of 5
			//This last vector holds the evaluated values starting from the top left going down from our original, so this one is represented columnwise.
			returnVector.push_back(Point::Point(ity->x, ity->y, ity->z)); //Created the final plane made up of the evaluated points evaluated using the row evaluations
		}
	}

	
	return returnVector;
}

#include "Bspline.h"

// The B-Spline drawing routine.  
// Remember to call drawSegment (auxiliary function) for each set of 4 points.
void Bspline::draw(int levelOfDetail) {
	

	connectTheDots();
	if((int)points.size() >= 4){
		for (Pvector::iterator it = points.begin(); it != (points.end()-3); it++){
			drawSegment(it, it+1, it+2, it+3, levelOfDetail);
			}
	}
	/* YOUR CODE HERE */
}

void Bspline::drawSegment(Pvector::iterator p1, Pvector::iterator p2, Pvector::iterator p3, Pvector::iterator p4, int levelOfDetail) {

	float x, y, Bx, By, Nx, Ny,P1X,P1Y, P2X,P2Y, P3X,P3Y, P4X,P4Y;
	P1X=p1->x*1/3 + p2->x*2/3;
	P1Y=p1->y*1/3 + p2->y*2/3;
	P2X=p2->x*2/3 + p3->x*1/3;
	P2Y=p2->y*2/3 + p3->y*1/3;
	//Getting the Points in between P1X and P2X
	P1X=P1X*1/2 + P2X*1/2;
	P1Y=P1Y*1/2 + P2Y*1/2;
	P3X=p2->x*1/3 + p3->x*2/3;
	P3Y=p2->y*1/3 + p3->y*2/3;
	P4X=p3->x*2/3 + p4->x*1/3;
	P4Y=p3->y*2/3 + p4->y*1/3;
	//Getting the Points in between P3X and P4X
	P4X=P3X*1/2 + P4X*1/2;
	P4Y=P3Y*1/2 + P4Y*1/2;
	x=P4X;
	y=P4Y;
	Bx=P1X;
	By=P1Y;
	/* YOUR CODE HERE */
	float u = 1/(float)levelOfDetail;
	for(float d = u ; d <= 1; d+=u){
		Nx=((P1X)*((1-d)*(1-d)*(1-d))) + P2X*(3*(d*(1-d)*(1-d))) + P3X*(3*d*d*(1-d)) + P4X*(d*d*d);
		Ny=((P1Y)*((1-d)*(1-d)*(1-d))) + P2Y*(3*(d*(1-d)*(1-d))) + P3Y*(3*d*d*(1-d)) + P4Y*(d*d*d);
		drawLine(Bx, By, Nx, Ny);
		Bx=Nx;
		By=Ny;
	}
	//draw segment
	
	//then create a Point to be drawn where the knot should be

	Point p(x, y);
	p.draw();
}

#include "Bezier2.h"

//This function is provided to aid you.
//It should be used in the spirit of recursion, though you may choose not to.
//This function takes an empty vector of points, accum
//It also takes a set of control points, pts, and fills accum with
//the control points that correspond to the next level of detail.
void accumulateNextLevel(Pvector* accum, Pvector pts) {
	if (pts.empty()) return; 
	accum->push_back(*(pts.begin()));
	if (pts.size() == 1) return;
	for (Pvector::iterator it = pts.begin(); it != pts.end() - 1; it++) {

		float midx = (it->x + (it+1)->x)/2;
		float midy = (it->y + (it+1)->y)/2;
		it->x = midx;
		it->y = midy;
		/* YOUR CODE HERE  (only one to three lines)*/
	}
	//save the last point
	Point last = *(pts.end()-1);
	pts.pop_back();
	//recursive call
	accumulateNextLevel(accum, pts);
	accum->push_back(last);
}


// The basic draw function for Bezier2.  Note that as opposed to Bezier, 
// this draws the curve by recursive subdivision.  So, levelofdetail 
// corresponds to how many times to recurse.  
void Bezier2::draw(int levelOfDetail) {
	//This is just a trick to find out if this is the top level call
	//All recursive calls will be given a negative integer, to be flipped here
	if (levelOfDetail > 0) {
		connectTheDots();
	} else {
		levelOfDetail = -levelOfDetail;
	}

	//Base case.  No more recursive calls.
	if (levelOfDetail <= 1) {
		if (points.size() >= 2) {
			for (Pvector::iterator it = points.begin(); it != points.end() - 1; it++) {

				Pvector::iterator next = it+1; //Since there are no more recurvise calls we draw lines
				drawLine(it->x, it->y, next->x, next->y);
				/* YOUR CODE HERE */
				
			}
		}
	} else {
		Pvector* accum = new Pvector();
		Bezier2 left, right;

		//add the correct points to 'left' and 'right'.
		//You may or may not use accum as you see fit.
		/* YOUR CODE HERE */
		accumulateNextLevel(accum, points);
		int half = (((int)accum->size())/2);
		if(points.size() >= 2){
			for(Pvector::iterator p = accum->begin(); p != (accum->end()-(half)); p++){
				left.addPoint(p->x, p->y);
				right.addPoint((p+half)->x, (p+half)->y);
			}
		}

		left.draw(1-levelOfDetail);
		right.draw(1-levelOfDetail);
		delete accum;
	}
}

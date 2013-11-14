/*****************************************************************************/
/* This is the program skeleton for homework 1 in CS 184 by Ravi Ramamoorthi */
/* Based on an assignment designed by former TA Aner Ben-Artzi               */
/* Rewritten to be modern in terms of shaders etc.                           */
/* The idea is to draw a teapot, that can be moved by a crystal ball         */
/* interface                                                                 */
/*****************************************************************************/

#include "stdafx.h"
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "shaders.h"
#include "Transform.h"

//For our point vectors
#include <vector>
#include "Point.h"
#include "Bezier.h"
// To account for C++ namespace issues
#define vector std::vector
typedef vector<Point> Pvector;

int amount; // The amount of rotation for each arrow press

vec3 eye; // The (regularly updated) vector coordinates of the eye location
vec3 up;  // The (regularly updated) vector coordinates of the up location
const vec3 eyeinit(0.0,0.0,5.0) ; // Initial eye position, also for resets
const vec3 upinit(0.0,1.0,0.0) ; // Initial up position, also for resets
bool useGlu; // Toggle use of "official" opengl/glm transform vs user code
int w, h; // width and height
GLuint vertexshader, fragmentshader, shaderprogram ; // shaders

	//For our bezier patch
	Pvector EvaluatedPointsTest;
	Pvector StartingPointsVector;
	Bezier BezierObject;
	int levelOfDetail;

// Constants to set up lighting on the teapot
const GLfloat light_position[] = {0, 5, 10, 1};    // Position of light 0
const GLfloat light_position1[] = {0, 5, -10, 1};  // Position of light 1
const GLfloat light_specular[] = {0.6, 0.3, 0, 1};    // Specular of light 0
const GLfloat light_specular1[] = {0, 0.3, 0.6, 1};   // Specular of light 1
const GLfloat one[] = {1, 1, 1, 1};                 // Specular on teapot
const GLfloat medium[] = {0.5, 0.5, 0.5, 1};        // Diffuse on teapot
const GLfloat small[] = {0.2, 0.2, 0.2, 1};         // Ambient on teapot
const GLfloat high[] = {100} ;                      // Shininess of teapot
GLfloat light0[4], light1[4] ;

// Variables to set uniform params for lighting fragment shader
GLuint islight ;
GLuint light0posn ;
GLuint light0color ;
GLuint light1posn ;
GLuint light1color ;
GLuint ambient ;
GLuint diffuse ;
GLuint specular ;
GLuint shininess ;

// New helper transformation function to transform vector by modelview
// May be better done using newer glm functionality.
void transformvec (const GLfloat input[4], GLfloat output[4]) {
  GLfloat modelview[16] ; // in column major order
  glGetFloatv(GL_MODELVIEW_MATRIX, modelview) ;

  for (int i = 0 ; i < 4 ; i++) {
    output[i] = 0 ;
    for (int j = 0 ; j < 4 ; j++)
      output[i] += modelview[4*j+i] * input[j] ;
  }
}


void printHelp() {
  std::cout << "\npress 'h' to print this message again.\n"
       << "press '+' or '-' to change the amount of rotation that\noccurs with each arrow press.\n"
	   << "press 'g' to switch between using glm::lookAt or your own LookAt.\n"
       << "press 'r' to reset the transformation (eye and up).\n"
       << "press ESC to quit.\n" ;
 }

void keyboard(unsigned char key, int x, int y) {
	switch(key) {
	case '+':
		amount++;
		//levelOfDetail++;
		std::cout << "amount set to " << amount << "\n" ;
		std::cout << "levelOfDetail set to " << amount << "\n" ;
		break;
	case '-':
		amount--;
		//levelOfDetail--;
		std::cout << "amount set to " << amount << "\n" ;
		std::cout << "levelOfDetail set to " << amount << "\n" ;
		break;
	case 'g':
		useGlu = !useGlu;
		std::cout << "Using glm::LookAt set to: " << (useGlu ? " true " : " false ") << "\n" ;
		break;
	case 'h':
		printHelp();
		break;
	case 27:  // Escape to quit
		exit(0) ;
        break ;
    case 'r': // reset
		eye = eyeinit ;
		up = upinit ;
		break ;
	}
	glutPostRedisplay();
}

//  You will need to enter code for the arrow keys
//  When an arrow key is pressed, it will call your transform functions

void specialKey(int key, int x, int y) {
	switch(key) {
	case 100: //left
		Transform::left(amount, eye,  up);
		break;
	case 101: //up
		Transform::up(amount,  eye,  up);
		break;
	case 102: //right
		Transform::left(-amount,  eye,  up);
		break;
	case 103: //down
		Transform::up(-amount,  eye,  up);
		break;
	}
	glutPostRedisplay();
}

// Uses the Projection matrices (technically deprecated) to set perspective
// We could also do this in a more modern fashion with glm.
void reshape(int width, int height){
	w = width;
	h = height;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90, w/(float)h, 0.1, 99);
	glViewport(0, 0, w, h);
}



void init() {

  // Set up initial position for eye, up and amount
  // As well as booleans

	eye = vec3(0.0, 0.0, 5.0);
	up = vec3(0.0, 1.0, 0.0);
	amount = 5;
	useGlu = true;

	glEnable(GL_DEPTH_TEST);

  // The lighting is enabled using the same framework as in mytest 3
  // Except that we use two point lights
  // For now, lights and materials are set in display.  Will move to init
  // later, per update lights

      vertexshader = initshaders(GL_VERTEX_SHADER, "shaders/light.vert.glsl") ;
      fragmentshader = initshaders(GL_FRAGMENT_SHADER, "shaders/light.frag.glsl") ;
      shaderprogram = initprogram(vertexshader, fragmentshader) ;
      islight = glGetUniformLocation(shaderprogram,"islight") ;
      light0posn = glGetUniformLocation(shaderprogram,"light0posn") ;
      light0color = glGetUniformLocation(shaderprogram,"light0color") ;
      light1posn = glGetUniformLocation(shaderprogram,"light1posn") ;
      light1color = glGetUniformLocation(shaderprogram,"light1color") ;
      ambient = glGetUniformLocation(shaderprogram,"ambient") ;
      diffuse = glGetUniformLocation(shaderprogram,"diffuse") ;
      specular = glGetUniformLocation(shaderprogram,"specular") ;
      shininess = glGetUniformLocation(shaderprogram,"shininess") ;

	printf("Welcome to Homework 6.\n");
	printf("For this Homework we implemented bezier patches.\n");
	printf("Given 16 points on 3D space the programm will create a bezier patch.\n");
	printf("These are your 16 Control Points.\n");

	levelOfDetail=9;

	//Test Patch 1
	/*
	StartingPointsVector.push_back(Point::Point(1, 1, 1.75));
	StartingPointsVector.push_back(Point::Point(1.25, 1, 1.75));
	StartingPointsVector.push_back(Point::Point(1.50, 1, 1.75));
	StartingPointsVector.push_back(Point::Point(1.75, 1, 1.75));

	StartingPointsVector.push_back(Point::Point(1,1, 1.50));
	StartingPointsVector.push_back(Point::Point(1.25, 2, 1.50));
	StartingPointsVector.push_back(Point::Point(1.50, 2, 1.50));
	StartingPointsVector.push_back(Point::Point(1.75, 1, 1.50));

	StartingPointsVector.push_back(Point::Point(1, 1, 1.25));
	StartingPointsVector.push_back(Point::Point(1.25, 2, 1.25));
	StartingPointsVector.push_back(Point::Point(1.50, 2, 1.25));
	StartingPointsVector.push_back(Point::Point(1.75, 1, 1.25));

	StartingPointsVector.push_back(Point::Point(1, 1, 1));
	StartingPointsVector.push_back(Point::Point(1.25, 1, 1));
	StartingPointsVector.push_back(Point::Point(1.50, 1, 1));
	StartingPointsVector.push_back(Point::Point(1.75, 1, 1));
	*/

	//Test Patch 2
	/*
	StartingPointsVector.push_back(Point::Point(1, 1, 1.75));
	StartingPointsVector.push_back(Point::Point(1.25, 1.5, 1.75));
	StartingPointsVector.push_back(Point::Point(1.50, 2, 1.75));
	StartingPointsVector.push_back(Point::Point(1.75, 1.75, 1.75));

	StartingPointsVector.push_back(Point::Point(1, 1, 1.50));
	StartingPointsVector.push_back(Point::Point(1.25, 1.5, 1.50));
	StartingPointsVector.push_back(Point::Point(1.50, 2, 1.50));
	StartingPointsVector.push_back(Point::Point(1.75, 1.75, 1.50));

	StartingPointsVector.push_back(Point::Point(2, 1, 1.25));
	StartingPointsVector.push_back(Point::Point(2.25, 1.5, 1.25));
	StartingPointsVector.push_back(Point::Point(2.50, 2, 1.25));
	StartingPointsVector.push_back(Point::Point(2.75, 1.75, 1.25));

	StartingPointsVector.push_back(Point::Point(2, 1, 1));
	StartingPointsVector.push_back(Point::Point(2.25, 1.5, 1));
	StartingPointsVector.push_back(Point::Point(2.50, 2, 1));
	StartingPointsVector.push_back(Point::Point(2.75, 1.75, 1));
	*/

	//Test Patch 3
	
	StartingPointsVector.push_back(Point::Point(-3, 3, 2));
	StartingPointsVector.push_back(Point::Point(-3, 1, 0));
	StartingPointsVector.push_back(Point::Point(-3, -1, 0));
	StartingPointsVector.push_back(Point::Point(-3, -3, 0));

	StartingPointsVector.push_back(Point::Point(-1,3, 0));
	StartingPointsVector.push_back(Point::Point(-1, 1, -1));
	StartingPointsVector.push_back(Point::Point(-1, -1, 3));
	StartingPointsVector.push_back(Point::Point(-1, -3, 3));

	StartingPointsVector.push_back(Point::Point(1, 3, 0));
	StartingPointsVector.push_back(Point::Point(1, 1, 0));
	StartingPointsVector.push_back(Point::Point(1, -1, 0));
	StartingPointsVector.push_back(Point::Point(1, -3, 0));

	StartingPointsVector.push_back(Point::Point(3, 3, 3));
	StartingPointsVector.push_back(Point::Point(3, 1, -1));
	StartingPointsVector.push_back(Point::Point(3, -1, -1));
	StartingPointsVector.push_back(Point::Point(3, -3, 3));
	
	//Test Pacth 4

	/*
	StartingPointsVector.push_back(Point::Point(-3, 3, 1));
	StartingPointsVector.push_back(Point::Point(-3, 1, -1));	
	StartingPointsVector.push_back(Point::Point(-3, -1, 0));
	StartingPointsVector.push_back(Point::Point(-3, -3, -1));

	StartingPointsVector.push_back(Point::Point(-1,10, 0));
	StartingPointsVector.push_back(Point::Point(-1, 1, 1));
	StartingPointsVector.push_back(Point::Point(-1, -1, 1));
	StartingPointsVector.push_back(Point::Point(-1, -3, 1));

	StartingPointsVector.push_back(Point::Point(1, 10, 0));
	StartingPointsVector.push_back(Point::Point(1, 1, 1));
	StartingPointsVector.push_back(Point::Point(1, -1, 1));
	StartingPointsVector.push_back(Point::Point(1, -3, 1));

	StartingPointsVector.push_back(Point::Point(3, 3, 1));
	StartingPointsVector.push_back(Point::Point(3, 1, -1));
	StartingPointsVector.push_back(Point::Point(3, -1, 0));
	StartingPointsVector.push_back(Point::Point(3, -3, -1));
	*/

	for (Pvector::iterator it = StartingPointsVector.begin(); it != StartingPointsVector.end(); it++){
		printf ("Point: %f %f %f \n", it->x, it->y, it->z);
	}

	printf("Here are your evaluated points.\n");
	printf("Note: They are column wise from original 16 control points. \n");
	EvaluatedPointsTest.clear();
	EvaluatedPointsTest = BezierObject.createPlane(StartingPointsVector, levelOfDetail);
	for (Pvector::iterator it = EvaluatedPointsTest.begin(); it != EvaluatedPointsTest.end(); it++){
		printf ("Point: %f %f %f \n", it->x, it->y, it->z);
	}
	printf("Size of final pacth: %d.\n", EvaluatedPointsTest.size());
	printf("Show our patch.\n");

}

void display() {
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glMatrixMode(GL_MODELVIEW);
	mat4 mv ;
    const vec3 center(0.0,0.0,0.0) ;

    if (useGlu) mv = glm::lookAt(eye,center,up) ;
	else {
			mv = Transform::lookAt(eye,up) ;
            mv = glm::transpose(mv) ; // accounting for row major
		}
    glLoadMatrixf(&mv[0][0]) ;

        // Set Light and Material properties for the teapot
        // Lights are transformed by current modelview matrix.
        // The shader can't do this globally.
        // So we need to do so manually.
        transformvec(light_position, light0) ;
        transformvec(light_position1, light1) ;

		glUniform4fv(light0posn, 1, light0) ;
        glUniform4fv(light0color, 1, light_specular) ;
        glUniform4fv(light1posn, 1, light1) ;
        glUniform4fv(light1color, 1, light_specular1) ;

        //glUniform4fv(ambient,1,small) ;
        //glUniform4fv(diffuse,1,medium) ;
		glUniform4fv(ambient,1,small) ;
        glUniform4fv(diffuse,1,small) ;
        glUniform4fv(specular,1,one) ;
        glUniform1fv(shininess,1,high) ;
        glUniform1i(islight,true) ;

	//glutSolidTeapot(2);

	glPushMatrix() ; //Right Octa
		//glScalef(2.5,2.5,2.5);
		glTranslatef(-1,-1,-1);
		glColor3f(1,1,1);
		glUniform4f(ambient,0.03, 0.06, 0.5, 1) ; 
        glUniform4f(diffuse,0.7, 0.7, 0.7, 1) ; 
        glUniform4f(specular, 1, 1, 1, 1) ; 
		glUniform1f(shininess,100) ; 

	//Making the quads
		printf("evalPoints size: %d\n", EvaluatedPointsTest.size());
        Pvector::iterator itt=EvaluatedPointsTest.begin();
        for(int col=0; col < levelOfDetail; col++){
            for(int row=0; row < levelOfDetail; row++){
                int colSize=levelOfDetail+1;
                int offset = col*(colSize);
                //**calculate normal**
                vec3 U((itt+row+offset+colSize)->x - (itt+row+offset)->x, (itt+row+offset+colSize)->y - (itt+row+offset)->y, (itt+row+offset+colSize)->z - (itt+row+offset)->z); //p2-p1
                vec3 V((itt+row+offset+colSize+1)->x - (itt+row+offset)->x, (itt+row+offset+colSize+1)->y - (itt+row+offset)->y, (itt+row+offset+colSize+1)->z - (itt+row+offset)->z); //p3-p1
                float Nx = U.y*V.z - U.z*V.y; //UyVz - UzVy
                float Ny = U.z*V.x - U.x*V.z; //UzVx - UxVz
                float Nz = U.x*V.y - U.y*V.x; //UxVy - UyVx
                //making quad using shit
                glBegin(GL_QUADS);
                    glNormal3f(Nx, Ny, Nz);
                    glVertex3f( (itt+row+offset)->x,  (itt+row+offset)->y,  (itt+row+offset)->z);

                    glVertex3f( (itt+row+offset+colSize)->x,  (itt+row+offset+colSize)->y,  (itt+row+offset+colSize)->z);
                    glVertex3f( (itt+row+offset+colSize+1)->x,  (itt+row+offset+colSize+1)->y,  (itt+row+offset+colSize+1)->z);
                    glVertex3f( (itt+row+offset+1)->x, (itt+row+offset+1)->y, (itt+row+offset+1)->z);\
                glEnd();
				}
			}

	glPopMatrix() ;

	glutSwapBuffers();
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutCreateWindow("HW6: Bezier Patch");

	GLenum err = glewInit() ;
	if (GLEW_OK != err) {
		std::cerr << "Error: " << glewGetString(err) << std::endl;
	}

	init();
	glutDisplayFunc(display);
	glutSpecialFunc(specialKey);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	glutReshapeWindow(600, 400);
	printHelp();
	glutMainLoop();
	return 0;
}

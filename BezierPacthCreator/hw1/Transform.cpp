// Transform.cpp: implementation of the Transform class.

#include "stdafx.h"
#include "Transform.h"

//Takes as input the current eye position, and the current up vector.
//up is always normalized to a length of 1.
//eye has a length indicating the distance from the viewer to the origin

// Helper rotation function.  Please implement this.  

mat3 Transform::rotate(const float degrees, const vec3& axis) {
  mat3 R ;
  	float convRadians = degrees*(pi)/180 ;
	float a = cos(convRadians) ;
	float b = sin(convRadians) ;
	vec3 Zp = axis;
	mat3 H (0, -Zp[2], Zp[1], Zp[2], 0, -Zp[0], -Zp[1] ,Zp[0] ,0);
	mat3 identity (1,0,0,0,1,0,0,0,1);
	mat3 dotResult (Zp[0]*Zp[0], Zp[0]*Zp[1], Zp[0]*Zp[2], Zp[0]*Zp[1], Zp[1]*Zp[1], Zp[1]*Zp[2], Zp[0]*Zp[2], Zp[1]*Zp[2], Zp[2]*Zp[2]);
	R = a*identity + ((1-a)*dotResult) + b*H;
  // FILL IN YOUR CODE HERE
  return R ; 
}

void Transform::left(float degrees, vec3& eye, vec3& up) {
	float convRadians = degrees*(pi)/180 ;
	float a = cos(convRadians) ;
	float b = sin(convRadians) ;
	mat3 RotationMatrix = Transform::rotate(degrees,up);
	vec3 RotatedEye= eye * RotationMatrix;
	eye = RotatedEye;
	up = up * RotationMatrix;
	//FILL IN YOUR CODE HERE
}

void Transform::up(float degrees, vec3& eye, vec3& up) {
	float convRadians = degrees*(pi)/180 ;
	float a = cos(convRadians) ;
	float b = sin(convRadians) ;	
	vec3 Neye = -(glm::normalize(eye));
	vec3 ZAxis = glm::cross(Neye,up) ;
	mat3 RotationMatrix = Transform::rotate(degrees,-ZAxis) ;
	eye = eye * RotationMatrix;
	up = up *RotationMatrix;
	//Rotate eye vector the opposite direction that you did the other vector
	//FILL IN YOUR CODE HERE
}

mat4 Transform::lookAt(vec3 eye, vec3 up) {
    mat4 M ; 
	vec3 W = glm::normalize(eye);
	vec3 U = glm::normalize(glm::cross(up,W));
	vec3 V = glm::normalize(glm::cross(W,U));
	mat4 RMatrix (U[0], U[1], U[2], 0, V[0], V[1], V[2], 0, W[0], W[1], W[2], 0, 0, 0, 0, 1);
	mat4 TMatrix (1, 0, 0, -eye[0], 0, 1, 0, -eye[1], 0, 0, 1, -eye[2], 0, 0, 0, 1);
	M = TMatrix * RMatrix;
	//FILL IN YOUR CODE HERE
    //You must return a row-major mat4 M that you create from this routine
	return M ; 
}

Transform::Transform()
{

}

Transform::~Transform()
{

}

// Some notes about using glm functions.
// You are ONLY permitted to use glm::dot glm::cross glm::normalize
// Do not use more advanced glm functions (in particular, directly using 
// glm::lookAt is of course prohibited).  

// You may use overloaded operators for matrix-vector multiplication 
// But BEWARE confusion between opengl (column major) and row major 
// conventions, as well as what glm implements. 
// In particular, vecnew = matrix * vecold may not implement what you think 
// it does.  It treats matrix as column-major, in essence using the transpose.
// We recommend using row-major and vecnew = vecold * matrix 
// Preferrably avoid matrix-matrix multiplication altogether for this hw.  

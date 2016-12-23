/**
 *@file  mathutil.h
 *@brief Contains the various constant/macro/struct definitions and functions for matrix/vector manipulation
*/

#ifndef MATHUTIL_H
#define MATHUTIL_H

typedef double vector[3]; // this was earlier float

typedef double quaternion[4];// change

typedef double matrix[3][3];// change

void copy_vector(vector v_src, vector v_dest);

void copy_quaternion(quaternion q_src, quaternion q_dest);

double vector_norm(vector v);// change

double vector_dot_product(vector v_a, vector v_b);// change

void add_vectors(vector v_a, vector v_b, vector v_res);

void vector_into_matrix(vector v, matrix m, vector v_res);

void vector_cross_product(vector v_a, vector v_b, vector v_res);

void scalar_into_vector(vector v, double s);// floatch

void scalar_into_quaternion(quaternion q, double s);// floatch

void convert_unit_vector(vector v);

void convert_unit_quaternion(quaternion v);

#endif

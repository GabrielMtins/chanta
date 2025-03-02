#include "vec3.h"

void Vec3_Add(Vec3 *result, const Vec3 *a, const Vec3 *b){
	result->x = a->x + b->x;
	result->y = a->y + b->y;
	result->z = a->z + b->z;
}

void Vec3_Sub(Vec3 *result, const Vec3 *a, const Vec3 *b){
	result->x = a->x - b->x;
	result->y = a->y - b->y;
	result->z = a->z - b->z;
}

void Vec3_Mul(Vec3 *result, const Vec3 *a, float m){
	result->x = a->x * m;
	result->y = a->y * m;
	result->z = a->z * m;
}

void Vec3_Div(Vec3 *result, const Vec3 *a, float m){
	result->x = a->x / m;
	result->y = a->y / m;
	result->z = a->z / m;
}

#ifndef VEC3_H
#define VEC3_H

typedef struct {
	float x, y, z;
} Vec3;

void Vec3_Add(Vec3 *result, const Vec3 *a, const Vec3 *b);

void Vec3_Sub(Vec3 *result, const Vec3 *a, const Vec3 *b);

void Vec3_Mul(Vec3 *result, const Vec3 *a, float m);

void Vec3_Div(Vec3 *result, const Vec3 *a, float m);

float Vec3_SizeSqr(const Vec3 *);

#endif

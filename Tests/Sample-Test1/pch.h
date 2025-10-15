#pragma once

#include "gtest/gtest.h"

#pragma once

#include <iostream>
#include <vector>
#include <cmath>

#define PI 3.14159265358979323846

class Vector2D {
public:
    float x, y;

    Vector2D(float _x, float _y) : x(_x), y(_y) {}

    void print() const;
    Vector2D InverseVector();
    Vector2D AdditionVector(Vector2D v1, Vector2D v2);
    Vector2D AddScalar(float scalar) const;
    Vector2D MultiplyVectors(Vector2D v) const;
    Vector2D MultiplyByScalar(float scalar) const;
    Vector2D operator+(const Vector2D& other) const;
    Vector2D operator+(float scalar) const;
    Vector2D operator-(const Vector2D& other) const;
    Vector2D operator*(float scalar) const;
    Vector2D operator*(const Vector2D& other) const;
    Vector2D operator/(float scalar) const;
    Vector2D& operator+=(const Vector2D& other);
    Vector2D& operator-=(const Vector2D& other);
    Vector2D& operator*=(float scalar);
    Vector2D& operator*=(const Vector2D& other);
    Vector2D& operator/=(float scalar);
    float Norm() const;
    float DotProduct(const Vector2D& v1, const Vector2D& v2) const;
};

class Vector3D {
public:
    float x, y, z;

    Vector3D(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

    void print() const;
    Vector3D InverseVector();
    Vector3D AdditionVector(Vector3D v1, Vector3D v2);
    Vector3D AddScalar(float scalar) const;
    Vector3D MultiplyVectors(Vector3D v) const;
    Vector3D MultiplyByScalar(float scalar) const;
    Vector3D operator+(const Vector3D& other) const;
    Vector3D operator+(float scalar) const;
    Vector3D operator-(const Vector3D& other) const;
    Vector3D operator*(float scalar) const;
    Vector3D operator*(const Vector3D& other) const;
    Vector3D operator/(float scalar) const;
    Vector3D& operator+=(const Vector3D& other);
    Vector3D& operator-=(const Vector3D& other);
    Vector3D& operator*=(float scalar);
    Vector3D& operator*=(const Vector3D& other);
    Vector3D& operator/=(float scalar);
    float Norm() const;
    float DotProduct(const Vector3D& other) const;
    Vector3D CrossProduct(const Vector3D& other) const;
};

class Vector4D
{
public:
    float x = 0;
    float y = 0;
    float z = 0;
    float w = 0;

    Vector4D()
    {
    }
    ~Vector4D()
    {
    }
    Vector4D(float a, float b, float c, float d)
    {
        x = a;
        y = b;
        z = c;
        w = d;
    }
    Vector4D(const Vector3D& v, float d = 0)
    {
        x = v.x;
        y = v.y;
        z = v.z;
        w = d;
    }
};

class Mat4 {
public:
    Mat4(float e00, float e01, float e02, float e03,
        float e10, float e11, float e12, float e13,
        float e20, float e21, float e22, float e23,
        float e30, float e31, float e32, float e33)
    {
        data[0] = e00;  data[1] = e01;  data[2] = e02;  data[3] = e03;
        data[4] = e10;  data[5] = e11;  data[6] = e12;  data[7] = e13;
        data[8] = e20;  data[9] = e21;  data[10] = e22; data[11] = e23;
        data[12] = e30; data[13] = e31; data[14] = e32; data[15] = e33;
    }

    Mat4() {
        for (int i = 0; i < 16; i++)
            data[i] = (i % 5 == 0) ? 1.0f : 0.0f;
    }

    float data[16];
    bool isOrthogonal = false;

    void Print();
    void IsOrthogonal();
    Mat4 Identity();
    static Mat4 SetTranslation(const Vector3D& position);
    static Mat4 SetScale(const Vector3D& position);
    Mat4 MultiplyMatrices(const Mat4& other);
    static Mat4 RotationX(float angle);
    static Mat4 RotationY(float angle);
    static Mat4 RotationZ(float angle);
    Mat4 InvertMatrix();
    Mat4 Transpose() const;
    static Mat4 TRS(Vector3D pos, Vector3D rot, Vector3D scale);

    static float DegToRad(float deg);

    Mat4 operator*(const Mat4& other) const;
    Vector3D operator*(const Vector3D& other) const;

    Vector4D operator*(const Vector4D& v) const
    {
        return Vector4D(
            data[0] * v.x + data[4] * v.y + data[8] * v.z + data[12] * v.w,
            data[1] * v.x + data[5] * v.y + data[9] * v.z + data[13] * v.w,
            data[2] * v.x + data[6] * v.y + data[10] * v.z + data[14] * v.w,
            data[3] * v.x + data[7] * v.y + data[11] * v.z + data[15] * v.w
        );
    }
};
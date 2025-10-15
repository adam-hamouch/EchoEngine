#include "pch.h"
#include "vec2.hpp"
#include "vec3.hpp"
#include "vec4.hpp"
#include "matrix.hpp"
#include "gtc/matrix_transform.hpp"

Vector2D v1 = Vector2D(1, 1);
Vector2D v2 = Vector2D(5, 6);

glm::vec2 v1g = glm::vec2(1, 1);
glm::vec2 v2g = glm::vec2(5, 6);

Vector3D v3 = Vector3D(1, 1, 1);
Vector3D v4 = Vector3D(5, 6, 2);

glm::vec3 v3g = glm::vec3(1, 1, 1);
glm::vec3 v4g = glm::vec3(5, 6,2);

Vector4D v5 = Vector4D(1, 1, 1, 1);
Vector4D v6 = Vector4D(5, 6, 2, 3);

glm::vec4 v5g = glm::vec4(1, 1, 1, 1);
glm::vec4 v6g = glm::vec4(5, 6, 2, 3);

Mat4 m1 = Mat4(1.0f, 2.0f, 1.0f, 3.0f, 1.0f, 4.0f, 3.0f, 2.0f, 2.0f, 2.0f, 1.0f, 4.0f, 5.0f, 3.0f, 1.0f, 1.0f);
Mat4 m2 = Mat4(3.0f, 1.0f, 1.0f, 4.0f, 2.0f, 1.0f, 1.0f, 3.0f, 2.0f, 4.0f, 5.0f, 1.0f, 1.0f, 2.0f, 2.0f, 2.0f);

glm::mat4x4 m1g = glm::mat4x4(1.0f, 2.0f, 1.0f, 3.0f, 1.0f, 4.0f, 3.0f, 2.0f, 2.0f, 2.0f, 1.0f, 4.0f, 5.0f, 3.0f, 1.0f, 1.0f);
glm::mat4x4 m2g = glm::mat4x4(3.0f, 1.0f, 1.0f, 4.0f, 2.0f, 1.0f, 1.0f, 3.0f, 2.0f, 4.0f, 5.0f, 1.0f, 1.0f, 2.0f, 2.0f, 2.0f);

TEST(VectorAdd, TestName)
{
	Vector2D vAdd = vAdd.AdditionVector(v1, v2);
	glm::vec2 vgAdd = v1g + v2g;
	
	EXPECT_EQ(vAdd.x, vgAdd.x);
	EXPECT_EQ(vAdd.y, vgAdd.y);
}

TEST(VectorAddScalar, TestName)
{
	Vector2D vs = v1.AddScalar(15);
	glm::vec2 vgAdd = v1g + 15.0f;

	EXPECT_EQ(vs.x, vgAdd.x);
	EXPECT_EQ(vs.y, vgAdd.y);
}

TEST(VectorPlus, TestName)
{
	Vector2D vPlus = v1 + v2;
	glm::vec2 vgAdd = v1g + v2g;

	EXPECT_EQ(vPlus.x, vgAdd.x);
	EXPECT_EQ(vPlus.y, vgAdd.y);
}

TEST(VectorSoustract, TestName)
{
	Vector2D vSoustract = v1 - v2;
	glm::vec2 vgSoustract = v1g - v2g;

	EXPECT_EQ(vSoustract.x, vgSoustract.x);
	EXPECT_EQ(vSoustract.y, vgSoustract.y);
}

TEST(VectorMultiply, TestName)
{
	Vector2D vMult = v1 * v2;
	glm::vec2 vgMult = v1g * v2g;

	EXPECT_EQ(vMult.x, vgMult.x);
	EXPECT_EQ(vMult.y, vgMult.y);
}

TEST(VectorMultiplyScalar, TestName)
{
	Vector2D vMult = v1 * 5;
	glm::vec2 vgMult = v1g * 5.0f;

	EXPECT_EQ(vMult.x, vgMult.x);
	EXPECT_EQ(vMult.y, vgMult.y);
}

TEST(VectorDivideScalar, TestName)
{
	Vector2D vMult = v1 / 5;
	glm::vec2 vgMult = v1g / 5.0f;

	EXPECT_EQ(vMult.x, vgMult.x);
	EXPECT_EQ(vMult.y, vgMult.y);
}

TEST(VectorAdd3, TestName)
{
	Vector3D vAdd = vAdd.AdditionVector(v3, v4);
	glm::vec3 vgAdd = v3g + v4g;

	EXPECT_EQ(vAdd.x, vgAdd.x);
	EXPECT_EQ(vAdd.y, vgAdd.y);
	EXPECT_EQ(vAdd.z, vgAdd.z);
}

TEST(VectorAddScalar3, TestName)
{
	Vector3D vs = v3.AddScalar(15);
	glm::vec3 vgAdd = v3g + 15.0f;

	EXPECT_EQ(vs.x, vgAdd.x);
	EXPECT_EQ(vs.y, vgAdd.y);
	EXPECT_EQ(vs.z, vgAdd.z);
}

TEST(VectorPlus3, TestName)
{
	Vector3D vPlus = v3 + v4;
	glm::vec3 vgAdd = v3g + v4g;

	EXPECT_EQ(vPlus.x, vgAdd.x);
	EXPECT_EQ(vPlus.y, vgAdd.y);
	EXPECT_EQ(vPlus.z, vgAdd.z);
}

TEST(VectorSoustract3, TestName)
{
	Vector3D vSoustract = v3 - v4;
	glm::vec3 vgSoustract = v3g - v4g;

	EXPECT_EQ(vSoustract.x, vgSoustract.x);
	EXPECT_EQ(vSoustract.y, vgSoustract.y);
	EXPECT_EQ(vSoustract.z, vgSoustract.z);
}

TEST(VectorMultiply3, TestName)
{
	Vector3D vMult = v3 * v4;
	glm::vec3 vgMult = v3g * v4g;

	EXPECT_EQ(vMult.x, vgMult.x);
	EXPECT_EQ(vMult.y, vgMult.y);
	EXPECT_EQ(vMult.z, vgMult.z);
}

TEST(VectorMultiplyScalar3, TestName)
{
	Vector3D vMult = v3 * 5;
	glm::vec3 vgMult = v3g * 5.0f;

	EXPECT_EQ(vMult.x, vgMult.x);
	EXPECT_EQ(vMult.y, vgMult.y);
	EXPECT_EQ(vMult.z, vgMult.z);
}

TEST(VectorDivideScalar3, TestName)
{
	Vector3D vMult = v3 / 5;
	glm::vec3 vgMult = v3g / 5.0f;

	EXPECT_EQ(vMult.x, vgMult.x);
	EXPECT_EQ(vMult.y, vgMult.y);
	EXPECT_EQ(vMult.z, vgMult.z);
}

TEST(VectorCrossProduct3, TestName)
{
	Vector3D v = v3.CrossProduct(v4);
	glm::vec3 vg = glm::cross(v3g, v4g);

	EXPECT_EQ(v.x, vg.x);
	EXPECT_EQ(v.y, vg.y);
	EXPECT_EQ(v.z, vg.z);
}

TEST(Transpose, TestName)
{
	Mat4 mat = m1;
	glm::mat4x4 mg = m1g;

	mat = mat.Transpose();
	mg = glm::transpose(mg);

	EXPECT_EQ(mat.data[0], mg[0][0]);
	EXPECT_EQ(mat.data[1], mg[0][1]);
	EXPECT_EQ(mat.data[2], mg[0][2]);
	EXPECT_EQ(mat.data[3], mg[0][3]);
	EXPECT_EQ(mat.data[4], mg[1][0]);
	EXPECT_EQ(mat.data[5], mg[1][1]);
	EXPECT_EQ(mat.data[6], mg[1][2]);
	EXPECT_EQ(mat.data[7], mg[1][3]);
	EXPECT_EQ(mat.data[8], mg[2][0]);
	EXPECT_EQ(mat.data[9], mg[2][1]);
	EXPECT_EQ(mat.data[10], mg[2][2]);
	EXPECT_EQ(mat.data[11], mg[2][3]);
	EXPECT_EQ(mat.data[12], mg[3][0]);
	EXPECT_EQ(mat.data[13], mg[3][1]);
	EXPECT_EQ(mat.data[14], mg[3][2]);	
	EXPECT_EQ(mat.data[15], mg[3][3]);
}

TEST(Invert, TestName)
{
	Mat4 mat = m1;
	glm::mat4x4 mg = m1g;

	mat = mat.InvertMatrix();
	mg = glm::inverse(mg);

	EXPECT_EQ(mat.data[0], mg[0][0]);
	EXPECT_EQ(mat.data[1], mg[0][1]);
	EXPECT_EQ(mat.data[2], mg[0][2]);
	EXPECT_EQ(mat.data[3], mg[0][3]);
	EXPECT_EQ(mat.data[4], mg[1][0]);
	EXPECT_EQ(mat.data[5], mg[1][1]);
	EXPECT_EQ(mat.data[6], mg[1][2]);
	EXPECT_EQ(mat.data[7], mg[1][3]);
	EXPECT_EQ(mat.data[8], mg[2][0]);
	EXPECT_EQ(mat.data[9], mg[2][1]);
	EXPECT_EQ(mat.data[10], mg[2][2]);
	EXPECT_EQ(mat.data[11], mg[2][3]);
	EXPECT_EQ(mat.data[12], mg[3][0]);
	EXPECT_EQ(mat.data[13], mg[3][1]);
	EXPECT_EQ(mat.data[14], mg[3][2]);
	EXPECT_EQ(mat.data[15], mg[3][3]);
}

TEST(MultiplyMat, TestName)
{
	Mat4 mat = m1;
	glm::mat4x4 mg = m1g;

	Mat4 mat2 = m2;
	glm::mat4x4 mg2 = m2g;

	Mat4 result = mat.MultiplyMatrices(mat2);
	glm::mat4x4 resultg = mg * mg2;

	EXPECT_EQ(result.data[0], resultg[0][0]);
	EXPECT_EQ(result.data[1], resultg[0][1]);
	EXPECT_EQ(result.data[2], resultg[0][2]);
	EXPECT_EQ(result.data[3], resultg[0][3]);
	EXPECT_EQ(result.data[4], resultg[1][0]);
	EXPECT_EQ(result.data[5], resultg[1][1]);
	EXPECT_EQ(result.data[6], resultg[1][2]);
	EXPECT_EQ(result.data[7], resultg[1][3]);
	EXPECT_EQ(result.data[8], resultg[2][0]);
	EXPECT_EQ(result.data[9], resultg[2][1]);
	EXPECT_EQ(result.data[10], resultg[2][2]);
	EXPECT_EQ(result.data[11], resultg[2][3]);
	EXPECT_EQ(result.data[12], resultg[3][0]);
	EXPECT_EQ(result.data[13], resultg[3][1]);
	EXPECT_EQ(result.data[14], resultg[3][2]);
	EXPECT_EQ(result.data[15], resultg[3][3]);
}

TEST(MultiplyMatVec4, TestName)
{
	Mat4 mat = m1;
	glm::mat4x4 mg = m1g;

	Vector4D v = v5;
	glm::vec4 vg = v5g;

	Vector4D result = mat * v;
	glm::vec4 gresult = m1g * v5g;

	EXPECT_EQ(result.x, gresult.x);
	EXPECT_EQ(result.y, gresult.y);
	EXPECT_EQ(result.z, gresult.z);
	EXPECT_EQ(result.w, gresult.w);
}


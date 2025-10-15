#include "pch.h"

Vector2D Vector2D::InverseVector() {
    return Vector2D(-x, -y);
}

Vector2D Vector2D::AdditionVector(Vector2D v1, Vector2D v2) {
    return Vector2D(v1.x + v2.x, v1.y + v2.y);
}

Vector2D Vector2D::AddScalar(float scalar) const {
    return Vector2D(x + scalar, y + scalar);
}

Vector2D Vector2D::MultiplyVectors(Vector2D v) const {
    return Vector2D(x * v.x, y * v.y);
}

Vector2D Vector2D::MultiplyByScalar(float scalar) const {
    return Vector2D(x * scalar, y * scalar);
}

Vector2D Vector2D::operator+(const Vector2D& other) const {
    return Vector2D(x + other.x, y + other.y);
}

Vector2D Vector2D::operator+(float scalar) const {
    return AddScalar(scalar);
}

Vector2D Vector2D::operator-(const Vector2D& other) const {
    return Vector2D(x - other.x, y - other.y);
}

Vector2D Vector2D::operator*(float scalar) const {
    return MultiplyByScalar(scalar);
}

Vector2D Vector2D::operator*(const Vector2D& other) const
{
    return MultiplyVectors(other);
}

Vector2D Vector2D::operator/(float scalar) const {
    if (scalar != 0.0) {
        return Vector2D(x / scalar, y / scalar);
    }
    else {
        std::cout << " Division by zero." << std::endl;
        return *this;
    }
}

Vector2D& Vector2D::operator+=(const Vector2D& other) {
    x += other.x;
    y += other.y;
    return *this;
}

Vector2D& Vector2D::operator-=(const Vector2D& other) {
    x -= other.x;
    y -= other.y;
    return *this;
}

Vector2D& Vector2D::operator*=(float scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
}

Vector2D& Vector2D::operator*=(const Vector2D& other)
{
    x *= other.x;
    y *= other.y;
    return *this;
}

Vector2D& Vector2D::operator/=(float scalar) {
    if (scalar != 0.0) {
        x /= scalar;
        y /= scalar;
    }
    else {
        std::cout << "can't divide by zero" << std::endl;
    }
    return *this;
}

float Vector2D::Norm() const {
    return std::sqrt(x * x + y * y);
}

float Vector2D::DotProduct(const Vector2D& v1, const Vector2D& v2) const {
    return v1.x * v2.x + v1.y * v2.y;
}

Vector3D Vector3D::InverseVector() {
    return Vector3D(-x, -y, -z);
}

Vector3D Vector3D::AdditionVector(Vector3D v1, Vector3D v2) {
    return Vector3D(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

Vector3D Vector3D::AddScalar(float scalar) const {
    return Vector3D(x + scalar, y + scalar, z + scalar);
}

Vector3D Vector3D::MultiplyVectors(Vector3D v) const {
    return Vector3D(x * v.x, y * v.y, z * v.z);
}

Vector3D Vector3D::MultiplyByScalar(float scalar) const {
    return Vector3D(x * scalar, y * scalar, z * scalar);
}

float Vector3D::Norm() const {
    return std::sqrt(x * x + y * y + z * z);
}

Vector3D Vector3D::operator+(const Vector3D& other) const {
    return Vector3D(x + other.x, y + other.y, z + other.z);
}

Vector3D Vector3D::operator+(float scalar) const {
    return AddScalar(scalar);
}

Vector3D Vector3D::operator-(const Vector3D& other) const {
    return Vector3D(x - other.x, y - other.y, z - other.z);
}

Vector3D Vector3D::operator*(float scalar) const {
    return MultiplyByScalar(scalar);
}

Vector3D Vector3D::operator*(const Vector3D& other) const
{
    return MultiplyVectors(other);
}

Vector3D Vector3D::operator/(float scalar) const {
    if (scalar != 0.0) {
        return Vector3D(x / scalar, y / scalar, z / scalar);
    }
    else {
        std::cerr << " Division by zero." << std::endl;
        return *this;
    }
}

Vector3D& Vector3D::operator+=(const Vector3D& other) {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
}

Vector3D& Vector3D::operator-=(const Vector3D& other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
}

Vector3D& Vector3D::operator*=(float scalar) {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
}

Vector3D& Vector3D::operator*=(const Vector3D& other)
{
    x *= other.x;
    y *= other.y;
    z *= other.z;

    return *this;
}

Vector3D& Vector3D::operator/=(float scalar) {
    if (scalar != 0.0) {
        x /= scalar;
        y /= scalar;
        z /= scalar;
    }
    else {
        std::cerr << " Division by zero." << std::endl;
    }
    return *this;
}

float Vector3D::DotProduct(const Vector3D& other) const {
    return x * other.x + y * other.y + z * other.z;
}

Vector3D Vector3D::CrossProduct(const Vector3D& other) const {
    float resultX = y * other.z - z * other.y;
    float resultY = z * other.x - x * other.z;
    float resultZ = x * other.y - y * other.x;

    return Vector3D(resultX, resultY, resultZ);
}

void Mat4::Print()
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            std::cout << data[i * 4 + j] << "\t";
        }
        std::cout << std::endl;
    }
}

Mat4 Mat4::Identity() {
    return Mat4(1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);
}

Mat4 Mat4::MultiplyMatrices(const Mat4& other)
{
    float result[16];
    Mat4 mat = mat.Identity();

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            result[i * 4 + j] =
                data[i * 4 + 0] * other.data[0 * 4 + j] +
                data[i * 4 + 1] * other.data[1 * 4 + j] +
                data[i * 4 + 2] * other.data[2 * 4 + j] +
                data[i * 4 + 3] * other.data[3 * 4 + j];
        }
    }

    for (int i = 0; i < 16; ++i) {
        mat.data[i] = result[i];
    }

    return mat;
}


Mat4 Mat4::SetTranslation(const Vector3D& other)
{
    Mat4 translation = translation.Identity();

    translation.data[3] = other.x;
    translation.data[7] = other.y;
    translation.data[11] = other.z;

    return translation;
}

Mat4 Mat4::SetScale(const Vector3D& other)
{
    Mat4 scale = scale.Identity();

    scale.data[0] *= other.x;
    scale.data[5] *= other.y;
    scale.data[10] *= other.z;

    return scale;
}

Mat4 Mat4::RotationX(float angle)
{
    float radAngle = DegToRad(angle);

    return Mat4(1, 0, 0, 0,
        0, cos(radAngle), -sin(radAngle), 0,
        0, sin(radAngle), cos(radAngle), 0,
        0, 0, 0, 1);
}

Mat4 Mat4::RotationY(float angle)
{
    float radAngle = DegToRad(angle);

    return Mat4(cos(radAngle), 0, sin(radAngle), 0,
        0, 1, 0, 0,
        -sin(radAngle), 0, cos(radAngle), 0,
        0, 0, 0, 1);
}

Mat4 Mat4::RotationZ(float angle)
{
    float radAngle = DegToRad(angle);

    printf("%f", radAngle);

    return Mat4(cos(radAngle), -sin(radAngle), 0, 0,
        sin(radAngle), cos(radAngle), 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1);
}

Mat4 Mat4::InvertMatrix()
{
    Mat4 result = result.Identity();
    double inv[16], det;
    int i;

    inv[0] = data[5] * data[10] * data[15] -
        data[5] * data[11] * data[14] -
        data[9] * data[6] * data[15] +
        data[9] * data[7] * data[14] +
        data[13] * data[6] * data[11] -
        data[13] * data[7] * data[10];

    inv[4] = -data[4] * data[10] * data[15] +
        data[4] * data[11] * data[14] +
        data[8] * data[6] * data[15] -
        data[8] * data[7] * data[14] -
        data[12] * data[6] * data[11] +
        data[12] * data[7] * data[10];

    inv[8] = data[4] * data[9] * data[15] -
        data[4] * data[11] * data[13] -
        data[8] * data[5] * data[15] +
        data[8] * data[7] * data[13] +
        data[12] * data[5] * data[11] -
        data[12] * data[7] * data[9];

    inv[12] = -data[4] * data[9] * data[14] +
        data[4] * data[10] * data[13] +
        data[8] * data[5] * data[14] -
        data[8] * data[6] * data[13] -
        data[12] * data[5] * data[10] +
        data[12] * data[6] * data[9];

    inv[1] = -data[1] * data[10] * data[15] +
        data[1] * data[11] * data[14] +
        data[9] * data[2] * data[15] -
        data[9] * data[3] * data[14] -
        data[13] * data[2] * data[11] +
        data[13] * data[3] * data[10];

    inv[5] = data[0] * data[10] * data[15] -
        data[0] * data[11] * data[14] -
        data[8] * data[2] * data[15] +
        data[8] * data[3] * data[14] +
        data[12] * data[2] * data[11] -
        data[12] * data[3] * data[10];

    inv[9] = -data[0] * data[9] * data[15] +
        data[0] * data[11] * data[13] +
        data[8] * data[1] * data[15] -
        data[8] * data[3] * data[13] -
        data[12] * data[1] * data[11] +
        data[12] * data[3] * data[9];

    inv[13] = data[0] * data[9] * data[14] -
        data[0] * data[10] * data[13] -
        data[8] * data[1] * data[14] +
        data[8] * data[2] * data[13] +
        data[12] * data[1] * data[10] -
        data[12] * data[2] * data[9];

    inv[2] = data[1] * data[6] * data[15] -
        data[1] * data[7] * data[14] -
        data[5] * data[2] * data[15] +
        data[5] * data[3] * data[14] +
        data[13] * data[2] * data[7] -
        data[13] * data[3] * data[6];

    inv[6] = -data[0] * data[6] * data[15] +
        data[0] * data[7] * data[14] +
        data[4] * data[2] * data[15] -
        data[4] * data[3] * data[14] -
        data[12] * data[2] * data[7] +
        data[12] * data[3] * data[6];

    inv[10] = data[0] * data[5] * data[15] -
        data[0] * data[7] * data[13] -
        data[4] * data[1] * data[15] +
        data[4] * data[3] * data[13] +
        data[12] * data[1] * data[7] -
        data[12] * data[3] * data[5];

    inv[14] = -data[0] * data[5] * data[14] +
        data[0] * data[6] * data[13] +
        data[4] * data[1] * data[14] -
        data[4] * data[2] * data[13] -
        data[12] * data[1] * data[6] +
        data[12] * data[2] * data[5];

    inv[3] = -data[1] * data[6] * data[11] +
        data[1] * data[7] * data[10] +
        data[5] * data[2] * data[11] -
        data[5] * data[3] * data[10] -
        data[9] * data[2] * data[7] +
        data[9] * data[3] * data[6];

    inv[7] = data[0] * data[6] * data[11] -
        data[0] * data[7] * data[10] -
        data[4] * data[2] * data[11] +
        data[4] * data[3] * data[10] +
        data[8] * data[2] * data[7] -
        data[8] * data[3] * data[6];

    inv[11] = -data[0] * data[5] * data[11] +
        data[0] * data[7] * data[9] +
        data[4] * data[1] * data[11] -
        data[4] * data[3] * data[9] -
        data[8] * data[1] * data[7] +
        data[8] * data[3] * data[5];

    inv[15] = data[0] * data[5] * data[10] -
        data[0] * data[6] * data[9] -
        data[4] * data[1] * data[10] +
        data[4] * data[2] * data[9] +
        data[8] * data[1] * data[6] -
        data[8] * data[2] * data[5];


    det = data[0] * inv[0] + data[1] * inv[4] + data[2] * inv[8] + data[3] * inv[12];

    if (det == 0) {
        std::cout << det << std::endl;
        return Identity();
    }

    det = 1.0 / det;

    for (i = 0; i < 16; i++)
        result.data[i] = inv[i] * det;

    return result;
}

Mat4 Mat4::operator*(const Mat4& other) const
{
    Mat4 result;

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            result.data[i * 4 + j] =
                data[i * 4 + 0] * other.data[0 * 4 + j] +
                data[i * 4 + 1] * other.data[1 * 4 + j] +
                data[i * 4 + 2] * other.data[2 * 4 + j] +
                data[i * 4 + 3] * other.data[3 * 4 + j];
        }
    }

    return result;
}
Vector3D Mat4::operator*(const Vector3D& other) const
{
    float x = data[0] * other.x + data[1] * other.y + data[2] * other.z + data[3] * 1;
    float y = data[4] * other.x + data[5] * other.y + data[6] * other.z + data[7] * 1;
    float z = data[8] * other.x + data[9] * other.y + data[10] * other.z + data[11] * 1;
    float w = data[12] * other.x + data[13] * other.y + data[14] * other.z + data[15] * 1;

    if (w != 0.0f) {
        x /= w;
        y /= w;
        z /= w;
    }

    return Vector3D(x, y, z);
}

Mat4 Mat4::Transpose() const {
    return Mat4(
        data[0], data[4], data[8], data[12],
        data[1], data[5], data[9], data[13],
        data[2], data[6], data[10], data[14],
        data[3], data[7], data[11], data[15]
    );
}

Mat4 Mat4::TRS(Vector3D pos, Vector3D rot, Vector3D scale)
{
    return SetTranslation(pos) * (RotationX(rot.x) * RotationY(rot.y) * RotationZ(rot.z)) * SetScale(scale);
}

float Mat4::DegToRad(float deg)
{
    return deg * PI / 180;
}

void Mat4::IsOrthogonal()
{
    float epsilon = 1e-5;
    Mat4* mat = this;


    Mat4 transpose = mat->Transpose();
    Mat4 product = mat->MultiplyMatrices(transpose);

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            float expected = (i == j) ? 1.0f : 0.0f;
            if (std::fabs(product.data[i * 4 + j] - expected) > epsilon) {
                isOrthogonal = false;
                return;
            }
        }
    }
    isOrthogonal = true;
}

//
// Created by motong on 2026/10/16.
//
#include <iostream>
#include <cmath>
#include <Eigen/Core>
#include <Eigen/Dense>

int main(int argc,char** argv) {
    using namespace Eigen;
    Matrix3d rotation_matrix = Matrix3d::Identity();
    //旋转向量使用AngleAxis,底层不是矩阵，而是四元数，但运算时会转换为矩阵
    AngleAxisd rotation_vector(M_PI/4,Vector3d(0,0,1));//旋转45度，绕z轴
    std::cout.precision(3);
    std::cout << "rotation matrix = \n" << rotation_vector.matrix() << std::endl;//用matrix转为旋转矩阵
    rotation_matrix = rotation_vector.toRotationMatrix();
    Vector3d v(1,0,0);
    Vector3d v_rotated = rotation_vector * v;
    std::cout << "(1,0,0)after rotation (by angle axis) = " << v_rotated.transpose() << std::endl;

    //用旋转矩阵
    v_rotated = rotation_matrix * v;
    std::cout << "(1,0,0)after rotation (by rotation matrix) = " << v_rotated.transpose() << std::endl;

    //将旋转矩阵转换为欧拉角
    Vector3d euler_angles = rotation_matrix.eulerAngles(2,1,0);//ZYX顺序，即yaw pitch roll
    std::cout << "yaw pitch roll = " << euler_angles.transpose() << std::endl;

    //欧氏变换矩阵使用Eigen::Isometry
    Isometry3d T = Isometry3d::Identity();//虽然称为3d，实质上是4*4的矩阵
    T.rotate(rotation_vector);//按照rotation_vector进行旋转
    T.pretranslate(Vector3d(12,45,34));//把平移向量设成(12,45,34)
    std::cout << "Transform matrix = \n" << T.matrix() << std::endl;

    //用变换矩阵进行坐标变换
    Vector3d v_transformed = T * v;//相当于R*v+t
    std::cout << "(1,0,0)after transformation = " << v_transformed.transpose() << std::endl;

    //仿射变换与射影变换使用Eigen::Affine3d和Eigen::Projective3d即可，略

    //四元数
    //可以直接把AngleAxis赋值给四元数，反之亦然
    Quaterniond q = Quaterniond(rotation_vector);
    std::cout << "quaternion from rotation vector = " << q.coeffs().transpose() << std::endl;
    //注意coeffs的顺序是(x,y,z,w),w在后
    //也可以把旋转矩阵赋给它
    q = Quaterniond(rotation_matrix);
    std::cout << "quaternion from rotation matrix = " << q.coeffs().transpose() << std::endl;
    //使用四元数旋转一个向量，使用重载的乘法即可
    v_rotated = q * v;//注意数学上是qvq^{-1}
    std::cout << "(1,0,0)after rotation = " << v_rotated.transpose() << std::endl;
    //用常规向量乘法表示，则应该如下计算
    std::cout << "should be equal to " << (q * Quaterniond(0,v(0),v(1),v(2))
                                                    * q.inverse()).coeffs().transpose() << std::endl;
    return 0;
}
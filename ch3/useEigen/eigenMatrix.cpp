//
// Created by motong on 2026/10/15.
//
#include<iostream>
#include "ctime"
#include <Eigen/Core>
#include <Eigen/Dense>

#define  MatrixSize 50

int main(int argc ,char** argv) {
    //Eigen中的矩阵和向量类型都是Eigen::Matrix<数据类型,行数,列数>，其中数据类型可以是float,double,int等，行数和列数可以是固定的，也可以是动态的。
    // Eigen::Matrix<float,2,3> matrix_23;
    //
    // //Eigen中也提供了很多常用的矩阵类型的别名，比如Eigen::Vector3d表示三维向量，Eigen::Matrix3d表示3x3矩阵，Eigen::MatrixXd表示动态大小的矩阵等。
    // Eigen::Vector3d v_3d;
    // Eigen::Matrix<float,3,1> vd_3d;
    //
    // Eigen::Matrix3d matrix_33 = Eigen::Matrix3d::Zero();//初始化为零
    // Eigen::Matrix<double ,Eigen::Dynamic, Eigen::Dynamic> matrix_dynamic;//动态大小矩阵
    //
    // Eigen::MatrixXd matrix_x;
    //
    // //输入数据
    // matrix_23 << 1,2,3,4,5,6;
    // //输出
    // std::cout << "matrix_23 = \n" << matrix_23 << std::endl;

    //用()访问矩阵中的元素，注意索引是从0开始的
    // std::cout << "print matrix2x3" << std::endl;
    // for (int i = 0 ;i<2;i++)
    // {
    //     for (int j = 0 ;j<3;j++)
    //     {
    //         std::cout << matrix_23(i,j) << "\t";
    //     }
    //     std::cout << std::endl;
    // }
    //
    // //矩阵和向量的运算，不能混合不同类型的矩阵，需要进行显示转换
    // v_3d << 1,2,3;
    // vd_3d << 4,5,6;
    // Eigen::Matrix<double,2,1> result = matrix_23.cast<double>() * v_3d;
    // std::cout << "[1 2 3;4 5 6] * [1 2 3] = " << result.transpose() << std::endl;
    //
    // Eigen::Matrix<float,2,1> result2 = matrix_23 * vd_3d;
    // std::cout << "[1 2 3;4 5 6] * [4 5 6] = " << result2.transpose() << std::endl;

    // matrix_33 = Eigen::Matrix3d::Random();//随机数矩阵
    // std::cout << "random matrix_33 = \n" << matrix_33 << std::endl << std::endl;
    // std::cout << "transpose matrix_33 = \n" << matrix_33.transpose() << std::endl << std::endl;//转置
    // std::cout << "sum of matrix_33 = " << matrix_33.sum() << std::endl<< std::endl;//求和
    // std::cout << "trace of matrix_33 = " << matrix_33.trace() << std::endl<< std::endl;//迹
    // std::cout << "times 10 of matrix_33 = \n" << 10 * matrix_33 << std::endl<< std::endl;//数乘
    // std::cout << "inverse of matrix_33 = \n" << matrix_33.inverse() << std::endl<<std::endl;//逆
    // std::cout << "determinant of matrix_33 = " << matrix_33.determinant() << std::endl<< std::endl;//行列式
    //
    // //特征值
    // //实对称矩阵可以保证特征值为实数，特征向量正交
    // Eigen::SelfAdjointEigenSolver<Eigen::Matrix3d> eigen_solver(matrix_33.transpose());
    // std::cout << "Eigen values = \n" << eigen_solver.eigenvalues() << std::endl;
    // std::cout << "Eigen vectors = \n" << eigen_solver.eigenvectors() << std::endl;

    // 解方程
    Eigen::Matrix<double,MatrixSize,MatrixSize> matrix_NN = Eigen::MatrixXd::Random(MatrixSize,MatrixSize);
    matrix_NN = matrix_NN * matrix_NN.transpose();//保证半正定
    const Eigen::Matrix<double,MatrixSize,1> v_Nd = Eigen::MatrixXd::Random(MatrixSize,1);

    clock_t time_stt = clock();//记录起始时间
    //直接求逆
    Eigen::Matrix<double,MatrixSize,1> x = matrix_NN.inverse() * v_Nd;
    std::cout << "time of normal inverse is " << (static_cast<double>(clock()) - static_cast<double>(time_stt)) * 1000
                                                 / static_cast<double>(CLOCKS_PER_SEC) <<"ms." << std::endl;
    std::cout << "x=" <<x.transpose() << std::endl;

    time_stt = clock();
    //利用QR分解求解
    x = matrix_NN.colPivHouseholderQr().solve(v_Nd);
    std::cout << "time of QR decomposition is " << (static_cast<double>(clock()) - static_cast<double>(time_stt)) * 1000
                                                 / static_cast<double>(CLOCKS_PER_SEC) <<"ms." << std::endl;
    std::cout << "x=" <<x.transpose() << std::endl;

    //对正定矩阵进行Cholesky分解求解
    time_stt = clock();
    x = matrix_NN.ldlt().solve(v_Nd);
    std::cout << "time of Cholesky decomposition is " << (static_cast<double>(clock()) - static_cast<double>(time_stt)) * 1000
                                                 / static_cast<double>(CLOCKS_PER_SEC) <<"ms." << std::endl;
    std::cout << "x=" <<x.transpose() << std::endl;
    return 0;
}
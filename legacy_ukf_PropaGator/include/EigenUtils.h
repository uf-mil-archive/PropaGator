#ifndef LIBSUB_MATH_EIGENUTILS_H
#define LIBSUB_MATH_EIGENUTILS_H

#include <istream>
#include <vector>

#include <Eigen/Dense>

namespace subjugator {
    // Common
    using Eigen::Vector2d;
    using Eigen::Vector3d;
    using Eigen::Vector4d;
    typedef Eigen::Matrix<double, 6, 1> Vector6d;
    using Eigen::VectorXd;
    typedef Eigen::Matrix<double, 4, 4> Matrix4d;

    // Controller
    typedef Eigen::Matrix<double, 6, 6> Matrix6d;
    typedef Eigen::Matrix<double, 19, 1> Vector19d;
    typedef Eigen::Matrix<double, 19, 19> Matrix19d;
    typedef Eigen::Matrix<double, 19, 5> Matrix19x5d;

    // LPOSVSS
    typedef Eigen::Matrix<double, 13, 1> Vector13d;
    typedef Eigen::Matrix<double, 1, 26> RowVector26d;
    typedef Eigen::Matrix<double, 1, 27> RowVector27d;
    typedef Eigen::Matrix<double, 13, 13> Matrix13d;
    typedef Eigen::Matrix<double, 7, 7> Matrix7d;
    typedef Eigen::Matrix<double, 7, 1> Vector7d;
    typedef Eigen::Matrix<double, 13, 7> Matrix13x7d;
    typedef Eigen::Matrix<double, 13, 12> Matrix13x12d;
    typedef Eigen::Matrix<double, 13, 26> Matrix13x26d;
    typedef Eigen::Matrix<double, 13, 27> Matrix13x27d;
    typedef Eigen::Matrix<double, 12, 13> Matrix12x13d;
    typedef Eigen::Matrix<double, 12, 12> Matrix12d;
    typedef Eigen::Matrix<double, 4, 27> Matrix4x27d;
    typedef Eigen::Matrix<double, 3,27> Matrix3x27d;
    typedef Eigen::Matrix<double, 7, 27> Matrix7x27d;
    typedef Eigen::Matrix<double, 7, 26> Matrix7x26d;

    template <typename T>
    inline T sign(T x) {
        if (x > 0)
            return 1;
        else if (x < 0)
            return -1;
        else
            return 0;
    }

    template <typename Derived>
    Eigen::MatrixXd signs(const Eigen::MatrixBase<Derived> &x) {
        Eigen::MatrixXd out(x.rows(), x.cols());

        for (int i=0; i<x.rows(); i++)
            for (int j=0; j<x.cols(); j++)
                out(i, j) = sign(x(i, j));

        return out;
    }
}



namespace Eigen {
    template <typename T, int M, int N>
    std::istream &operator>>(std::istream &in, Eigen::Matrix<T, M, N> &mat) {
        for (int row=0; row<M; row++)
            for (int col=0; col<N; col++)
                in >> mat(row, col);
        return in;
    }

    template <typename T>
    std::istream &operator>>(std::istream &in, Eigen::Matrix<T, -1, 1> &mat) {
        std::vector<double> vals;
        while (true) {
            double val;
            in >> val;
            if (!in)
                break;
            vals.push_back(val);
        }
        in.clear();

        mat.resize(vals.size());
        for (unsigned int i=0; i<vals.size(); i++)
            mat(i) = vals[i];

        return in;
    }
}

#endif

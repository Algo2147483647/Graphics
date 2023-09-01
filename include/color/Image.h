#ifndef IMAGE_H
#define IMAGE_H

#include <Eigen/Dense>
#include "Color.h"

typedef Eigen::Matrix<ARGB, Eigen::Dynamic, Eigen::Dynamic> Image;
typedef Eigen::Matrix<RGB, Eigen::Dynamic, Eigen::Dynamic> Image_RGB;

template <typename Type>
using Mat = Eigen::Matrix<Type, Eigen::Dynamic, Eigen::Dynamic>;

#endif // !IMAGE_H

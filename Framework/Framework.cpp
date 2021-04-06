#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <cstdlib>
#include <algorithm>

int returnTriangle(int rows, int cols, cv::Point& point) {
    double ratio = rows / cols;
    double function1 = ratio * point.x;
    double function2 = -ratio * point.x + rows;
    if (point.y > function1) {
        if (point.y > function2) {
            return 1;//r+y
        }
        else return 0; //r-grey
    }
    else {
        if (point.y > function2) {
            return 2; //nop
        }
        else return 3; //r+x
	}
}

cv::Vec<int,8> perform(cv::Mat& I){
  CV_Assert(I.depth() != sizeof(uchar));
  cv::Mat_<cv::Vec3b> _I = I;
  cv::Vec<int,8> hist ( 0,0,0,0,0,0,0,0 );
  double x = 1.25;
  double y = 50;
  for (int i = 0; i < I.rows; ++i) {
     for (int j = 0; j < I.cols; ++j) {
            int triangle = returnTriangle(_I.rows,_I.cols, cv::Point(j, i));
            uchar grey = (_I(i, j)[0] + _I(i, j)[1] + _I(i, j)[2]) / 3;
            switch (triangle) {
                case 0: { //r-grey
                    _I(i, j)[0] = std::max(std::min(_I(i, j)[2] - grey, 255), 0);
                    _I(i, j)[1] = std::max(std::min(_I(i, j)[2] - grey, 255), 0);
                    _I(i, j)[2] = std::max(std::min(_I(i, j)[2] - grey, 255), 0);
                    break;
                }
                case 1: { //r+y
                    _I(i, j)[0] = std::max(std::min(_I(i, j)[0] + y, 255.0), 0.0);
                    _I(i, j)[1] = std::max(std::min(_I(i, j)[1] + y, 255.0), 0.0);
                    _I(i, j)[2] = std::max(std::min(_I(i, j)[2] + y, 255.0), 0.0);
                    break;
                }
                case 2: {   //nop
                    break;
                }
                case 3: {//r*x
                    _I(i, j)[0] = std::max(std::min(_I(i, j)[0]*x,255.0),0.0);
                    _I(i, j)[1] = std::max(std::min(_I(i, j)[1] * x, 255.0), 0.0);
                    _I(i, j)[2] = std::max(std::min(_I(i, j)[2] * x, 255.0), 0.0);
                    break;
                }
                default: break;
            }
            ++hist[grey / 32];
        }
    }
    I = _I;
    return hist;
}

int main(int, char *[]) {
    std::cout << "Start ..." << std::endl;
    cv::Mat image = cv::imread("Lena.png");
    int width = image.rows;
    int cols = image.cols;
    //cv::Mat image2 = image(cv::Rect(100,100,100,100));
    cv::Vec<int,8> hist = perform(image);
    for (int i = 0; i < 8; ++i) {
        std::cout << "Pikseli o jasnoœci " << i * 32 << "-" << (i + 1) * 32 - 1 << " jest" << hist[i] << std::endl;
    }
    std::cout << "Suma pikseli " << width * cols;
    //cv::Mat max = selectMax(image);
    cv::imshow("Lena",image);
    //std::cout << image2.isContinuous() << max.isContinuous() << std::endl;
    cv::imwrite("LenaResult.png",image);
    cv::waitKey(-1);
    return 0;
}

#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<iostream>
#include<opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include<fstream>
#include "opencv2/imgproc.hpp"
#include <set>
using namespace cv;


Mat Erosion(Mat img,int kernel[][5]) {
	Mat d(512, 512, CV_8UC1, Scalar(0));

	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
				int min=256;
				bool pass = true;
				for (int k = 0; k < 5; k++) {
					for (int l = 0; l < 5; l++) {
						if (kernel[k][l] == 1) {
							if ((i + k - 2 >= 0) && (i + k - 2 < 512) && (j + l - 2) >= 0 && (j + l - 2) < 512) {
								if (img.at<uchar>(i + k - 2, j + l - 2)  <min) {
									min = img.at<uchar>(i + k - 2, j + l - 2);
								}
							}
						}
					}
				}
				d.at<uchar>(i , j ) = min;
		}
	}
	return d;
}
Mat Dilation(Mat img,int kernel[][5]) {
	Mat d(512, 512, CV_8UC1, Scalar(0));
	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
				int max = 0;
				for (int k = 0; k < 5; k++) {
					for (int l = 0; l < 5; l++) {
						if (kernel[k][l] == 1) {
							if ((i + k - 2 >= 0) && (i + k - 2 < 512) && (j + l - 2) >= 0 && (j + l - 2) < 512) {
								if (img.at<uchar>(i + k - 2, j + l - 2) > max) {
									max = img.at<uchar>(i + k - 2, j + l - 2);
								}
							}
						}
					}
				}

			d.at<uchar>(i , j) = max;
							
						
					
				
			
		}
	}
	return d;
}
Mat Closing(Mat img, int kernel[][5]) {
	return Erosion(Dilation(img, kernel), kernel);
}
Mat Opening(Mat img,int kernel[][5]) {
	return Dilation(Erosion(img, kernel),kernel);
}

int main()
{
	// read image
	Mat img = imread("lena.bmp", CV_8UC1);
	int kernel[5][5] = { 0 };
	kernel[0][1] = 1; kernel[0][2] = 1; kernel[0][3] = 1;
	kernel[1][0] = 1; kernel[1][1] = 1; kernel[1][2] = 1; kernel[1][3] = 1; kernel[1][4] = 1;
	kernel[2][0] = 1; kernel[2][1] = 1; kernel[2][2] = 1; kernel[2][3] = 1; kernel[2][4] = 1;
	kernel[3][0] = 1; kernel[3][1] = 1; kernel[3][2] = 1; kernel[3][3] = 1; kernel[3][4] = 1;
	kernel[4][1] = 1; kernel[4][2] = 1; kernel[4][3] = 1;
	Mat d=Dilation(img, kernel);
	imwrite("dilation.bmp", d);
	Mat e = Erosion(img, kernel);
	imwrite("Erosion.bmp", e);
	Mat c = Closing(img, kernel);
	imwrite("Closing.bmp", c);
	Mat o = Opening(img, kernel);
	imwrite("Opening.bmp", o);

	return 0;
}


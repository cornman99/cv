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

void binarize(Mat img) {
	for (int i = 0; i < img.rows ; i++) {
		for (int j = 0; j < img.cols; j++) {

			if (img.at<uchar>(i, j) > 127){
				img.at<uchar>(i, j) = 255;
			}
			else img.at<uchar>(i, j) = 0;
		}
	}
	
}
Mat Erosion(Mat img,int kernel[][5]) {
	Mat d(512, 512, CV_8UC1, Scalar(0));

	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			
				bool pass = true;
				for (int k = 0; k < 5; k++) {
					for (int l = 0; l < 5; l++) {
						if (kernel[k][l] == 1) {
							if ((i + k - 2 < 0) || (i + k - 2 > 511) || (j + l - 2) < 0 || (j + l - 2) > 511 || img.at<uchar>(i + k - 2, j + l - 2) == 0) {
								pass = false;
							}
						}
					}
				}
				if (pass==true) {
					d.at<uchar>(i, j) = 255;
				}
				else { d.at<uchar>(i, j) = 0; }
			
		}
	}
	return d;
}
Mat Dilation(Mat img,int kernel[][5]) {
	Mat d(512, 512, CV_8UC1, Scalar(0));
	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			if (img.at<uchar>(i, j) != 0) {
				for (int k = 0; k < 5; k++) {
					for (int l = 0; l < 5; l++) {
						if (kernel[k][l] == 1) {
							if ((i + k - 2 >= 0) && (i + k - 2 < 512) && (j + l - 2) >= 0 && (j + l - 2) < 512) {
								d.at<uchar>(i + k - 2, j + l - 2) = 255;
							}
						}
					}
				}
			}
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
void hit (Mat img, int j[][5], int k[][5]) {
	Mat comp = 255 - img;
	Mat a = Erosion(img, j);
	Mat b = Erosion(comp, k);
	Mat res = Mat(img.rows, img.cols, CV_8UC1);
	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			if (a.at<uchar>(i, j) == 255&& b.at<uchar>(i, j) == 255) {
				res.at<uchar>(i, j) = 255;
			}
			else { res.at<uchar>(i, j) = 0; }
		}
	}
	imwrite("Hit and Miss.png", res);
	waitKey(0);
	
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
	binarize(img);
	int j[5][5] = { 0 };
	int k[5][5] = { 0 };
	k[1][2] = 1;
	k[1][3] = 1;
	k[2][3] = 1;
	j[2][1] = 1;
	j[2][2] = 1;
	j[3][2] = 1;
	Mat d=Dilation(img, kernel);
	imwrite("dilation.bmp", d);
	Mat e = Erosion(img, kernel);
	imwrite("Erosion.bmp", e);
	Mat c = Closing(img, kernel);
	imwrite("Closing.bmp", c);
	Mat o = Opening(img, kernel);
	imwrite("Opening.bmp", o);
	hit(img, j,k);
	return 0;
}


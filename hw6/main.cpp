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


Mat  binarize(Mat img) {
	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {

			if (img.at<uchar>(i, j) > 128) {
				img.at<uchar>(i, j) = 1;
			}
			else img.at<uchar>(i, j) = 0;
		}
	}
	// write image
	return img;
}
char h(int b, int c, int d, int e) {
	if (b == c && (d != b || e != b))
		return 'q';
	if (b == c && (d == b && e == b))
		return 'r';
	return 's';
}
int main()
{
	Mat img = imread("lena.bmp", CV_8UC1);
	Mat bin = binarize(img);
	Mat d(64, 64, CV_8UC1, Scalar(0));
	for (int i = 0; i < 64; i++) {
		for (int j = 0; j < 64; j++) {
			d.at<uchar>(i, j) = bin.at<uchar>(i*8, 8*j);
		}
	}
	for (int i = 0; i < 64; i++) {
		for (int j = 0; j < 64; j++) {
			if (d.at<uchar>(i, j)==1){
				int x7 = (i<64 && i>-1 && j > -1 && j < 64) ? d.at<uchar>(i - 1, j - 1) : 0;
				int x2 = (i<64 && i>-1 && j > -1 && j < 64) ? d.at<uchar>(i - 1, j) : 0;
				int x6 = (i<64 && i>-1 && j > -1 && j < 64) ? d.at<uchar>(i - 1, j + 1) : 0;
				int x3 = (i<64 && i>-1 && j > -1 && j < 64) ? d.at<uchar>(i, j - 1) : 0;
				int x0 = (i<64 && i>-1 && j > -1 && j < 64) ? d.at<uchar>(i, j) : 0;
				int x1 = (i<64 && i>-1 && j > -1 && j < 64) ? d.at<uchar>(i, j + 1) : 0;
				int x8 = (i<64 && i>-1 && j > -1 && j < 64) ? d.at<uchar>(i + 1, j - 1) : 0;
				int x4 = (i<64 && i>-1 && j > -1 && j < 64) ? d.at<uchar>(i + 1, j) : 0;
				int x5 = (i<64 && i>-1 && j > -1 && j < 64) ? d.at<uchar>(i + 1, j + 1) : 0;
				char a1 = h(x0, x1, x6, x2);
				char a2 = h(x0, x2, x7, x3);
				char a3 = h(x0, x3, x8, x4);
				char a4 = h(x0, x4, x5, x1);
				int t = 0;
				if (a1 == 'r' && a2 == 'r' && a3 == 'r' && a4 == 'r') {
					t = 5;
				}
				if (a1 == 'q')
					t += 1;
				if (a2 == 'q')
					t += 1;
				if (a3 == 'q')
					t +=1;
				if (a4 == 'q')
					t += 1;



				std::cout<< t<<" ";

			}
			else 
				std::cout  << "  ";
		}
		std::cout << '\n';
	}
	waitKey(500000);
	return 0;
}


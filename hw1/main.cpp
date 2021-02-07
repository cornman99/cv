#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<iostream>
#include<opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
void upsidedown(Mat img) {
	for (int i = 0; i < img.rows / 2; i++) {
		for (int j = 0; j < img.cols; j++) {
			int temp = img.at<uchar>(i, j);
			img.at<uchar>(i, j) = img.at<uchar>(img.rows - i-1, j);
			img.at<uchar>(img.rows - i-1, j) = temp;
		}
	}

	// write image 

	imwrite("upsidedownLena.jpg", img);
}
void binarize(Mat img) {
	for (int i = 0; i < img.rows ; i++) {
		for (int j = 0; j < img.cols; j++) {
			
			if (img.at<uchar>(i, j) > 128){
				img.at<uchar>(i, j) = 255;
			}
			else img.at<uchar>(i, j) = 1;
		}
	}

	// write image 

	imwrite("binar.jpg", img);
}
void leftsideright(Mat img) {
	for (int i = 0; i < img.rows ; i++) {
		for (int j = 0; j < img.cols/2; j++) {
			int temp = img.at<uchar>(i, j);
			img.at<uchar>(i, j) = img.at<uchar>(i, img.cols-j-1);
			img.at<uchar>(i , img.cols-j-1) = temp;
		}
	}

	// write image 
	imwrite("leftsideright.jpg", img);
}

void diagonallyflip(Mat img) {
	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols / 2; j++) {
			int temp = img.at<uchar>(i, j);
			img.at<uchar>(i, j) = img.at<uchar>(i, img.cols - j - 1);
			img.at<uchar>(i, img.cols - j - 1) = temp;
		}
	}
	for (int i = 0; i < img.rows / 2; i++) {
		for (int j = 0; j < img.cols; j++) {
			int temp = img.at<uchar>(i, j);
			img.at<uchar>(i, j) = img.at<uchar>(img.rows - i - 1, j);
			img.at<uchar>(img.rows - i - 1, j) = temp;
		}
	}

	// write image 
	imwrite("diagonallyflip.jpg", img);
}
int main()
{
	// read image 
	Mat img = imread("lena.bmp", CV_8UC1);
	Mat img1 = imread("lena.bmp", CV_8UC1);
	Mat img2 = imread("lena.bmp", CV_8UC1);
	Mat img3 = imread("lena.bmp", CV_8UC1);
	diagonallyflip(img);
	upsidedown(img2);
	leftsideright(img3);
	binarize(img1);
	return 0;
}


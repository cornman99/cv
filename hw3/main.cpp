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


void histogram(Mat img) {
	int hist[256] = { 0 };
	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			hist[img.at<uchar>(i, j)]++;
		}
	}

	// write image
	std::fstream f;
	f.open("histogram.csv", std::ios::out);
	for (int i = 0; i < 256; i++)
	{
		f << hist[i];
		f << '\n';
	}

}
void dv3(Mat img) {
	img /= 3;

	int hist[256] = { 0 };
	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			hist[img.at<uchar>(i, j)]++;
		}
	}

	// write image
	std::fstream f;
	f.open("histogramdv3.csv", std::ios::out);
	for (int i = 0; i < 256; i++)
	{
		f << hist[i];
		f << '\n';
	}
	imwrite("divideby3.jpg", img);

}
void equalization(Mat img) {
	int hist[256] = { 0 };
	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			hist[img.at<uchar>(i, j)]++;
		}
	}
	double cdf[256] = { 0 };
	int total = 0;
	for (int i = 0; i < 256; i++) {
		total += hist[i];
		cdf[i] = total;
	}
	for (int i = 0; i < 256; i++) {
		
		cdf[i] /= total;
	}

	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			img.at<uchar>(i, j) = cdf[img.at<uchar>(i, j)] * 255;
		}
	}
	int hist2[256] = { 0 };
	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			hist2[img.at<uchar>(i, j)]++;
		}
	}
	std::fstream f;
	f.open("equalization.csv", std::ios::out);
	for (int i = 0; i < 256; i++)
	{
		f << hist2[i];
		f << '\n';
	}
	imwrite("equalization.jpg", img);
}


int main()
{
	// read image
	Mat img = imread("lena.bmp", CV_8UC1);
	histogram(img);
	dv3(img);
	equalization( img);
	return 0;
}


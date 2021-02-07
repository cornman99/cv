#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<iostream>
#include<opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include<fstream>
#include "opencv2/imgproc.hpp"
#include <set>
#include<vector>
using namespace cv;

int h(int b, int c, int d, int e) {
	if (b != c)
		return 0;
	else if (b == d && d == e)
		return 2;
	else
		return 1;
}
int f(int b, int c, int d, int e) {
	int tmp = 0;
	if (b == c && c == d && d == e && e == 2)
		return 5;
	else
		return ((b == 1) + (c == 1) + (d == 1) + (e == 1));
}
int main()
{
	Mat img = imread("lena.bmp", CV_8UC1);
	Mat d1(64, 64, CV_8UC1, Scalar(0));
	for (int i = 0; i < 64; i++) {
		for (int j = 0; j < 64; j++) {

			if (img.at<uchar>(8*i,8* j) < 128) {
				d1.at<uchar>(i, j) = 0;
			}
			else d1.at<uchar>(i, j) = 255;
		}
	}
	std::vector<std::vector<int>>d;
	for (int i = 0; i < 64; i++) {
		std::vector<int>temp;
		for (int j = 0; j < 64; j++) {
			temp.push_back(d1.at<uchar>(i, j));
		}
		d.push_back(temp);
	}
	for (int q = 0; q < 7;q++) {
		std::vector<std::vector<int>>yo;
		for (int i = 0; i < 64; i++) {
			std::vector<int>temp;
			for (int j = 0; j < 64; j++) {
				int x7 = (i-1>-1 && j-1> -1 ) ? d[i-1][j-1] : 0;
				int x2 = (i-1>-1) ? d[i-1][j] : 0;
				int x6 = ( i-1>-1 && j+1 < 64) ? d[i-1][j+1] : 0;
				int x3 = (j-1 > -1) ? d[i][j-1] : 0;
				int x0 = d[i][j];
				int x1 = ( j+1< 64) ? d[i][j+1] : 0;
				int x8 = (i+1<64 && j-1> -1) ? d[i+1][j-1] : 0;
				int x4 = (i+1<64) ? d[i+1][j] : 0;
				int x5 = (i+1<64  && j+1< 64) ? d[i+1][j+1] : 0;
			if (d[i][j] == 0) {
				temp.push_back(0);
			}
			else {
				int a1 = h(x0, x1, x6, x2);
				int a2 = h(x0, x2, x7, x3);
				int a3 = h(x0, x3, x8, x4);
				int a4 = h(x0, x4, x5, x1);
				temp.push_back(f(a1, a2, a3, a4));
			}
			}
			yo.push_back(temp);
		}
		std::vector<std::vector<int>>pair;
		for (int i = 0; i < 64; i++) {
			std::vector<int>temp;
			for (int j = 0; j < 64; j++) {
				if (d[i][j] == 0) { temp.push_back(0); }
				else {
					if (yo[i][j] != 1) { temp.push_back(2); }
					else {
						int tmp = 0;
						if (j + 1 < 64) {
							if (yo[i][j + 1] == 1) {
								tmp += 1;
							}
						}
						if (i + 1 < 64) {
							if (yo[i + 1][j] == 1) {
								tmp += 1;
							}
						}
						if (i - 1 >= 0) {
							if (yo[i - 1][j] == 1) {
								tmp += 1;
							}
						}
						if (j - 1 >= 0) {
							if (yo[i][j - 1] == 1) {
								tmp += 1;
							}
						}
						if (tmp >= 1)
							temp.push_back(1);
						else
							temp.push_back(2);
					}
				}
			}
			pair.push_back(temp);
		}
		for (int i = 0; i < 64; i++) {
			for (int j = 0; j < 64; j++) {
				if (pair[i][j] == 1 && d[i][j] == 255) {
					int qc = 0;
					int x7 = (i - 1 > -1 && j - 1 > -1) ? d[i-1][j-1] : 0;
					int x2 = (i - 1 > -1) ? d[i-1][j] : 0;
					int x6 = (i - 1 > -1 && j + 1 < 64) ? d[i-1][j+1] : 0;
					int x3 = (j - 1 > -1) ? d[i][j-1] : 0;
					int x0 = d[i][j];
					int x1 = (j + 1 < 64) ? d[i][j+1] : 0;
					int x8 = (i + 1 < 64 && j - 1 > -1) ? d[i+1][j-1] : 0;
					int x4 = (i + 1 < 64) ? d[i+1][j] : 0;
					int x5 = (i + 1 < 64 && j + 1 < 64) ? d[i+1][j+1] : 0;
					int a1 = h(x0, x1, x6, x2);
					int a2 = h(x0, x2, x7, x3);
					int a3 = h(x0, x3, x8, x4);
					int a4 = h(x0, x4, x5, x1);
					qc = f(a1, a2, a3, a4);
					if (qc == 1) {
						d[i][j] = 0;
					}
				}
			}
		}
	}
	Mat d2(64, 64, CV_8UC1, Scalar(0));
	for (int i = 0; i < 64; i++) {
		for (int j = 0; j < 64; j++) {
			if(d[i][j]==255)
			d2.at<uchar>(i, j) = d[i][j];
		}
	}
	imwrite("thin2.bmp", d2);
	return 0;
}


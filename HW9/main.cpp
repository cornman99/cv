#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<iostream>
#include<opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include<fstream>
#include "opencv2/imgproc.hpp"
#include <set>
#include <random>
#include<vector>
#include<math.h>;
using namespace cv;

Mat robert(Mat img, int threshold) {
	Mat res= Mat(512, 512, CV_8UC1);
	for (int i = 0; i < 512; i++) {
		for (int j = 0; j < 512; j++) {
			double k[2][2];
			for (int m = 0; m < 2; m++) {
				for (int n = 0; n < 2; n++) {
					k[m][n] = (i + m <= 511 && j + n <=511) ? img.at<uchar>(i + m,j + n) : 0;
				}
			}
			int m1 = k[0][0] - k[1][1];
			int m2 = k[0][1] - k[1][0];
			double temp = sqrt(pow(m1, 2) + pow(m2, 2));
			res.at < uchar >(i , j) = (temp > threshold) ? 0 : 255;
		}
		
	}
	return res;
}
Mat prewitt(Mat img, int threshold) {
	Mat res = Mat(512, 512, CV_8UC1);
	for (int i = 0; i < 512; i++) {
		for (int j = 0; j < 512; j++) {
			double k[3][3];
			for (int m = 0; m < 3; m++) {
				for (int n = 0; n < 3; n++) {
					k[m][n] = (i -1+ m <= 511 && j-1 + n <= 511 && i - 1 + m >=0 && j - 1 + n >=0) ? img.at<uchar>(i -1+ m, j-1 + n) : img.at<uchar>(i , j );
				}
			}
			int m1 = (k[2][0] + k[2][1] + k[2][2]) - (k[0][0] + k[0][1] + k[0][2]);
			int m2 = (k[0][2] + k[1][2] + k[2][2]) - (k[0][0] + k[1][0] + k[2][0]);
			double temp = sqrt(pow(m1, 2) + pow(m2, 2));
			res.at < uchar >(i, j) = (temp > threshold) ? 0 : 255;
		}

	}
	return res;
}
Mat sobel(Mat img, int threshold) {
	Mat res = Mat(512, 512, CV_8UC1);
	for (int i = 0; i < 512; i++) {
		for (int j = 0; j < 512; j++) {
			double k[3][3];
			for (int m = 0; m < 3; m++) {
				for (int n = 0; n < 3; n++) {
					k[m][n] = (i - 1 + m <= 511 && j - 1 + n <= 511 && i - 1 + m >= 0 && j - 1 + n >= 0) ? img.at<uchar>(i - 1 + m, j - 1 + n) : img.at<uchar>(i, j);
				}
			}
			int m1 = (k[2][0] + k[2][1]*2 + k[2][2]) - (k[0][0] + k[0][1]*2 + k[0][2]);
			int m2 = (k[0][2] + k[1][2]*2 + k[2][2]) - (k[0][0] + k[1][0]*2+ k[2][0]);
			double temp = sqrt(pow(m1, 2) + pow(m2, 2));
			res.at < uchar >(i, j) = (temp > threshold) ? 0 : 255;
		}

	}
	return res;
}
Mat frei(Mat img, int threshold) {
	Mat res = Mat(512, 512, CV_8UC1);
	for (int i = 0; i < 512; i++) {
		for (int j = 0; j < 512; j++) {
			double k[3][3];
			for (int m = 0; m < 3; m++) {
				for (int n = 0; n < 3; n++) {
					k[m][n] = (i - 1 + m <= 511 && j - 1 + n <= 511 && i - 1 + m >= 0 && j - 1 + n >= 0) ? img.at<uchar>(i - 1 + m, j - 1 + n) : img.at<uchar>(i, j);
				}
			}
			double m1 = double(k[2][0] + k[2][1] * sqrt(2) + k[2][2]) - double(k[0][0] + k[0][1] * sqrt(2) + k[0][2]);
			double m2 = double(k[0][2] + k[1][2] * sqrt(2) + k[2][2]) - double(k[0][0] + k[1][0] * sqrt(2) + k[2][0]);
			double temp = sqrt(pow(m1, 2) + pow(m2, 2));
			res.at < uchar >(i, j) = (temp > threshold) ? 0 : 255;
		}

	}
	return res;
}
Mat kirsch(Mat img, int threshold) {
	Mat res = Mat(512, 512, CV_8UC1);
	for (int i = 0; i < 512; i++) {
		for (int j = 0; j < 512; j++) {
			double k[3][3];
			for (int m = 0; m < 3; m++) {
				for (int n = 0; n < 3; n++) {
					k[m][n] = (i - 1 + m <= 511 && j - 1 + n <= 511 && i - 1 + m >= 0 && j - 1 + n >= 0) ? img.at<uchar>(i - 1 + m, j - 1 + n) : img.at<uchar>(i, j);
				}
			}
			int mk[8], ma= -5 * 255;
			mk[0] = 5 * (k[0][2] + k[1][2] + k[2][2]) - 3 * (k[0][0] + k[0][1] + k[1][0] + k[2][0] + k[2][1]);
			mk[1] = 5 * (k[0][1] + k[0][2] + k[1][2]) - 3 * (k[0][0] + k[1][0] + k[2][0] + k[2][1] + k[2][2]);
			mk[2] = 5 * (k[0][0] + k[0][1] + k[0][2]) - 3 * (k[1][0] + k[1][2] + k[2][0] + k[2][1] + k[2][2]);
			mk[3] = 5 * (k[0][0] + k[0][1] + k[1][0]) - 3 * (k[0][2] + k[1][2] + k[2][0] + k[2][1] + k[2][2]);
			mk[4] = 5 * (k[0][0] + k[1][0] + k[2][0]) - 3 * (k[0][1] + k[0][2] + k[1][2] + k[2][1] + k[2][2]);
			mk[5] = 5 * (k[1][0] + k[2][0] + k[2][1]) - 3 * (k[0][0] + k[0][1] + k[0][2] + k[1][2] + k[2][2]);
			mk[6] = 5 * (k[2][0] + k[2][1] + k[2][2]) - 3 * (k[0][0] + k[0][1] + k[0][2] + k[1][0] + k[1][2]);
			mk[7] = 5 * (k[1][2] + k[2][1] + k[2][2]) - 3 * (k[0][0] + k[0][1] + k[0][2] + k[1][0] + k[2][0]);
			for (int m = 0; m < 8; m++) {
				ma = (ma > mk[m]) ? ma : mk[m];
			}
			res.at < uchar >(i, j) = (ma > threshold) ? 0 : 255;
		}
	}
	return res;
}
Mat robinson(Mat img, int threshold) {
	Mat res = Mat(512, 512, CV_8UC1);
	for (int i = 0; i < 512; i++) {
		for (int j = 0; j < 512; j++) {
			double k[3][3];
			for (int m = 0; m < 3; m++) {
				for (int n = 0; n < 3; n++) {
					k[m][n] = (i - 1 + m <= 511 && j - 1 + n <= 511 && i - 1 + m >= 0 && j - 1 + n >= 0) ? img.at<uchar>(i - 1 + m, j - 1 + n) : img.at<uchar>(i, j);
				}
			}
			int mk[8], ma = -5 * 255;
			mk[0] = (k[0][2] + k[1][2] * 2 + k[2][2]) - (k[0][0] + k[1][0] * 2 + k[2][0]);
			mk[1] = (k[0][1] + k[0][2] * 2 + k[1][2]) - (k[1][0] + k[2][0] * 2 + k[2][1]);
			mk[2] = (k[0][0] + k[0][1] * 2 + k[0][2]) - (k[2][0] + k[2][1] * 2 + k[2][2]);
			mk[3] = (k[0][1] + k[0][0] * 2 + k[1][0]) - (k[2][1] + k[2][2] * 2 + k[1][2]);
			mk[4] = (k[0][0] + k[1][0] * 2 + k[2][0]) - (k[0][2] + k[1][2] * 2 + k[2][2]);
			mk[5] = (k[1][0] + k[2][0] * 2 + k[2][1]) - (k[0][1] + k[0][2] * 2 + k[1][2]);
			mk[6] = (k[2][0] + k[2][1] * 2 + k[2][2]) - (k[0][0] + k[0][1] * 2 + k[0][2]);
			mk[7] = (k[1][2] + k[2][2] * 2 + k[2][1]) - (k[0][1] + k[0][0] * 2 + k[1][0]);
			for (int m = 0; m < 8; m++) {
				ma = (ma > mk[m]) ? ma : mk[m];
			}
			res.at < uchar >(i, j) = (ma > threshold) ? 0 : 255;
		}
	}
	return res;
}
Mat nevatia(Mat img, int threshold) {
	Mat res = Mat(512, 512, CV_8UC1);
	for (int i = 0; i < 512; i++) {
		for (int j = 0; j < 512; j++) {
			double k[5][5];
			for (int m = 0; m < 5; m++) {
				for (int n = 0; n < 5; n++) {
					k[m][n] = (i - 2 + m <= 511 && j - 2 + n <= 511 && i - 2 + m >= 0 && j - 2 + n >= 0) ? img.at<uchar>(i - 2 + m, j - 2 + n) : 0;
				}
			}
			int mk[6], ma = INT_MIN;
			mk[0] = 100 * ((k[0][0] + k[0][1] + k[0][2] + k[0][3] + k[0][4] + k[1][0] + k[1][1] + k[1][2] + k[1][3] + k[1][4]) -
				(k[3][0] + k[3][1] + k[3][2] + k[3][3] + k[3][4] + k[4][0] + k[4][1] + k[4][2] + k[4][3] + k[4][4]));
			mk[1] = 100 * ((k[0][0] + k[0][1] + k[0][2] + k[0][3] + k[0][4] + k[1][0] + k[1][1] + k[1][2] + k[2][0]) -
				(k[2][4] + k[3][2] + k[3][3] + k[3][4] + k[4][0] + k[4][1] + k[4][2] + k[4][3] + k[4][4])) +
				32 * (k[3][0] - k[1][4]) + 92 * (k[2][1] - k[2][3]) + 78 * (k[1][3] - k[3][1]);
			mk[2] = 100 * ((k[0][0] + k[0][1] + k[0][2] + k[1][0] + k[1][1] + k[2][0] + k[2][1] + k[3][0] + k[4][0]) -
				(k[0][4] + k[1][4] + k[2][3] + k[2][4] + k[3][3] + k[3][4] + k[4][2] + k[4][3] + k[4][4]));
			mk[3] = 100 * ((k[0][3] + k[0][3] + k[1][3] + k[1][4] + k[2][3] + k[2][4] + k[3][3] + k[3][4] + k[4][3] + k[4][4]) -
				(k[0][0] + k[0][1] + k[1][0] + k[1][1] + k[2][0] + k[2][1] + k[3][0] + k[3][1] + k[4][0] + k[4][1]));
			mk[4] = 100 * ((k[0][2] + k[0][3] + k[0][4] + k[1][3] + k[1][4] + k[2][3] + k[2][4] + k[3][4] + k[4][4]) -
				(k[0][0] + k[1][0] + k[2][0] + k[2][1] + k[3][0] + k[3][1] + k[4][0] + k[4][1] + k[4][2])) +
				32 * (k[0][1] - k[4][3]) + 92 * (k[1][2] - k[3][2]) + 78 * (k[3][3] - k[1][1]);
			mk[5] = 100 * ((k[0][0] + k[0][1] + k[0][2] + k[0][3] + k[0][4] + k[1][2] + k[1][3] + k[1][4] + k[2][4]) -
				(k[2][0] + k[3][0] + k[3][1] + k[3][2] + k[4][0] + k[4][1] + k[4][2] + k[4][3] + k[4][4])) +
				32 * (k[3][4] - k[1][0]) + 92 * (k[2][3] - k[2][1]) + 78 * (k[1][1] - k[3][3]);
			for (int m = 0; m < 6; m++) {
				ma = (ma > mk[m]) ? ma : mk[m];
			}
			res.at < uchar >(i, j) = (ma > threshold) ? 0 : 255;
		}
	}
	return res;
}
int main()
{

	Mat img = imread("lena.bmp", CV_8UC1);
	imwrite("Robert.bmp", robert(img, 12));
	imwrite("Prewitt.bmp", prewitt(img, 24));
	imwrite("sobel.bmp", sobel(img, 38));
	imwrite("Frei and Chen's.bmp", frei(img, 30));
	imwrite("Kirsch.bmp", kirsch(img, 135));
	imwrite("Robinson.bmp", robinson(img, 43));
	imwrite("Nevatia-Babu.bmp", nevatia(img, 12500));
	return 0;
}


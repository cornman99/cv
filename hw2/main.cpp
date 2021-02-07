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

			if (img.at<uchar>(i, j) > 128){
				img.at<uchar>(i, j) = 255;
			}
			else img.at<uchar>(i, j) = 1;
		}
	}
	// write image
	imwrite("binarize.jpg", img);
}
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
void cca(Mat img) {


	int label[512][512] = { 0 };
	int start = 1;
	for (int i = 0; i < 512; i++) {
		for (int j = 0; j < 512; j++) {
			if (img.at<uchar>(i, j) ==255 ) {
				label[i][j] = start;
				start++;
			}
		}
	}
	int count[512*512];
	bool running = 1;
	while (running) {
		running = 0;
		for (int i = 0; i < 512; i++) {
			for (int j = 0; j < 512; j++) {
				if (label[i][j]>0){
                    if((i-1>0)&&(label[i-1][j]>0)&&(label[i-1][j]<label[i][j])){
                       running=1;
                       label[i][j]=label[i-1][j];
                       }
                    if((j-1>0)&&(label[i][j-1]>0)&&(label[i][j-1]<label[i][j])){
                       running=1;
                       label[i][j]=label[i][j-1];

                       }
				}
			}
		}
        for (int i = 511; i >-1; i--) {
			for (int j = 511; j > -1; j--) {
				if (label[i][j]>0){
                    if((i+1<512)&&(label[i+1][j]>0)&&(label[i+1][j]<label[i][j])){
                       running=1;
                       label[i][j]=label[i+1][j];
                       }
                    if((j+1<512)&&(label[i][j+1]>0)&&(label[i][j+1]<label[i][j])){
                       running=1;
                       label[i][j]=label[i][j+1];

                       }
				}
			}
		}
	}
	for (int i = 0; i < 512; i++) {
		for (int j = 0; j < 512; j++) {
			count[label[i][j]]++;
		}
	}
	int l, r, t, d,cr,cc;
	for (int o=0; o < 512 * 512; o++) {
		if (count[o] > 500) {
			l = 512; t = 512; r = -1; d = -1;
			cr = 0; cc = 0;
			for (int i = 0; i < 512; i++) {
				for (int j = 0; j < 512; j++) {
					if (label[i][j] == o) {
						if (j < l)l = j;
						if (i < t)t = i;
						if (j > r)r = j;
						if (i > d)d = i;
						cr += (i);
						cc += (j);
					}
					
				}
			}
			if (o > 0) {
				std::cout << " " << l << " " << r << " " << t << " " << d << std::endl;
				std::cout << " " << cc/count[o ]<< " "<<cr / count[o] << std::endl;
				cv::rectangle(img, Rect(l, t, r - l, d-t), Scalar(255, 255, 0), 1, 1, 0);
				cv::circle(img, Point(cc / count[o],cr / count[o]), 10, (255, 255, 0), 3);
			}
		}
	}
	imwrite("cca.jpg", img);
	imshow("cca.jpg", img);
	waitKey(0);
	}



int main()
{
	// read image
	Mat img = imread("lena.bmp", CV_8UC1);
	Mat img2 = imread("lena.bmp", CV_8UC1);
	histogram(img);
	binarize(img);
	cca(img);
	return 0;
}


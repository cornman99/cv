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
Mat gnoise(Mat img,int amp) {
	Mat res(512, 512, CV_8UC1, Scalar(0));
	std::random_device rd;
	std::mt19937 gen(rd());
	std::normal_distribution<double> d(0, 1.0);
	for (int i = 0; i < 512; i++) {
		for (int j = 0; j < 512; j++) {
			res.at<uchar>(i,j) = img.at<uchar>(i, j)+d(gen)*amp;
		}
	}
	return res;
}
double snr(Mat o, Mat n) {
	double m[512][512] ;
	double nn[512][512] ;
	double mean = 0, meann = 0;
	for (int i = 0; i < 512; i++) {
		for (int j = 0; j < 512; j++) {
			m[i][j] = double(o.at<uchar>(i, j))/255;
			nn[i][j] = double(n.at<uchar>(i, j))/255;
			mean += m[i][j];
			meann += (m[i][j]-nn[i][j]);
		}
	}
	
	mean /= (512 * 512);
	meann/= (512 * 512);
	
	double vs=0,vn=0;
	for (int i = 0; i < 512; i++) {
		for (int j = 0; j < 512; j++) {
			double temp = m[i][j] - mean;
			vs += (temp * temp);
			double temp2 = nn[i][j] - (m[i][j]+meann);
			vn += temp2 * temp2;
		}
	}
	vs/= (512 * 512);
	vn/= (512 * 512);
	vs = sqrt(vs);
	vn = sqrt(vn);
	double res = 20 * log10(vs / vn);

	return res;
}
Mat sandp(Mat img, double l,double h,double threshold) {
	Mat res(512, 512, CV_8UC1, Scalar(0));
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<double> d(l, h);
	for (int i = 0; i < 512; i++) {
		for (int j = 0; j < 512; j++) {
			if(d(gen)<threshold)
				res.at<uchar>(i, j) =0;
			else if (d(gen) > 1-threshold)res.at<uchar>(i, j) = 255;
			else res.at<uchar>(i, j) = img.at<uchar>(i, j);
		}
	}
	return res;
}
Mat box(Mat img,int size ) {
	Mat res(512, 512, CV_8UC1, Scalar(0));
	int center = size / 2;
	for (int i = 0; i < 512; i++) {
		for (int j = 0; j < 512; j++) {
			int mean = 0, count = 0;
			for (int k = 0; k < size; k++) {
				for (int l = 0; l < size; l++) {
					if (0 <= (i + k - center) && (i + k - center) < 512 && 0 <= (j + l - center) && (j + l - center) < 512) {
						mean += img.at<uchar>(i + k - center, j + l - center);
						count++;
					}
				}

			}
			
			res.at<uchar>(i, j) = mean / count;
		}
	}
	return res;
}
Mat boxm(Mat img, int size) {
	Mat res(512, 512, CV_8UC1, Scalar(0));
	int center = size / 2;
	for (int i = 0; i < 512; i++) {
		for (int j = 0; j < 512; j++) {
			int  count = 0;
			std::vector<int> d;
			for (int k = 0; k < size; k++) {
				for (int l = 0; l < size; l++) {
					if (0 <= (i + k - center) && (i + k - center) < 512 && 0 <= (j + l - center) && (j + l - center) < 512) {
						d.push_back(img.at<uchar>(i + k - center, j + l - center));
						count++;
					}
				}

			}
			sort(d.begin(), d.begin() +count);
			res.at<uchar>(i, j) = d[count / 2];
		}
	}
	return res;
}
Mat Erosion(Mat img, int kernel[][5]) {
	Mat d(512, 512, CV_8UC1, Scalar(0));

	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			int min = 256;
			bool pass = true;
			for (int k = 0; k < 5; k++) {
				for (int l = 0; l < 5; l++) {
					if (kernel[k][l] == 1) {
						if ((i + k - 2 >= 0) && (i + k - 2 < 512) && (j + l - 2) >= 0 && (j + l - 2) < 512) {
							if (img.at<uchar>(i + k - 2, j + l - 2) < min) {
								min = img.at<uchar>(i + k - 2, j + l - 2);
							}
						}
					}
				}
			}
			d.at<uchar>(i, j) = min;
		}
	}
	return d;
}
Mat Dilation(Mat img, int kernel[][5]) {
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
			d.at<uchar>(i, j) = max;

		}
	}
	return d;
}
Mat Closing(Mat img, int kernel[][5]) {
	return Erosion(Dilation(img, kernel), kernel);
}
Mat Opening(Mat img, int kernel[][5]) {
	return Dilation(Erosion(img, kernel), kernel);
}

int main()
{
	int kernel[5][5] = { 0 };
	kernel[0][1] = 1; kernel[0][2] = 1; kernel[0][3] = 1;
	kernel[1][0] = 1; kernel[1][1] = 1; kernel[1][2] = 1; kernel[1][3] = 1; kernel[1][4] = 1;
	kernel[2][0] = 1; kernel[2][1] = 1; kernel[2][2] = 1; kernel[2][3] = 1; kernel[2][4] = 1;
	kernel[3][0] = 1; kernel[3][1] = 1; kernel[3][2] = 1; kernel[3][3] = 1; kernel[3][4] = 1;
	kernel[4][1] = 1; kernel[4][2] = 1; kernel[4][3] = 1;
	Mat img = imread("lena.bmp", CV_8UC1);
	Mat g10(512, 512, CV_8UC1, Scalar(0));
	g10 = gnoise(img, 10);
	imwrite("g10.bmp", g10);
	std::cout <<"g10"<< snr(img, g10)<<std::endl;
	Mat g30(512, 512, CV_8UC1, Scalar(0));
	g30 = gnoise(img, 30);
	imwrite("g30.bmp", g30);
	std::cout << "g30" << snr(img, g30) <<std::endl;
	Mat sp5(512, 512, CV_8UC1, Scalar(0));
	sp5 = sandp(img, 0,1,0.05);
	imwrite("sp5.bmp", sp5);
	std::cout << "sp5" << snr(img,sp5) << std::endl;
	Mat sp10(512, 512, CV_8UC1, Scalar(0));
	sp10 = sandp(img, 0, 1, 0.1);
	imwrite("sp10.bmp", sp10);
	std::cout << "sp10" << snr(img, sp10) << std::endl;
	Mat g10box(512, 512, CV_8UC1, Scalar(0));
	g10box = box(g10,3);
	imwrite("g10box3.bmp", g10box);
	std::cout << "g10box" << snr(img, g10box) << std::endl;
	Mat g30box(512, 512, CV_8UC1, Scalar(0));
	g30box = box(g30, 3);
	imwrite("g30box3.bmp", g30box);
	std::cout << "g30box" << snr(img, g30box) << std::endl;
	Mat sp5box(512, 512, CV_8UC1, Scalar(0));
	sp5box = box(sp5, 3);
	imwrite("sp5box3.bmp", sp5box);
	std::cout << "sp5box3" << snr(img, sp5box) << std::endl;
	Mat sp10box(512, 512, CV_8UC1, Scalar(0));
	sp10box = box(sp10, 3);
	imwrite("sp10box3.bmp", sp10box);
	std::cout << "sp10box3" << snr(img, sp10box) << std::endl;
	Mat g10box5(512, 512, CV_8UC1, Scalar(0));
	g10box5 = box(g10, 5);
	imwrite("g10box5.bmp", g10box5);
	std::cout << "g10box5" << snr(img, g10box5) << std::endl;
	Mat g30box5(512, 512, CV_8UC1, Scalar(0));
	g30box5 = box(g30, 5);
	imwrite("g30box5.bmp", g30box5);
	std::cout << "g30box5" << snr(img, g30box5) << std::endl;
	Mat sp5box5(512, 512, CV_8UC1, Scalar(0));
	sp5box5 = box(sp5, 5);
	imwrite("sp5box5.bmp", sp5box5);
	std::cout << "sp5box5" << snr(img, sp5box5) << std::endl;
	Mat sp10box5(512, 512, CV_8UC1, Scalar(0));
	sp10box5 = box(sp10, 5);
	imwrite("sp10box5.bmp", sp10box5);
	std::cout << "sp10box5" << snr(img, sp10box5) << std::endl;
	

	Mat g10boxm(512, 512, CV_8UC1, Scalar(0));
	g10boxm = boxm(g10, 3);
	imwrite("g10boxm3.bmp", g10boxm);
	std::cout << "g10boxm3" << snr(img, g10boxm) << std::endl;
	Mat g30boxm(512, 512, CV_8UC1, Scalar(0));
	g30boxm = boxm(g30, 3);
	imwrite("g30boxm3.bmp", g30boxm);
	std::cout << "g30boxm3" << snr(img, g30boxm) << std::endl;
	Mat sp5boxm(512, 512, CV_8UC1, Scalar(0));
	sp5boxm = boxm(sp5, 3);
	imwrite("sp5boxm3.bmp", sp5boxm);
	std::cout << "sp5boxm3" << snr(img, sp5boxm) << std::endl;
	Mat sp10boxm(512, 512, CV_8UC1, Scalar(0));
	sp10boxm = boxm(sp10, 3);
	imwrite("sp10boxm3.bmp", sp10boxm);
	std::cout << "sp10boxm3" << snr(img, sp10boxm) << std::endl;
	Mat g10boxm5(512, 512, CV_8UC1, Scalar(0));
	g10boxm5 = boxm(g10, 5);
	imwrite("g10boxm5.bmp", g10boxm5);
	std::cout << "g10boxm5" << snr(img, g10boxm5) << std::endl;
	Mat g30boxm5(512, 512, CV_8UC1, Scalar(0));
	g30boxm5 = boxm(g30, 5);
	imwrite("g30boxm5.bmp", g30boxm5);
	std::cout << "g30boxm5" << snr(img, g30boxm5) << std::endl;
	Mat sp5boxm5(512, 512, CV_8UC1, Scalar(0));
	sp5boxm5 = boxm(sp5, 5);
	imwrite("sp5boxm5.bmp", sp5boxm5);
	std::cout << "sp5boxm5" << snr(img, sp5boxm5) << std::endl;
	Mat sp10boxm5(512, 512, CV_8UC1, Scalar(0));
	sp10boxm5 = boxm(sp10, 5);
	imwrite("sp10boxm5.bmp", sp10boxm5);
	std::cout << "sp10boxm5" << snr(img, sp10boxm5) << std::endl;

	Mat g10otc(512, 512, CV_8UC1, Scalar(0));
	g10otc = Closing(Opening(g10,kernel),kernel);
	imwrite("g10otc.bmp", g10otc);
	std::cout << "g10otc" << snr(img, g10otc) << std::endl;
	Mat g30otc(512, 512, CV_8UC1, Scalar(0));
	g30otc = Closing(Opening(g30, kernel), kernel);
	imwrite("g30otc.bmp", g30otc);
	std::cout << "g30otc" << snr(img, g30otc) << std::endl;
	Mat sp5otc(512, 512, CV_8UC1, Scalar(0));
	sp5otc = Closing(Opening(sp5, kernel), kernel);
	imwrite("sp5otc.bmp", sp5otc);
	std::cout << "sp5otc" << snr(img, sp5otc) << std::endl;
	Mat sp10otc(512, 512, CV_8UC1, Scalar(0));
	sp10otc = Closing(Opening(sp10, kernel), kernel);
	imwrite("sp10otc.bmp", sp10otc);
	std::cout << "sp10otc" << snr(img, sp10otc) << std::endl;
	Mat g10cto(512, 512, CV_8UC1, Scalar(0));
	g10cto = Opening(Closing(g10, kernel), kernel);
	imwrite("g10cto.bmp", g10cto);
	std::cout << "g10cto" << snr(img, g10cto) << std::endl;
	Mat g30cto(512, 512, CV_8UC1, Scalar(0));
	g30cto = Opening(Closing(g30, kernel), kernel);
	imwrite("g30cto.bmp", g30cto);
	std::cout << "g30cto" << snr(img, g30cto) << std::endl;
	Mat sp5cto(512, 512, CV_8UC1, Scalar(0));
	sp5cto = Opening(Closing(sp5, kernel), kernel);
	imwrite("sp5cto.bmp", sp5cto);
	std::cout << "sp5cto" << snr(img, sp5cto) << std::endl;
	Mat sp10cto(512, 512, CV_8UC1, Scalar(0));
	sp10cto =Opening(Closing(sp10, kernel), kernel);
	imwrite("sp10cto.bmp", sp10cto);
	std::cout << "sp10cto" << snr(img, sp10cto) << std::endl;
	
	return 0;
}


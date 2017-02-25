#include <opencv2\opencv.hpp>
#include <iostream>
#include <string>
#include <cmath>
#include"stdio.h"
using namespace cv;
using namespace std;
int main()
{
	double findThreshold(IplImage*, double, double);
	IplImage* img = cvLoadImage("pic/pic1.png");
	if (!img)
	{
		cout << "error";
		return -1;
	}
	IplImage* img2 = cvCreateImage(cvGetSize(img), img->depth, 1);
	cvCvtColor(img, img2, CV_BGR2GRAY);//转为灰度图img2
	double threshold, error, x;
	cout << "Please input the minimum error\n";
	cin >> error;
	cout << "Please input the estimated threshold\n";
	cin >> x;
	threshold = findThreshold(img2, error, x);//算法
	cvThreshold(img2, img2, threshold, 255, 0);
	cvNamedWindow("result", CV_WINDOW_AUTOSIZE);
	cvShowImage("result", img2);
	waitKey();
	cvSaveImage("pic/result.png", img2);
	cout << "Object picture stored in the disk successfully!\n";
	return 0;
}

double findThreshold(IplImage* temp, double error, double x)
{
	int height = temp->height;
	int width = temp->width;
	int step = temp->widthStep / sizeof(uchar);
	uchar *data = (uchar*)temp->imageData;
	int a[256] = { 0 };//直方图
	int totalGray = 0;//灰度值和
	int totalPixel = 0;//像素数和
	uchar bt;//某点的灰度值
	double threshold1, threshold2;
	uchar max = 0, min = 255;//反向初始化，方便比较
	double averageGray1, averageGray2;
	for (int i = 0; i<height; i++)
	{
		for (int j = 0; j<width; j++)
		{
			bt = data[i*step + j];
			if (bt<min)
				min = bt;
			if (bt>max)
				max = bt;
			a[bt]++;
		}
	}
	threshold1 = 0;
	threshold2 = x;
	int m;
	for (m = 0; (fabs(threshold1 - threshold2)>error); m++)//迭代中止条件
	{
		threshold1 = threshold2;
		//小于当前阀值部分的平均灰度值
		for (int i = min; i<threshold1; i++)
		{
			totalGray += a[i] * i;//灰度值和
			totalPixel += a[i];//像素和
		}
		averageGray1 = double(totalGray) / totalPixel;
		//大于当前阀值部分的平均灰度值
		totalPixel = 0;
		totalGray = 0;
		for (int j = threshold1 + 1; j<max; j++)
		{
			totalGray += a[j] * j;
			totalPixel += a[j];
		}
		averageGray2 = double(totalGray) / totalPixel;
		threshold2 = (averageGray1 + averageGray2) / 2;        //新阀值
	}
	cout << "After " << m << " iterations,the threshold should be: " << threshold1 << endl;
	return threshold1;
}

#include "XulyAnh.h"

void Negative(Mat imgin, Mat imgout)
{
	int M = imgin.size().height;
	int N = imgin.size().width;
	int x, y;
	double r, s;
	for (x = 0; x < M; x++)
		for (y = 0; y < N; y++)
		{
			r = imgin.at<uchar>(x, y);
			s = L - 1 - r;
			imgout.at<uchar>(x, y) = (uchar)s;
		}
	return;
}

void LocalHistogram(Mat imgin, Mat imgout)
{
	int m = 3, n = 3;
	Mat win = Mat(m, n, CV_8UC1);
	Mat wout = Mat(m, n, CV_8UC1);
	int M = imgin.size().height;
	int N = imgin.size().width;
	int x, y, s, t;
	int a = m / 2, b = m / 2;
	for (x = a; x < M - a; x++)
		for (y = b; y < N - b; y++) {
			for (s = -a; s <= a; s++)
				for (t = -b; t <= b; t++)
					win.at<uchar>(s + a, t + b) = imgin.at<uchar>(x + s, y + t);
			equalizeHist(win, wout);
			imgout.at<uchar>(x, y) = wout.at<uchar>(a, b);
		}
	return;
}

void HistogramStatistics(Mat imgin, Mat imgout)
{
	Scalar mean, stddev;
	meanStdDev(imgin, mean, stddev);
	double mG = mean.val[0];
	double sigmaG = stddev.val[0];
	double msxy, sigmasxy;

	int m = 3, n = 3;
	Mat w = Mat(m, n, CV_8UC1);
	int M = imgin.size().height;
	int N = imgin.size().width;
	int x, y, s, t;
	int a = m / 2, b = m / 2;
	double C = 22.8, k0 = 0.0, k1 = 0.1, k2 = 0.0, k3 = 0.1;
	for (x = a; x < M - a; x++)
		for (y = b; y < N - b; y++) {
			for (s = -a; s <= a; s++)
				for (t = -b; t <= b; t++)
					w.at<uchar>(s + a, t + b) = imgin.at<uchar>(x + s, y + t);
			meanStdDev(w, mean, stddev);
			msxy = mean.val[0];
			sigmasxy = stddev.val[0];
			if ((k0 * mG <= msxy && msxy <= k1 * mG) && (k2*sigmaG <= sigmasxy && sigmasxy <= k3 * sigmaG))
				imgout.at<uchar>(x, y) = (uchar)(C*imgin.at<uchar>(x, y));
			else
				imgout.at<uchar>(x, y) = imgin.at<uchar>(x, y);
		}
	return;
}

void LowpassFilter(Mat imgin, Mat imgout)
{
	//boxFilter(imgin, imgout, CV_8UC1, Size(11, 11));
	GaussianBlur(imgin, imgout, Size(21, 21), 3.5);
}


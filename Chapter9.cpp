#include "XuLyAnh.h"

void GrayScaleErosion(Mat imgin, Mat imgout)
{
	int m = 5, n = 5;
	Mat w = Mat(m, n, CV_8UC1);
	int s, t, x, y, a, b;
	for (s = 0; s < m; s++)
		for (t = 0; t < n; t++)
			w.at<uchar>(s, t) = 1;
	w.at<uchar>(0, 0) = 0;
	w.at<uchar>(0, n - 1) = 0;
	w.at<uchar>(m - 1, 0) = 0;
	w.at<uchar>(m - 1, n - 1) = 0;
	a = m / 2;
	b = n / 2;
	int M = imgin.size().height;
	int N = imgin.size().width;
	uchar min;
	for (x=a; x<M-a; x++)
		for (y = b; y < N - b; y++)
		{
			min = 255;
			for (s = -a; s <= a; s++)
				for (t = -b; t <= b; t++)
					if (w.at<char>(s + a, t + b) == 1)
						if (imgin.at<uchar>(x + s, y + b) < min)
							min = imgin.at<uchar>(x + s, y + b);
			imgout.at<uchar>(x, y) = min;
		}
	return;
}

void GrayScaleDilation(Mat imgin, Mat imgout)
{
	int m = 5, n = 5;
	Mat w = Mat(m, n, CV_8UC1);
	int s, t, x, y, a, b;
	for (s = 0; s < m; s++)
		for (t = 0; t < n; t++)
			w.at<uchar>(s, t) = 1;
	w.at<uchar>(0, 0) = 0;
	w.at<uchar>(0, n - 1) = 0;
	w.at<uchar>(m - 1, 0) = 0;
	w.at<uchar>(m - 1, n - 1) = 0;
	a = m / 2;
	b = n / 2;
	int M = imgin.size().height;
	int N = imgin.size().width;
	uchar max;
	for (x = a; x < M - a; x++)
		for (y = b; y < N - b; y++)
		{
			max = 0;
			for (s = -a; s <= a; s++)
				for (t = -b; t <= b; t++)
					if (w.at<char>(s + a, t + b) == 1)
						if (imgin.at<uchar>(x + s, y + b) > max)
							max = imgin.at<uchar>(x + s, y + b);
			imgout.at<uchar>(x, y) = max;
		}
	return;
}

void GrayScaleOpening(Mat imgin, Mat imgout)
{
	int m = 5, n = 5;
	Mat w = Mat(m, n, CV_8UC1);
	int s, t, x, y, a, b;
	for (s = 0; s < m; s++)
		for (t = 0; t < n; t++)
			w.at<uchar>(s, t) = 1;
	w.at<uchar>(0, 0) = 0;
	w.at<uchar>(0, n - 1) = 0;
	w.at<uchar>(m - 1, 0) = 0;
	w.at<uchar>(m - 1, n - 1) = 0;

	a = m / 2;
	b = n / 2;
	int M = imgin.size().height;
	int N = imgin.size().width;
	Mat temp = Mat(M, N, CV_8UC1);

	uchar min;
	for (x = a; x < M - a; x++)
		for (y = b; y < N - b; y++)
		{
			min = 255;
			for (s = -a; s <= a; s++)
				for (t = -b; t <= b; t++)
					if (w.at<char>(s + a, t + b) == 1)
						if (imgin.at<uchar>(x + s, y + b) < min)
							min = imgin.at<uchar>(x + s, y + b);
			temp.at<uchar>(x, y) = min;
		}

	uchar max;
	for (x = a; x < M - a; x++)
		for (y = b; y < N - b; y++)
		{
			max = 0;
			for (s = -a; s <= a; s++)
				for (t = -b; t <= b; t++)
					if (w.at<char>(s + a, t + b) == 1)
						if (temp.at<uchar>(x + s, y + b) > max)
							max = temp.at<uchar>(x + s, y + b);
			imgout.at<uchar>(x, y) = max;
		}

	return;
}

void GrayScaleClosing(Mat imgin, Mat imgout)
{
	Mat w = getStructuringElement(MORPH_ELLIPSE, Size(11, 11));
	morphologyEx(imgin, imgout, MORPH_CLOSE, w);
}

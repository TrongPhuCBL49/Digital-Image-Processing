#include "XulyAnh.h"

void Fourier(Mat imgin, Mat imgout)
{
	int M = imgin.size().height;
	int N = imgin.size().width;
	int P = getOptimalDFTSize(M);
	int Q = getOptimalDFTSize(N);
	Mat fp = Mat(P, Q, CV_32FC2, CV_RGB(0, 0, 0));
	int x, y;
	
	
	//Buoc 1 va 2
	for (x = 0; x < M; x++)
		for (y = 0; y < N; y++)
			if ((x+y)%2 == 0)
				fp.at<Vec2f>(x, y)[0] = (float)(1.0*imgin.at<uchar>(x, y)/(L-1));
			else
				fp.at<Vec2f>(x, y)[0] = (float)(-1.0*imgin.at<uchar>(x, y)/(L-1));
	
	Mat F = Mat(P, Q, CV_32FC2);

	//Buoc 3
	dft(fp, F);

	//Buoc 4
	int u, v;
	float r;
	float real, imag;
	for (u = 0; u < P; u++)
		for (v = 0; v < Q; v++)
		{
			real = F.at<Vec2f>(u, v)[0];
			imag = F.at<Vec2f>(u, v)[1];
			r = sqrt(real*real + imag * imag);
			if (r > L - 1)
				r = L - 1;
			imgout.at<uchar>(u, v) = (uchar)r;
		}
	return;
}

void FilterFrequency(Mat imgin, Mat imgout)
{
	int M = imgin.size().height;
	int N = imgin.size().width;
	int P = getOptimalDFTSize(M);
	int Q = getOptimalDFTSize(N);
	Mat fp = Mat(P, Q, CV_32FC2, CV_RGB(0, 0, 0));
	int x, y;


	//Buoc 1,2 va 3
	for (x = 0; x < M; x++)
		for (y = 0; y < N; y++)
			if ((x + y) % 2 == 0)
				fp.at<Vec2f>(x, y)[0] = (float)(1.0*imgin.at<uchar>(x, y));
			else
				fp.at<Vec2f>(x, y)[0] = (float)(-1.0*imgin.at<uchar>(x, y));

	Mat F = Mat(P, Q, CV_32FC2);

	//Buoc 4
	dft(fp, F);

	//Tao bo loc H
	Mat H = Mat(P, Q, CV_32FC2, CV_RGB(0, 0, 0));
	//Butterworth lowpass
	float D0 = 50;
	float n = 2.25;
	float Duv;
	int u, v;
	for (u = 0; u < P; u++)
		for (v = 0; v < Q; v++)
		{
			Duv = sqrt((u - P / 2)*(u - P / 2) + (v - Q / 2)*(v - Q / 2));
			H.at<Vec2f>(u, v)[0] = (float)(1.0 / (1 + pow(Duv / D0, 2 * n)));
		}
	Mat G = Mat(P, Q, CV_32FC2);
	mulSpectrums(F, H, G, DFT_ROWS);

	//Buoc 7
	Mat g = Mat(P, Q, CV_32FC2);
	idft(G, g, DFT_SCALE);

	//Buoc 8
	float r;
	for (x=0; x<M; x++)
		for (y = 0; y < N; y++)
		{
			r = g.at<Vec2f>(x, y)[0];
			if ((x + y) % 2 == 1)
				r = -r;
			if (r < 0)
				r = 0;
			if (r > L - 1)
				r = L - 1;
			imgout.at<uchar>(x, y) = (uchar)r;
		}
}

void MoireRemove(Mat imgin, Mat imgout)
{
	int M = imgin.size().height;
	int N = imgin.size().width;

	// Buoc 1, 2, 3
	int P = getOptimalDFTSize(M);
	int Q = getOptimalDFTSize(N);
	Mat f = Mat(P, Q, CV_32FC2, CV_RGB(0, 0, 0));
	int x, y, u, v;
	for (x = 0; x < M; x++)
		for (y = 0; y < N; y++)
			if ((x + y) % 2 == 0)
				f.at<Vec2f>(x, y)[0] = (float)(1.0*imgin.at<uchar>(x, y));
			else
				f.at<Vec2f>(x, y)[0] = (float)(-1.0*imgin.at<uchar>(x, y));

	// Buoc 4
	Mat F = Mat(P, Q, CV_32FC2, CV_RGB(0, 0, 0));
	dft(f, F);

	// Buoc 5
	Mat H = Mat(P, Q, CV_32FC2, CV_RGB(0, 0, 0));
	int u1 = 45, v1 = 58;
	int u2 = 40, v2 = 119;
	int u3 = 86, v3 = 58;
	int u4 = 82, v4 = 119;
	double D01 = 5, D02 = 5, D03 = 15, D04 = 15;
	double Duv, DuvTru;
	double response;
	for (u = 0; u < P; u++)
		for (v = 0; v < Q; v++) {
			Duv = sqrt(1.0*(u - u1)*(u - u1) + 1.0*(v - v1)*(v - v1));
			DuvTru = sqrt(1.0*(u - P + u1)*(u - P + u1) + 1.0*(v - Q + v1)*(v - Q + v1));
			response = (1 / (1 + pow(D01 / Duv, 2 * 2)))*(1 / (1 + pow(D01 / DuvTru, 2 * 2)));

			Duv = sqrt(1.0*(u - u2)*(u - u2) + 1.0*(v - v2)*(v - v2));
			DuvTru = sqrt(1.0*(u - P + u2)*(u - P + u2) + 1.0*(v - Q + v2)*(v - Q + v2));
			response *= (1 / (1 + pow(D02 / Duv, 2 * 2)))*(1 / (1 + pow(D02 / DuvTru, 2 * 2)));

			Duv = sqrt(1.0*(u - u3)*(u - u3) + 1.0*(v - v3)*(v - v3));
			DuvTru = sqrt(1.0*(u - P + u3)*(u - P + u3) + 1.0*(v - Q + v3)*(v - Q + v3));
			response *= (1 / (1 + pow(D03 / Duv, 2 * 2)))*(1 / (1 + pow(D03 / DuvTru, 2 * 2)));

			Duv = sqrt(1.0*(u - u4)*(u - u4) + 1.0*(v - v4)*(v - v4));
			DuvTru = sqrt(1.0*(u - P + u4)*(u - P + u4) + 1.0*(v - Q + v4)*(v - Q + v4));
			response *= (1 / (1 + pow(D04 / Duv, 2 * 2)))*(1 / (1 + pow(D04 / DuvTru, 2 * 2)));

			H.at<Vec2f>(u, v)[0] = (float)response;
		}

	Mat G = Mat(P, Q, CV_32FC2, CV_RGB(0, 0, 0));
	mulSpectrums(F, H, G, DFT_ROWS);

	// Buoc 6, 7
	Mat g = Mat(P, Q, CV_32FC2, CV_RGB(0, 0, 0));
	idft(G, g, DFT_SCALE);

	float r;
	for (x = 0; x < M; x++)
		for (y = 0; y < N; y++) {
			if ((x + y) % 2 == 0)
				r = g.at<Vec2f>(x, y)[0];
			else
				r = -g.at<Vec2f>(x, y)[0];
			if (r < 0)
				r = 0;
			if (r > L - 1)
				r = L - 1;
			imgout.at<uchar>(x, y) = (uchar)r;
		}
	return;
}

void TaoBoLocChuNhat(Mat imgin)
{
	int M = imgin.size().height;
	int N = imgin.size().width;
	Mat temp = Mat(M, N, CV_32FC1);
	int x, y;
	int vitri_y0 = 23;
	int vitri_y1 = 188;
	int vitri_y2 = 612;
	int vitri_y3 = 777;
	double D0 = 5;
	double n = 3;
	double Du;
	double H, H0, H1, H2, H3;
	for (x = 0; x < M; x++)
		for (y = 0; y < N; y++)
		{
			Du = y - vitri_y0;
			H0 = 1.0 / (1.0 + pow(D0 / Du, 2 * n));
			Du = y - vitri_y1;
			H1 = 1.0 / (1.0 + pow(D0 / Du, 2 * n));
			Du = y - vitri_y2;
			H2 = 1.0 / (1.0 + pow(D0 / Du, 2 * n));
			Du = y - vitri_y3;
			H3 = 1.0 / (1.0 + pow(D0 / Du, 2 * n));
			H = H0*H1*H2*H3;
			temp.at<float>(x, y) = (float)H;
		}
	int vitri_x0 = 16;
	int vitri_x1 = 121;
	int vitri_x2 = 391;
	int vitri_x3 = 496;
	double Dv;
	for (y = 0; y < N; y++)
		for (x = 0; x < M; x++)
		{
			Dv = x - vitri_x0;
			H0 = 1.0 / (1.0 + pow(D0 / Dv, 2 * n));
			Dv = x - vitri_x1;
			H1 = 1.0 / (1.0 + pow(D0 / Dv, 2 * n));
			Dv = x - vitri_x2;
			H2 = 1.0 / (1.0 + pow(D0 / Dv, 2 * n));
			Dv = x - vitri_x3;
			H3 = 1.0 / (1.0 + pow(D0 / Dv, 2 * n));
			H = temp.at<float>(x, y);
			H = H * H0 * H1 * H2 * H3;
			temp.at<float>(x, y) = (float)H;
			imgin.at<uchar>(x, y) = (uchar)(H*L);
		}
	//for (x = 0; x < M; x++)
	//	for (y = 0; y < N; y++)
	//	{
	//		H = temp.at<float>(x, y);
	//		imgin.at<uchar>(x, y) = (uchar)(H*L);
	//	}
	return;
}

void PeriodRemove(Mat imgin, Mat imgout)
{
	int M = imgin.size().height;
	int N = imgin.size().width;

	// Buoc 1, 2, 3
	int P = getOptimalDFTSize(M);
	int Q = getOptimalDFTSize(N);
	Mat f = Mat(P, Q, CV_32FC2, CV_RGB(0, 0, 0));
	int x, y;
	for (x = 0; x < M; x++)
		for (y = 0; y < N; y++)
			if ((x + y) % 2 == 0)
				f.at<Vec2f>(x, y)[0] = (float)(1.0*imgin.at<uchar>(x, y));
			else
				f.at<Vec2f>(x, y)[0] = (float)(-1.0*imgin.at<uchar>(x, y));

	// Buoc 4
	Mat F = Mat(P, Q, CV_32FC2, CV_RGB(0, 0, 0));
	dft(f, F);

	// Buoc 5
	Mat H = Mat(P, Q, CV_32FC2, CV_RGB(0, 0, 0));

	int vitri_y0 = 23;
	int vitri_y1 = 188;
	int vitri_y2 = 612;
	int vitri_y3 = 777;
	int vitri_y4 = 400;
	double D0 = 5;
	double n = 4;
	double Du;
	double HH, H0, H1, H2, H3, H4;
	for (x = 0; x < P; x++)
		for (y = 0; y < Q; y++)
		{
			Du = y - vitri_y0;
			H0 = 1.0 / (1.0 + pow(D0 / Du, 2 * n));
			Du = y - vitri_y1;
			H1 = 1.0 / (1.0 + pow(D0 / Du, 2 * n));
			Du = y - vitri_y2;
			H2 = 1.0 / (1.0 + pow(D0 / Du, 2 * n));
			Du = y - vitri_y3;
			H3 = 1.0 / (1.0 + pow(D0 / Du, 2 * n));
			Du = y - vitri_y4;
			H4 = 1.0 / (1.0 + pow(D0 / Du, 2 * n));

			HH = H4;
			H.at<Vec2f>(x, y)[0] = (float)HH;
		}
	int vitri_x0 = 16;
	int vitri_x1 = 121;
	int vitri_x2 = 391;
	int vitri_x3 = 496;
	int vitri_x4 = 256;
	double Dv;
	for (y = 0; y < Q; y++)
		for (x = 0; x < P; x++)
		{
			Dv = x - vitri_x0;
			H0 = 1.0 / (1.0 + pow(D0 / Dv, 2 * n));
			Dv = x - vitri_x1;
			H1 = 1.0 / (1.0 + pow(D0 / Dv, 2 * n));
			Dv = x - vitri_x2;
			H2 = 1.0 / (1.0 + pow(D0 / Dv, 2 * n));
			Dv = x - vitri_x3;
			H3 = 1.0 / (1.0 + pow(D0 / Dv, 2 * n));
			Dv = x - vitri_x4;
			H4 = 1.0 / (1.0 + pow(D0 / Dv, 2 * n));
			HH = H.at<Vec2f>(x, y)[0];
			HH = HH * H4;
			H.at<Vec2f>(x, y) = (float)HH;
			H.at<Vec2f>(x, y)[0] = (float)HH;
		}


	Mat G = Mat(P, Q, CV_32FC2, CV_RGB(0, 0, 0));
	mulSpectrums(F, H, G, DFT_ROWS);

	// Buoc 6, 7
	Mat g = Mat(P, Q, CV_32FC2, CV_RGB(0, 0, 0));
	idft(G, g, DFT_SCALE);

	float r;
	for (x = 0; x < M; x++)
		for (y = 0; y < N; y++) {
			if ((x + y) % 2 == 0)
				r = g.at<Vec2f>(x, y)[0];
			else
				r = -g.at<Vec2f>(x, y)[0];
			if (r < 0)
				r = 0;
			if (r > L - 1)
				r = L - 1;
			imgout.at<uchar>(x, y) = (uchar)r;
		}
	return;
}
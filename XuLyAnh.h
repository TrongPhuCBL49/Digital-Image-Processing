#pragma once
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>

using namespace cv;
using namespace std;

#define L 256

//Chuong 3
void Negative(Mat imgin, Mat imgout);
void LocalHistogram(Mat imgin, Mat imgout);
void HistogramStatistics(Mat imgin, Mat imgout);
void LowpassFilter(Mat imgin, Mat imgout);

//Chuong 4
void Fourier(Mat imgin, Mat imgout);
void FilterFrequency(Mat imgin, Mat imgout);
void MoireRemove(Mat imgin, Mat imgout);
void TaoBoLocChuNhat(Mat imgin);
void PeriodRemove(Mat imgin, Mat imgout);

//Chuong 9
void GrayScaleErosion(Mat imgin, Mat imgout);
void GrayScaleDilation(Mat imgin, Mat imgout);
void GrayScaleOpening(Mat imgin, Mat imgout);
void GrayScaleClosing(Mat imgin, Mat imgout);
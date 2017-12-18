#ifndef TYPEEXG_OPENCV_EIG_H
#define TYPEEXG_OPENCV_EIG_H

// Copyright (C) 2017 Kyaw Kyaw Htike @ Ali Abdul Ghafur. All rights reserved.

#include "opencv2/opencv.hpp"
#include "Eigen/Dense"
#include <vector>

#define EigenMatrix Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>
#define EIGEN_NO_DEBUG // to remove bound checking to get faster element access

/////////// *************** ///////////////////
/////////// *************** ///////////////////
/////////// CORRECTNESS TEST PASSED ///////////
/////////// *************** ///////////////////
/////////// *************** ///////////////////

namespace hpers_TEOpencvEig
{
	// convert typename and nchannels to opencv mat type such as CV_32FC1
	template <typename T>
	int getOpencvType(int nchannels)
	{
		int depth = cv::DataType<T>::depth;
		return (CV_MAT_DEPTH(depth) + (((nchannels)-1) << CV_CN_SHIFT));
	}
}

template <typename T>
void opencv2eigen(const cv::Mat &mat_in, EigenMatrix &mat_out)
{
	int nrows = mat_in.rows;
	int ncols = mat_in.cols;

	mat_out.setZero(nrows, ncols);

	T *ptr = (T*)mat_out.data();
	unsigned long count = 0;

	for (int j = 0; j < ncols; j++)
		for (int i = 0; i < nrows; i++)
			ptr[count++] = mat_in.at<T>(i, j);			
}

template <typename T>
void eigen2opencv(const EigenMatrix &mat_in, cv::Mat &mat_out)
{
	int nrows = mat_in.rows();
	int ncols = mat_in.cols();
	const int nchannels = 1;

	mat_out.create(nrows, ncols, hpers_TEOpencvEig::getOpencvType<T>(nchannels));
				
	for (int j = 0; j < ncols; j++)
		for (int i = 0; i < nrows; i++)
			mat_out.at<T>(i, j) = mat_in(i,j);
}


template <typename T, int nchannels>
void opencv2eigen(const cv::Mat &mat_in, std::vector<EigenMatrix> &mat_out)
{
	int nrows = mat_in.rows;
	int ncols = mat_in.cols;
	//int nchannels = mat_in.channels();

	mat_out.resize(nchannels);	
	
	for (int k = 0; k < nchannels; k++)
	{
		mat_out[k].setZero(nrows, ncols);
		T *dst_pointer = (T*)mat_out[k].data();
		unsigned long count = 0;
		for (int j = 0; j < ncols; j++)
			for (int i = 0; i < nrows; i++)
				dst_pointer[count++] = mat_in.at<cv::Vec<T, nchannels>>(i, j)[k];			
	}						
}

template <typename T, int nchannels>
void eigen2opencv(const std::vector<EigenMatrix> &mat_in, cv::Mat &mat_out)
{	
	int nrows = mat_in[0].rows();
	int ncols = mat_in[0].cols();
	//int nchannels = mat_in.size();

	mat_out.create(nrows, ncols, hpers_TEOpencvEig::getOpencvType<T>(nchannels));
	
	EigenMatrix temp;
	
	for (int k = 0; k < nchannels; k++)
	{
		temp = mat_in[k];
		for (int j = 0; j < ncols; j++)
		{
			for (int i = 0; i < nrows; i++)
			{
				mat_out.at<cv::Vec<T, nchannels>>(i, j)[k] = temp(i,j);				
			}				
		}	
	}

		
}

#undef EigenMatrix
#undef EIGEN_NO_DEBUG

#endif 
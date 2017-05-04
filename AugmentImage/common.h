#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <io.h>
#include <cstdlib>
#include <cstdio>
#include <direct.h>
#include <chrono>
#include <random>
#include <algorithm>

#include "opencv2/opencv.hpp"
using namespace std;
using namespace cv;

namespace SK
{
	unsigned seed = 1;
	//default_random_engine e;
	std::mt19937 e;

	// 初始化随机生成器
	void InitRandomEngine()
	{
		seed = std::chrono::system_clock::now().time_since_epoch().count();
		//e = default_random_engine(seed);
		e = std::mt19937(seed);
	}

	//split string
	vector<string> Split(string str, const string& pattern)
	{
		std::string::size_type pos;
		std::vector<std::string> result;
		str += pattern;//扩展字符串以方便操作
		size_t size = str.size();

		for (std::size_t i = 0; i < size; i++)
		{
			pos = str.find(pattern, i);
			if (pos < size)
			{
				std::string s = str.substr(i, pos - i);
				result.push_back(s);
				i = pos + pattern.size() - 1;
			}
		}
		return result;
	}

	//去除string两端的空格
	std::string& Trim(std::string &s)
	{
		if (s.empty())
		{
			return s;
		}

		s.erase(0, s.find_first_not_of(" "));
		s.erase(s.find_last_not_of(" ") + 1);
		return s;
	}

	// 遍历根目录中文件数量，只遍历一层目录
	bool GetFileList(string baseDir, string fileType, std::vector<string>& filesVec)
	{
		//本函数用于过滤指定的filetype类型的文件
		filesVec.clear();
		string str = baseDir + "/*." + fileType;
		_finddata_t fileInfo;
		intptr_t handle = 0;
		handle = _findfirst(str.c_str(), &fileInfo);
		if (-1 == handle)
		{
			cout << " fail to traversal the path: " << baseDir << endl;
			return false;
		}
		do 
		{
			if (fileInfo.attrib == _A_SUBDIR)
			{
				/*if (strcmp(fileInfo.name, "..") != 0 && strcmp(fileInfo.name, ".") != 0)
				{
				vector<string> vecFiles;
				Traversal(baseDir + '\\' + fileInfo.name, fileType, vecFiles);
				}*/
			}
			else
			{
				string fileFullPath = baseDir + '\\' + fileInfo.name;
				filesVec.push_back(fileFullPath);
				
			}
		} while (!_findnext(handle, &fileInfo));

		return true;
	}

	// 遍历根目录中文件夹，只遍历一层目录
	bool GetFolderList(string baseDir, std::vector<string>& foldersVec)
	{
		foldersVec.clear();
		string str = baseDir + "/*.*";
		_finddata_t fileInfo;
		intptr_t handle = 0;
		handle = _findfirst(str.c_str(), &fileInfo);
		if (-1 == handle)
		{
			cout << " fail to traversal the path: " << baseDir << endl;
			return false;
		}
		do
		{
			if (fileInfo.attrib == _A_SUBDIR)
			{
				if (strcmp(fileInfo.name, "..") != 0 && strcmp(fileInfo.name, ".") != 0)
				{
					string str = baseDir + '\\' + fileInfo.name;
					foldersVec.push_back(str);
				}
			}
		} while (!_findnext(handle, &fileInfo));

		return true;
	}

	//获取时间戳
	string GetTimeStamp()
	{
		std::chrono::time_point<std::chrono::system_clock, std::chrono::microseconds> tp = std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::system_clock::now());
		auto tmp = std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch());
		time_t timestamp = tmp.count();
		return to_string(timestamp);
	}

	//生成随机数
	unsigned GeneRandNum()
	{
		//unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
		//default_random_engine e(seed);

#ifdef _DEBUG
		for (int i = 0; i < 10; i++)
		{
			cout << e() << endl;
		}

#endif // DEBUG
		return e();
	}

	//生成指定范围的随机数
	int GeneUniformNum(int low, int high, bool bPositive = true)
	{
		//unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
		//default_random_engine e(seed);
		
		int retValu = 0;
		if (false == bPositive)
		{
			uniform_int_distribution<unsigned> u((low << 1), (high << 1));
			retValu = u(e);
			retValu = (retValu % 2 == 0) ? (floor(retValu * 0.5 )) : -(floor(retValu * 0.5 ));
		}
		else
		{
			uniform_int_distribution<unsigned> u(low, high);
			retValu = u(e);
		}

		return retValu;
	}

	//灰度化图像
	bool GrayTrans(string srcImg, int threshLow = 0, int threshHigh = 30)
	{
		if (srcImg.empty())
		{
			cout << "srcImg is empty" << endl;
			return false;
		}

		Mat img = cv::imread(srcImg);
		if (img.data == nullptr)
		{
			cout << "read failed, " << srcImg << endl;
			return false;
		}

		if (img.channels() == 1)
		{
			//cout << "this is a gray image, not to trans, " << srcImg << endl;
			//return false;
			cvtColor(img, img, cv::COLOR_GRAY2BGR);
		}

		/*int imgHeight = img.rows;
		int imgWidth = img.cols;
		int sameNum = 0;
		for (int i = 0; i < 3; i++)
		{
			unsigned int randomH = GeneUniformNum(0, imgHeight - 1);
			unsigned int randomW = GeneUniformNum(0, imgWidth - 1);
			if (img.at<Vec3b>(randomH, randomW)[0] == img.at<Vec3b>(randomH, randomW)[1] &&
				img.at<Vec3b>(randomH, randomW)[0] == img.at<Vec3b>(randomH, randomW)[2] &&
				img.at<Vec3b>(randomH, randomW)[1] == img.at<Vec3b>(randomH, randomW)[2])
			{
				sameNum++;
			}
		}
		if (3 == sameNum)
		{
			return false;
		}*/

		Mat grayMat;
		cvtColor(img, grayMat, cv::COLOR_BGR2GRAY);
		int detal = GeneUniformNum(threshLow, threshHigh, false);
		grayMat += detal;

		//cv::saturate_cast<uchar>(grayMat);

		string strTmp = GetTimeStamp();
		size_t pos = srcImg.find(".");

		string newName = srcImg.substr(0, pos);
		newName = newName + "_" + strTmp + ".jpg";
		imwrite(newName, grayMat);
		return true;
	}

	// 部分遮挡
	bool BlockTrans(string srcImg, int threshLow = 10, int threshHigh = 15)
	{
		if (srcImg.empty())
		{
			cout << "srcImg is empty" << endl;
			return false;
		}

		Mat img = cv::imread(srcImg);
		if (img.data == nullptr)
		{
			cout << "read failed, " << srcImg << endl;
			return false;
		}
		if (img.channels() == 1)
		{
			cvtColor(img, img, cv::COLOR_GRAY2BGR);
		}

		int imgHeight = img.rows;
		int imgWidth = img.cols;
		int area = imgHeight * imgWidth;
		int radio = GeneUniformNum(threshLow, threshHigh, true);
		int smallArea = static_cast<int>(area * radio * 0.01);

		unsigned int randomH = GeneUniformNum(5, imgHeight - 5);
		unsigned int randomW = GeneUniformNum(5, imgWidth - 5);
		unsigned int smallH = 0;
		unsigned int smallW = 0;

		int nType = radio % 3;
		if (nType == 0)
		{
			//正方形
			smallW = static_cast<unsigned int>(sqrt(smallArea));
			smallH = smallW;
		}
		else if (nType == 1)
		{
			// 1:2
			smallW = static_cast<unsigned int>(sqrt(smallArea * 0.5));
			smallH = smallW * 2;
		}
		else if (nType == 2)
		{
			// 2:1
			smallH = static_cast<unsigned int>(sqrt(smallArea * 0.5));
			smallW = smallH * 2;
		}

		if ((randomW + smallW) > imgWidth)
		{
		smallW = imgWidth - randomW;
		}
		if (randomH + smallH > imgHeight)
		{
		smallH = imgHeight - randomH;
		}

		Mat  matBlock = img(Rect(randomW, randomH, smallW, smallH));
		unsigned int B = GeneUniformNum(0, 255);
		unsigned int G = GeneUniformNum(0, 255);
		unsigned int R = GeneUniformNum(0, 255);
		matBlock = cv::Scalar(B, G, R);

		string strTmp = GetTimeStamp();
		size_t pos = srcImg.find(".");

		string newName = srcImg.substr(0, pos);
		newName = newName + "_" + strTmp + ".jpg";
		imwrite(newName, img);

		return true;
	}

	// 色调变换
	bool HsvTrans(string srcImg, int threshLow = 3, int threshHigh = 20)
	{
		if (srcImg.empty())
		{
			cout << "srcImg is empty" << endl;
			return false;
		}

		Mat img = cv::imread(srcImg);
		if (img.data == nullptr)
		{
			cout << "read failed, " << srcImg << endl;
			return false;
		}
		if (img.channels() == 1)
		{
			cvtColor(img, img, cv::COLOR_GRAY2BGR);
		}

		bool hasColor = true;
		int imgHeight = img.rows;
		int imgWidth = img.cols;
		int sameNum = 0;
		for (int i = 0; i < 3; i++)
		{
			unsigned int randomH = GeneUniformNum(0, imgHeight - 1);
			unsigned int randomW = GeneUniformNum(0, imgWidth - 1);
			if (img.at<Vec3b>(randomH, randomW)[0] == img.at<Vec3b>(randomH, randomW)[1] &&
				img.at<Vec3b>(randomH, randomW)[0] == img.at<Vec3b>(randomH, randomW)[2] &&
				img.at<Vec3b>(randomH, randomW)[1] == img.at<Vec3b>(randomH, randomW)[2])
			{
				sameNum++;
			}
		}
		if (3 == sameNum)
		{
			hasColor = false;
		}

		Mat hsvMat;
		cvtColor(img, hsvMat, COLOR_BGR2HSV);
		int detal = GeneUniformNum(threshLow, threshHigh, false);
		int detal2 = GeneUniformNum(25, 60);

		//int detal = rand() % 60 - 30;
		//cout << "detal = " << detal << endl;
		//hsvMat += Scalar(detal, 0, 0);
		//int imgHeight = img.rows;
		//int imgWidth = img.cols;
		float frate = (rand() % 40) / 100.0f;
		for (int i = 0; i < imgHeight; i++)
		{
			for (int j = 0; j < imgWidth; j++)
			{
				uchar dataValue = hsvMat.at<Vec3b>(i, j)[0];
				uchar dataValue2 = hsvMat.at<Vec3b>(i, j)[1];

				int H = dataValue + detal;

				float fS = 255.0f *((dataValue2 / 255.0f)*frate);

				if (hasColor == false)
				{
					//fS = frate*255.0f;
					//int S = (int)fS;
					hsvMat.at<Vec3b>(i, j)[1] = detal2;
				}			
				/*if ( H > 180)
				{
					hsvMat.at<Vec3b>(i, j)[0] = H - 180;
				}
				else if (H < 0)
				{
					hsvMat.at<Vec3b>(i, j)[0] = H + 180;
				}
				else
				{
					hsvMat.at<Vec3b>(i, j)[0] = H;
				}*/
				hsvMat.at<Vec3b>(i, j)[0] = (H + 180) % 180;

				
			}
		}
		cvtColor(hsvMat, img, COLOR_HSV2BGR);

		string strTmp = GetTimeStamp();
		size_t pos = srcImg.find(".");

		string newName = srcImg.substr(0, pos);
		newName = newName + "_" + strTmp + ".jpg";
		imwrite(newName, img);
		//cout << "newName = " << newName << " ;" << detal << endl;
	}

	// 尺度变换
	bool ScaleTrans(string srcImg, int threshLow = 3, int threshHigh = 9)
	{
		if (srcImg.empty())
		{
			cout << "srcImg is empty" << endl;
			return false;
		}

		Mat img = cv::imread(srcImg);
		if (img.data == nullptr)
		{
			cout << "read failed, " << srcImg << endl;
			return false;
		}
		if (img.channels() == 1)
		{
			cvtColor(img, img, cv::COLOR_GRAY2BGR);
		}
		cv::Size srcSize(img.cols, img.rows);
		
		double detal = static_cast<double>(GeneUniformNum(threshLow, threshHigh)) * 0.1;
		//cout << "deta1 = " << detal << endl;
		Mat scalMat;
		cv::resize(img, scalMat, cv::Size(), detal, detal /*,CV_INTER_AREA*/);
		cv::resize(scalMat, img, srcSize, 0, 0);

		string strTmp = GetTimeStamp();
		size_t pos = srcImg.find(".");

		string newName = srcImg.substr(0, pos);
		newName = newName + "_" + strTmp + ".jpg";
		imwrite(newName, img);
	}

	// 判断文件内的图像是否都是灰度图像
	bool AllIsGray(vector<string>& vecImgs)
	{
		int nNum = 0;
		size_t n = vecImgs.size();
		for (size_t i = 0; i < n; i++)
		{
			Mat img = cv::imread(vecImgs[i]);
			if (img.data == nullptr)
			{
				cout << "无法解析的图像, " << vecImgs[i] << endl;
				continue;
			}

			if (img.channels() == 1)
			{
				//cvtColor(img, img, cv::COLOR_GRAY2BGR);
				nNum++;
				continue;
			}

			int imgHeight = img.rows;
			int imgWidth = img.cols;
			int pixNum = 0;
			for (int i = 0; i < imgHeight; i++)
			{
				for (int j = 0; j < imgWidth; j++)
				{
					if (img.at<Vec3b>(i, j)[0] == img.at<Vec3b>(i, j)[1] &&
						img.at<Vec3b>(i, j)[0] == img.at<Vec3b>(i, j)[2] &&
						img.at<Vec3b>(i, j)[1] == img.at<Vec3b>(i, j)[2])
					{
						pixNum++;
					}
				}
			}
			if (imgHeight*imgWidth == pixNum)
			{
				nNum++;
			}
		}
		return n == nNum ? true : false;
	}
} // namespace SK
#include <iostream>
#include <fstream>
#include <direct.h>
#include "opencv2/opencv.hpp"
#include "common.h"
#include <ctime>

using namespace std;
using namespace cv;

void main()
{
	/*string str = "d:\\test\\003.jpg";
	string str2 = "d:\\test\\004.jpg";
	for (int i = 0; i < 10; i++)
	{
	SK::HsvTrans(str);
	SK::HsvTrans(str2);
	}*/
	/*
	说明：扩充算法分成四种，
	灰度化：0.1的比例
	缩放：  0.2的比例
	部分遮挡： 0.3的比例
	色调变化： 0.4的比例
	产生[0-9] 10个随机数，[0]       --灰度化 -- 1
	                     [1,2]     --缩放   -- 2
						 [3,4,5]   --遮挡   -- 3
						 [6,7,8,9] - 色调   -- 4
	*/
	cout << "**************Begin******************" << endl;
	cout << "扩充类型：1-灰度化  2-缩放  3-遮挡  4-色调调整" << endl;
	float grayRatio  = 0.1;
	float scaleRatio = 0.2;
	float blockRatio = 0.3;
	float hsvRatio   = 0.4;

	SK::InitRandomEngine();

	char buff[1024];
	memset(buff, 0, 1024);
	_getcwd(buff, 1024); //获取当前工作路径

	//printf("%s\n", buff);
	string currentPathStr(buff);
	
	string configTxtPath = currentPathStr + "\\config.txt";
	ifstream infile(configTxtPath);
	if (false == infile.is_open())
	{
		cout << "配置文件打开失败,请检查是否存在配置文件" << endl;
		return;
	}

	string rootDir = "";
	string fileType = "jpg";
	int    totalNum = 0;

	string readStr;
	while (getline(infile, readStr))
	{
		readStr = SK::Trim(readStr);
		vector<string> resVec = SK::Split(readStr, " ");
		if (resVec[0] == "rootDir")
		{
			rootDir = resVec[1];
		}
		else if (resVec[0] == "fileType")
		{
			fileType = resVec[1];
		}
		else if (resVec[0] == "gross")
		{
			totalNum = stoi(resVec[1]);
		}
	}

	bool bRet = false;
	vector<string> foldVec;
	bRet = SK::GetFolderList(rootDir, foldVec);
	if (bRet == false)
	{
		cout << "获取根目录下的文件夹失败" << endl;
	}

	for (size_t i = 0; i < foldVec.size(); i++)
	{
		vector<string> imglist;
		//string baseDir = rootDir + "\\" + foldVec[i];
		bRet = SK::GetFileList(foldVec[i], fileType, imglist);
		if (bRet == true)
		{
			size_t itemNum = imglist.size();
			int offNum = totalNum - itemNum;
			int grayTranNum = 0;
			int scaleTranNum = 0;
			int blockTranNum = 0;
			int hTranNum = 0;
			if (offNum > 0)
			{
				grayTranNum = static_cast<int>(offNum * grayRatio);
				scaleTranNum = static_cast<int>(offNum * scaleRatio);
				blockTranNum = static_cast<int>(offNum * blockRatio);
				hTranNum = offNum - grayTranNum - scaleTranNum - blockTranNum;
			}
			while (grayTranNum > 0)
			{
				//随机选择一张图片
				int index = SK::GeneUniformNum(0, itemNum - 1);
				string imgPath = imglist[index];
				if (SK::GrayTrans(imgPath))
				{
					grayTranNum--;
				}
			}
			while (scaleTranNum > 0)
			{
				int index = SK::GeneUniformNum(0, itemNum - 1);
				string imgPath = imglist[index];
				if (SK::ScaleTrans(imgPath))
				{
					scaleTranNum--;
				}
			}
			while (blockTranNum > 0)
			{
				int index = SK::GeneUniformNum(0, itemNum - 1);
				string imgPath = imglist[index];
				if (SK::BlockTrans(imgPath))
				{
					blockTranNum--;
				}
			}
			while (hTranNum > 0)
			{
				int index = SK::GeneUniformNum(0, itemNum - 1);
				string imgPath = imglist[index];
				if (SK::HsvTrans(imgPath))
				{
					hTranNum--;
				}
			}
		}
		cout << "****************end for " << foldVec[i] << endl;
	}
	cout << "*********************end**********************" << endl;
	system("pause");
}
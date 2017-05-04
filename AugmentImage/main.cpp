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
	˵���������㷨�ֳ����֣�
	�ҶȻ���0.1�ı���
	���ţ�  0.2�ı���
	�����ڵ��� 0.3�ı���
	ɫ���仯�� 0.4�ı���
	����[0-9] 10���������[0]       --�ҶȻ� -- 1
	                     [1,2]     --����   -- 2
						 [3,4,5]   --�ڵ�   -- 3
						 [6,7,8,9] - ɫ��   -- 4
	*/
	cout << "**************Begin******************" << endl;
	cout << "�������ͣ�1-�ҶȻ�  2-����  3-�ڵ�  4-ɫ������" << endl;
	float grayRatio  = 0.1;
	float scaleRatio = 0.2;
	float blockRatio = 0.3;
	float hsvRatio   = 0.4;

	SK::InitRandomEngine();

	char buff[1024];
	memset(buff, 0, 1024);
	_getcwd(buff, 1024); //��ȡ��ǰ����·��

	//printf("%s\n", buff);
	string currentPathStr(buff);
	
	string configTxtPath = currentPathStr + "\\config.txt";
	ifstream infile(configTxtPath);
	if (false == infile.is_open())
	{
		cout << "�����ļ���ʧ��,�����Ƿ���������ļ�" << endl;
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
		cout << "��ȡ��Ŀ¼�µ��ļ���ʧ��" << endl;
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
				//���ѡ��һ��ͼƬ
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
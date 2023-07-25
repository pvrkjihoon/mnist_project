#include "../Common/Common.h"
#include <filesystem>
namespace fs = std::filesystem;

int main()
{
	string fileDir = "../MNIST_Dataset/MNIST_JPG_training/0";
	vector<string> filelist;
	string result_file;
	for (const auto& entry : fs::directory_iterator(fileDir))
	{
		if (entry.is_regular_file() && entry.path().extension() == ".jpg" ||
			entry.is_regular_file() && entry.path().extension() == ".png")
			filelist.push_back(entry.path().string());
	}
	// ptrn_img ¸¸µé±â
	Mat ptrn_img = Mat::zeros(Size(28, 28), CV_8UC1);
	int count = 0;
	float ptrn[28 * 28] = { 0, };
	for (const std::string& filePath : filelist)
	{
		Mat input_img = cv::imread(filePath, cv::ImreadModes::IMREAD_ANYCOLOR);
		for (int row = 0; row < input_img.rows; row++)
		{
			for (int col = 0; col < input_img.cols; col++)
			{
				float tmp = static_cast<float>(input_img.data[row * ptrn_img.cols + col]);
				tmp = tmp / filelist.size();
				ptrn[row * ptrn_img.cols + col] += tmp;
			}
		}


		count++;
	}
	int a = 1;
	for (int row = 0; row < ptrn_img.rows; row++)
	{
		for (int col = 0; col < ptrn_img.cols; col++)
		{
			ptrn_img.data[row * ptrn_img.cols + col] = ptrn[row * ptrn_img.cols + col];
		}
	}
	int aa = 1;
	result_file = "../MNIST_Dataset/MNIST_JPG_training/result_0_" + to_string(count) + ".jpg";
	cv::imwrite(result_file, ptrn_img);
}
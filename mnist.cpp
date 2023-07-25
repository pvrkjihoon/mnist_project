#include "../Common/Common.h"
#include <filesystem>
namespace fs = std::filesystem;

int main() {
    // load ptrn_img
    //string ptrn_fileDir = "../MNIST_Dataset/MNIST_JPG_training";
    string ptrn_fileDir = "../MNIST_Dataset/";
    vector<string> ptrn_filelist;
    for (const auto& entry : fs::directory_iterator(ptrn_fileDir))
    {
        if (entry.is_regular_file() && entry.path().extension() == ".jpg" ||
            entry.is_regular_file() && entry.path().extension() == ".png")
            ptrn_filelist.push_back(entry.path().string());
    }
    vector<Mat> ptrns;
    for (const std::string& ptrn_filePath : ptrn_filelist)
    {
        Mat ptrn = cv::imread(ptrn_filePath, cv::ImreadModes::IMREAD_ANYCOLOR);
        ptrns.push_back(ptrn);
    }


    // test
    for (size_t num = 0; num < 10; num++)
    {
        string fileDir = "../MNIST_Dataset/MNIST_JPG_testing/" + to_string(num);
        vector<string> filelist;
        for (const auto& entry : fs::directory_iterator(fileDir))
        {
            if (entry.is_regular_file() && entry.path().extension() == ".jpg" ||
                entry.is_regular_file() && entry.path().extension() == ".png")
                filelist.push_back(entry.path().string());
        }
        int count = 0;
        int result[10] = { 0, };
        string fail[10];
        for (const std::string& filePath : filelist)
        {
            Mat input_img = cv::imread(filePath, cv::ImreadModes::IMREAD_ANYCOLOR);

            float find_value[10] = { 0, };
            int idx = 0;
            int min_idx = 0;

            for (const auto& ptrn : ptrns)
            {
                for (int row = 0; row < input_img.rows; row++)
                {
                    for (int col = 0; col < input_img.cols; col++)
                    {
                        uchar p1 = input_img.data[row * input_img.cols + col];
                        uchar p2 = ptrn.data[row * ptrn.cols + col];
                        float tmp = p2 - p1;
                        tmp = std::abs<float>(tmp);
                        find_value[idx] += tmp;
                    }
                }
                size_t len = ptrn.total();
                find_value[idx] /= len;

                idx++;
            }
            min_idx = distance(find_value, min_element(find_value, find_value + 10));
            result[min_idx]++;
            count++;
        }
        cout << "num = " << num << "  : " << count << " tested" << "\n";
        float accuracy = static_cast<float>(result[num]) / static_cast<float>(filelist.size()) * 100;
        cout << "accuracy : " << accuracy << "%\n";
        for (int i = 0; i < 10; i++)
        {
            cout << i << " : " << result[i] << "\n";
        }
    }
    return 0;
}
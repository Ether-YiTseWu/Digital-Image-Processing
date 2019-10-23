#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <fstream>
#include <string>

using namespace cv;
using namespace std;

int ClassMatrix[][16] =
{ {204, 0, 5, 33, 51, 59, 23,118, 54, 69, 40,160,169,110,168,188},
{ 3, 6, 22, 36, 60, 50, 74,115,140, 82,147,164,171,142,220,214},
{ 14, 7, 42, 16, 63, 52, 94, 56,133,152,158,177,179,208,222, 1},
{ 15, 26, 43, 75, 79, 84,148, 81,139,136,166,102,217,219,226, 4},
{ 17, 39, 72, 92,103,108,150,135,157,193,190,100,223,225,227, 13},
{ 28,111, 99, 87,116,131,155,112,183,196,181,224,232,228, 12, 21},
{ 47,120, 91,105,125,132,172,180,184,205,175,233,245, 8, 20, 41},
{ 76, 65,129,137,165,145,178,194,206,170,229,244,246, 19, 24, 49},
{ 80, 73,106,138,176,182,174,197,218,235,242,249,247, 18, 48, 68},
{101,107,134,153,185,163,202,173,231,241,248,253, 44, 88, 70, 45},
{123,141,149, 61,195,200,221,234,240,243,254, 38, 46, 77,104,109},
{ 85, 96,156,130,203,215,230,250,251,252,255, 53, 62, 93, 86,117},
{151,167,189,207,201,216,236,239, 25, 31, 34,113, 83, 95,124,114},
{144,146,191,209,213,237,238, 29, 32, 55, 64, 97,126, 78,128,159},
{187,192,198,212, 9, 10, 30, 35, 58, 67, 90, 71,122,127,154,161},
{199,210,211, 2, 11, 27, 37, 57, 66, 89, 98,121,119,143,162,186} };

double ErrorArray[3][3] = { {0.38459,1,0.38459},{1,0,1},{0.38459,1,0.38459} };
double HalftoneImage[1944][2592], errorvalue, Weightsum;
int Index_X, Index_Y;

int main(int argc, char** argv)
{
	Mat imgGRAY, imgDouble;
	imgGRAY = imread("C:\\Users\\Administrator\\source\\repos\\river.JPG", IMREAD_GRAYSCALE);
	if (imgGRAY.empty())
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}

	uchar* arr = imgGRAY.isContinuous() ? imgGRAY.data : imgGRAY.clone().data;

	int seq = -1;
	for (int i = 0; i < 1944; i++)
	{
		seq++;
		for (int j = 0; j < 2592; j++)
			HalftoneImage[i][j] = imgGRAY.data[seq];
	}

	int Index_X, Index_Y;
	double errorvalue;
	double Weightsum;

	for (int k = 0; k < 256; k++)
	{
		for (int i = 0; i < 1944; i = i + 16)
		{
			for (int j = 0; j < 2592; j = j + 16)
			{
				for (int x = 0; x < 16; x++)
				{
					for (int y = 0; y < 16; y++)
					{
						if (k == ClassMatrix[x][y])
						{
							if (HalftoneImage[i + x][j + y] < 128)
							{
								errorvalue = (HalftoneImage[i + x][j + y] - 0);
								HalftoneImage[i + x][j + y] = 0;
							}
							else
							{
								errorvalue = (HalftoneImage[i + x][j + y] - 255);
								HalftoneImage[i + x][j + y] = 255;
							}

							Weightsum = 0;
							for (int p = -1; p <= 1; p++)
							{

								if ((i + x + p) < 0 || (i + x + p) >= 1944)
									continue;
								for (int q = -1; q <= 1; q++)
								{
									if ((j + y + q) < 0 || (j + y + q) >= 2592)
										continue;
									Index_X = (x + p) % 16;
									Index_Y = (y + q) % 16;

									if ((x + p) < 0)
										Index_X = 16 + Index_X;
									if ((y + q) < 0)
										Index_Y = 16 + Index_Y;
									if (ClassMatrix[Index_X][Index_Y] > k)
										Weightsum = (Weightsum + ClassMatrix[(p + 1)][(q + 1)]);
								}
							}
							Weightsum = 0;
							for (int p = -1; p <= 1; p++)
							{
								if ((i + x + p) < 0 || (i + x + p) >= 1944)
									continue;
								for (int q = -1; q <= 1; q++)
								{
									if ((j + y + q) < 0 || (j + y + q) >= 2592)
										continue;

									Index_X = (x + p) % 16;
									Index_Y = (y + q) % 16;
									if ((x + p) < 0)
										Index_X = 16 + Index_X;
									if ((y + q) < 0)
										Index_Y = 16 + Index_Y;
									if (ClassMatrix[Index_X][Index_Y] > k&& Weightsum != 0)
										HalftoneImage[i + x + p][j + y + q] = HalftoneImage[i + x + p][j + y + q] + (errorvalue * ClassMatrix[(p + 1)][(q + 1)] / Weightsum);
								}
							}
						}
					}
				}
			}
		}
	}

	ofstream myfile("ans.txt");
	if (myfile.is_open())
	{
		for (int i = 0; i < 1944; i++)
			for (int j = 0; j < 2592; j++)
				myfile << HalftoneImage[i][j];
			cout << endl;
		myfile.close();
	}
	else
		cout << "Unable to open file";

	return 0;
}
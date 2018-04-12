//Stereo Panorama Program
//©All rights reserved to Avia Maza, Sabina Aledort and Gal Zaidman

//includes
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/stitching.hpp"
#include <string> 
//Namespace:
using namespace std;
using namespace cv;

//Declarations:
int calculateFramesPerSecond(VideoCapture video);
void convertImageToGrayScale(Mat& input, Mat& output);
void createTheFramesVectorFromVideo(VideoCapture video, int framesPerSecond, int framesFrequency, vector<Mat>& framesVec, int transposeRight);
void resizeFunc(vector<Mat>& framesVec);
void featureMatching(vector<Mat>& framesVec, vector<int>& dxsVec, int& totalSumOfdxs);
void createPanorama(vector<Mat> framesVec, vector<int> dxsVec, int numberOfSlice, int totalSumOfdxs);
void createPanoramaForSlice(vector<Mat> framesVec, vector<int> dxsVec, int position, int sliceWidth, int height, Mat& image, int sliceNumber, char strFileName[]);
void createStereoPanorama(Mat& leftEye, Mat& rightEye, int leftPosition, int rightPosition, Mat& stereo, char strFileName[]);
void combineLeftAndRightPanorama(Mat& leftEye, Mat& rightEye, Mat3b& combine, int leftUp, char strFileName[], char strLeft[], char strRight[]);

//Finals:
//Suffix
#define SUFFIX_JPG ".jpg"

//Max number for str
#define MAX_STR_NUM 100

//STR
#define STR_SPERATE_LINE "-----------------------------------------------------------------------\n"
#define STR_COPYRIGHT_SIGN "U+00A9"
#define STR_LEFT "Left"
#define STR_RIGHT "Right"
#define STR_PANORAMA "Panorama"
#define STR_STEREO "Stereo"
#define STR_CENTRAL "Central"

//File Names
#define FN_STEREO "StereoPanorama"
#define FN_LEFT "LeftPanorama"
#define FN_RIGHT "RightPanorama"
#define FN_L_AND_R "Left&RightPanorama"
#define FN_R_AND_L "Right&LeftPanorama"

//Video file name
#define VIDEO_FILE_MOUNTAIN "mountain.mov" //TRANSPOSE_RIGHT
#define VIDEO_FN_ROOFTOP "rooftop.mp4" //TRANSPOSE_LEFT
#define VIDEO_FN_BRIDGE "WilliamsburgBridge.mp4" //TRANSPOSE_RIGHT
#define VIDEO_FN_STREET "street.mp4"  //TRANSPOSE_RIGHT
#define VIDEO_FN_POOL "pool.mp4"  //TRANSPOSE_RIGHT
#define VIDEO_FN_JAFFA_PORT "JaffaPort.mp4"  //TRANSPOSE_RIGHT
#define VIDEO_FN_BUCHAREST_CENTER "Bucharest.mp4"//NO_TRANSPOSE
#define VIDEO_FN_BUCHAREST_PARK "ArrestoPark.mp4"//NO_TRANSPOSE
#define VIDEO_FN_DARK_POOL "DarkPool.mp4"//NO_TRANSPOSE
#define VIDEO_FN_HOUSE "House.mp4"//TRANSPOSE_RIGHT


int main()
{
	//Program variables
	int videoFileNameNum = 0;
	char videoForPanoramaStr[MAX_STR_NUM] = "";
	int fContinue = 1;
	VideoCapture videoForPanorama;
	int frameFrequency = 0;
	int numberOfSlices = 0;
	int numberOfTotalFramesInVideo = 0;
	int framesPerSecond = 0;
	int framesForVector = 0;
	int videoWidthOriginal = 0;
	int	videoHeightOriginal = 0;
	int transposeRight = -1;
	int totalSumOfdxs = 0;
	Mat firstFrame;
	int frameWidthUpdated = 0;
	int	frameHeightUpdated = 0;

	vector<Mat> framesVec;
	vector<int> dxsVec;

	//Opening Remarks
	cout << "Welcome to Stereo Panorama Program\n"
		<< "All rights reserved to Avia Maza, Sabina Aledort and Gal Zaidman\n"
		<< "Enjoy! (=\n"
		<< STR_SPERATE_LINE;

	//Menu 1: Choose a video to work with
	while (fContinue)
	{

		cout << endl
			<< "Menu 1:Choose a video to work with\n"
			<< "Please enter The number of Video File Name you want to work with\n"
			<< " 1 - Mountain\n"
			<< " 2 - Rooftop\n"
			<< " 3 - Williamsburg Bridge\n"
			<< " 4 - Street\n"
			<< " 5 - pool\n"
			<< " 6 - JaffaPort\n"
			<< " 7 - Bucharest Center\n"
			<< " 8 - Bucharest Park\n"
			<< " 9 - Dark Pool\n"
			<< " 10- House\n"
			<< " 11 - Enter other file name\n"
			<< " 12- Exit\n"
			<< " Enter your choice and press enter:\n----> ";
		cin >> videoFileNameNum;

		switch (videoFileNameNum)
		{
		case 1:
			videoForPanorama.open(VIDEO_FILE_MOUNTAIN);
			sprintf(videoForPanoramaStr, "%s", videoForPanoramaStr);
			fContinue = 0;
			break;
		case 2:
			videoForPanorama.open(VIDEO_FN_ROOFTOP);
			sprintf(videoForPanoramaStr, "%s", VIDEO_FN_ROOFTOP);
			fContinue = 0;
			break;
		case 3:
			videoForPanorama.open(VIDEO_FN_BRIDGE);
			sprintf(videoForPanoramaStr, "%s", VIDEO_FN_BRIDGE);
			fContinue = 0;
			break;
		case 4:
			videoForPanorama.open(VIDEO_FN_STREET);
			sprintf(videoForPanoramaStr, "%s", VIDEO_FN_STREET);
			fContinue = 0;
			break;
		case 5:
			videoForPanorama.open(VIDEO_FN_POOL);
			sprintf(videoForPanoramaStr, "%s", VIDEO_FN_POOL);
			fContinue = 0;
			break;
		case 6:
			videoForPanorama.open(VIDEO_FN_JAFFA_PORT);
			sprintf(videoForPanoramaStr, "%s", VIDEO_FN_JAFFA_PORT);
			fContinue = 0;
			break;
		case 7:
			videoForPanorama.open(VIDEO_FN_BUCHAREST_CENTER);
			sprintf(videoForPanoramaStr, "%s", VIDEO_FN_BUCHAREST_CENTER);
			fContinue = 0;
			break;
		case 8:
			videoForPanorama.open(VIDEO_FN_BUCHAREST_PARK);
			sprintf(videoForPanoramaStr, "%s", VIDEO_FN_BUCHAREST_PARK);
			fContinue = 0;
			break;
		case 9:
			videoForPanorama.open(VIDEO_FN_DARK_POOL);
			sprintf(videoForPanoramaStr, "%s", VIDEO_FN_DARK_POOL);
			fContinue = 0;
			break;
		case 10:
			videoForPanorama.open(VIDEO_FN_HOUSE);
			sprintf(videoForPanoramaStr, "%s", VIDEO_FN_HOUSE);
			fContinue = 0;
			break;
		case 11:
			cout << "Pleae enter video file name with suffix\n--->";
			cin >> videoForPanoramaStr;
			videoForPanorama.open(videoForPanoramaStr);
			fContinue = 0;
			break;
		case 12:
			cout << "End of Program.\n GoodBye(=\n";
			system("pause");
			exit(-1);
			break;
		default:
			cout << "Not a Valid Choice. \n"
				<< "Choose again.\n";
			break;
		}

		//Check if video exist
		if (!videoForPanorama.isOpened())
		{
			cout << " ***Could not initialize capturing with this video, please try else...***\n";
			fContinue = 1;
		}

	}//end menu 1
	cout << STR_SPERATE_LINE;

	//Menu 2: Choose if transpose right or left	
	while (!(transposeRight == 1 || transposeRight == 2 || transposeRight == 3))
	{
		cout << endl
			<< "Menu 2: Choose if transpose right or left \n"
			<< "Enter the number for frame frequency you want\n"
			<< " 1 - right\n"
			<< " 2 - left\n"
			<< " 3 - no transpose\n"
			<< " Enter your choice and press enter:\n----> ";
		cin >> transposeRight;

		if (!(transposeRight == 1 || transposeRight == 2 || transposeRight == 3))
			cout << "\value need to be 1 or 2 or 3, please try again";
	}//end menu 2
	cout << STR_SPERATE_LINE;

	//Menu 3: Choose the video frame frequency
	while (frameFrequency < 1 || frameFrequency>4)
	{
		cout << endl
			<< "Menu 3: Choose the video frame frequency(frames skip) \n"
			<< "Enter the number for frame frequency you want\n"
			<< " 1 - very slow\n"
			<< " 2 - slow\n"
			<< " 3 - fast\n"
			<< " 4 - very fast\n"
			<< " Enter your choice and press enter:\n----> ";
		cin >> frameFrequency;

		if (frameFrequency < 1 || frameFrequency>4)
			cout << "\nframe frequency need to be between 1-4, please try again";
	}//end menu 3
	cout << STR_SPERATE_LINE;

	//Menu 4: Choose the number of slices you want	
	while (!(numberOfSlices == 7 || numberOfSlices == 11))
	{
		cout << endl
			<< "Menu 4: Choose the number of slices you want to use \n"
			<< "Enter the number for frame frequency you want\n"
			<< " 7  - good results\n"
			<< " 11 - very good results\n"
			<< " Enter your choice and press enter:\n----> ";
		cin >> numberOfSlices;

		if (!(numberOfSlices == 7 || numberOfSlices == 11))
			cout << "\nframe frequency need to 7 or 11, please try again";
	}//end menu 4
	cout << STR_SPERATE_LINE;

	//Caculate Details
	numberOfTotalFramesInVideo = videoForPanorama.get(CV_CAP_PROP_FRAME_COUNT);
	framesForVector = numberOfTotalFramesInVideo / frameFrequency;
	framesPerSecond = calculateFramesPerSecond(videoForPanorama);
	videoWidthOriginal = videoForPanorama.get(CV_CAP_PROP_FRAME_WIDTH);
	videoHeightOriginal = videoForPanorama.get(CV_CAP_PROP_FRAME_HEIGHT);

	//Present Video Deatails , after user choises
	cout << endl;
	cout << "Main Details:" << endl;
	cout << "Video File Name: " << videoForPanoramaStr << endl;
	cout << "Number Of Total Frames In Video: " << numberOfTotalFramesInVideo << endl;
	cout << "Frame Frequency: " << frameFrequency << endl;
	cout << "Frames For Vector: " << framesForVector << endl;
	cout << "Frames Per Second: " << framesPerSecond << endl;
	cout << "Video frame size Original: " << videoWidthOriginal << "X" << videoHeightOriginal << endl;
	cout << "Number of slices: " << numberOfSlices << endl;
	cout << "\nNow Start process...\n";


	cout << "Part 1: Create the frames Vector from video frames" << endl;
	createTheFramesVectorFromVideo(videoForPanorama, framesPerSecond, frameFrequency, framesVec, transposeRight);
	videoForPanorama.release();
	resizeFunc(framesVec);
	firstFrame = framesVec.at(0);
	frameWidthUpdated = firstFrame.cols;
	frameHeightUpdated = firstFrame.rows;
	cout << "Frame size Updated: " << frameWidthUpdated << "X" << frameHeightUpdated << endl;
	cout << "Part 2: Do the feature matching and find the dx's and calculate the total sum of dx's.\n";
	featureMatching(framesVec, dxsVec, totalSumOfdxs);
	cout << "Part 3-7: Create the panoramas pictures:" << endl;
	createPanorama(framesVec, dxsVec, numberOfSlices, totalSumOfdxs);
	return 0;
}

int calculateFramesPerSecond(VideoCapture video) {
	int framesPerSecond = video.get(CV_CAP_PROP_FPS);

	if (framesPerSecond <= 0)
		framesPerSecond = 10;
	else
		framesPerSecond = 1000 / framesPerSecond;
	return framesPerSecond;
}


void createTheFramesVectorFromVideo(VideoCapture video, int framesPerSecond, int framesFrequency, vector<Mat>& framesVec, int transposeRight)
{

	Mat image;
	int counter = 0;
	while (video.read(image))
	{
		if (counter % framesFrequency == 0)
		{
			if (transposeRight == 1)
				transpose(image, image);
			else if (transposeRight == 2)
			{
				transpose(image, image);
				flip(image, image, 0);
			}
			framesVec.push_back(image.clone());
		}
		waitKey(framesPerSecond);
		counter++;
	}
	image.release();
}

void resizeFunc(vector<Mat>& framesVec)
{
	if (framesVec.at(0).cols > 1000 && framesVec.at(0).rows > 1000)
		for (int i = 0; i < framesVec.size(); i++)
			resize(framesVec.at(i), framesVec.at(i), Size(framesVec.at(i).rows / 2, framesVec.at(i).cols / 2));
}

void convertImageToGrayScale(Mat& input, Mat& output) {
	cvtColor(input, output, CV_RGB2GRAY);
	if (!output.data)
	{
		std::cout << " --(!) Error reading images " << std::endl;
		exit(-1);
	}
}

void featureMatching(vector<Mat>& framesVec, vector<int>& dxsVec, int& totalSumOfdxs)
{
	ofstream fmfile;
	fmfile.open("FM.txt");
	for (int j = 0; j < framesVec.size() - 1; j++)
	{


		fmfile << "Part 2: Do the feature matching and find the dx's and calculate the total sum of dx's.\n";

		cout << "dx num:" << j << endl;
		fmfile << "dx num:" << j << endl;
		Mat gray_image1;
		Mat gray_image2;
		fmfile << "number of frames for calculate this delta x: 2" << endl;
		fmfile << "gray_image1: frame num: " << j + 1 << endl;
		fmfile << "gray_image2: frame num: " << j << endl;
		convertImageToGrayScale(framesVec.at(j + 1), gray_image1);
		convertImageToGrayScale(framesVec.at(j), gray_image2);

		Ptr<ORB> detector = cv::ORB::create(7000, 1.200000048F, 12, 31, 0, 2, ORB::HARRIS_SCORE, 31, 20);

		std::vector< KeyPoint > keypoints_object, keypoints_scene;

		detector->detect(gray_image1, keypoints_object);
		detector->detect(gray_image2, keypoints_scene);

		Ptr<ORB> extractor = cv::ORB::create(7000, 1.200000048F, 12, 31, 0, 2, ORB::HARRIS_SCORE, 31, 20);

		Mat descriptors_object, descriptors_scene;

		extractor->compute(gray_image1, keypoints_object, descriptors_object);
		extractor->compute(gray_image2, keypoints_scene, descriptors_scene);

		BFMatcher matcher;
		std::vector< DMatch > matches;

		matcher.match(descriptors_object, descriptors_scene, matches);

		double max_dist = 0;
		double min_dist = 100;

		for (int i = 0; i < descriptors_object.rows; i++)
		{
			double dist = matches[i].distance;
			if (dist < min_dist) min_dist = dist;
			if (dist > max_dist) max_dist = dist;
		}
		fmfile << "min_dist: " << min_dist << endl;
		fmfile << "max_dist: " << max_dist << endl;
		std::vector< DMatch > good_matches;
		int good = 0;
		double count = 2;
		while (good < 50)
		{
			good_matches.clear();
			for (int i = 0; i < descriptors_object.rows; i++)
			{
				if (matches[i].distance <= count * (min_dist + 1))
				{
					fmfile << "matches[" << i << "].queryIdx: " << matches[i].queryIdx << endl;
					fmfile << "matches[" << i << "].trainIdx: " << matches[i].trainIdx << endl;
					good_matches.push_back(matches[i]);
				}
			}
			good = good_matches.size();
			count += 0.5;
		}

		std::vector< Point2f > obj;
		std::vector< Point2f > scene;
		fmfile << "good_matches.size(): " << good_matches.size() << endl;
		for (int i = 0; i < good_matches.size(); i++)
		{
			obj.push_back(keypoints_object[good_matches[i].queryIdx].pt);
			fmfile << "keypoints_object[good_matches[" << i << "].queryIdx].pt: " << keypoints_object[good_matches[i].queryIdx].pt << " was added to obj vec" << endl;
			scene.push_back(keypoints_scene[good_matches[i].trainIdx].pt);
			fmfile << "keypoints_scene[good_matches[" << i << "].trainIdx].pt: " << keypoints_scene[good_matches[i].trainIdx].pt << " was added to scene vec" << endl;
		}

		Mat H = findHomography(obj, scene, CV_RANSAC);

		fmfile << "H :" << H.size() << endl;

		double dx00 = (H.at<double>(Point(0, 0)));;
		fmfile << "double dx00 Point(0, 0)) :" << dx00 << endl;
		double dx01 = (H.at<double>(Point(0, 1)));;
		fmfile << "double dx01 Point(0, 1)) :" << dx01 << endl;
		double dx02 = (H.at<double>(Point(0, 2)));;
		fmfile << "double dx02 Point(0, 2)) :" << dx02 << endl;
		double dx10 = (H.at<double>(Point(1, 0)));;
		fmfile << "double dx10 Point(1, 0)) :" << dx10 << endl;
		double dx11 = (H.at<double>(Point(1, 1)));;
		fmfile << "double dx11 Point(1, 1)) :" << dx11 << endl;
		double dx12 = (H.at<double>(Point(1, 2)));;
		fmfile << "double dx12 Point(1, 2)) :" << dx12 << endl;
		double dx20 = (H.at<double>(Point(2, 0)));;
		fmfile << "double dx20 Point(2, 0)) :" << dx20 << endl;
		double dx21 = (H.at<double>(Point(2, 1)));;
		fmfile << "double dx21 Point(2, 1)) :" << dx21 << endl;
		double dx22 = (H.at<double>(Point(2, 2)));;
		fmfile << "double dx22 Point(2, 2)) :" << dx22 << endl;

		double dx2 = (H.at<double>(Point(2, 0)));;
		cout << "double dx2 Point(2, 0)) :" << dx2 << endl;
		fmfile << "double dx2 Point(2, 0)) :" << dx2 << endl;

		if (dx2 < 0)
			dx2 *= -1;
		int dx = (int)(H.at<double>(Point(2, 0)));
		if (dx < 0)
			dx *= -1;
		double difference = dx2 - dx;
		fmfile << "difference" << difference << endl;
		cout << "difference" << difference << endl;
		if (difference > 0.5)
			dx++;


		fmfile << "dx :" << dx << endl;
		cout << "dx :" << dx << endl;

		totalSumOfdxs += dx;
		dxsVec.push_back(dx);

		//When find the dx, release all
		gray_image1.release();
		gray_image2.release();
		descriptors_object.release();
		descriptors_scene.release();
		H.release();
	}
	fmfile.close();
}

void createPanorama(vector<Mat> framesVec, vector<int> dxsVec, int numberOfSlice, int totalSumOfdxs)
{

	Mat firstFrame = framesVec.at(0);
	int right = 0;
	int left = numberOfSlice - 1;
	int middle = numberOfSlice / 2;
	int width = firstFrame.cols;
	int height = firstFrame.rows;
	int sliceWidth = width / numberOfSlice;
	int widthForResult = totalSumOfdxs + sliceWidth;
	int leftPosition = 0;
	int rightPosition = 0;
	int middlePosition = middle * sliceWidth;
	char strFileNameForLeft[MAX_STR_NUM];
	char strFileNameForRight[MAX_STR_NUM];
	char strFileNameForCombineLeftRight[MAX_STR_NUM];
	char strFileNameForCombineRightLeft[MAX_STR_NUM];
	char strFileNameForStereo[MAX_STR_NUM];
	char strFileNameForCentralPanorama[MAX_STR_NUM];
	char strFileNameForCombineLeftRightCentral[MAX_STR_NUM];

	//Create middle slice
	cout << "Create middle slice (the central panorama).\n";
	Mat panoramaMiddle(height, widthForResult, 16);
	Mat3b combineForMiddle(height, widthForResult, 16);

	sprintf(strFileNameForCentralPanorama, "%s%s%d%s", STR_CENTRAL, STR_PANORAMA, middle, SUFFIX_JPG);
	sprintf(strFileNameForCombineLeftRightCentral, "%s%s%s%s%d%s", STR_LEFT, STR_RIGHT, STR_CENTRAL, STR_PANORAMA, middle, SUFFIX_JPG);

	//For Middle
	createPanoramaForSlice(framesVec, dxsVec, middlePosition, sliceWidth, height, panoramaMiddle, middle, strFileNameForCentralPanorama);

	//For Combine
	combineLeftAndRightPanorama(panoramaMiddle, panoramaMiddle, combineForMiddle, 1, strFileNameForCombineLeftRightCentral, strFileNameForCentralPanorama, strFileNameForCentralPanorama);

	panoramaMiddle.release();
	combineForMiddle.release();

	for (right = 0, left = numberOfSlice - 1; right < middle; right++, left--)
	{
		cout << "Create pair: " << right << " and " << left << " slices.\n";

		sprintf(strFileNameForLeft, "%s%s%d%s", STR_LEFT, STR_PANORAMA, left, SUFFIX_JPG);
		sprintf(strFileNameForRight, "%s%s%d%s", STR_RIGHT, STR_PANORAMA, right, SUFFIX_JPG);
		sprintf(strFileNameForCombineLeftRight, "%s%s%s%d%d%s", STR_LEFT, STR_RIGHT, STR_PANORAMA, left, right, SUFFIX_JPG);
		sprintf(strFileNameForCombineRightLeft, "%s%s%s%d%d%s", STR_RIGHT, STR_LEFT, STR_PANORAMA, right, left, SUFFIX_JPG);
		sprintf(strFileNameForStereo, "%s%s%d%d%s", STR_STEREO, STR_PANORAMA, right, left, SUFFIX_JPG);

		//init mats
		Mat stereoPanorama;
		Mat panoramaLeft(height, widthForResult, 16);
		Mat panoramaRight(height, widthForResult, 16);
		Mat3b combine(height, widthForResult, 16);

		//init postion
		leftPosition = left * sliceWidth;
		rightPosition = right * sliceWidth;

		//For left
		createPanoramaForSlice(framesVec, dxsVec, leftPosition, sliceWidth, height, panoramaLeft, left, strFileNameForLeft);

		//For Right
		createPanoramaForSlice(framesVec, dxsVec, rightPosition, sliceWidth, height, panoramaRight, right, strFileNameForRight);

		//For Combine -Left Up
		combineLeftAndRightPanorama(panoramaLeft, panoramaRight, combine, 1, strFileNameForCombineLeftRight, strFileNameForLeft, strFileNameForRight);

		//For Combine -Right Up
		combineLeftAndRightPanorama(panoramaLeft, panoramaRight, combine, 0, strFileNameForCombineRightLeft, strFileNameForLeft, strFileNameForRight);

		//For Stereo
		createStereoPanorama(panoramaLeft, panoramaRight, leftPosition, rightPosition, stereoPanorama, strFileNameForStereo);

		//After create and save results, release all
		stereoPanorama.release();
		panoramaLeft.release();
		panoramaRight.release();
		combine.release();

	}

}


void createPanoramaForSlice(vector<Mat> framesVec, vector<int> dxsVec, int position, int sliceWidth, int height, Mat& image, int sliceNumber, char strFileName[])
{
	Mat temp = framesVec.at(0)(Rect(position, 0, sliceWidth, height));
	temp.copyTo(image(Rect(0, 0, sliceWidth, height)));
	int currentColAtRes = sliceWidth;
	int xStartDrawPixsle = position;
	int howManyColsDraw = sliceWidth;
	for (int i = 1; i < framesVec.size(); i++)
	{
		if (dxsVec.at(i - 1) < sliceWidth)
		{
			xStartDrawPixsle = position + sliceWidth - dxsVec.at(i - 1);
			howManyColsDraw = dxsVec.at(i - 1);
		}
		else
		{
			xStartDrawPixsle = position;
			howManyColsDraw = sliceWidth;
		}
		Mat temp = framesVec.at(i)(Rect(xStartDrawPixsle, 0, howManyColsDraw, height));
		temp.copyTo(image(Rect(currentColAtRes, 0, howManyColsDraw, height)));
		currentColAtRes += howManyColsDraw;
	}
	imwrite(strFileName, image);
}


void createStereoPanorama(Mat& leftEye, Mat& rightEye, int leftPosition, int rightPosition, Mat& stereo, char strFileName[])
{
	rightEye = rightEye(Rect(leftPosition - rightPosition, 0, rightEye.cols - (leftPosition - rightPosition), rightEye.rows));
	if (leftEye.cols <= rightEye.cols)
		rightEye = rightEye(Rect(0, 0, leftEye.cols, rightEye.rows));
	else
		leftEye = leftEye(Rect(0, 0, rightEye.cols, leftEye.rows));
	for (int i = 0; i < rightEye.cols; i++)
		for (int j = 0; j < rightEye.rows; j++)
			(rightEye.at<Vec3b>(Point(i, j))).val[2] = 0;
	for (int i = 0; i < leftEye.cols; i++)
		for (int j = 0; j < leftEye.rows; j++)
		{
			(leftEye.at<Vec3b>(Point(i, j))).val[1] = 0;
			(leftEye.at<Vec3b>(Point(i, j))).val[0] = 0;
		}
	stereo = leftEye + rightEye;
	imwrite(strFileName, stereo);
}

void combineLeftAndRightPanorama(Mat& leftEye, Mat& rightEye, Mat3b& combine, int leftUp, char strFileName[], char strLeft[], char strRight[])
{

	Mat3b img1, img2;
	if (leftUp)
	{
		img1 = imread(strLeft);
		img2 = imread(strRight);
	}
	else
	{
		img1 = imread(strRight);
		img2 = imread(strLeft);
	}
	if (!img1.data || !img2.data)
	{
		std::cout << " --(!) Error reading images " << std::endl;
		system("pause");
		exit(-1);
	}
	// Get dimension of final image
	int cols = max(img1.cols, img2.cols);
	int rows = img1.rows + img2.rows;

	// Create a black image
	Mat3b res(rows, cols, Vec3b(0, 0, 0));

	// Copy images in correct position
	img1.copyTo(res(Rect(0, 0, img1.cols, img1.rows)));

	img2.copyTo(res(Rect(0, img1.rows, img2.cols, img2.rows)));

	imwrite(strFileName, res);
}


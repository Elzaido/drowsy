#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <Windows.h>
#include "header.h"


int main() {
    cv::VideoCapture cap(0); // Open the default camera (usually the webcam)

    cv::CascadeClassifier face_cascade;
    cv::CascadeClassifier eye_cascade;

    if (!face_cascade.load("C:/Users/Hp/opencv/sources/data/haarcascades_cuda/haarcascade_frontalface_alt.xml") ||
        !eye_cascade.load("C:/Users/Hp/opencv/sources/data/haarcascades_cuda/haarcascade_eye.xml")) {
        std::cerr << "Error loading Haar Cascade XML files." << std::endl;
        return -1;
    }

    /*if (!face_cascade.load("D:/Thesis_Things/New HaarCascade Classifier/reduced_haarcascade_frontalface_alt.xml") ||
        !eye_cascade.load("D:/Thesis_Things/New HaarCascade Classifier/reduced_haarcascade_eye.xml")) {
        std::cerr << "Error loading Haar Cascade XML files." << std::endl;
        return -1;
    }*/

    detectDrowsiness(cap, face_cascade, eye_cascade);

    return 0;
}



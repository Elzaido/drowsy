#pragma once

// Function to perform drowsiness detection
void detectDrowsiness(cv::VideoCapture& cap, cv::CascadeClassifier& face_cascade, cv::CascadeClassifier& eye_cascade);

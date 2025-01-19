#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <Windows.h>

void detectDrowsiness(cv::VideoCapture& cap, cv::CascadeClassifier& face_cascade, cv::CascadeClassifier& eye_cascade) {
    bool alarmOn = false;
    time_t alarmStartTime = 0;
    const int alarmDuration = 3; // Alarm duration in seconds

    while (true) {
        cv::Mat frame;
        cap >> frame; // Read a frame from the camera

        cv::Mat gray;
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY); // Convert the frame to grayscale
        cv::equalizeHist(gray, gray); // Equalize the histogram to improve contrast

        std::vector<cv::Rect> faces;
        face_cascade.detectMultiScale(gray, faces, 1.1, 2, 0 | cv::CASCADE_SCALE_IMAGE, cv::Size(30, 30)); // Detect faces in the grayscale image

        alarmOn = false; // Reset the alarm flag

        for (size_t i = 0; i < faces.size(); i++) {
            cv::Rect face = faces[i];
            cv::rectangle(frame, face, cv::Scalar(0, 255, 0), 2); // Draw a rectangle around the detected face

            cv::Mat faceROI = gray(face); // Extract the region of interest (ROI) that corresponds to the detected face
            std::vector<cv::Rect> eyes;

            eye_cascade.detectMultiScale(faceROI, eyes, 1.1, 2, 0 | cv::CASCADE_SCALE_IMAGE, cv::Size(30, 30)); // Detect eyes in the face ROI

            if (eyes.empty()) {
                // Eyes are closed
                if (alarmOn) {
                    if (time(nullptr) - alarmStartTime >= alarmDuration) {
                        // Deactivate the alarm after the specified duration
                        alarmOn = false;
                        // Stop the alarm sound
                        Beep(0, 0); // Silence the beep
                    }
                }
                else {
                    // Activate the alarm
                    alarmOn = true;
                    alarmStartTime = time(nullptr);
                    // Start the alarm sound
                    Beep(1000, 3000); // Beep at 1000 Hz for 3 seconds
                }
                // Draw "Drowsy" text inside the face rectangle
                cv::putText(frame, "Drowsy", cv::Point(face.x, face.y - 10), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255), 2);
            }
            else {
                // Eyes are open
                alarmOn = false;
                // Silence the beep
                Beep(0, 0);
                // Draw "Awake" text inside the face rectangle
                cv::putText(frame, "Awake", cv::Point(face.x, face.y - 10), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0), 2);
            }

            for (size_t j = 0; j < eyes.size(); j++) {
                cv::Rect eye = eyes[j];
                cv::Point eye_center(face.x + eye.x + eye.width / 2, face.y + eye.y + eye.height / 2);
                int radius = cvRound((eye.width + eye.height) * 0.25);
                cv::circle(frame, eye_center, radius, cv::Scalar(255, 0, 0), 2); // Draw circles around the detected eyes
            }
        }

        cv::imshow("Drowsiness Detection", frame); // Display the frame with detected eyes

        if (cv::waitKey(30) == 27) // Exit if the 'Esc' key is pressed
            break;
    }

    cap.release(); // Release the camera
    cv::destroyAllWindows(); // Close all OpenCV windows
}
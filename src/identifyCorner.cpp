#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>
#include <opencv2/aruco/charuco.hpp>

int main() {
    // Load the image
    std::string image_path = "/home/raghav/calibration/st_handeye_graph/calibration_data/20231011/000_image.jpg";
    cv::Mat image = cv::imread(image_path);

    // Check if the image is loaded
    if (image.empty()) {
        std::cout << "Image not loaded. Check if the file exists at " << image_path << std::endl;
        return -1;
    }

    // Convert the image to grayscale
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

    // Optionally, apply Gaussian blur to reduce noise
    cv::Mat blurred;
    cv::GaussianBlur(gray, blurred, cv::Size(5, 5), 0);

    // Define Charuco dictionary and board
    cv::Ptr<cv::aruco::Dictionary> aruco_dict = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_4X4_250);
    float square_length = 0.024f;  // Adjust based on your board, in meters
    float marker_length = 0.018f;  // Adjust based on your board, in meters
    cv::Ptr<cv::aruco::CharucoBoard> charuco_board = cv::aruco::CharucoBoard::create(5, 7, square_length, marker_length, aruco_dict);

    // Detect Aruco markers
    std::vector<std::vector<cv::Point2f>> markers;
    std::vector<int> ids;
    std::vector<std::vector<cv::Point2f>> rejectedImgPoints;
    cv::aruco::detectMarkers(blurred, aruco_dict, markers, ids, cv::aruco::DetectorParameters::create(), rejectedImgPoints);

    // Interpolate Charuco corners
    cv::Mat charuco_corners, charuco_ids;
    cv::aruco::interpolateCornersCharuco(markers, ids, blurred, charuco_board, charuco_corners, charuco_ids);

    // Print all detected Charuco corner IDs
    if (charuco_ids.empty()) {
        std::cout << "No Charuco corners were detected." << std::endl;
    } else {
        std::cout << "Detected Charuco corner IDs: ";
        for (int i = 0; i < charuco_ids.rows; ++i) {
            std::cout << charuco_ids.at<int>(i, 0);
            if (i < charuco_ids.rows - 1) {
                std::cout << ", ";
            }
        }
        std::cout << std::endl;
    }

    // Choose a specific corner to look at
    int desired_id = 16;  // Replace with the ID of the corner interested in
    int index = -1;
    for (int i = 0; i < charuco_ids.rows; ++i) {
        if (charuco_ids.at<int>(i, 0) == desired_id) {
            index = i;
            break;
        }
    }

    if (index >= 0) {
        cv::Point2f corner_coordinates = charuco_corners.at<cv::Point2f>(index, 0);
        std::cout << "Corner coordinates for ID " << desired_id << ": " << corner_coordinates << std::endl;
    } else {
        std::cout << "Desired corner ID not found" << std::endl;
    }

    return 0;
}
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{

    //Checking if image was given
    if(argc != 2)
    {
      cout <<" Usage: display_image ImageToLoadAndDisplay" << endl;
      return -1;
    }
    Mat image_gray, image_trsh, image_trsh2, image_dilate, image_distTransf;

    std::vector<std::vector<Point> > contours;
    vector<Vec4i> hierarchy;




    image_gray = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);

    //Check if image was successfuly open
    if(!image_gray.data)
    {
      cout << "Could not open or find the image!" << std::endl;
      return -1;
    }

    //set structuring element to use on the dilation
    Mat element = getStructuringElement( MORPH_RECT,
                                         Size( 5, 5 ),
                                         Point( 0, 0) );


    //uses a threshold to binarize the image
    threshold(image_gray, image_trsh, 185, 255, THRESH_BINARY);

    //dilate and normalize the binarized image
    dilate(image_trsh, image_dilate, element, Point(0, 0), 3);
    normalize(image_dilate, image_dilate, 0, 1, NORM_MINMAX);

    //use distance transform to set a gradient on the image
    distanceTransform(image_dilate, image_distTransf, DIST_C, DIST_MASK_3 );

    threshold(image_distTransf, image_trsh2, 0.35, 255, THRESH_BINARY);

    image_trsh2.convertTo(image_trsh2, CV_8U);

    //find the contours of the image and sends to a matrix of points
    findContours(image_trsh2, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

    /// Draw contours
    Mat drawing = Mat::zeros( image_trsh2.size(), CV_8UC3 );
    for( int i = 0; i< contours.size(); i++ )
    {
        drawContours( drawing, contours, i, 255, 2, 8, hierarchy, 0, Point() );
    }

    //Write the resultant images on the root directory of the project
    imwrite( "./distanceTransform_test.jpg",  image_distTransf);
    imwrite( "./thresh2.jpg",  image_trsh2);
    imwrite("./drawing.jpg", drawing);

    waitKey(0);
    return 0;

}

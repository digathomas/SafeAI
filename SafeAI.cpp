//OpenCV library
#include "opencv2/opencv.hpp"
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"


#include <chrono>
#include <ctime>
#include <cstdlib>
#include <iostream>


using namespace std;
using namespace cv;


void detectGun( Mat frame );
CascadeClassifier gun_cascade;

//Accuracy Default
int accuracy = 0;

//main
int main( int argc, const char** argv )
{

 cout<<" -----------------------------------------------------"<<endl;
 cout<<endl;
 cout<<endl;
 cout<<" W E L C O M E  T O  S A F E . A I "<<endl;
 cout<<endl;
 cout<<endl;
 cout<<endl;
 cout<<endl;
 cout<<" Below you will find a log of all guns detected with relevant information "<<endl;
 cout<<endl;
 cout<<endl;
  cout<<" -----------------------------------------------------"<<endl;


    //Classifier name.
    String gun_cascade_name = "cascade_final.xml";

    //Checks if classifier is succesfully loaded.
    if( !gun_cascade.load( gun_cascade_name ) )
    {
        cout <<"Error loading classifer. "<<endl;
        return -1;
    };

    //Gets webcam feed and creates VideoCapture device.
    int camera_device = 0;
    VideoCapture capture;

    //Checks if Video feed is succesfully opened.
    capture.open( camera_device );
    if ( ! capture.isOpened() )
    {
        cout << "Error loading security camera. "<<endl;
        return -1;
    }

    Mat frame;

    //Loops through frames of provided video feed
    while ( capture.read(frame) )
    {
        //Checks if feed is empty
        if( frame.empty() )
        {
            cout <<"No feed detected."<<endl;
            break;
        }

       //Detects if gun is present in frame.
        detectGun(frame);

       //Exits program on command key
        if(waitKey(10)==27)
        {
          break;
        }

    }
    return 0;
}


void detectGun( Mat frame )
{
    Mat frame_gray;
    cvtColor( frame, frame_gray, COLOR_BGR2GRAY );
    equalizeHist( frame_gray, frame_gray );

    //Stores gun objects in vector
    std::vector<Rect> guns;
    gun_cascade.detectMultiScale( frame_gray, guns);


    for ( size_t i = 0; i < guns.size(); i++ )
    {

       //Vertices of detected gun(s)
       Point p1(guns[i].x, guns[i].y);
       Point p2((guns[i].x + guns[i].height), (guns[i].y + guns[i].width));

       Point center( guns[i].x + guns[i].width/2, guns[i].y + guns[i].height/2 );

       //Rectangle object
      rectangle(frame, p1, p2 , Scalar(0, 0, 255), 2, 8, 0);


        Mat gunROI = frame_gray( guns[i] );

        //Gets current time
        time_t now = time(0);
        char* dt = ctime(&now);

       //Low accuracy. Only potential threat.
        if(accuracy<5)
        {
        cout<<"  W E A P O N  D E T E C T E D :   "<<dt<<endl;
        cout<<"  L O C A T I O N : 14 Engineering Dr, Hanover, NH 03755 "<<endl;
        cout<<" Certainity : "<<accuracy*10<<"% "<<endl;
        cout<<endl;cout<<endl;cout<<endl;
        accuracy++;
        }

        //High accuracy. Multiple gun sightings.
        if(accuracy>=5)
        {
          cout<<"  ====    ||   || "<<endl;
          cout<<" ||  ||   ||   || "<<endl;
          cout<<"  =====   ||   || "<<endl;
          cout<<"     ||   ||   || "<<endl;
          cout<<endl;
          cout<<"A T H O R I T I E S  H A V E  B E E N  C O N T A C T E D "<<endl;
          cout<<endl;
          cout<<"S u s p e c t e d   s p o t t e d   a t :   "<<dt<<endl;
          cout<<"L O C A T I O N : 14 Engineering Dr, Hanover, NH 03755 "<<endl;
          accuracy = 0;
        }
    }
    imshow( "Safe.AI - Gun Detection", frame );
}

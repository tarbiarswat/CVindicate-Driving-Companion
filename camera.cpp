#include <opencv2/opencv.hpp>
#include <raspicam_cv.h>
#include <iostream>
#include <chrono>
#include <ctime>


using namespace std;
using namespace cv;
using namespace raspicam;

Mat frame, Matrix, framePers;

RaspiCam_Cv Camera;

Point2f Source[] = {Point2f(55,135),Point2f(315,135),Point2f(15,185), Point2f(355,185)};
Point2f Destination[] = {Point2f(60,0),Point2f(300,0),Point2f(60,240), Point2f(300,240)};

void Setup ( int argc,char **argv, RaspiCam_Cv &Camera )
{
    Camera.set ( CAP_PROP_FRAME_WIDTH,  ( "-w",argc,argv,360 ) );
    Camera.set ( CAP_PROP_FRAME_HEIGHT,  ( "-h",argc,argv,240 ) );
    Camera.set ( CAP_PROP_BRIGHTNESS, ( "-br",argc,argv,50 ) );
    Camera.set ( CAP_PROP_CONTRAST ,( "-co",argc,argv,50 ) );
    Camera.set ( CAP_PROP_SATURATION,  ( "-sa",argc,argv,50 ) );
    Camera.set ( CAP_PROP_GAIN,  ( "-g",argc,argv ,50 ) );
    Camera.set ( CAP_PROP_FPS,  ( "-fps",argc,argv,30));

}

void Perspective()
{
	line(frame,Source[0], Source[1], Scalar(0,0,255), 2);
	line(frame,Source[1], Source[3], Scalar(0,0,255), 2);
	line(frame,Source[3], Source[2], Scalar(0,0,255), 2);
	line(frame,Source[2], Source[0], Scalar(0,0,255), 2);
	
       
	
	Matrix = getPerspectiveTransform(Source, Destination);
	warpPerspective(frame, framePers, Matrix, Size(350,240));
}

void Capture()
{
    Camera.grab();
    Camera.retrieve(frame);
    cvtColor(frame, frame, COLOR_BGR2RGB);
}


int main(int argc, char **argv)
{
	
    
    Setup(argc, argv, Camera);
	cout<<"Connecting to camera"<<endl;
	if (!Camera.open())	
	{
		cout<<"Failed to Connect"<<endl;
		return -1;
    }
    cout<<"Camera Id = "<<Camera.getId()<<endl;
    
    while(1)
    {
	auto start = std::chrono::system_clock::now();
	Capture();
	Perspective();
	
	namedWindow("FrontView", WINDOW_KEEPRATIO);
	moveWindow("FrontView", 150, 100);
	resizeWindow("FrontView", 640, 480);
	imshow("FrontView", frame);
	
	namedWindow("Birds Eye View", WINDOW_KEEPRATIO);
	moveWindow("Birds Eye View", 150, 100);
	resizeWindow("Birds Eye View", 640, 480);
	imshow("Birds Eye View" ,framePers);
	
	waitKey(1);
	auto end = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = end-start;
	float t = elapsed_seconds.count();
	int FPS = 1/t;
	cout<<"FPS = "<<FPS<<endl;
    
	
	}
	return 0;
     
    }

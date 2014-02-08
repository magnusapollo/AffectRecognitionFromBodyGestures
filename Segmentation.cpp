#include "cv.h"
#include "highgui.h"
#include <ctype.h>
#include "cxcore.h"
#include "cvaux.h"

int ConnectedComponent[640*480][2];
int i =0, j =0;

int checkForSkinColor(int x, int y, IplImage *img);
int connectedComp(int x, int y, IplImage *img);

int main(int argc, char ** argv)
{
	CvCapture* capture = cvCreateCameraCapture( 0 );
	assert(capture);

    /* print a welcome message, and the OpenCV version */
    
	//   CV_VERSION,
	//    CV_MAJOR_VERSION, CV_MINOR_VERSION, CV_SUBMINOR_VERSION);

    /* Capture 1 video frame for initialization */
	IplImage* videoFrame = NULL;
	videoFrame = cvQueryFrame(capture);
	
    if(!videoFrame)
	{
		printf("Bad frame \n");
		exit(0);	
	}
		    // Create windows
	cvNamedWindow("BG", 1);
    cvNamedWindow("FG", 1);

	// Select parameters for Gaussian model.
	CvGaussBGStatModelParams* params = new CvGaussBGStatModelParams;						
	params->win_size=2;	
    params->n_gauss=3;
	params->bg_threshold=0.9;
	params->std_threshold=.5;
	params->minArea=15;
	params->weight_init=0.05;
	params->variance_init=30; 
	
    // Creat CvBGStatModel
    // cvCreateGaussianBGModel( IplImage* first_frame, CvGaussBGStatModelParams* parameters )
    // or
    // cvCreateGaussianBGModel( IplImage* first_frame )
    CvBGStatModel* bgModel = cvCreateGaussianBGModel(videoFrame ,params);

    int key=-1;
   // while(key != 'q')
    //{
		// Grab a fram
		videoFrame = cvQueryFrame(capture);
		//if( !videoFrame )
		//	break;
		int sv = 2;

		// Step number 1: Update model(Background subtraction)
//		cvUpdateBGStatModel(videoFrame,bgModel);
        
		for(int x = 0; x<videoFrame->width; x++)
		{
			for(int y = 0; y<videoFrame->height; y++)
			{
				if(checkForSkinColor(x,y, videoFrame))
				{
					connectedComp(x,y, videoFrame);

				}
			}
		}
		
		for(int k =0;k < i; k++)
		{
			printf("\n");
			for( int j =0; j<2; j++)
				printf("%d", ConnectedComponent[k][j]);
		}
		
        // Display results
      	//cvShowImage("BG", bgModel->background);
		//cvShowImage("FG", bgModel->foreground);	

    //   	key = cvWaitKey(100);
  //  }	

	cvDestroyWindow("BG");
    cvDestroyWindow("FG");
	cvReleaseBGStatModel( &bgModel );
	cvReleaseCapture(&capture);
	return 0;
}

int checkForSkinColor(int x, int y, IplImage * img)
{
	if(x<0||y<0)
		return 0;
	if(
		(((uchar*)(img->imageData + img->widthStep*y))[x*3+0] >= 80 && ((uchar*)(img->imageData + img->widthStep*y))[x*3+0] <= 160) &&
		(((uchar*)(img->imageData + img->widthStep*y))[x*3+1] >= 122 && ((uchar*)(img->imageData + img->widthStep*y))[x*3+1] <= 170) &&
		(((uchar*)(img->imageData + img->widthStep*y))[x*3+2] >= 189 && ((uchar*)(img->imageData + img->widthStep*y))[x*3+2] <= 235)
	)
		return 1;
	
	return 0;
}

int connectedComp(int x, int y,  IplImage * img)
{
	if(x<0||y<0 || !checkForSkinColor(x,y, img))
		return 0;

	else
	{
		ConnectedComponent[i][0] =x;
		ConnectedComponent[i][1] =y;
		i++;
		if(checkForSkinColor(x-1,y-1, img))
		{
			ConnectedComponent[i][0] = x-1;
			ConnectedComponent[1][1] = y-1;
			i++;
		}
		if(checkForSkinColor(x,y-1, img))
		{
			ConnectedComponent[i][0] = x;
			ConnectedComponent[1][1] = y-1;
			i++;
		}
		if(checkForSkinColor(x+1,y-1, img))
		{
			ConnectedComponent[i][0] = x+1;
			ConnectedComponent[1][1] = y-1;
			i++;
		}
		if(checkForSkinColor(x-1,y, img))
		{
			ConnectedComponent[i][0] = x-1;
			ConnectedComponent[1][1] = y;
			i++;
		}
		if(checkForSkinColor(x+1,y, img))
		{
			ConnectedComponent[i][0] = x+1;
			ConnectedComponent[1][1] = y;
			i++;
		}
		if(checkForSkinColor(x-1,y+1, img))
		{
			ConnectedComponent[i][0] = x-1;
			ConnectedComponent[1][1] = y+1;
			i++;
		}
		if(checkForSkinColor(x,y+1, img))
		{
			ConnectedComponent[i][0] = x;
			ConnectedComponent[1][1] = y+1;
			i++;
		}
		if(checkForSkinColor(x+1,y+1, img))
		{
			ConnectedComponent[i][0] = x+1;
			ConnectedComponent[1][1] = y+1;
			i++;
		}
	
		connectedComp(x-1,y-1, img);
		connectedComp(x,y-1, img);
		connectedComp(x+1,y-1, img);
		connectedComp(x-1,y, img);
		connectedComp(x+1,y, img);
		connectedComp(x-1,y+1, img);
		connectedComp(x,y+1, img);
		connectedComp(x+1,y+1, img);
		return 1;
	}
	return 1;
}
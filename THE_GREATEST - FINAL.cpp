#include <stdio.h>
#include <stdlib.h>
#include <libserialport.h>
#include <Windows.h>

#define BAUD 9600

#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <vector>
#include <stdio.h>

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char* argv[])
{
	/*Variables for camera function*/
	int n = 0;
	char filename[200];
	string window_name = "video | q or esc to quit";
	Mat frame;

	// Initialization of the positions of the blocks
	int bluePos = 0;
	int redPos = 0;
	int greenPos = 0;
	/*Setup camera and check for camera*/
	namedWindow(window_name);
	VideoCapture cap(0);
	if (!cap.isOpened())
	{
		cout << "cannot open camera";
	}

	/*Variables for serial comms */
	struct sp_port* port;
	int err;
	int key = 0;

	/* Set up and open the port */
	/* check port usage */
	if (argc < 2)
	{
		/* return error */
		fprintf(stderr, " Port use\n");
		exit(1);
	}

	/* get port name */
	err = sp_get_port_by_name(argv[1], &port);

	if (err == SP_OK)
	{
		/* open port */
		err = sp_open(port, SP_MODE_WRITE);
	}

	if (err != SP_OK)
	{
		/* return error */
		fprintf(stderr, " Can't open port %s\n", argv[1]);
		exit(2);
	}

	/* set Baud rate */
	sp_set_baudrate(port, BAUD);
	/* set the number of bits */
	sp_set_bits(port, 8);


	// Empty Char Value
	char cmd0 = 0;

	// Read the camera frame
	cap >> frame;

	// Initialize bgr values for each positions
	Vec3b intensity1 = frame.at<Vec3b>(230, 130);
	int blue1 = intensity1.val[0];
	int green1 = intensity1.val[1];
	int red1 = intensity1.val[2];

	Vec3b intensity2 = frame.at<Vec3b>(230, 190);
	int blue2 = intensity2.val[0];
	int green2 = intensity2.val[1];
	int red2 = intensity2.val[2];

	Vec3b intensity3 = frame.at<Vec3b>(230, 250);
	int blue3 = intensity3.val[0];
	int green3 = intensity3.val[1];
	int red3 = intensity3.val[2];

	/* set up to exit when q key is entered */
	while (key != 'q')
	{
		// Initialise variables
		char cmd;

		// Initialise variable that stores the input of the user
		char x;
		cout << "Enter A to Pick All Boxes" << endl;
		cout << "Enter B to Pick Blue Box" << endl;
		cout << "Enter G to Pick Green Box" << endl;
		cout << "Enter R to Pick Red Box" << endl;

		// Wait until user inputs selection
		x = getchar();

		// Read the camera frame
		cap >> frame;

		// Go to this condition when the user inputs a correct selection
		if ((x == 'A') || (x == 'a') || (x == 'R') || (x == 'r') || (x == 'G') || (x == 'g') || (x == 'B') || (x == 'b')) {

			// This for loop is to reduce the overshoot of bgr values obtained by the camera
			// Takes 100 frames to settle down lighting into usable values
			for (int i = 0; i < 100; ++i) {
				cap >> frame;

				intensity1 = frame.at<Vec3b>(230, 130);
				blue1 = intensity1.val[0];
				green1 = intensity1.val[1];
				red1 = intensity1.val[2];

				intensity2 = frame.at<Vec3b>(230, 190);
				blue2 = intensity2.val[0];
				green2 = intensity2.val[1];
				red2 = intensity2.val[2];

				intensity3 = frame.at<Vec3b>(230, 250);
				blue3 = intensity3.val[0];
				green3 = intensity3.val[1];
				red3 = intensity3.val[2];
			}
			// Assign position 1 to the correct colour depending on the bgr values taken from Position 1
			if (blue1 > 60)
				bluePos = 1;
			else if (red1 > 100)
				redPos = 1;
			else
				greenPos = 1;

			// Assign position 2 to the correct colour depending on the bgr values taken from Position 2
			if (blue2 > 60)
				bluePos = 2;
			else if (red2 > 100)
				redPos = 2;
			else
				greenPos = 2;

			// Assign position 3 to the correct colour depending on the bgr values taken from Position 3
			if (blue3 > 60)
				bluePos = 3;
			else if (red3 > 100)
				redPos = 3;
			else
				greenPos = 3;

			//Concatenates the values of Blue Red and Green Respectively to a new variable.
			//These values will now contain the position of each coloured block
			int brg = bluePos * 100 + redPos * 10 + greenPos;
			// The for loop will activate if the user selection is 'A' or 'a'. This will collect all the blocks.
			if ((x == 'A') || (x == 'a')) {
				if (brg == 123) {
					cmd = 3;
					sp_blocking_write(port, &cmd, 1, 100);
					cout << "BRG" << endl;
				}
				else if (brg == 132) {
					cmd = 5;
					sp_blocking_write(port, &cmd, 1, 100);
					cout << "BGR" << endl;
				}
				else if (brg == 213) {
					cmd = 9;
					sp_blocking_write(port, &cmd, 1, 100);
					cout << "RBG" << endl;
				}
				else if (brg == 231) {
					cmd = 17;
					sp_blocking_write(port, &cmd, 1, 100);
					cout << "GBR" << endl;
				}
				else if (brg == 321) {
					cmd = 33;
					sp_blocking_write(port, &cmd, 1, 100);
					cout << "GRB" << endl;
				}
				else if (brg == 312) {
					cmd = 65;
					sp_blocking_write(port, &cmd, 1, 100);
					cout << "RGB" << endl;
				}
				else {
					cmd = 0;
					sp_blocking_write(port, &cmd, 1, 100);
					cout << "COLOUR DETECTED ERROR" << endl;
				}
				// Wait 100 milliseconds to send the signal to robot
				Sleep(100);
			}

			// The for loop will activate if the user selection is 'R' or 'r'. This will pick the red block only. 
			if ((x == 'R') || (x == 'r')) {
				switch (redPos) {
				case 1:
					cmd = 6;
					sp_blocking_write(port, &cmd, 1, 100);
					cout << "Red in Pos 1" << endl;
					break;
				case 2:
					cmd = 10;
					sp_blocking_write(port, &cmd, 1, 100);
					cout << "Red in Pos 2" << endl;
					break;
				case 3:
					cmd = 18;
					sp_blocking_write(port, &cmd, 1, 100);
					cout << "Red in Pos 3" << endl;
					break;
				default:
					break;
				}
				// Wait 100 milliseconds to send the signal to robot
				Sleep(100);

			}

			// The for loop will activate if the user selection is 'B' or 'b'. This will pick the blue block only. 
			if ((x == 'B') || (x == 'b')) {
				switch (bluePos) {
				case 1:
					cmd = 12;
					sp_blocking_write(port, &cmd, 1, 100);
					cout << "Blue in Pos 1" << endl;
					break;
				case 2:
					cmd = 20;
					sp_blocking_write(port, &cmd, 1, 100);
					cout << "Blue in Pos 2" << endl;
					break;
				case 3:
					cmd = 36;
					sp_blocking_write(port, &cmd, 1, 100);
					cout << "Blue in Pos 3" << endl;
					break;
				default:
					break;
				}
				// Wait 100 milliseconds to send the signal to robot
				Sleep(100);

			}

			// The for loop will activate if the user selection is 'G' or 'g'. This will pick the green block only. 
			if ((x == 'G') || (x == 'g')) {
				switch (greenPos) {
				case 1:
					cmd = 24;
					sp_blocking_write(port, &cmd, 1, 100);
					cout << "Green in Pos 1" << endl;
					break;
				case 2:
					cmd = 40;
					sp_blocking_write(port, &cmd, 1, 100);
					cout << "Green in Pos 2" << endl;
					break;
				case 3:
					cmd = 72;
					sp_blocking_write(port, &cmd, 1, 100);
					cout << "Green in Pos 3" << endl;
					break;
				default:
					break;
				}
				// Wait 100 milliseconds to send the signal to robot
				Sleep(100);
			}
		}
		// If the selection is wrong or when the signal has gone to the robot after 100 millseconds, send the zero signal to the robot.
		else {
			sp_blocking_write(port, &cmd0, 1, 100);
		}

		/*The code contained here modifies the output pixel values*/
		/* Modify the pixels of the RGB image */
		for (int i = 230; i < frame.rows; i++)
		{
			for (int j = 130; j < frame.cols; j++)
			{
				/*The following lines make the red and blue channels zero
				(this section of the image will be shades of green)*/
				frame.at<Vec3b>(i, j)[0] = 0;
				frame.at<Vec3b>(i, j)[2] = 0;

				// Define the center and radius of the dot
				Point center(130, 230);
				int radius = 3;

				// Draw a red dot at the specified center
				Scalar red(0, 0, 255);									// BGR color (red)
				circle(frame, center, radius, red, -1); // -1 means fill the circle
			}
		}
		/*End of modifying pixel values*/

		imshow(window_name, frame);
		char key = (char)waitKey(25);

		///* specify the command to send to the port */
		// cmd = 1; // Define the binary number that we want to send
		///* write the number "cmd" to the port */
		// sp_blocking_write(port, &cmd, 1, 100); // Send the cmd to the controller

		switch (key)
		{
		case 'q':
		case 'Q':
		case 27: // escape key
			return 0;
		case ' ': // Save an image
			sprintf_s(filename, "filename%.3d.jpg", n++);
			imwrite(filename, frame);
			cout << "Saved " << filename << endl;
			break;
		default:
			break;
		}
		fflush(stdin);
	}

	/* close the port */
	sp_close(port);
	return 0;
}
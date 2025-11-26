/// 
//  mandel.c
//  Based on example code found here:
//  https://users.cs.fiu.edu/~cpoellab/teaching/cop4610_fall22/project3.html
//
//  Converted to use jpg instead of BMP and other minor changes
//  
///
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <sys/wait.h>
#include "jpegrw.h"
#include <pthread.h>

//this is my struct for threading
typedef struct {
	imgRawImage *img; 
	double xmin, xmax, ymin, ymax;
	int max; 
	int start_row, end_row;
	int width, height;
} thread_args;

// local routines
static int iteration_to_color( int i, int max );
static int iterations_at_point( double x, double y, int max );
static void show_help();
void *compute_thread(void *arg);


//method to compute thread
void *compute_thread(void *arg){
	//convert our void to the struct i made
	thread_args *t = (thread_args*)arg;
//loop thru rows
	for(int j = t -> start_row; j < t -> end_row; j++){
		//loop thru columns in the row
		for(int i = 0; i < t -> width; i++){
			//convert pixel coords by mandelbrot x/y
			double x = t -> xmin + i * (t -> xmax - t -> xmin)/ t->width;
			double y = t -> ymin + j * (t -> ymax - t -> ymin) / t -> height;
			int iters = iterations_at_point(x,y, t->max); //compute x and y and run the function, compute iterations
			setPixelCOLOR(t -> img, i, j, iteration_to_color(iters, t -> max)); //set the pixel image on the iteration count
		}
	}
	return NULL;
}

int main( int argc, char *argv[] )
{
	char c;

	// These are the default configuration values used
	// if no command line arguments are given.
	const char *outfile = "mandel%d.jpg";
	double xcenter = 0;
	double ycenter = 0;
	double xscale = 4;
	double yscale = 0; // calc later
	int    image_width = 1000;
	int    image_height = 1000;
	int    max = 1000;
	double zoomfactor = 0.97;
	int process_count = 4;
	int thread_count = 1; //added a variable to keep track of threads

	// For each command line argument given,
	// override the appropriate configuration value.

	

	

	while((c = getopt(argc,argv,"t:x:y:s:W:H:m:o:p:h"))!=-1) { //added -t case
		switch(c) 
		{
			case 't':
				thread_count = atoi(optarg);
				if(thread_count < 1) {
				thread_count = 1; 
				} else if (thread_count > 20){
					thread_count = 20; 
				}
				break;
			case 'x':
				xcenter = atof(optarg);
				break;
			case 'y':
				ycenter = atof(optarg);
				break;
			case 's':
				xscale = atof(optarg);
				break;
			case 'W':
				image_width = atoi(optarg);
				break;
			case 'H':
				image_height = atoi(optarg);
				break;
			case 'm':
				max = atoi(optarg);
				break;
			case 'o':
				outfile = optarg;
				break;
			case 'p':
				process_count = atoi(optarg);
				break;
			case 'h':
				show_help();
				exit(1);
				break;
		}
	}


	int img_count = ceil(50.0 / process_count); //round up from what the user inputted

	for(int n = 0; n < process_count; n++) {
		if(fork() != 0) continue; //make sure we are not the parent

		for(int i = img_count * n; i < img_count * (n + 1); i ++) {
			if (i >= 50) exit(0);

			// Calculate y scale based on x scale (settable) and image sizes in X and Y (settable)
			double newXscale = xscale * pow(zoomfactor, i);
			yscale = newXscale / image_width * image_height;

			char new_outfile[100];
			sprintf(new_outfile, outfile, i);
		
			// Display the configuration of the image.
			printf("mandel: x=%lf y=%lf xscale=%lf yscale=%1f max=%d outfile=%s\n",xcenter,ycenter,newXscale,yscale,max,new_outfile);
		
			// Create a raw image of the appropriate size.
			imgRawImage* img = initRawImage(image_width,image_height);
		
			// Fill it with a black
			setImageCOLOR(img,0);
			
			// get rid of compute image and use threading!
			int width = img -> width; //grab imagge width to avoid using the struct
			int height = img -> height; //get height as well
			pthread_t tids[thread_count]; //mthis array will hold all thread ids
			thread_args args[thread_count]; //make struct array filled with struct arguments - each thread gets one
			int rows_per_thread = height/thread_count; //split image by the threadcount so we don't overwork

			//assign struct variables
			for(int t = 0; t < thread_count; t++){
				args[t].img = img;
				args[t].xmin = xcenter - newXscale/2;
				args[t].xmax = xcenter + newXscale/2;
				args[t].ymin = ycenter - yscale/2;
				args[t].ymax = ycenter + yscale/2;
				args[t].max = max;
				args[t].width = width;
				args[t].height = height;
				args[t].start_row = t * rows_per_thread;

				if(t == thread_count - 1){
					args[t].end_row = height; //thread after does this job

				} 

				else {

					args[t].end_row = (t+1)*rows_per_thread;
				}

			}
	for(int t = 0; t <thread_count; t++){
					//fill args
					//set start and end!
				pthread_create(&tids[t], NULL, compute_thread, &args[t]); //make your thread now
			}
				
				//wait to avoid deadlocks and other issues
	for(int t = 0; t < thread_count; t++){
					pthread_join(tids[t], NULL); //combined threads for shared memory;
		}

			// Save the image in the stated file.
			storeJpegImageFile(img,new_outfile);

			// free the mallocs
			freeRawImage(img);
		}
		
		exit(0); //keep child
	}

	for(int i =0; i < process_count; i++){
		wait(NULL);
	}

	return 0;
}




/*
Return the number of iterations at point x, y
in the Mandelbrot space, up to a maximum of max.
*/

int iterations_at_point( double x, double y, int max )
{
	double x0 = x;
	double y0 = y;

	int iter = 0;

	while( (x*x + y*y <= 4) && iter < max ) {

		double xt = x*x - y*y + x0;
		double yt = 2*x*y + y0;

		x = xt;
		y = yt;

		iter++;
	}

	return iter;
}





/*
Convert a iteration number to a color.
Here, we just scale to gray with a maximum of imax.
Modify this function to make more interesting colors.
*/
int iteration_to_color( int iters, int max )
{
	int color = 0xFFFFFF*iters/(double)max;
	return color;
}

// Show help message
void show_help()
{
	printf("Use: mandel [options]\n");
	printf("Where options are:\n");
	printf("-m <max>    The maximum number of iterations per point. (default=1000)\n");
	printf("-x <coord>  X coordinate of image center point. (default=0)\n");
	printf("-y <coord>  Y coordinate of image center point. (default=0)\n");
	printf("-s <scale>  Scale of the image in Mandlebrot coordinates (X-axis). (default=4)\n");
	printf("-W <pixels> Width of the image in pixels. (default=1000)\n");
	printf("-H <pixels> Height of the image in pixels. (default=1000)\n");
	printf("-o <file>   Set output file. (default=mandel.bmp)\n");
	printf("-p 			enter amount of process you want!\n");
	printf("-h          Show this help text.\n");
	printf("\nSome examples are:\n");
	printf("mandel -x -0.5 -y -0.5 -s 0.2\n");
	printf("mandel -x -.38 -y -.665 -s .05 -m 100\n");
	printf("mandel -x 0.286932 -y 0.014287 -s .0005 -m 1000\n\n");
}


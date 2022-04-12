/*
    MOHAMMED AHMED ZAKIUDDIN
    1001675091
*/

#include "bitmap.h"

#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <errno.h>
#include <string.h>
#include<time.h>
#include<unistd.h>
#include<pthread.h> 
#include<sys/time.h>

int iteration_to_color( int i, int max );
int iterations_at_point( double x, double y, int max );
void* compute_image(void* arg); 

int max; 
double xmin;
double xmax;
double ymin;
double ymax; 

struct bitmap* bm;

struct parameters
{
	int x_start;
	int y_end;
	int thread_id;
};

void show_help()
{
	printf("Use: mandel [options]\n");
	printf("Where options are:\n");
	printf("-m <max>    The maximum number of iterations per point. (default=1000)\n");
	printf("-x <coord>  X coordinate of image center point. (default=0)\n");
	printf("-y <coord>  Y coordinate of image center point. (default=0)\n");
	printf("-s <scale>  Scale of the image in Mandlebrot coordinates. (default=4)\n");
	printf("-W <pixels> Width of the image in pixels. (default=500)\n");
	printf("-H <pixels> Height of the image in pixels. (default=500)\n");
	printf("-n <threads> Number of threads (default = 1)\n");
	printf("-o <file>   Set output file. (default=mandel.bmp)\n");
	printf("-h          Show this help text.\n");
	printf("\nSome examples are:\n");
	printf("mandel -x -0.5 -y -0.5 -s 0.2\n");
	printf("mandel -x -.38 -y -.665 -s .05 -m 100\n");
	printf("mandel -x 0.286932 -y 0.014287 -s .0005 -m 1000\n\n");
}

int main( int argc, char *argv[] )
{
	int i;
	char c;

	// These are the default configuration values used
	// if no command line arguments are given.

	const char *outfile = "mandel.bmp";
	double xcenter = 0;
	double ycenter = 0;
	double scale = 4;
	int    image_width = 500;
	int    image_height = 500;

	max = 1000;

	int no_of_threads = 1;

	struct timeval begin_time;
 	struct timeval end_time;

	// For each command line argument given,
	// override the appropriate configuration value.

	while((c = getopt(argc,argv,"x:y:s:W:H:m:n:o:h"))!=-1) {
		switch(c) {
			case 'x':
				xcenter = atof(optarg);
				break;
			case 'y':
				ycenter = atof(optarg);
				break;
			case 's':
				scale = atof(optarg);
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
			case 'n':
                no_of_threads = atoi(optarg);
                break;
			case 'o':
				outfile = optarg;
				break;
			case 'h':
				show_help();
				exit(1);
				break;
		}
	}

	// Display the configuration of the image.
	printf("mandel: x = %lf y = %lf scale = %lf max = %d outfile = %s number_of_threads = %d\n",xcenter,ycenter,scale,max,outfile,no_of_threads);

	// Prints the size of the image used for each thread.
	int pixel = image_width / no_of_threads;
	printf("Size of the image is: %d pixels\n", pixel);

	//thread_id
	pthread_t tid[no_of_threads];

	struct parameters params[no_of_threads];

	// Create a bitmap of the appropriate size.
	bm = bitmap_create(image_width,image_height);

	// Fill it with a dark blue, for debugging
	bitmap_reset(bm, MAKE_RGBA(0, 0, 255, 0));

	xmin = xcenter - scale;
	xmax = xcenter + scale;
	ymin = ycenter - scale;
	ymax = ycenter + scale;

	// Computes the time taken to for processing the image.
	gettimeofday(&begin_time, NULL);

	// Computing the Mandelbrot image
	for(i = 0;i < no_of_threads;i++)
	{
		if(i == 0)
		{
			params[i].x_start = 0;
			params[i].y_end = pixel;
			params[i].thread_id = 0;
		}
		else
		{
			params[i].x_start = params[i - 1].y_end;
			params[i].y_end = params[i].x_start + pixel;
		}
		// Creating a new thread
		pthread_create(&tid[i], NULL, compute_image, (void *) &params[i]);
	}

	for(i = 0; i < no_of_threads; i++) 
	{
		// Waiting for a thread to terminate its process.
		pthread_join(tid[i], NULL);
	}

  	gettimeofday(&end_time, NULL);
	// gets the time in microseconds.
  	long time_to_execute = ( end_time.tv_sec * 1000000 + end_time.tv_usec ) -
                         ( begin_time.tv_sec * 1000000 + begin_time.tv_usec );

	double time_in_sec;

	time_in_sec = (double) (time_to_execute)/ 1000000;

 	printf("This code took %ld microseconds to execute & these many %f seconds \n", time_to_execute, time_in_sec);

	// Save the image in the stated file.
	if(!bitmap_save(bm,outfile)) 
	{
		fprintf(stderr,"mandel: couldn't write to %s: %s\n",outfile,strerror(errno));
		return 1;
	}
	// terminates the calling of thread.
	pthread_exit(NULL);
}

/*
Compute an entire Mandelbrot image, writing each point to the given bitmap.
Scale the image to the range (xmin-xmax,ymin-ymax), limiting iterations to "max"
*/

void * compute_image(void *arg)
{
	int i,j;

	int width = bitmap_width(bm);
	int height = bitmap_height(bm);

	struct parameters * params = (struct parameters *) arg;

	// For every pixel in the image...
	int begin = params -> x_start;
	int end = params -> y_end;

	for(j = begin;j < end; j++) 
	{

		for(i = 0;i < width;i++) 
		{

			// Determine the point in x,y space for that pixel.
			double x = xmin + i * (xmax - xmin) / width;
			double y = ymin + j * (ymax - ymin) / height;

			// Compute the iterations at that point.
			int iters = iterations_at_point(x, y, max);

			// Set the pixel in the bitmap.
			bitmap_set(bm, i, j, iters);
		}
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

	while( (x * x + y * y <= 4) && iter < max ) {

		double xt = x * x - y * y + x0;
		double yt = 2 * x * y + y0;

		x = xt;
		y = yt;

		iter++;
	}

	return iteration_to_color(iter,max);
}

/*
Convert a iteration number to an RGBA color.
Here, we just scale to gray with a maximum of imax.
Modify this function to make more interesting colors.
*/

int iteration_to_color( int i, int max )
{
	int gray = 255 * i / max;
	return MAKE_RGBA(gray, gray, gray, 0);
}




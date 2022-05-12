# Mandel_Brot (Fractal) 

We will generate images in the Mandel Brot which is a set of well known fractal structure. <br />
The above code increases the efficiency of the code by 500%. <br />
The images genreated are in .bmp <br />
You can zoom into any part and find swirls, spirals, snowflakes, and other fun structures, as long as you are inputting different computation <br />

Instructions: (To run the code)

* ./mandel -x -0.5 -y 0 -s 2 -o mandel1.bmp
* ./mandel -x -0.5 -y -0.5 -s 1 -o mandel2.bmp
* ./mandel -x -0.5 -y -0.5 -s 0.005 -o mandel3.bmp
* ./mandel -x -0.5 -y -0.5 -s 0.05 -o mandel3.bmp
* ./mandel -x -0.5 -y -0.5 -s 0.05 -o mandel4.bmp


If you run the code with no arguments the code generates a default image and writes it to .bmp. <br />

Explaination: (How the code functions)

For each pixel in the image, it starts with the x and y position, and then computes a recurrence relation until it exceeds a fixed value or runs for max iterations. <br />
Then, the pixel is assigned a color according to the number of iterations completed. <br />
An easy color scheme is to assign a gray value proportional to the number of iterations. <br />
The max value controls the amount of work done by the algorithm. <br />
If we increase max,then we can see much more detail in the set, but it may take much longer to compute. <br />
Generally speaking, you need to turn the max value higher as you zoom in. <br />
For example, here is the same area in the set computed with four different values of max: <br />

* ./mandel -x 0.286932 -y 0.014287 -s .0005 -m 50 -o mandel1.bmp
* ./mandel -x 0.286932 -y 0.014287 -s .0005 -m 100 -o mandel2.bmp
* ./mandel -x 0.286932 -y 0.014287 -s .0005 -m 500 -o mandel3.bmp
* ./mandel -x 0.286932 -y 0.014287 -s .0005 -m 1000 -o mandel4.bmp
* ./mandel -x 0.286932 -y 0.014287 -s .0005 -m 2000 -o mandel5.bmp

Modify: (Instructions accrodingly)

You can see all of the command line options with mandel -h, and use them to override the defaults. <br />
This program uses the escape time algorithm. <br />

Parallel Programming: 

It can take a long time to compute a Mandelbrot image. <br />
The larger the image, the closer it is to the origin, and the higher the max value, the longer it will take. <br />
Suppose that you want to create a movie of high resolution Mandelbrot images, and it is going to take a long time. <br />
Your job is to speed up the process by using multiple CPUs. <br />
You will do this in two different ways: using multiple processes and using multiple threads. <br />

Conclusion:

This code was built to increase the performance and by running multiple programs at once, we can take a different approach of making use of each individual process faster by using multiple threads. <br />
Modify mandel.c to use an arbitrary number of threads to compute the image. <br />
Each thread should compute a completely separate band of the image. <br />
For example, if you specify three threads and the image is 500 pixels high, then thread 0 should work on lines 0-165, thread 1 should work on lines 166-331, and thread 2 should work on lines 332-499. <br /> 
Add a new command line argument -n to allow the user to specify the number of threads. <br />
If -n is not given, assume a default of one thread. <br />

#include <mpi.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>
#include <glut.h>
#include <math.h>
#include <windows.h>
#include <vector>

int frame;
int Nori,N;
double *posorix,*posoriy;
double **posx=NULL,**posy=NULL;
int *mass;
FILE *fileori,*file;
char *filenameori= "input.txt";
char *filename= "result.txt";
int step=0;

char title[] = "N-body";  // Windowed mode's title
int windowWidth  = 1000;     // Windowed mode's width
int windowHeight = 1000;     // Windowed mode's height

struct Point
{
    double x, y;
    unsigned char r, g, b, a;
};
std::vector< Point > points;

double **alloc_2D_double()
{
		int i=0;
		double *data = (double *)malloc(N*frame*sizeof(double));
		double **array = (double **)malloc(N*sizeof(double*));
		for(i=0;i<N;i++)
		{
			array[i] = &(data[frame*i]);
		}
		return array;
}

/* Initialize OpenGL Graphics */
void initGL() {
 glClearColor(0.0, 0.0, 0.0, 1.0); // Set background (clear) color to black
}

void timer( int value )
{
	for( int i = 0; i < N;i++ ){
        points[i].x = posx[i][step];
		points[i].y = posy[i][step];
	}

	step++;
	if (step>frame) step=0;
	glutPostRedisplay();
	glutTimerFunc(5, timer, frame); // subsequent timer call at milliseconds

}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set to 2D orthographic projection with the specified clipping area
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();                 // Reset Projection matrix
	gluOrtho2D(-400.0, 800.0, -500.0, 700.0); // Set clipping area's left, right, bottom, top

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

    // draw
    glColor3ub( 255, 255, 255 );
    glEnableClientState( GL_VERTEX_ARRAY );
    glEnableClientState( GL_COLOR_ARRAY );
  	glPointSize( 1.5 );
    glVertexPointer( 2, GL_DOUBLE, sizeof(Point), &points[0].x );
    glColorPointer( 4, GL_UNSIGNED_BYTE, sizeof(Point), &points[0].r );
    glDrawArrays( GL_POINTS, 0, N );
    glDisableClientState( GL_VERTEX_ARRAY );
    glDisableClientState( GL_COLOR_ARRAY );

    glFlush();
    glutSwapBuffers();
}

void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
}

int main(int argc, char **argv)
{



	fileori = fopen(filenameori,"r");
	if(!fileori)	printf("File ORIGINAL Not Found!\n");
	else	printf("File ORIGINAL Found!\n");
	fscanf(fileori, "%d", &Nori);

	posorix = (double*)malloc(Nori * sizeof(double));
	posoriy = (double*)malloc(Nori * sizeof(double));
	mass = (int*)malloc(Nori * sizeof(int));

	while (!feof(fileori))
	for(int i=0; i<Nori; i++)
	  fscanf(fileori, "%lf %lf %d", &posorix[i],&posoriy[i],&mass[i]);
	fclose(fileori);

	file = fopen(filename,"r");
	if(!file)	printf("File Not Found!\n");
	else	printf("File Found!\n");
	fscanf(file, "%d %d", &N, &frame);
	printf("Iteration = %d\n",frame);

	posx=alloc_2D_double();
	posy=alloc_2D_double();

	while (!feof(file))
		for(int i=0; i<frame; i++)
			for(int j=0; j<N; j++)
				fscanf(file, "%lf %lf", &posx[j][i],&posy[j][i]);
	fclose(file);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);

	glutInitWindowSize(windowWidth, windowHeight);  // Initial window width and height
    glutCreateWindow(title);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

     // populate points
    for( size_t i = 0; i < N; ++i )
    {
        Point pt;
        pt.x = posorix[i];
		    pt.y = posoriy[i];
        pt.r = 0;
        pt.g = rand() % 255;
        pt.b = rand() % 255;
        pt.a = 255;
        points.push_back(pt);
    }
	initGL();
	glutTimerFunc(0, timer, 1);
    glutMainLoop();
    return 0;
}

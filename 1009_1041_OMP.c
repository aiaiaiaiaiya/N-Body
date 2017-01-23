#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <glut.h>
#include <math.h>
#define frame 1000
#define omp_num 5

int main(){
    FILE *fp,*fp1;
    double *positionx,*positiony,*velx,*vely,*mass,*newpositionx,*newpositiony;
    double time,G,forcex,forcey,diffx,diffy,gmm,ax,ay;
    int n,i,j,k;
    double starttime,endtime;
    G = 6.67428e-11;
    time = 250;

    fp = fopen("input.txt", "r");
    fscanf(fp,"%d",&n);
    fp1 = fopen("result.txt", "w");
    fprintf(fp1, "%d %d\n",n,frame);

    printf("%d\n",n);

    positionx = (double*)malloc(n*sizeof(double));
    positiony = (double*)malloc(n*sizeof(double));
    velx = (double*)malloc(n*sizeof(double));
    vely = (double*)malloc(n*sizeof(double));
    mass = (double*)malloc(n*sizeof(double));
    newpositionx = (double*)malloc(n*n*sizeof(double));
    newpositiony = (double*)malloc(n*n*sizeof(double));


    for (i=0; i<n; i++) {
        fscanf(fp, "%lf %lf %lf",&positionx[i],&positiony[i],&mass[i]);
    }

    starttime = omp_get_wtime();
    omp_set_num_threads(omp_num);
    # pragma omp parallel for
    for (i=0; i<n; i++) {
        velx[i] = 0;
        vely[i] = 0;
    }
    # pragma omp parallel for
    for (i=0; i<n*n; i++) {
        newpositionx[i] = 0;
        newpositiony[i] = 0;
    }

    for (k=0; k<frame; k++) {
        #pragma omp for private(j,diffx,diffy,gmm,forcex,forcey,ax,ay)
        for (i=0; i<n; i++) {
            forcex=0; forcey=0;
            for (j=0; j<n; j++) {
                if (i!=j) {
                    diffx = positionx[j]-positionx[i];
                    diffy = positiony[j]-positiony[i];
                    gmm = G*mass[i]*mass[j]/(diffx*diffx+diffy*diffy);
                    forcex += gmm*(diffx);
                    forcey += gmm*(diffy);
                }
            }
            ax = forcex/mass[i];
            positionx[i] += velx[i]*time+(forcex/mass[i]*time*time/2);
            velx[i] = velx[i] + ax*time;
            ay = forcey/mass[i];
            positiony[i] += vely[i]*time+(forcey/mass[i]*time*time/2);
            vely[i] = vely[i] + ay*time;

            newpositionx[k*n+i] = positionx[i];
            newpositiony[k*n+i] = positiony[i];
        }
    }

    endtime = omp_get_wtime();

    for (i=0; i<n*n; i++) {
        fprintf(fp1,"%lf %lf \n",newpositionx[i],newpositiony[i]);
    }

    for (i=0; i<n; i++) {
        printf("%lf %lf %.0lf\n",positionx[i],positiony[i],mass[i]);
    }
    printf("TIME %lf\n",endtime-starttime);

    fclose(fp);
    fclose(fp1);

	getchar();
}

//
//  main.cpp
//  Physics Simulation OpenGl
//
//  Created by Steven Jin on 9/8/15.
//  Copyright (c) 2015 Steven Jin. All rights reserved.
//



#include <GLUT/glut.h>       // for Macs
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#define window_size 800

double t=0;
//double dt=1e-5;
double dt=3e-2; // The change in time

double x[10000]={0}; //coordinates
double y[10000]={0};
double vx[10000]={0}; //velocity
double vy[10000]={0};
double GM=4*M_PI*M_PI;
int nextdraw=-1;
int walltime;
int last_walltime;
int steps=0;
int frames=0;
int check =0;
int power = 1;

int mousedown = 0; //boolean for whether mouse is down
double vx1[10000]; 
double vy1[10000];
static double mouseux;
static double mouseuy;
static double xu[10000];
static double yu[10000];
static double umag[10000];
double mx1;
double my1;
static int reverse = 1;
static int distance = 1;
static int viscous = 1;
static double dsquared = 1;
static double blackhole = -1;

void idle(void) // GLUT calls this functino in a continuous loop
{
    //printf("Mousex: %lf, Mousey: %lf\n",(mx1-350)/350,-1*(my1-350)/350);
    
    if(mousedown)
    { mouseux = (mx1-350.0)/350.0;
        mouseuy = -1*(my1-350.0)/350.0;
        for (int i = 0; i<10000; i++)
        {
      
        umag[i] = sqrt(pow((mouseux-x[i]),2)+pow((mouseuy-y[i]),2));
        
        vx1[i] = (mouseux-x[i])/umag[i];
        vy1[i] = (mouseuy-y[i])/umag[i];
            
            if (distance ==1){
            dsquared = pow(double (x[i])-mouseux,2)+pow(double (y[i])-mouseuy,2);
                dsquared = 10*dsquared;}
            
           if (reverse ==1)
    {
            vx[i] += power*vx1[i]/(1000*dsquared);
        vy[i] += power*vy1[i]/(1000*dsquared);}
        else
        {vx[i] -= power*vx1[i]/(1000*dsquared);
            vy[i] -= power*vy1[i]/(1000*dsquared);}
        

        }}
    for (int i = 0; i<10000; i++){
    if (viscous ==1)
    {
        vx[i] = vx[i]/1.005;
        vy[i] = vy[i]/1.005;
    }}
    
    steps++;   // keep track of how many steps we've done, for statistics (press F)
    static double xmid,ymid,vxmid,vymid,r; // declare these as static to avoid overhead from memory reallocation each time idle() is called
   //rk2 halfstep
    //xmid=x+vx*dt/2;
    //ymid=y+vy*dt/2;
  //r=sqrt(x*x+y*y);
   //vxmid=vx-x*GM/(r*r*r)*dt/2;
    //vymid=vy-y*GM/(r*r*r)*dt/2;
    
    //rk2 step
    for (int i = 0; i<10000; i++){
    if(y[i]<-1 && vy[i] <0) vy[i] = -vy[i];
    if(x[i]<-1 && vx[i] <0) vx[i] = -vx[i];
    if(x[i]>1 && vx[i] >0) vx[i] = -vx[i];
    if(y[i]>1 && vy[i] >0) vy[i] = -vy[i];
    
    x[i]+=vx[i]*dt;
        y[i]+=vy[i]*dt; }
  //  x+=vxmid*dt;
   // y+=vymid*dt;
   // r=sqrt(xmid*xmid+ymid*ymid);
    //vx+=-xmid*GM/(r*r*r)*dt;
    //vy+=-ymid*GM/(r*r*r)*dt;
    
    t+=dt;
    
    usleep(400); // slow this down by waiting one millisecond, so we don't do millions of timesteps per second
    walltime=glutGet(GLUT_ELAPSED_TIME); // what time is it now?
    if (walltime > nextdraw) glutPostRedisplay();  // nextdraw keeps track of when we think the screen needs updating
}

void circle(double x, double y, double r) // a function of mine to draw a crude circle
{
    int i;
    double angle;
    glBegin(GL_LINE_LOOP); // this does continuous lines at all the vertices; you can also do GL_LINES for single lines
    for (i=0;i<8;i++)
    {
        angle = i*2*M_PI/8;
        glVertex3f((x+cos(angle)*r), (y+sin(angle)*r),0); // I don't know how to use GL's perspective stuff, so the z-coordinate here is 0
    }
    glEnd(); // end the drawing command we started with glBegin
}


void disp(void) // this function will trigger whenever glutPostRedisplay() is called
{
    frames++;
    glClear(GL_COLOR_BUFFER_BIT); // reset the screen to something blank
    static double colorx;
    static double colory;
    static double energy;
    
    //glColor3f(1.0f,1.0f,1.0f);
   for (int i =0; i<10000; i++)
    {
        energy = pow(vx[i],2) + pow(vy[i],2);
      /*
     if(energy < .05)
        {glColor3f(1.0f,1.0f,1.0f);}
        else if(energy > 1)
        {glColor3f(1.0f,0.0f,0.0f);}
        else
        glColor3f(0.0f,0.0f,1.0f);
        */
        
        circle(x[i],y[i],0.002);}
    glutSwapBuffers(); // this is the equivalent of anim's "F" command: "write the frame we've completed to the screen and start working on a new one"
    nextdraw=glutGet(GLUT_ELAPSED_TIME)+15; // schedule the next screen refresh to be 15ms from now, which is about 60fps
}

void keyb(unsigned char key, int m, int n)    // this function will trigger whenever the user presses a key
{
    if (key == 'Q') {exit(0);} // quit
    if (key == 'F') {printf("Drawn %d frames and simulated %d RK2 steps in %d milliseconds\n",frames,steps,glutGet(GLUT_ELAPSED_TIME)-last_walltime);
        last_walltime=glutGet(GLUT_ELAPSED_TIME);
        frames=0;
        steps=0;
    }
    if (key == 'R'){
        for(int i =0; i<100; i++)
        {
            for(int j =0; j<100; j++)
            {
                x[(100*i+j)] = .02*i-.99;
                y[(100*i+j)] = j*.02-.99;
                vx[(100*i+j)] = 0;
                vy[(100*i+j)] = 0;
            }
        }

       }
    if (key == 'V'){
    for (int i =0; i<100; i++)
    {printf("Speedx %d: %lf \t Speedy %d: %lf\n",i,vx[i],i,vy[i]);}
    }
    if (key == 'r')//reverses direction of force
    {reverse = reverse*-1;}
    if (key == 'd')//decreases force based on distance squared
    {distance = distance*-1;}
    if (key == 'v')//places objects in viscous surrounding
    {viscous=viscous*-1;}
}

void mouse(int button, int state, int mx, int my)
{

    mx1 = mx;
    my1 = my;

    
    // Save the left button state
   if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN))
    {
        mousedown = 1;
  
    }
    if (state == GLUT_UP)
    {
        mousedown = 0;
    }
  
}

int main(int argc, char **argv)
{
    int boundaries = 0;
    
    for(int i =0; i<100; i++)
    {
        for(int j =0; j<100; j++)
        {
        x[(100*i+j)] = .02*i-.99;
        y[(100*i+j)] = j*.02-.99;
        }
    }

  /*
    printf("Do you want to set boundary conditions? 1)Yes 2)No: ");
    scanf(" %d",&boundaries);
    if (boundaries==1)
    {
         printf("Enter the x y coordinate for projectile(-1<x,y<1): ");
    scanf(" %lf %lf",&x,&y);
    
    printf("Enter the vx vy coordinate for projectile(-10<vx,vy<10): ");
    scanf(" %lf %lf",&vx,&vy);
    
    printf("Enter the power of your clicks: ");
        scanf(" %d",&power);}*/
    
    printf("Press 'R' to reset the Screen\nPress 'd' to turn off distance based force\nPress 'r' to reverse the direction of the force\nPress 'v' to turn off damping to all particles");
    
    //INITIALIZATION
    glutInit(&argc, argv);
    
    //set rgba and double buffering
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE |  GLUT_MULTISAMPLE);
    
    //set window size and position and title
    glutInitWindowSize(700,800); // modify if you have a low resolution screen
    glutInitWindowPosition(100,100);
    glutCreateWindow("Rotation");
    
    //SET CALLBACKS
    glutDisplayFunc(disp); // function to execute whenever glutPostRedisplay is called
    glutKeyboardFunc(keyb); // function to execute whenever a key is pressed
    glutMouseFunc(mouse);
    glutIdleFunc(idle); // functino to execute whenever we have nothing else to do
    
    //DO OPENGL INIT
    glEnable(GL_BLEND); // turn on alpha channel
    glEnable(GL_MULTISAMPLE); // turn on antialiasing
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); // some magic that has to do with alpha blending; this is the only thing I use here
    glClearColor(0.0, 0.0, 0.0, 1.0); // background color: RGB = 0, alpha = 1
    
    glMatrixMode(GL_PROJECTION); // I don't do OpenGL 3D perspective, but this controls it (somehow) -- I found this in a tutorial. Need to learn how this works in more detail!
    glLoadIdentity();
    
    walltime=last_walltime=glutGet(GLUT_ELAPSED_TIME); // update our clocks
    glutMainLoop(); // start the thing
}

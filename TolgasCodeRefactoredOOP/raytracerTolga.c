#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int nx,ny;
float dist,left,right,top,bottom;

typedef struct
{
	float x,y,z;
} vec3f;

vec3f e;
vec3f gaze;
vec3f u,v,w;

typedef struct
{
	vec3f center;
	float radius;
	vec3f color;
} sphere;

sphere spheres[2];
int numSpheres = 2;

vec3f light;


typedef struct
{
	int x,y,z;
} vec3i;

typedef struct
{
	vec3f o,d;
} ray;

vec3i **image;

vec3f multS(vec3f a,float s)
{
	vec3f result;
	result.x = a.x*s;
	result.y = a.y*s;
	result.z = a.z*s;
	return result;
}

vec3f add(vec3f a, vec3f b)
{
	vec3f result;
	result.x = a.x+b.x;
	result.y = a.y+b.y;
	result.z = a.z+b.z;
	return result;
}

ray generateRay(int i, int j)
{
	ray result;
	float su,sv;
	vec3f m,q,s;
	
	su = (i+0.5)*(right-left)/nx;
	sv = (j+0.5)*(top-bottom)/ny;
	
	m = add(e,multS(gaze,dist));
	
	q = add(m,add(multS(u,left),multS(v,top)));
	
	s = add(q,add(multS(u,su),multS(v,-sv)));
	
	result.o = e;
	result.d = add(s,multS(e,-1));
	
	return result;
}
void writePPM(vec3i **image,int nx,int ny)
{
	int i,j;
	FILE *fp;
	
	fp = fopen("output.ppm","w");
	
	fprintf(fp,"P3\n");
	fprintf(fp,"#output.ppm\n");
	fprintf(fp,"%d %d\n",nx,ny);
	fprintf(fp,"255\n");
	for (j=0;j<ny;j++)
	{
		for (i=0;i<nx;i++)
		{
			fprintf(fp,"%d %d %d\t",image[i][j].x,image[i][j].y,image[i][j].z);
		}
		fprintf(fp,"\n");
	}
	//system("convert output.ppm output.png");
	//system("rm output.ppm");
	
}

sphere readSphere()
{
	sphere s;
	scanf("%f %f %f %f %f %f %f",&(s.center.x),&(s.center.y),&(s.center.z),&(s.radius),&(s.color.x),&(s.color.y),&(s.color.z));
	return s;
}

float intersectSphere(ray r, sphere s)
{
	float A,B,C; //constants for the quadratic equation
	
	float delta;
	
	vec3f c;
	
	c = s.center;
	
	float t,t1,t2;
	
	C = (r.o.x-c.x)*(r.o.x-c.x)+(r.o.y-c.y)*(r.o.y-c.y)+(r.o.z-c.z)*(r.o.z-c.z)-s.radius*s.radius;

	B = 2*r.d.x*(r.o.x-c.x)+2*r.d.y*(r.o.y-c.y)+2*r.d.z*(r.o.z-c.z);
	
	A = r.d.x*r.d.x+r.d.y*r.d.y+r.d.z*r.d.z;
	
	delta = B*B-4*A*C;
	
	if (delta<0) return -1;
	else if (delta==0)
	{
		t = -B / (2*A);
	}
	else
	{
		delta = sqrt(delta);
		A = 2*A;
		t1 = (-B + delta) / A;
		t2 = (-B - delta) / A;
				
		if (t1<t2) t=t1; else t=t2;
	}
	
	return t;
}
float dot(vec3f a,vec3f b)
{
	return a.x*b.x+a.y*b.y+a.z*b.z;
}

vec3f normalize(vec3f a)
{
	return multS(a,1.0/sqrt(dot(a,a)));
}

vec3f computeColor(ray r)
{
	int i;
	vec3f c;
	float minT = 90000; // some large number
	float t;
	vec3f L,N;
	vec3f P;
	int minI;
	
	c.x=c.y=c.z=0;
	minI = -1;
	for (i=0;i<numSpheres;i++)
	{
		t = intersectSphere(r,spheres[i]);
		if (t<minT && t>=0)
		{
			c = spheres[i].color; // can be replaced with any material property
			minI = i;
			minT = t;
		}
	}

	if (minI!=-1)
	{
		P = add(r.o,multS(r.d,minT));
		L = add(light,multS(P,-1));
		N = add(P,multS(spheres[minI].center,-1));
		L = normalize(L);
		N = normalize(N);
		c = multS(c,dot(L,N));
	}
	return c;
}

int main()
{
	int i,j;
	
	
	light.x = 0;
	light.y = 50;
	light.z = 0;

	scanf("%d %d",&nx,&ny);

	scanf("%f %f %f %f %f",&dist,&left,&right,&bottom,&top);
	
	scanf("%f %f %f",&(e.x),&(e.y),&(e.z));
	scanf("%f %f %f",&(gaze.x),&(gaze.y),&(gaze.z));
	scanf("%f %f %f",&(v.x),&(v.y),&(v.z));
	
	spheres[0] = readSphere();
	spheres[1] = readSphere();

	w = multS(gaze,-1);
	u.x = 1.0;
	u.y = u.z = 0.0;
	
	image = (vec3i **)malloc(sizeof(vec3i*)*nx);
	for (i=0;i<nx;i++)
		image[i] = (vec3i *)malloc(sizeof(vec3i)*ny);
	
	for (j=0;j<ny;j++)
		for (i=0;i<nx;i++)
			image[i][j].x = image[i][j].y = image[i][j].z = 0;

	/* main raytracing loop */
	for (j=0;j<ny;j++)
		for (i=0;i<nx;i++)
		{
			ray myray = generateRay(i,j);
			vec3f pixel;
			vec3f rayColor;
			pixel = add(myray.o,myray.d);
			rayColor = computeColor(myray);
			image[i][j].x = (int)(rayColor.x*255+0.5);
			image[i][j].y = (int)(rayColor.y*255+0.5);
			image[i][j].z = (int)(rayColor.z*255+0.5);
			//printf("%.4f %.4f %.4f\n",myray.d.x,myray.d.y,myray.d.z);
			//printf("%.4f %.4f %.4f\n",pixel.x,pixel.y,pixel.z);
		}
		
	writePPM(image,nx,ny);
}
//===========================//
// IMAGE PROCESSING IN C

// S. BHAVYESH
// 191210045

// CSB 102 DATA STRUCTURES
//===========================//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

typedef struct pixel	// pixel structure
{
	// ordering is done this way as pixel data is
	//stored in RLE of BGR,BGR,BGR....

	unsigned char blue;
	unsigned char green;
	unsigned char red;

}pixel;


typedef struct image	// image data type
{
	char path[50];				// image path address in the system
	unsigned char header[54];	// image header
	pixel** buffer;				// pixel data
	int height;
	int width;
	int bitdepth;

}image;


int roundoff(int n)	// auxiliary function to stay within the limits of 0-255 for pixel components
{
	if(n>=255) return 255;
	else if(n<=0) return 0;
	else return n;
}


void swap(int *a,int *b)	// to swap elements
{
	int temp=*a;
	*a=*b;
	*b=temp;
}

//------------BASIC FUNCTIONS-------------//


void readImage(FILE* f,image* img)	// to read images from system
{
	f=fopen(img->path,"rb");

	if(!f) printf("FILE DNE");  //Checking the existence of image file
	else
	{
		fread(img->header,sizeof(unsigned char),54,f);

		img->width=*(int*)&(img->header[18]);
		img->height=*(int*)&(img->header[22]);
		img->bitdepth=*(int*)&(img->header[28]);

		img->buffer=malloc(img->height*sizeof(pixel*));  // Reading the header

		for(int i=0;i<img->height;i++)
			img->buffer[i]=malloc(img->width*sizeof(pixel));

		for(int i=0;i<img->height;i++) fread(img->buffer[i],sizeof(pixel),img->width,f);
		// reading pixel data into buffer
	}

	fclose(f);
}


void writeImage(FILE* f,image* img)	// to write images into system
{
	f=fopen(img->path,"wb");

	fwrite(img->header,sizeof(unsigned char),54,f);

	for(int i=0;i<img->height;i++)
	fwrite(img->buffer[i],sizeof(pixel),img->width,f);

	fclose(f);
}


void getInfo(image* img)	// getting necessary details from image
{
	printf("\n image path: %s",img->path);
	printf("\n image height: %d",img->height);
	printf("\n image width: %d",img->width);
	printf("\n image bit depth: %d",img->bitdepth);
}


void getPixelval(image* img,int r,int c)	// pixel component values
{
	printf("\n\n red value:%d\n",img->buffer[r][c].red);
	printf(" green value:%d\n",img->buffer[r][c].green);
	printf(" blue value:%d\n",img->buffer[r][c].blue);
}


void setPixelval(image* img,int r,int c)	// changing pixel value
{
	image img_temp=*img;

	int temp;

	printf("\n\n set red value:");
	scanf("%d",&temp);
	img_temp.buffer[r][c].red=temp;

	printf("\n\n set green value:");
	scanf("%d",&temp);
	img_temp.buffer[r][c].green=temp;

	printf("\n\n set red value:");
	scanf("%d",&temp);
	img_temp.buffer[r][c].blue=temp;

	FILE* f=NULL;
	strcpy(img_temp.path,"C:/Images/pixel_change.bmp");
	writeImage(f,&img_temp);
}


void meanGray(image* img)	// for average gray value or brightness of image
{
	float sum=0;
	int i=0;

	for(i=0;i<img->height;i++)
	for(int j=0;j<img->width;j++)
		sum+=0.11*img->buffer[i][j].blue + 0.59*img->buffer[i][j].green + 0.29*img->buffer[i][j].red;
	//Using the formula for luminance /gray value

	sum/=(img->height*img->width);

	printf("\n%f is the mean gray value\n",sum);
}


void rgb_to_gray(image* img)	//to convert colored image to gray
{
	image *img_temp=malloc(sizeof(image));
	*img_temp=*img;

	for(int i=0;i<img_temp->height;i++)
	{
		for(int j=0;j<img_temp->width;j++)
		{
			int y=0.11*img->buffer[i][j].blue + 0.59*img->buffer[i][j].green + 0.29*img->buffer[i][j].red;
			//Using the formula for luminance

			img_temp->buffer[i][j].blue=img_temp->buffer[i][j].green=img_temp->buffer[i][j].red=y;
		}
	}

	// writing into a file

	FILE* f=NULL;
	strcpy(img_temp->path,"C:/Images/rgb_to_gray.bmp");
	writeImage(f,img_temp);
	free(img_temp);
}


void gray_to_binary(image* img)	// converting gray image to binary
{
	image *img_temp=malloc(sizeof(image));
	*img_temp=*img;

	for(int i=0;i<img_temp->height;i++)
	{
		for(int j=0;j<img_temp->width;j++)
		{
			int y=0.11*img_temp->buffer[i][j].blue+0.59*img_temp->buffer[i][j].green+0.29*img_temp->buffer[i][j].red;
			//Using the formula for luminance

			if(y>=128) img_temp->buffer[i][j].blue=img_temp->buffer[i][j].green=img_temp->buffer[i][j].red=255;
			else img_temp->buffer[i][j].blue=img_temp->buffer[i][j].green=img_temp->buffer[i][j].red=0;
			//Assigning value to each component of pixel
		}
	}

	//writing into a file

	FILE* f=NULL;
	strcpy(img_temp->path,"C:/Images/gray_to_binary.bmp");
	writeImage(f,img_temp);
}


void reflectImage(image* img,int flag)	// obtain reflection of images
{
	image* img_temp=malloc(sizeof(image));
	*img_temp=*img;

	if(flag)	// Vertical reflection
	{
		for(int i=0;i<img_temp->height/2;i++)	//Swapping pixels about the horizontal axis
		{
			for(int j=0;j<img_temp->width;j++)
			{
				pixel temp=img_temp->buffer[i][j];
				img_temp->buffer[i][j]=img_temp->buffer[img->height-i-1][j];
				img_temp->buffer[img->height-i-1][j]=temp;
			}
		}
	}

	else	//Horizontal reflection
	{
		for(int i=0;i<img->height;i++)
				{
					for(int j=0;j<img->width/2;j++)	//Swapping pixels about the vertical axis
					{
						pixel temp=img_temp->buffer[i][j];
						img_temp->buffer[i][j]=img_temp->buffer[i][img->width-j-1];
						img_temp->buffer[i][img->width-j-1]=temp;
					}
			}
	}

		// writing into a file

		FILE* f=NULL;
		strcpy(img_temp->path,"C:/Images/reflect.bmp");
		writeImage(f,img_temp);
		free(img_temp);
}


void cropImage(image* img,int down,int up,int left,int right)	// obtain a sub-image
{

		image* img_temp=malloc(sizeof(image));
		*img_temp=*img;
		int width=right-left;
		int height=up-down;

		//allocation of buffer

		pixel**	temp=malloc(height*sizeof(pixel*));
		for(int i=0;i<height;i++) temp[i]=malloc(width*sizeof(pixel));

		for(int i=down;i<up;i++)
		for(int j=left;j<right;j++) temp[i-down][j-left]=img->buffer[i][j];
		//Pixels within the desired space are read into buffer

		*(int*)&img_temp->header[22]=img_temp->height=height;
		*(int*)&img_temp->header[18]=img_temp->width=width;
		//Header data is modified accordingly

		printf("width %d height %d",*(int*)&img_temp->header[18],*(int*)&img_temp->header[22]);
		printf("width %d height %d",*(int*)&img->header[18],*(int*)&img->header[22]);

		//Buffer data is written into file
		FILE* fo=fopen("C:/Images/crop.bmp","wb+");
		fwrite(img_temp->header,sizeof(unsigned char),54,fo);
		for(int i=0;i<height;i++) fwrite(temp[i],sizeof(pixel),width,fo);

		fclose(fo);
		for(int i=0;i<height;i++) free(temp[i]);
		free(temp);
		free(img_temp);
}


//------------BASIC FUNCTIONS-------------//



//------------EXTRA CREDITS---------------//


void Enhance(image* img,int median,int edge,int con,int bright)	// enhance image quality
{
	image* img_temp=malloc(sizeof(image));
	*img_temp=*img;

	// Median Filtering begins

	if(median)
	{

		for(int i=1;i<img->height-1;i++) // ignoring edge cases
		{
			for(int j=1;j<img->width-1;j++) // ignoring edge cases
			{
				int red[9],blue[9],green[9]; // array of RGB components in the 3x3 matrix
				for(int k=-1;k<=1;k++)
					for(int l=-1;l<=1;l++) // looping around neighbors
					{
						red[3*(k+1)+l+1]=img_temp->buffer[i+k][j+l].red;
						green[3*(k+1)+l+1]=img_temp->buffer[i+k][j+l].green;
						blue[3*(k+1)+l+1]=img_temp->buffer[i+k][j+l].blue;
					}

				for(int i=0;i<9;i++)
				{
					for(int j=0;j<8;j++)
					{
						//sorting the arrays

						if(red[j]>red[j+1]) swap(red+j,red+j+1);
						if(green[j]>green[j+1]) swap(green+j,green+j+1);
						if(blue[j]>blue[j+1]) swap(blue+j,blue+j+1);
					}
				}

				img_temp->buffer[i][j].red=red[4];  // assigning median values to the concerned pixel
				img_temp->buffer[i][j].green=green[4];
				img_temp->buffer[i][j].blue=blue[4];
			}
		}
	}


	// Median filtering ends


	// Image sharpening with Sobel edge detection begins

	if(edge)
	{
		// declaring a temporary buffer
		pixel** temp_buffer=malloc(img->height*sizeof(pixel*));
		for(int i=0;i<img->height;i++) temp_buffer[i]=malloc(img->width*sizeof(pixel));


		// declaring a buffer of horizontal gradient
		pixel** temp_x=malloc(img->height*sizeof(pixel*));
		for(int i=0;i<img->height;i++) temp_x[i]=malloc(img->width*sizeof(pixel));

		// declaring a buffer of vertical gradient
		pixel** temp_y=malloc(img->height*sizeof(pixel*));
		for(int i=0;i<img->height;i++) temp_y[i]=malloc(img->width*sizeof(pixel));

		//grayscale conversion
		for(int i=0;i<img->height;i++)
		{
			for(int j=0;j<img->width;j++)
			{
				int y=0.11*img->buffer[i][j].blue + 0.59*img->buffer[i][j].green + 0.29*img->buffer[i][j].red;
				temp_buffer[i][j].blue=temp_buffer[i][j].green=temp_buffer[i][j].red=y;
			}
		}


		// kernels for horizontal and vertical gradients
		float kernel_x[3][3]={{-0.25,0,0.25},{-0.5,0,0.5},{-0.25,0,0.25}};
		float kernel_y[3][3]={{0.25,0.5,0.25},{0,0,0},{-0.25,-0.50,-0.25}};

		// computing horizontal and vertical gradients of each pixel
		for(int i=1;i<img->height-1;i++)
		{
			for(int j=1;j<img->width-1;j++)
			{
				float sum1=0,sum2=0;
				for(int k=-1;k<=1;k++)
				{
					for(int l=-1;l<=1;l++)
					{
						sum1+=temp_buffer[i+k][j+l].red*kernel_x[k+1][l+1];
						//since image is 24 bit grayscale all components RGB will have same value
						sum2+=temp_buffer[i+k][j+l].red*kernel_y[k+1][l+1];
					}
				}
				temp_x[i][j].red=temp_x[i][j].green=temp_x[i][j].blue=roundoff((int)sum1);
				temp_y[i][j].red=temp_y[i][j].green=temp_y[i][j].blue=roundoff((int)sum2);
			}
		}


		// effective gradient computation
		for(int i=0;i<img->height;i++)
		{
			for(int j=0;j<img->width;j++)
			{
				int temp=(int)(sqrt(pow(temp_x[i][j].red,2)+pow(temp_y[i][j].red,2)));
				temp_buffer[i][j].green=temp_buffer[i][j].blue=temp_buffer[i][j].red=temp;
			}
		}

		// saving a copy of Sobel edge detection
		FILE* fo=fopen("C:/Images/Sobel.bmp","wb+");
		fwrite(img->header,sizeof(unsigned char),54,fo);
		for(int i=0;i<img->height;i++) fwrite(temp_buffer[i],sizeof(pixel),img->width,fo);
		fclose(fo);

		// adding the colored version and Sobel version
		for(int i=0;i<img->height;i++)
		{
			for(int j=0;j<img->width;j++)
			{
				img_temp->buffer[i][j].red=roundoff(temp_buffer[i][j].red+img->buffer[i][j].red);
				img_temp->buffer[i][j].green=roundoff(temp_buffer[i][j].green+img->buffer[i][j].green);
				img_temp->buffer[i][j].blue=roundoff(temp_buffer[i][j].blue+img->buffer[i][j].blue);
			}
		}
		for(int i=0;i<img->height;i++) free(temp_buffer[i]);
		free(temp_buffer);

		for(int i=0;i<img->height;i++) free(temp_x[i]);
		free(temp_x);

		for(int i=0;i<img->height;i++) free(temp_y[i]);
		free(temp_y);

	}




	// Image sharpening ends



	// Increasing contrast begins

	float contra=(259*(255+con))/(255*(259-con));
	// contrast adjustment of 'con' units.


	for(int i=0;i<img->height;i++)
	{
		for(int j=0;j<img->width;j++)
		{
			// new pixel value.
			img_temp->buffer[i][j].red=roundoff((int)(contra*(img_temp->buffer[i][j].red-128)+128));
			img_temp->buffer[i][j].green=roundoff((int)(contra*(img_temp->buffer[i][j].green-128)+128));
			img_temp->buffer[i][j].blue=roundoff((int)(contra*(img_temp->buffer[i][j].blue-128)+128));
		}
	}

	// Increasing contrast ends


	// Brightness increase begins

	for(int i=0;i<img->height;i++)
	{
		for(int j=0;j<img->width;j++)
		{
			// simple addition by 'bright' to each component

			img_temp->buffer[i][j].red=roundoff(img_temp->buffer[i][j].red+bright);
			img_temp->buffer[i][j].green=roundoff(img_temp->buffer[i][j].green+bright);
			img_temp->buffer[i][j].blue=roundoff(img_temp->buffer[i][j].blue+bright);
		}
	}

	// Brightness increase ends

	// writing into a file

	FILE* f=NULL;
	strcpy(img_temp->path,"C:/Images/enhance.bmp");
	writeImage(f,img_temp);
	free(img_temp);

}


void Shrink(image* img,float s)		// shrink an image
{
	image* img_temp=malloc(sizeof(image));
	*img_temp=*img;

	// modified values of height and width for new image
	int width=(int)(img_temp->width/s);
	int height=(int)(img_temp->height/s);

	// Declaring a 1D buffer
	pixel* temp=malloc(height*width*sizeof(pixel));

	// implementing the basic idea
	for(int i=0;i<height;i++)
		for(int j=0;j<width;j++)
			temp[i*width+j]=img->buffer[(int)(i*s)][(int)(j*s)];

	// making changes in header of image
	*(int*)&img_temp->header[22]=height;
	*(int*)&img_temp->header[18]=width;

	//printing height, width for confirmation
	printf("width %d height %d",*(int*)&img_temp->header[18],*(int*)&img_temp->header[22]);

	// writing into a file
	FILE* fo=fopen("C:/Images/shrink.bmp","wb");
	fwrite(img_temp->header,sizeof(unsigned char),54,fo);
	fwrite(temp,sizeof(pixel),height*width,fo);

	fclose(fo);
	free(img_temp);
	free(temp);
}


void Enlarge(image* img,float s)	// enlarge an image
{
	image* img_temp=malloc(sizeof(image));
	*img_temp=*img;

	// modified values of height and width for new image

	int width=(int)(img_temp->width*s);
	int height=(int)(img_temp->height*s);

	// Declaring a 1D buffer
	pixel* temp=malloc(height*width*sizeof(pixel));

	// implementing the basic idea
	for(int i=0;i<height;i++)
		for(int j=0;j<width;j++)
			temp[i*width+j]=img->buffer[(int)(i/s)][(int)(j/s)];

	// making changes in header of image
	*(int*)&img_temp->header[22]=height;
	*(int*)&img_temp->header[18]=width;

	//printing height, width for confirmation
	printf("width %d height %d",*(int*)&img_temp->header[18],*(int*)&img_temp->header[22]);

	// writing into a file
	FILE* fo=fopen("C:/Images/enlarge.bmp","wb+");
	fwrite(img_temp->header,sizeof(unsigned char),54,fo);
	fwrite(temp,sizeof(pixel),height*width,fo);

	fclose(fo);
	free(img_temp);
	free(temp);
}



void Translate(image* img,int t,int dirn)	// translate an image in any of the 4 directions
{
	image* img_temp=malloc(sizeof(image));
	*img_temp=*img;

	switch(dirn)
	{
	case 1:	// towards right
	{
		// shifting pixels to the right
		for(int i=0;i<img_temp->height;i++)
			for(int j=img_temp->width-1;j>=t;j--)
				img_temp->buffer[i][j]=img_temp->buffer[i][j-t];

		// colouring the required area black
		for(int i=0;i<img->height;i++)
			for(int j=0;j<t;j++)
				img_temp->buffer[i][j].red=img_temp->buffer[i][j].blue=img_temp->buffer[i][j].green=0;
	}break;

	case 2:	// towards left
	{
		// shifting pixels to the left
		for(int i=0;i<img_temp->height;i++)
			for(int j=t;j<img->width;j++)
				img_temp->buffer[i][j-t]=img_temp->buffer[i][j];

		// colouring the required area black
		for(int i=0;i<img->height;i++)
			for(int j=img->width-t;j<img->width;j++)
				img_temp->buffer[i][j].red=img_temp->buffer[i][j].blue=img_temp->buffer[i][j].green=0;
	}break;

	case 3:	// upwards
	{
		// shifting pixels upwards
		for(int j=0;j<img_temp->width;j++)
			for(int i=img->height-1;i>=t;i--)
				img_temp->buffer[i][j]=img_temp->buffer[i-t][j];

		// colouring the required area black
		for(int j=0;j<img_temp->width;j++)
			for(int i=0;i<t;i++)
				img_temp->buffer[i][j].red=img_temp->buffer[i][j].blue=img_temp->buffer[i][j].green=0;
	}break;

	case 4:	// downwards
	{
		// shifting pixels downwards
		for(int j=0;j<img_temp->width;j++)
			for(int i=t;i<img->height;i++)
				img_temp->buffer[i-t][j]=img_temp->buffer[i][j];

		// colouring required area black
		for(int j=0;j<img_temp->width;j++)
			for(int i=img->height-1;i>=img->height-t;i--)
				img_temp->buffer[i][j].red=img_temp->buffer[i][j].blue=img_temp->buffer[i][j].green=0;
	}break;

	default: break;
	}

	// writing into a file
	FILE* f=NULL;
	strcpy(img_temp->path,"C:/Images/translate.bmp");
	writeImage(f,img_temp);
	free(img_temp);
}



void Rotate(image* img,int theta)	// rotating image by any arbitrary angle
{
	image* img_temp=malloc(sizeof(image));
	*img_temp=*img;

	// converting from degrees to radians
	float rad=theta*(float)3.141592/180;

	// defining centre of source image
	int x_center=img_temp->width/2;
	int y_center=img_temp->height/2;

	printf("width %d height %d",x_center,y_center);

	// computing length of square buffer
	int len=sqrt(pow(img->width,2)+pow(img->height,2));
	if(len%4!=0) len+=4-len%4;		// round off to nearest multiple of 4

	printf("len %d",len);

	// buffer declaration
	pixel** temp=malloc(len*sizeof(pixel*));
	for(int i=0;i<len;i++) temp[i]=malloc(len*sizeof(pixel));

	// assigning every pixel the colour white in temp buffer
	for(int i=0;i<len;i++)
		for(int j=0;j<len;j++)
			temp[i][j].red=temp[i][j].green=temp[i][j].blue=255;

	for(int i=0;i<img->height;i++)
	{
		for(int j=0;j<img->width;j++)
		{
			// assigning new coordinates to the pixel
			int new_vert=(int)(cos(rad)*(i-y_center)-sin(rad)*(j-x_center)+y_center+(len-img->height)/2);
			int new_hrzn=(int)(sin(rad)*(i-y_center)+cos(rad)*(j-x_center)+x_center+(len-img->width)/2);

			// checking if linear interpolation will result in going
			//‘out-of bounds’ of the buffer.
			if(new_vert - 1 >=0 && new_vert + 1 < len && new_hrzn - 1 >=0 && new_hrzn + 1 <len)

				// if okay, then linear interpolation is done to cover up stray white points
				for(int k=-1;k<=1;k++) for(int l=-1;l<=1;l++) temp[new_vert+k][new_hrzn+l]=img_temp->buffer[i][j];

		}
	}

	// updating header values
	*(int*)&img_temp->header[18]=len;
	*(int*)&img_temp->header[22]=len;


	// writing into a file
	FILE* f=fopen("C:/Images/rot.bmp","wb");
	fwrite(img_temp->header,sizeof(unsigned char),54,f);
	for(int i=0;i<len;i++) fwrite(temp[i],sizeof(pixel),len,f);
	fclose(f);

	for(int i=0;i<len;i++) free(temp[i]);
	free(temp);
}



void Morph(image* img,image* img2,float f)	// weighted sum of two images
{
	image *img3=malloc(sizeof(image));
	*img3=*img;

	// checking if image characteristics are same
	if(img->height!=img2->height || img->width!=img2->width || img->bitdepth!=img2->bitdepth) {}
	else
	{
		for(int i=0;i<img3->height;i++)
		for(int j=0;j<img3->width;j++)
		{
			// weighted addition of red components
			img3->buffer[i][j].red=(int)((float)img->buffer[i][j].red*(f)+(float)img2->buffer[i][j].red*(1-f));

			// weighted addition of green components
			img3->buffer[i][j].green=(int)((float)img->buffer[i][j].green*(f)+(float)img2->buffer[i][j].green*(1-f));

			// weighted addition of blue components
			img3->buffer[i][j].blue=(int)((float)img->buffer[i][j].blue*(f)+(float)img2->buffer[i][j].blue*(1-f));
		}
	}

	// writing into a file

	FILE* fo=NULL;
	strcpy(img3->path,"C:/Images/morph.bmp");
	writeImage(fo,img3);
	free(img3);
}



void Compare(image* img,image* img2)	// comparison and absolute difference of images
{
	image* img3=malloc(sizeof(image));
	*img3=*img;

	// checking if image characteristics are same
	if(img->height!=img2->height || img->width!=img2->width || img->bitdepth!=img2->bitdepth) {}
	else
	{
		float sum=0;
		for(int i=0;i<img->height;i++)
		{
			for(int j=0;j<img->width;j++)
			{
				// absolute difference of red components
				img3->buffer[i][j].red=abs(img->buffer[i][j].red-img2->buffer[i][j].red);
				sum+=img3->buffer[i][j].red;

				// absolute difference of green components
				img3->buffer[i][j].green=abs(img->buffer[i][j].green-img2->buffer[i][j].green);
				sum+=img3->buffer[i][j].green;

				// absolute difference of blue components
				img3->buffer[i][j].blue=abs(img->buffer[i][j].blue-img2->buffer[i][j].blue);
				sum+=img3->buffer[i][j].blue;
			}
		}

		// calculation and display of percentage difference

		sum/=img->height*img->width*3*255;
		sum*=100;
		printf("%lf percent difference",sum);

		// writing into file

		FILE* f=NULL;
		strcpy(img3->path,"C:/Images/compare.bmp");
		writeImage(f,img3);
		free(img3);
	}
}



void Negative(image* img)	// negative of the image
{
	image *img_temp=malloc(sizeof(image));
	*img_temp=*img;

	for(int i=0;i<img_temp->height;i++)
	{
		for(int j=0;j<img_temp->width;j++)
		{
			// computing the negative of each component

			img_temp->buffer[i][j].red=255-img_temp->buffer[i][j].red;
			img_temp->buffer[i][j].green=255-img_temp->buffer[i][j].green;
			img_temp->buffer[i][j].blue=255-img_temp->buffer[i][j].blue;
		}
	}

			// writing into a file

			FILE* fo=fopen("C:/Images/negative.bmp","wb+");
			fwrite(img_temp->header,sizeof(unsigned char),54,fo);
			for(int i=0;i<img_temp->height;i++) fwrite(img_temp->buffer[i],sizeof(pixel),img_temp->width,fo);

			fclose(fo);
			free(img_temp);
}


//------------EXTRA CREDITS---------------//



int main()
{
	FILE* f=NULL; image img; char name[50];

	printf("enter image path:");
	scanf("%s",name);
	strcpy(img.path,name);

	readImage(f,&img);

	int option;
	do
	{
		int choice;

		printf("\n========================\n");

		printf("1. Create a copy\n");
		printf("2. Obtain information of image\n");
		printf("3. Get RGB values of a particular pixel\n");
		printf("4. Change RGB values of a particular pixel\n");
		printf("5. Gray value of image\n");
		printf("6. Convert to gray-scale\n");
		printf("7. Convert to binary\n");
		printf("8. Reflect the image\n");
		printf("9. Crop the image\n");
		printf("\n\n\n");
		printf("10. Enhance the image\n");
		printf("11. Shrink the image\n");
		printf("12. Enlarge the image\n");
		printf("13. Translate the image\n");
		printf("14. Rotate image\n");
		printf("15. Morph with another image\n");
		printf("16. Compare with another image\n");
		printf("17. Negative of the image\n");


		printf("\n========================\n");

		scanf("\n%d",&choice);
		if(choice < 1 || choice > 17) printf("\nInvalid Input!!!\n");
		else
		{
			switch(choice)
			{
			case 1:
			{
				image img2=img; FILE* f=NULL; char name1[50];
				printf("\nEnter copy destination:\n");
				scanf("%s",name1);
				strcpy(img2.path,name1);
				writeImage(f,&img2);
			}break;

			case 2: getInfo(&img);
					break;

			case 3:
			{
				int r,c;
				printf("\nEnter x,y coordinates of pixel >= 0:");
				scanf("%d%d",&r,&c);
				if(r>=img.height || c>=img.width) printf("Invalid coordinates");
				else getPixelval(&img,c,r);
			}break;

			case 4:
			{
				int r,c;
				printf("\nEnter x,y coordinates of pixel > 0:");
				scanf("%d%d",&r,&c);
				if(r>=img.height || c>=img.width) printf("Invalid coordinates");
				else setPixelval(&img,c,r);
			}break;

			case 5: meanGray(&img);
					break;

			case 6: rgb_to_gray(&img);
					break;

			case 7: gray_to_binary(&img);
					break;

			case 8:
			{
				int flag;
				printf("\nEnter 1 for vertical reflection\n");
				printf("\nEnter 0 for horizontal reflection\n");

				scanf("%d",&flag);
				if(flag!=1 && flag!=0) printf("Invalid choice");
				else reflectImage(&img,flag);
			}break;

			case 9:
			{
				int up,down,left,right;
				printf("\n enter values of down, up, left, right for region to crop.\n");
				printf("\n kindly ensure that down<=up,left<=right ");
				printf("\n and that (up-down) and (right-left) both are divisible by 4:");
				scanf("%d%d%d%d",&down,&up,&left,&right);
				cropImage(&img,down,up,left,right);
			}break;

			case 10:
			{
				int median,edge,cont,bright;
				printf("\n Do you want median filtering? 1 for yes 0 for no : ");
				scanf("%d",&median);
				printf("\n Do you want edge sharpening? 1 for yes 0 for no : ");
				scanf("%d",&edge);
				printf("\n Enter amount of contrast (0 for no contrast) : ");
				scanf("%d",&cont);
				printf("\n Enter amount of brightness (0 for none): ");
				scanf("%d",&bright);
				Enhance(&img,median,edge,cont,bright);
			}break;

			case 11:
			{
				float f;
				printf("\nEnter the shrinking factor >=1 :");
				scanf("%f",&f);
				if(f<1) printf("Invalid factor");
				else Shrink(&img,f);
			}break;

			case 12:
			{
				float f;
				printf("\nEnter the enlarging factor >= 1:");
				scanf("%f",&f);
				if(f<1) printf("Invalid factor");
				else Enlarge(&img,f);
			}break;

			case 13:
			{
				int x,dirn;
				printf("\nEnter the translation amount in pixels > 0 and < height or width (depending on direction) :");
				scanf("%d",&x);
				printf("\nEnter the direction (1 for right, 2 for left, 3 for up, 4 for down):");
				scanf("%d",&dirn);
				Translate(&img,x,dirn);
			}break;

			case 14:
			{

					int theta;
					printf("\nEnter the angle of rotation:");
					scanf("%d",&theta);
					Rotate(&img,theta);
			}break;

			case 15:
			{
				char name1[50]; float f;
				printf("\nEnter path of second image with same height width and bitdepth:");
				scanf("%s",name1);
				printf("\nEnter merge factor between 0 and 1:");
				scanf("%f",&f);
				image img2=img; FILE* fo=NULL;
				strcpy(img2.path,name1);
				readImage(fo,&img2);
				Morph(&img,&img2,f);
			}break;

			case 16:
			{
				char name1[50];
				printf("\nEnter path of second image with same height width and bitdepth:");
				scanf("%s",name1);
				image img2=img; FILE* f=NULL;
				strcpy(img2.path,name1);
				readImage(f,&img2);
				Compare(&img,&img2);
			}break;

			case 17: Negative(&img);
					break;

			default : break;
		}
		}

		printf("\nContinue?  1 for yes, 0 for no\n");
		scanf("%d",&option);

	}while(option==1);

	printf("Exiting...");
	return 0;
}

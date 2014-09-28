//*****************************************************************************
//
// Image.cpp : Defines the class operations on images
//
// Author - Parag Havaldar
// Code used by students as starter code to display and modify images
//
//*****************************************************************************
#include "Image.h"
#include "math.h"
// Constructor and Desctructors
MyImage::MyImage() 
{
	Data = NULL;
	Width = -1;
	Height = -1;
	ImagePath[0] = 0;
}

MyImage::~MyImage()
{
	if ( Data )
		delete Data;
}


// Copy constructor
MyImage::MyImage( MyImage *otherImage)
{
	Height = otherImage->Height;
	Width  = otherImage->Width;
	Data   = new unsigned char[Width*Height*3];
	strcpy(otherImage->ImagePath, ImagePath );

	for ( int i=0; i<(Height*Width*3); i++ )
	{
		Data[i]	= otherImage->Data[i];
	}


}



// = operator overload
MyImage & MyImage::operator= (const MyImage &otherImage)
{
	Height = otherImage.Height;
	Width  = otherImage.Width;
	Data   = new unsigned char[Width*Height*3];
	strcpy( (char *)otherImage.ImagePath, ImagePath );

	for ( int i=0; i<(Height*Width*3); i++ )
	{
		Data[i]	= otherImage.Data[i];
	}
	
	return *this;

}


// MyImage::ReadImage
// Function to read the image given a path
bool MyImage::ReadImage(int frame_number)
{

	// Verify ImagePath
	if (ImagePath[0] == 0 || Width < 0 || Height < 0 )
	{
		fprintf(stderr, "Image or Image properties not defined");
		fprintf(stderr, "Usage is `Image.exe Imagefile w h`");
		return false;
	}
	
	// Create a valid output file pointer
	FILE *IN_FILE;
	IN_FILE = fopen(ImagePath, "rb");
	if ( IN_FILE == NULL ) 
	{
		fprintf(stderr, "Error Opening File for Reading");
		return false;
	}

	fseek(IN_FILE, frame_number * 352 * 288 * 3, SEEK_SET);

	// Create and populate RGB buffers
	int i;
	char *Rbuf = new char[Height*Width]; 
	char *Gbuf = new char[Height*Width]; 
	char *Bbuf = new char[Height*Width]; 

	for (i = 0; i < Width*Height; i ++)
	{
		Rbuf[i] = fgetc(IN_FILE);
	}
	for (i = 0; i < Width*Height; i ++)
	{
		Gbuf[i] = fgetc(IN_FILE);
	}
	for (i = 0; i < Width*Height; i ++)
	{
		Bbuf[i] = fgetc(IN_FILE);
	}
	
	// Allocate Data structure and copy
	Data = new unsigned char[Width*Height*3];
	for (i = 0; i < Height*Width; i++)
	{
		Data[3*i]	= Bbuf[i];
		Data[3*i+1]	= Gbuf[i];
		Data[3*i+2]	= Rbuf[i];
	}

	// Clean up and return
	delete Rbuf;
	delete Gbuf;
	delete Bbuf;
	fclose(IN_FILE);

	return true;

}



// MyImage functions defined here
bool MyImage::WriteImage(int frame_number)
{
	// Verify ImagePath
	// Verify ImagePath
	if (ImagePath[0] == 0 || Width < 0 || Height < 0 )
	{
		fprintf(stderr, "Image or Image properties not defined");
		return false;
	}
	
	// Create a valid output file pointer
	FILE *OUT_FILE;
	OUT_FILE = fopen(ImagePath, "wb");
	if ( OUT_FILE == NULL ) 
	{
		fprintf(stderr, "Error Opening File for Writing");
		return false;
	}

	// Create and populate RGB buffers
	int i;
	char *Rbuf = new char[Height*Width]; 
	char *Gbuf = new char[Height*Width]; 
	char *Bbuf = new char[Height*Width]; 

	for (i = 0; i < Height*Width; i++)
	{
		Bbuf[i] = Data[3*i];
		Gbuf[i] = Data[3*i+1];
		Rbuf[i] = Data[3*i+2];
	}

	
	// Write data to file
	for (i = 0; i < Width*Height; i ++)
	{
		fputc(Rbuf[i], OUT_FILE);
	}
	for (i = 0; i < Width*Height; i ++)
	{
		fputc(Gbuf[i], OUT_FILE);
	}
	for (i = 0; i < Width*Height; i ++)
	{
		fputc(Bbuf[i], OUT_FILE);
	}
	
	// Clean up and return
	delete Rbuf;
	delete Gbuf;
	delete Bbuf;
	fclose(OUT_FILE);

	return true;

}




// Here is where you would place your code to modify an image
// eg Filtering, Transformation, Cropping, etc.
bool MyImage::Modify(MyImage &inImage)
{
	Data = new unsigned char[Width*Height*3];	//Allocation for outImage

	MyImage inImage_copy;
	inImage_copy.Data = new unsigned char[inImage.Width* inImage.Height * 3];
	inImage_copy.setHeight(inImage.getHeight());
	inImage_copy.setWidth(inImage.getWidth());
	
	const int ws = 3;
	const int sigma = 1;
	double kernel[ws][ws]; //discrete convolution operator

	double kernel_sum = 0.0;
	int kernel_radius = ws / 2;
	double frac = 0.0;

	for (int row = -kernel_radius; row <= kernel_radius; row++)
		for (int col = -kernel_radius; col <= kernel_radius; col++)
		{
		frac = exp(-1.0 * ((row*row) + (col*col)) / (2.0 * (sigma*sigma)));
		kernel[row + kernel_radius][col + kernel_radius] = frac;

		kernel_sum += kernel[row + kernel_radius][col + kernel_radius];
		}

	for (int i = 0; i < ws; i++)
		for (int j = 0; j < ws; j++)
			kernel[i][j] = kernel[i][j] / kernel_sum; //Normalized

	//No Gaussian filter
	for (int i = 0; i < inImage.Height; i++)
		for (int j = 0; j < inImage.Width; j++)
			for (int k = 0; k < 3; k++)
				inImage_copy.setValue(i, j, k, inImage.getValue(i,j,k));
	
	//Applying Gaussian Filter
	/*for (int i = 0; i < inImage.Height; i++)
		for (int j = 0; j < inImage.Width; j++)
			for (int k = 0; k < 3; k++)
			{
				int values[ws][ws];
				for (int row = 0; row < ws; row++)
					for (int col = 0; col < ws; col++)
						values[row][col] = 0;
				int values_avg = 0;

				for (int m = -kernel_radius, row = 0; m <= kernel_radius; m++, row++)
					for (int n = -kernel_radius, col = 0; n <= kernel_radius; n++, col++)
					{
						if ((i + m >= 0) && (i + m < inImage.Height) && (j + n >= 0) && (j + n < inImage.Width))
							values[row][col] = inImage.getValue((i + m), (j + n), k) * kernel[row][col];
						else if (((i + m < 0) || (i + m >= inImage.Height)) && ((j + n > 0) && (j + n < inImage.Width)))
							values[row][col] = 1.0 * inImage.getValue((i - m), (j + n), k) * kernel[row][col];
						else if (((i + m > 0) && (i + m < inImage.Height)) && ((j + n < 0) || (j + n >= inImage.Width)))
							values[row][col] = 1.0 * inImage.getValue((i + m), (j - n), k) * kernel[row][col];
						else
							values[row][col] = 1.0 * inImage.getValue((i - m), (j - n), k) * kernel[row][col];
					}

				for (int row = 0; row < ws; row++)
					for (int col = 0; col < ws; col++)
						values_avg += values[row][col];

				inImage_copy.setValue(i, j, k, values_avg);
			}*/

	// TO DO by student
	
	//Non-Linear Transfer Function
	float X, Y, x, y, a, b, one, two, three, four, first, second;

	int Row1 = inImage_copy.getHeight(), Col1 = inImage_copy.getWidth();
	int Row2 = Height, Col2 = Width;
	float boundary = 5;
	float range = (boundary)-(-boundary);
	float atanrange = atan(boundary) - atan(-boundary);
	float valx_row = 0.0, valy_row = 0.0, valx_col = 0.0, valy_col = 0.0;
	float value = 0.0;
	for (int i = 0; i < Row2; i++)
		for (int j = 0; j < Col2; j++)
		{
			valx_row = (1.0 * i / Row2)*range + (-boundary);
			valy_row = atan(valx_row/3);
			x = (valy_row / atanrange)*Row1 + (Row1/2);

			valx_col = (1.0 * j / Col2)*range + (-boundary);
			valy_col = atan(valx_col/3);
			y = (valy_col / atanrange)*Col1 + (Col1/2);

			X = floor(x);
			Y = floor(y);

			a = y - Y;
			b = x - X;

			for (int k = 0; k < 3; k++)
			{
				first = inImage_copy.getValue(X, Y, k);
				second = inImage_copy.getValue(X, Y + 1, k);
				one = (inImage_copy.getValue(X, Y, k)*(1.0 - a)*(1.0 - b));
				two = (inImage_copy.getValue(X, Y + 1, k)*(a)*(1.0 - b));
				three = (inImage_copy.getValue(X + 1, Y, k)*(1.0 - a)*(b));
				four = (inImage_copy.getValue(X + 1, Y + 1, k)*(a)*(b));

				value = (inImage_copy.getValue(X, Y, k)*(1.0 - a)*(1.0 - b))
					+ (inImage_copy.getValue(X, Y + 1, k)*(a)*(1.0 - b))
					+ (inImage_copy.getValue(X + 1, Y, k)*(1.0 - a)*(b))
					+ (inImage_copy.getValue(X + 1, Y + 1, k)*(a)*(b));		//using 4 surrounding co-ordinates

				setValue(i, j, k, value);
			}
		}


	//for (int i = 0; i < Height; i++)
	//	for (int j = 0; j < Width; j++)
	//	{
	//		x = inImage.getHeight() * (1.0 * i / Height);	//Scaling in y direction
	//		y = inImage.getWidth() * (1.0 * j / Width);	//Scaling in x direction

	//		X = floor(x);
	//		Y = floor(y);

	//		a = y - Y;
	//		b = x - X;

	//		for (int k = 0; k < 3; k++)
	//			setValue(i, j, k,((inImage.getValue(X, Y, k)*(1.0 - a)*(1.0 - b))
	//							+ (inImage.getValue(X, Y + 1, k)*(a)*(1.0 - b))
	//							+ (inImage.getValue(X + 1, Y, k)*(1.0 - a)*(b))
	//							+ (inImage.getValue(X + 1, Y + 1, k)*(a)*(b))		//using 4 surrounding co-ordinates
	//							)
	//			);															//BILINEAR INTERPOLATION FORMULATION
	//	}

	// sample operation
	/*for ( int i=0; i<Width*Height; i++ )
	{
		Data[3*i] = 0;
		Data[3*i+1] = 0;
		Data[3*i+2] = 0;
	}*/

	/*for (int i = 0; i < Height; i++)
		for (int j = 0; j < Width; j++)
			for (int k = 0; k < 3; k++)
				setValue(i, j, k, inImage.getValue(i,j,k));*/

	return false;
	//return true;
}
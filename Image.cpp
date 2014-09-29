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

void generate_kernel(double kernel[][3], int sigma);

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
	/*if (ImagePath[0] == 0 || Width < 0 || Height < 0 )
	{
		fprintf(stderr, "Image or Image properties not defined");
		return false;
	}*/
	
	// Create a valid output file pointer
	FILE *OUT_FILE;
	//OUT_FILE = fopen(ImagePath, "wb");
	OUT_FILE = fopen("Sample_file.rgb", "wb");
	if (OUT_FILE == NULL)
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
bool MyImage::Modify(MyImage &inImage, float sw, float sh, int anti_alias, int option)
{
	Data = new unsigned char[Width*Height*3];	//Allocation for outImage

	MyImage inImage_copy;
	inImage_copy.Data = new unsigned char[inImage.Width* inImage.Height * 3];
	inImage_copy.setHeight(inImage.getHeight());
	inImage_copy.setWidth(inImage.getWidth());
	
	const int ws = 3;
	const int sigma = 1;
	double kernel[3][3]; //discrete convolution operator
	generate_kernel(kernel, sigma);
	
	if (anti_alias == 1)
		//No Gaussian filter
	{
		for (int i = 0; i < inImage.Height; i++)
			for (int j = 0; j < inImage.Width; j++)
				for (int k = 0; k < 3; k++)
					inImage_copy.setValue(i, j, k, inImage.getValue(i, j, k));
	}
	else
	{

		//Applying Gaussian Filter
		for (int i = 0; i < inImage.Height; i++)
			for (int j = 0; j < inImage.Width; j++)
				for (int k = 0; k < 3; k++)
				{
			int values[ws][ws];
			for (int row = 0; row < ws; row++)
				for (int col = 0; col < ws; col++)
					values[row][col] = 0;
			int values_avg = 0;
			int kernel_radius = 3 / 2;
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
				}

	}
////#####################################################################
	float X, Y, x, y, a, b, ss;

	if (option == 0)
	{

		int value;
		for (int i = 0; i < Height; i++)
			for (int j = 0; j < Width; j++)
			{
			x = inImage.getHeight() * (1.0 * i / Height);	//Scaling in y direction
			y = inImage.getWidth() * (1.0 * j / Width);	//Scaling in x direction

			X = floor(x);
			Y = floor(y);

			a = y - Y;
			b = x - X;

			for (int k = 0; k < 3; k++)
			{
				setValue(i, j, k, ((inImage.getValue(X, Y, k)*(1.0 - a)*(1.0 - b))
					+ (inImage.getValue(X, Y + 1, k)*(a)*(1.0 - b))
					+ (inImage.getValue(X + 1, Y, k)*(1.0 - a)*(b))
					+ (inImage.getValue(X + 1, Y + 1, k)*(a)*(b))		//using 4 surrounding co-ordinates
					)
					);
			}//BILINEAR INTERPOLATION FORMULATION
			}
	}
	else if (option == 1)
	{
		float sw = 1.0 * Width / inImage_copy.getWidth();
		float sh = 1.0 * Height / inImage_copy.getHeight();
		if (sw < sh)	ss = sw;
		else			ss = sh;

		int maxHeight = inImage_copy.getHeight()*ss;
		int maxWidth = inImage_copy.getWidth()*ss;

		for (int i = 0; i < maxHeight; i++)
			for (int j = 0; j < maxWidth; j++)
			{
			x = i / ss;	//Scaling in y direction
			y = j / ss;	//Scaling in x direction

			X = floor(x);
			Y = floor(y);

			a = y - Y;
			b = x - X;

			for (int k = 0; k < 3; k++)
				setValue((i + (Height - maxHeight) / 2), (j + (Width - maxWidth) / 2), k, ((inImage_copy.getValue(X, Y, k)*(1.0 - a)*(1.0 - b))
				+ (inImage_copy.getValue(X, Y + 1, k)*(a)*(1.0 - b))
				+ (inImage_copy.getValue(X + 1, Y, k)*(1.0 - a)*(b))
				+ (inImage_copy.getValue(X + 1, Y + 1, k)*(a)*(b))		//using 4 surrounding co-ordinates
				)
				);															//BILINEAR INTERPOLATION FORMULATION
			}

		MyImage outImage_copy;
		outImage_copy.Data = new unsigned char[Width * Height * 3];
		outImage_copy.setHeight(getHeight());
		outImage_copy.setWidth(getWidth());

		for (int i = 0; i < Height; i++)
			for (int j = 0; j < Width; j++)
				for (int k = 0; k < 3; k++)
					outImage_copy.setValue(i, j, k, getValue(i, j, k));

		int diff, flag;
		float scale;
		int centerHeight = (int)(0.2*maxHeight);
		int centerWidth = (int)(0.2*maxWidth);
		if ((Height - maxHeight) / 2 > 0)
		{
			diff = (Height - maxHeight) / 2;
			scale = ((centerHeight)+diff) / (centerHeight);
			flag = 1;
		}
		else
		{
			diff = (Width - maxWidth) / 2;
			scale = ((centerWidth)+diff) / (centerWidth);
			flag = 0;
		}

		if (flag == 1)
		{
			for (int i = 0; i < (diff + centerHeight); i++)
				for (int j = 0; j < Width; j++)
				{
				x = (i / scale) + diff;	//Scaling in y direction
				y = j / 1.0;	//Scaling in x direction

				X = floor(x);
				Y = floor(y);

				a = y - Y;
				b = x - X;

				for (int k = 0; k < 3; k++)
					setValue(i, j, k, ((outImage_copy.getValue(X, Y, k)*(1.0 - a)*(1.0 - b))
					+ (outImage_copy.getValue(X, Y + 1, k)*(a)*(1.0 - b))
					+ (outImage_copy.getValue(X + 1, Y, k)*(1.0 - a)*(b))
					+ (outImage_copy.getValue(X + 1, Y + 1, k)*(a)*(b))		//using 4 surrounding co-ordinates
					)
					);
				}

			for (int i = 0; i < (diff + centerHeight); i++)
				for (int j = 0; j < Width; j++)
				{
				x = (i / scale) + (Height - diff - centerHeight);	//Scaling in y direction
				y = j / 1.0;	//Scaling in x direction

				X = floor(x);
				Y = floor(y);

				a = y - Y;
				b = x - X;

				for (int k = 0; k < 3; k++)
					setValue((i + (Height - diff - centerHeight)), j, k, ((outImage_copy.getValue(X, Y, k)*(1.0 - a)*(1.0 - b))
					+ (outImage_copy.getValue(X, Y + 1, k)*(a)*(1.0 - b))
					+ (outImage_copy.getValue(X + 1, Y, k)*(1.0 - a)*(b))
					+ (outImage_copy.getValue(X + 1, Y + 1, k)*(a)*(b))		//using 4 surrounding co-ordinates
					)
					);
				}
		}

		else
		{
			for (int i = 0; i < Height - 1; i++)
				for (int j = 0; j < (diff + centerWidth); j++)
				{
				x = i / 1.0;
				y = (j / scale) + diff;

				X = floor(x);
				Y = floor(y);

				a = y - Y;
				b = x - X;

				for (int k = 0; k < 3; k++)
					setValue(i, j, k, ((outImage_copy.getValue(X, Y, k)*(1.0 - a)*(1.0 - b))
					+ (outImage_copy.getValue(X, Y + 1, k)*(a)*(1.0 - b))
					+ (outImage_copy.getValue(X + 1, Y, k)*(1.0 - a)*(b))
					+ (outImage_copy.getValue(X + 1, Y + 1, k)*(a)*(b))		//using 4 surrounding co-ordinates
					)
					);
				}

			for (int i = 0; i < Height - 1; i++)
				for (int j = 0; j < (diff + centerWidth); j++)
				{
				x = i / 1.0;
				y = (j / scale) + (Width - diff - centerWidth);

				X = floor(x);
				Y = floor(y);

				a = y - Y;
				b = x - X;

				for (int k = 0; k < 3; k++)
					setValue(i, (j + (Width - diff - centerWidth)), k,
					((outImage_copy.getValue(X, Y, k)*(1.0 - a)*(1.0 - b))
					+ (outImage_copy.getValue(X, Y + 1, k)*(a)*(1.0 - b))
					+ (outImage_copy.getValue(X + 1, Y, k)*(1.0 - a)*(b))
					+ (outImage_copy.getValue(X + 1, Y + 1, k)*(a)*(b))		//using 4 surrounding co-ordinates
					)
					);
				}
		}
	}

	return false;
}

void generate_kernel(double kernel[][3], int sigma)
{
	double kernel_sum = 0.0;
	int kernel_radius = (int)(3 / 2);
	double frac = 0.0;

	for (int row = -kernel_radius; row <= kernel_radius; row++)
		for (int col = -kernel_radius; col <= kernel_radius; col++)
		{
		frac = exp(-1.0 * ((row*row) + (col*col)) / (2.0 * (sigma*sigma)));
		kernel[row + kernel_radius][col + kernel_radius] = frac;

		kernel_sum += kernel[row + kernel_radius][col + kernel_radius];
		}

	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			kernel[i][j] = kernel[i][j] / kernel_sum; //Normalized
}
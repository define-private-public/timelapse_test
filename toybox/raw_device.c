#include <stdio.h>
#include <stdlib.h>

#define SIZE 640*420


int main(int argc, char *argv[]) {
	FILE *camera, *image;
	float data[SIZE];

	// Require a device and a filename	
	// ./raw_device <camera_dev> <filename>
	if (argc < 3)
		return -1;

	camera = fopen(argv[1], "rb");
	image = fopen(argv[2], "wb");
	
	if ((camera == NULL) || (image == NULL))
		return -2;
	
	// Read and write the data
	fread(data, sizeof(data[0]), SIZE, camera);
	fwrite(data, sizeof(data[0]), SIZE, image);

	// Close
	fclose(camera);
	fclose(image);

	return 0;
}


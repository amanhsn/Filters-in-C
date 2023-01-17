# Filters-in-C
Simple Program to implement Image filters in C using image processing and 2D arrays for fop project.
Use Case:
./filter <-filter-specification> <INFILE>.bmp <OUTFILE>.bmp
filter-specification:
  -b : blurs image
  -g : implements grayscale
  -s : implements sepia
  -r : reflects image about y-axis
  -e : uses sobel operator to implement edges-filter
INFILE must be in bmp format;
  use https://online-converting.com/image/convert2bmp/#google_vignette to properly convert image to bmp, works on every image!,image size must be 600x400!

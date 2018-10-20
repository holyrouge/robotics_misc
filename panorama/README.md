### Panorama Builder
---
#### Flow
* Create a panorama from provided images.
* Takes path to input and output directories as argument.
* Creates output directory if not present.
* Separates the images from the left and the right eye by cropping the original image in two (width/2) and stores in a folder left and right in output directory.
* Reads one third of the images from the left directory and divides them into sets of ten.
* Finds one image from each set that has the highest keypoints matches.
* For the first two sets it compares the all the images in the first set with all the images from the second set.
* For the rest of the sets compares the best image found in the previous set to all the images in the current set and chooses the image with the highest keypoint matching.
* Send all the images that were found using the above process to stitch method of stitcher class as input.

#### Usage
python pano.py \<path to input directory> \<path to output directory>

#### Requirements
* python 3.6.4
* opencv 3.4.1
#### Reference
* https://www.pyimagesearch.com/2016/01/11/opencv-panorama-stitching/

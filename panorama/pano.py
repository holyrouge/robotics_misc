import argparse
import cv2
import numpy as np
import math
import timeit
import os
import sys


def show_img (name, img):
    '''
    displays a image till a key is pressed    

    name: title of the image
    img: image matrix
    '''
    cv2.imshow(name, img)
    cv2.waitKey(0)

def chk_dir(ip_dir, op_dir):
    '''
    checks if given directories exist. If input directory is not present exits
    if output directory not present creates directories at specified path
    
    ip_dir: path to input directory
    op_dir: path to output directory
    '''
    if not os.path.isdir(ip_dir):
        print("input directory does not exist")
        sys.exit()

    #left for images from left eye of the camera.
    #right for images from the right eye of the camera.
    for sub_dir in ["left", "right"]:
        try: os.makedirs(os.path.join(op_dir,sub_dir))
        except OSError: pass    

def get_keyPts(img):
    '''
    detects keypoints and calculates SIFT features from a image

    img:image to process        
    '''           
    desc = cv2.xfeatures2d.SIFT_create()
    (key_pts, ftrs) = desc.detectAndCompute(img, None)
    key_pts = [kp.pt for kp in key_pts]
    return (key_pts, ftrs)

def match_keyPts(f1, f2, ratio):
    '''
    find the number of common features between two images

    f1: feature descriptors of the first image
    f2: feature descriptors of the second image
    ratio: ratio threshold for Lowes ratio test
    '''
    matcher = cv2.DescriptorMatcher_create("BruteForce")    
    rawMatches = matcher.knnMatch(f1, f2, 2)
    
    matches = []

    #Lowes ratio test
    for m in rawMatches:
        if len(m) == 2 and m[0].distance < m[1].distance * ratio:
            matches.append((m[0].trainIdx, m[0].queryIdx))    
    return len(matches)


if __name__ == '__main__':
    #take command line arguments
    parser = argparse.ArgumentParser(description = "build panorama from images provided in directory")
    parser.add_argument("input_dir", help="specify path to the input directory")
    parser.add_argument("output_dir", help="specify path the output directory;will create if it doesnt exist")
    args = parser.parse_args()
    ip_dir = os.path.abspath(args.input_dir)+"/"
    op_dir = os.path.abspath(args.output_dir)+"/"


    start = timeit.default_timer()

    #check if input, output directories exist
    chk_dir(ip_dir, op_dir)

    #read files from input directory and write files to output directory
    #images from the left eye are stored in the "left" subdirectory
    #images from the right eye are stored in the "right" subdirectory
    print("writing files to output directory...")
    extsn=""
    prfx=""
    count = 0
   
    os.chdir(ip_dir)
    for f in os.listdir(str(ip_dir)):
        if os.path.isfile(f):
            if(count == 0):
                #get file extsn eg .jpg
                extsn="."+f.split(".")[-1]
                #get prefix e.g engi_quad_
                tmp=f.split("_")
                tmp=tmp[:-1]
                prfx = "_".join(tmp)+"_"                    
            img = cv2.imread(ip_dir+f)
            border = int(img.shape[1]/2)
            img_l = img2 = img[:,:border,:]
            img_r = img2 = img[:,border:,:]        
            cv2.imwrite(op_dir+"left/"+f, img_l)        
            cv2.imwrite(op_dir+"right/"+f, img_r)
            count = count+1

            
    print("done writing files to output directory...")

    #Generate sets of images
    # A set consists of 10 images in order 1-10, 11-20, 21-30, ...
    print("Generating Sets...")

    #glbl_lst=[[img1, img2, ...,img10],[img11,img12,...,img20],...[img101...img110]]
    glbl_lst = []
        
    #only one third of the images from the left eye are used    
    lim = math.ceil(count/30)+2
    for i in range(1, lim):
        lcl_lst = []
        # print("i", i)
        for j in range(((i-1)*10)+1, (10*i)+1):                
            # print("j", j)
            img = cv2.imread(op_dir+"left/"+prfx+str(j)+extsn)
            # print("op path", op_dir+"left/"+prfx+str(j)+extsn)
            (kps, ftrs) = get_keyPts(img)
            lcl_lst.append((kps, ftrs, img))        
        glbl_lst.append(lcl_lst)


    #find best matches between the first two sets
    print("Sets created")
    print("Finding Best match between first two sets...")
    l1 = glbl_lst[0]
    l2 = glbl_lst[1]
    max_match = 0
    data_set=[]
    for(k1, f1, i1) in l1:
        for(k2, f2, i2) in l2:
            matches = match_keyPts(f1, f2, 0.75)    
            if(matches > max_match):
                max_match = matches            
                d1 = (k1, f1, i1)
                d2 = (k2, f2, i2)

    data_set.append(d1)
    data_set.append(d2)


    #find best matches in the rest of the sets wrt to the best matches found till now
    print("Finding best match between rest of the sets...")
    for j in range(2, len(glbl_lst)):
        lcl_lst = glbl_lst[j]
        d = data_set[-1]
        max_match=0
        for(k, f, i) in lcl_lst:
            matches = match_keyPts(d[1], f, 0.75)    
            if(matches > max_match):
                max_match = matches
                d_tmp = (k, f, i)
                
        data_set.append(d_tmp)

    print("Best matches found")

    print("Generating panorama...")
    #append images from left ot right
    img_set=[]
    for (k,f,i) in data_set:
        img_set.append(i)

    stitcher = cv2.createStitcher(False)
    result = stitcher.stitch(img_set)

    stop = timeit.default_timer()
    print("time taken", stop - start)

    show_img("res",result[1])
    cv2.imwrite("res_stitched.jpg", result[1])
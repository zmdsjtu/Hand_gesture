cmake_minimum_required(VERSION 3.4)

add_compile_options(-std=c++11)
project (Gesture)
include_directories("/usr/local/include")
include_directories("/home/zmd/dlib-master")
include_directories("/src")
LINK_DIRECTORIES("/usr/local/lib")
LINK_DIRECTORIES("/home/zmd/dlib-master/build/dlib")
LINK_LIBRARIES(opencv_aruco
opencv_bgsegm
opencv_bioinspired
opencv_calib3d
opencv_ccalib
opencv_core
opencv_datasets
opencv_dnn
opencv_dnn_objdetect
opencv_dpm
opencv_face
opencv_features2d
opencv_flann
opencv_freetype
opencv_fuzzy
opencv_hfs
opencv_highgui
opencv_imgcodecs
opencv_imgproc
opencv_img_hash
opencv_line_descriptor
opencv_ml
opencv_objdetect
opencv_optflow
opencv_phase_unwrapping
opencv_photo
opencv_plot
opencv_reg
opencv_rgbd
opencv_saliency
opencv_shape
opencv_stereo
opencv_stitching
opencv_structured_light
opencv_superres
opencv_surface_matching
opencv_text
opencv_tracking
opencv_video
opencv_videoio
opencv_videostab
opencv_xfeatures2d
opencv_ximgproc
opencv_xobjdetect
opencv_xphoto
dlib)
add_executable(Gesture RegionGrow.cpp  cvui.h   jigsaw.h  RegionGrow.h 
Find_contours.cpp  Main.cpp Find_contours.h     
)

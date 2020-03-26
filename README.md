Repository for the homeworks of Computer Vision course

XCode project is configured to include OpenCV library. 
OpenCV is installed using command `brew install opencv`. The installation of OpenCV on the machine is located at '/usr/local/opt/opencv@4'

If it's needed, add OpenCV to the PATH:
`echo 'export PATH="/usr/local/opt/opencv@4/bin:$PATH"' >> ~/.bash_profile`

**XCODE SETUP**

To complete the configuration on XCode do the following steps:
1. Go to Project file name
2. Go to **Build Settings**
3. Check **All** and **Levels**
4. Search **Linking**
5. Under **Linking** go to **Other Linker Flags**
6. On **Debug**, click + and add this `-lopencv_gapi -lopencv_stitching -lopencv_aruco -lopencv_bgsegm -lopencv_bioinspired -lopencv_ccalib -lopencv_dnn_objdetect -lopencv_dnn_superres -lopencv_dpm -lopencv_highgui -lopencv_face -lopencv_freetype -lopencv_fuzzy -lopencv_hfs -lopencv_img_hash -lopencv_line_descriptor -lopencv_quality -lopencv_reg -lopencv_rgbd -lopencv_saliency -lopencv_sfm -lopencv_stereo -lopencv_structured_light -lopencv_phase_unwrapping -lopencv_superres -lopencv_optflow -lopencv_surface_matching -lopencv_tracking -lopencv_datasets -lopencv_text -lopencv_dnn -lopencv_plot -lopencv_videostab -lopencv_videoio -lopencv_xfeatures2d -lopencv_shape -lopencv_ml -lopencv_ximgproc -lopencv_video -lopencv_xobjdetect -lopencv_objdetect -lopencv_calib3d -lopencv_imgcodecs -lopencv_features2d -lopencv_flann -lopencv_xphoto -lopencv_photo -lopencv_imgproc -lopencv_core`
7. Now go to **Search Paths**
8. On **Header Search Paths**, go to **Debug**, click + and add this `/usr/local/opt/opencv@4/include/opencv4` 
9. On **Library Search Paths**, go to **Debug**, click + and add this `/usr/local/opt/opencv@4/lib`

After these steps, the OpenCV library is loaded in XCode.
Make sure also in **Signing & Capabilities** tab, that is checked **Disable Library Validation**. Otherwise XCode will return an error in compile time.

The XCode setup is finished.





**BUILD FROM TERMINAL**

Follow this if it's needed to build through command line. It's not needed when is using XCode.
To compile use this command:

`g++ -I/usr/local/opt/opencv@4/include/opencv4 -L/usr/local/opt/opencv@4/lib -std=c++11 ./test.cpp -o ./test.o`

And add the following flags needed for the linker based on modules that are used on the project:

`-lopencv_gapi -lopencv_stitching -lopencv_aruco -lopencv_bgsegm -lopencv_bioinspired -lopencv_ccalib -lopencv_dnn_objdetect -lopencv_dnn_superres -lopencv_dpm -lopencv_highgui -lopencv_face -lopencv_freetype -lopencv_fuzzy -lopencv_hfs -lopencv_img_hash -lopencv_line_descriptor -lopencv_quality -lopencv_reg -lopencv_rgbd -lopencv_saliency -lopencv_sfm -lopencv_stereo -lopencv_structured_light -lopencv_phase_unwrapping -lopencv_superres -lopencv_optflow -lopencv_surface_matching -lopencv_tracking -lopencv_datasets -lopencv_text -lopencv_dnn -lopencv_plot -lopencv_videostab -lopencv_videoio -lopencv_xfeatures2d -lopencv_shape -lopencv_ml -lopencv_ximgproc -lopencv_video -lopencv_xobjdetect -lopencv_objdetect -lopencv_calib3d -lopencv_imgcodecs -lopencv_features2d -lopencv_flann -lopencv_xphoto -lopencv_photo -lopencv_imgproc -lopencv_core`

An example, if the core and highgui modules are used, the command to build is:

`g++ -I/usr/local/opt/opencv@4/include/opencv4 -L/usr/local/opt/opencv@4/lib -lopencv_core -lopencv_highgui -std=c++11 ./test.cpp -o ./test.o`

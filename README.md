# 3D-StereoPanoramaCreator
A Cpp software with Opencv library, which takes a video as input and creates a 3D Stereo panorama

Introduction / Background / Related Work:
The Stereo Panorama Image problem is a known and common problem in computer vison, and you can see uses of it in different fields such as movies, art, advertising etc.
Stereo Panoramas can be created using images captured with a regular camera rotating about an axis behind it. Pasting together strips taken from each image approximates the panoramic image cylinder. When the strips are taken from the center of the images an ordinary panorama is obtained. When the strips are taken from the left side of each image, the viewing direction is tilted counter clockwise from the image surface, obtaining the right-eye panorama. When the strips are taken from the right side of each image, the left-eye panorama is obtained.
The main challenge was to find the best division of a frame to strips to get left eye and right eye panorama images. 
There are a many existing solutions for creating a stereo panorama image, but the most widely used and affective ones based on feature matching and dividing each frame into stripes accordingly.

The proposed solution / method / Algorithm:
Our program is divided to several parts:
Part 1: Create a Vector from Frames.
Part 2: Do Feature Matching and find the delta X's.
Part 3: Create the central Panorama
Part 4: Create the Left Eye Panorama
Part 5: Create the Right Eye Panorama
Part 6: Create Stereo Panorama
Part 7: Combine the Panoramas to one picture. (Left and Right)
Part 8: Create a video that illustrate the movement between sides.

# SigNN - Real-time ASL Alphabet Translator

SigNN is the first app which can translate ASL (American Sign Language) in real-time. [It is free and available for download on the Play Store](https://play.google.com/store/apps/details?id=com.signn.MediaPipe.apps.handtrackinggpu). Most of our raw data is also [publically available on Kaggle](https://www.kaggle.com/signnteam/asl-sign-language-pictures-minus-j-z).

When using SigNN, please sign with your left hand. Right-hand support is not implemented. Additionally, please make sure your hand is not at an extreme angle when signing the ASL alphabet.

This tool can help people learn sign language by validating if they are clearly signing their word. If SigNN can understand your ASL sign, then an ASL signer certainly could as well.

This tool may also get people interested in ASL and the facilitation of communication with those who have difficulty communicating through speech. There right now exists expensive sign language translation gloves, but we hope to see more affordable and scalable solutions in the future.

SigNN stands for Sign Neural Network and is a proof of concept built upon MediaPipe, a Machine-Learning Pipeline made by Google. It shows that real-time sign language translation is now possible on mobile devices. SigNN was developed by a group of students at the University of California, Santa Barbara.

A company or group with more resources can create a more accurate neural network and provide support for not only the ASL alphabet, but the more common words. While we may be the first on the app store, we hope that many improved versions will follow.

## Contributors

- Arian Alavi
- Vahe Barseghyan
- Rafael Trinidad
- Gokul Deep
- Kenny Yip
- John Kirchner
- Daniel Lohn
- Conor O'Brien

## Compile Insturctions

SigNN is only supported on Ubuntu 18 and Android. If you have an Android phone, it is possible to run SigNN on Ubuntu and use the Android as a webcam.

### To do before compiling SigNN for any operating system

In order to compile, first [download the pre-requisites for MediaPipe](https://google.github.io/MediaPipe/getting_started/install.html#installing-on-debian-and-ubuntu) so that you may compile the normal "handtracking" example that they have for your target OS. For mobile and Ubuntu (GPU) it is required to install GPU support as well, so a virtual machine will not work.

### Ubuntu (Using GPU, non-VM) 

To compile our project, run:

```bazel build -c opt --copt -DMESA_EGL_NO_X11_HEADERS --copt -DEGL_NO_X11 MediaPipe/examples/desktop/hand_tracking:hand_tracking_gpu```

If you own a webcam, run: 

```sudo GLOG_logtostderr=1 bazel-bin/MediaPipe/examples/desktop/hand_tracking/hand_tracking_gpu --calculator_graph_config_file=MediaPipe/graphs/hand_tracking/hand_tracking_desktop_signn.pbtxt```

### Ubuntu (Without GPU, VM Supported) 

To compile our project, run:

```bazel build -c opt --define MediaPipe_DISABLE_GPU=1 MediaPipe/examples/desktop/multi_hand_tracking:multi_hand_tracking_cpu```

If you own a webcam, run:

```sudo GLOG_logtostderr=1 bazel-bin/MediaPipe/examples/desktop/hand_tracking/hand_tracking_cpu --calculator_graph_config_file=MediaPipe/graphs/hand_tracking/hand_tracking_desktop_signn.pbtxt```

### Ubuntu without Webcam (using Droidcam)

If you do not own a webcam: Download droidcam [on the play store for Android](https://play.google.com/store/apps/details?id=com.dev47apps.droidcam&hl=en_US&gl=US) and (follow their steps to setup Droidcam on Linux)[https://www.dev47apps.com/droidcam/linux/]. 

Prefix your run command with ```sudo LD_PRELOAD=/usr/lib/x86_64-linux-gnu/libv4l/v4l2convert.so ``` to make use of Droidcam. 

Example for GPU:

 ```sudo LD_PRELOAD=/usr/lib/x86_64-linux-gnu/libv4l/v4l2convert.so GLOG_logtostderr=1 bazel-bin/MediaPipe/examples/desktop/hand_tracking/hand_tracking_gpu --calculator_graph_config_file=MediaPipe/graphs/hand_tracking/hand_tracking_desktop_signn.pbtxt``` 

### Android

It is recomended to use NDK Version 21. To compile our project, run:

```bazel build -c opt --config=android_arm64 MediaPipe/examples/android/src/java/com/google/MediaPipe/apps/signnonehand:signnonehand```

Transfer to Android phone through ADB by running:

```adb install bazel-bin/MediaPipe/examples/android/src/java/com/google/MediaPipe/apps/signnonehand/signnonehand.apk```




# A Deep Look Into SigNN and our Process

## Abstract

The goal of SigNN is to develop a software which is capable of real-time translation of American Sign Language (ASL) into text. Due to manpower constraints, the scope has been limited to completing reliable translation for the ASL alphabet. We have achived this objective using MediaPipe: an alpha-stage framework for building multimodal, cross platform, applied ML pipelines. Almost all data used to train our neural network was self-collected, aggregated, and analyzed through the use of scripts written in Google Colab.

## Motivation

Developing a tool for sign language has been a popular project for the past two decades. A primitive start to the project of sign language translation was the sign language glove. [In 2002, a MIT student was among the first to achieve a basic form of translation through the use of a glove.](https://lemelson.mit.edu/resources/ryan-patterson "The system includes a soft, leather glove outfitted with ten sensors that a signer wears on his or her hand, and a small computer that associates each hand position with a corresponding letter. By finger-spelling words using the standard American Sign Language alphabet, each letter would be transmitted to and captured by the processing unit") Since then, many different types of sign language glove prototypes have been made. Yet, gloves have had many limitations. One obvious limitation is the impracticality of carrying around a glove at all times. The high cost for the gloves is another limitation. Finally, sign language translation requires not only the use of hand tracking, but facial tracking as well. The last problem with gloves is that it's dehumanizing to those who rely on ASL as their primary form of communication, asking them to put on a glove to understand them.

As AI and computation power has improved, people attempted to develop a tool for sign language translation through the use of computer vision. These attempts have been done through the direct interpretation of pixels for sign lagnuage translation - it has been found to be very ineffective. Another popular path to vision-based sign language translastion has been to build the translator on top of a framework which already provides the coordinates of the hands. This simplifies the problem greatly from "Given an image ,what am I signing?" to "Given a series of hand coordinates, what am I signing"

### OpenPose

There have been a few different frameworks created which can detect the position of hands. The most famous of them is of course, OpenPose. OpenPose not only interprets the hands of the image, but also the arms and face, which are other vital parts of sign language translation.

![OpenPose demo](https://user-images.githubusercontent.com/49175620/76171514-8e87e900-6149-11ea-8c7e-963cc5c74173.gif)

*From OpenPose*

As impressive as OpenPose is, it faces one major challenge: speed. It is extremely slow and cannot be run on the average smartphone or computer in the foreseeable future. As impressive as OpenPose is, it faces one major challenge: speed. It is extremely slow and cannot be run on the average smartphone or computer in the forseeable future. By the developers' own measurements, [the model runs at .1 FPS to .3 FPS average depending on the OpenPose model used. ](https://github.com/CMU-Perceptual-Computing-Lab/openpose/blob/master/doc/speed_up_openpose.md "The CPU version runs at about 0.3 FPS on the COCO model, and at about 0.1 FPS (i.e., about 15 sec / frame) on the default BODY_25 model."). Such speed is nowhere near sufficient to enable real-time sign language translation. Perhaps if it is run in a powerful, remote server it could work. Then, it would require users to be prepared to pay an expensive monthly subscription. Without the issue of speed, Openpose genrally was found to be an effective framework for sign language translation and the choice of many machine learning researhers.

### MediaPipe

In June 2019, the developers at Google released MediaPipe into the open-source enviornment on GitHub. Unlike Openpose, MediaPipe is not just a neural network but is an entire framework for the management and execution of neural networks on streams of data (such as video and audio). MediaPipe is still in the alpha stages and lacks much comprehensive documentation. Compared to OpenPose, it should be less effective for general sign language translation, given that the model only detects hands and not the entire arm plus face. (As of October 2020, we have found that MediaPipe has added  OpenPose-like tracking and detailed facial feature tracking). Yet, what makes MediaPipe more impressive the OpenPose in practice is its impressive framerate and ability to run on any environment. [More information is available in this review of MediaPipe](https://medium.com/@arianalavi1/a-review-of-googles-new-mobile-friendly-ai-framework-MediaPipe-25d62cd482a1?source=friends_link&sk=260a518e203d1b011a449443165fef79). 


![MediaPipe demo](https://github.com/AriAlavi/SigNN/blob/master/docs/images/mobile/hand_tracking_3d_android_gpu.gif?raw=true)

*From MediaPipe*

Sign language translation has yet to be done comprehensively with MediaPipe, given that it is a brand new and yet-to-be documented technology. Our motivation for this project is **to be among the first to use this promising new technology and apply it to a problem that many have attempted to tackle in the past.**

## Methodology

### Three phases

When it comes to *complete* sign language translation, there are **three main phases**:

1. Recognition of hands in motion, with some idea of hand persistence (left hand versus right hand)

2. The translation of individual words

3. The interpretation of words into sentences (ASL does not use the structured grammatical rules of English)

![Credit to DeepASL](https://user-images.githubusercontent.com/49175620/76172748-170c8680-6156-11ea-87d0-2045b5b1e373.png)
Credit to DeepASL

**Phase 1** is partially done by the hand tracking neural network provided by MediaPipe. MediaPipe can only detect the presence and position of hands , but cannot differentiate between the left hand and right hand (not true as of October 2020). Our team has had many discussions on how to tackle this issue, here are some solutions we proposed:

- Detecting the subtle difference in left and right hand finger lengths

- Using the difference in skin color on the front and back of the hand

- Asking the user to calibrate by having their hands to their sides and then associating that position with left and right. Then updating left and right each frame to whichever hand is closest to the last known position of left and right

In the end, the lack of hand persistence did not pose a substantial problem due to the limited scope of the sign language translation we aimed to complete. If we wished to extend our scope to words, it would be necessary to track the handedness of each hand as well.

**Phase 2** is in the realm of SigNN. We are to, given the coordinates from phase 1, output which character the user has signed. To completely finish phase 2, the neural network must be able to identify most *words* in ASL. This task would have been far too ambitious for our team, as we do not have the resources to collect all that data necessary. 

### MediaPipe

**We began our project** by trying to break into the black box that was MediaPipe. While there is some basic documentation, much of what we learned was through experimentation and modification of the source code. After a month, we finally had a good idea of how to modify MediaPipe for our purposes.  

![Diagram of a MediaPipe graph](https://raw.githubusercontent.com/google/MediaPipe/master/MediaPipe/docs/images/mobile/hand_tracking_mobile.png)

The diagram above shows the MediaPipe graph for detecting and rendering the position of a hand in a video. MediaPipe works through a system of graphs, subgraphs, calculators, and packets.

- Packets: Packets is simply any data structure with a time stamp. In the shown diagram, input_video is the packet that is fed into the graph. The HandLandmark subgraph takes in the packets: NORM_RECT and IMAGE while outputs the packets: NORM_RECT, PRESENCE, and LANDMARKS. Packets are sent between calculators on each frame.

- Graph: A graph is the structure of the entire program. The entire diagram itself is a graph called "Hand Tracking CPU". Graphs are defined in special .pbtxt files and are read at the start of run-time, meaning that they can be modified without recompiling the code

- Calculators: Calculators can have inputs and outputs. They run code on creation, per frame, and on close. An example of a calculator could be one that takes in coordinates of hands and outputs those coordinates being normalized. Another example of a calculator could be one that takes in a tensorflow session and a series of tensors and outputs detections. Some examples of calculators in the diagram are: FlowLimiter, PreviousLoopback, and Gate

- Subgraph: A subgraph is a series of calculators grouped into a graph. Subgraphs have defined inputs and outputs and help to abstract what would be an otherwise over-complicated .pbtxt file and diagram. The subgraphs in the diagram are in blue and are: HandLandmark, HandDetection, and Renderer

[Read more about MediaPipe's structure here](https://google.github.io/mediapipe/framework_concepts/framework_concepts.html)

We were then able to modify the structure of MediaPipe's hand tracking graph to output the coordinates of the hands, which were hiding in HandLandmark as the packet called LANDMARKS.

![Coordinate console logger](https://user-images.githubusercontent.com/49175620/76173312-a1a3b480-615b-11ea-9645-7541a397f169.PNG)

The example shown above is the modification we made to the Multiple Hand Detection CPU graph in order to get output to the console. From there, the output could be piped into a python application for data collection.

![e](https://user-images.githubusercontent.com/49175620/76174303-cb60d980-6163-11ea-933b-f0c0fd09d32f.PNG)

This is an example of the output we modified MediaPipe to output. On the left is the rendered image that MediaPipe normally outputs and on the right is a graph of the series of coordinates that we modified MediaPipe to output.

### OpenPose

**We also looked at OpenPose** as an alternative to MediaPipe. We figured that tracking the arms and the face would significantly increase our accuracy levels. However, most of our collected data did not include the entire upper body. As a result, only ~1,000 / ~6,000 images could be used to train the neural network. After training the neural network we received an unsatisfactory ~77% accuracy. Lastly, OpenPose was prohibitively slow on our laptops and we found that it could not be used to translate sign language in real-time. As a result, we decided to stay with our original framework of MediaPipe.

### Data Collection

**Data collection** was a crucial part of increasing our accuracy. We would have preferred to use publicly available data to train our neural network. However, there were very few sign language data sets and the ones that did exist were of very low quality. As a result, we decided to create our own dataset. While the idea seems simple, it quickly became a problem for us as we took more and more pictures.

 We started with a data set of 100 pictures and over winter break managed to expand to about 500 pictures, impressive at the time. After running testing with the neural network, we found our accuracy was unacceptable, about 60%. Up until this point, all data processing (turning of images into coordinates) was done semi-manually and all data was stored on a hard drive. We played around with the idea of a central website for the group (and maybe some volunteers) to be able to upload their hands, however, we found a much better solution: Goolge Colab and Google Drive.

![Data collection process](https://user-images.githubusercontent.com/49175620/76173861-7a9bb180-6160-11ea-9c34-68871054c4c5.png)

We created a series of Google Colab scripts in order to streamline data collection and processing:

- Database structure: This is a data structure created in order to be able to interact with the same Google Drive folder without having to all share the same account. It allows for downloading, uploading, and listing of files based on character.

- Data collection script [(MediaPipe or Openpose)](https://colab.research.google.com/drive/1o-Ao3_oe0CKDDUJlDplGvhB70wcKaBMA): The script records the webcam and take (n) pictures with a delay of (m) milliseconds. We stuck to taking 30 pictures with 300 millisecond delay. This allows us to create a lot of data quickly. After taking the pictures, it uploads them to a Google Drive database of raw images, each file with a unique name.

- Json creation script ([MediaPipe](https://colab.research.google.com/drive/1RUSUH9C91uZt3MO56yqZMglqol0aPVoX) / [Openpose](https://colab.research.google.com/drive/1zWzXpi_9n0R23Dk_pA3PPs5J_6pk-9Bw)): This script will get a list of all raw images and all raw json files. Then the script will process all raw images that have not been found in the raw json folder. It can do this because one image will create one json file of the same name and upload it to the raw json folder.

- Json formation script ([Openpose](https://colab.research.google.com/drive/1F4Zw9shdp73fVYtX0jMD7idA-TecCpCJ)): This script will download the formed json file for each character (Formed json is the collection of many json files into one json list or object). If the length of the list of the formed json file is equal to the number of raw json files associated with that character, we know that there have been no additional json files added. Otherwise, we will delete the formed_json file (as we cannot discriminately modify the formed json file) and reform it with the data from the raw json folder for that associated character. When all characters are formed, they will combine to create a complete data.json and be uploaded to a unique database that only holds the formed file.

Through the use of these scripts we managed to accumulate about *six thousand* different pictures for sign language characters.

### Algorithmic Approach

**Before the neural network** we decided to work on a algorithmic solution in order to get a better idea of the challenges we would face with the network. We developed two different methods:

- Z-score method: For each frame, take all x coordinates and convert them into z scores and then take all y coordinates and convert them into z scores.

- Angle method: For each frame, take the angle between each (x, y) coordinate and the next. This will allow the hand to be at any angle (rather than only just head-on).

Both these methods used minimize error min(modeled_coordinate_i - actual_coordinate) in order to predict which sign was being displayed. Of course, the algorithmic approach was unsuccessful. The task is too complicated for a simple algorithm to solve. However, the two methods we came up with heavily influenced on work on the neural network.

![I in sign language](https://user-images.githubusercontent.com/49175620/76174415-59d55b00-6164-11ea-97d7-74a348007bf7.png)
![M in sign language](https://user-images.githubusercontent.com/49175620/76174417-5a6df180-6164-11ea-8238-ebf46d90c60e.png)

Signs such as M and I were mistaken for each other often under the algorithimc approach due to their similiar characteristics.

### Neural Network

**Working on the neural network**, we didn't find much success with simply feeding in the data to the network. Our accuracy was hovering in the 60% range. We assumed that the neural network would figure out how to best interpret the data internally, but we soon got the idea of preprocessing the data in some way. The angle and z-score methods we used in the algorithmic approach made their way back. We hoped that both methods would reduce variability between samples (though they were already normalized) but didn't know which would be more effective. We saw a great boost in our accuracy when we used both methods:

- Z-score method: ~85% accuracy

- Angle method: ~75% accuracy

Since the Z-score method was significantly more accurate, we decided to go with that over the angle method.

There also came the problem of network architecture. Research told us that the best layers for the job would be Relu and we tinkered with the number of layers and density, adding some dropoffs and seeing what was optimal.  Eventually, we decided to see if we could [algorithimically find the most effective architecture and created a script on Google Colab to do so](https://colab.research.google.com/drive/1kiLfHN8at88c2F1hkQqxmH96P7mjcJGq). After many hours of computing time, we procedurally generated 270 different neural networks with different combinations of layer counts,layer types, layer densities and found the most optimal neural network to be cone shaped. Specifically:

Relu(x900) -> Dropout(.15) -> Relu(x400) -> Dropout(.25) -> Tanh(x200) -> Dropout(.4) -> Softmax(x24)

Note that other than rounding the numbers to be more human friendly, the architecture of this neural network was found to be the most optimal by a computer. Even without human biases, the architecture that was developed has a clear pattern to it. Density decreases throughout the layers while dropout increases.

## Key Results and Summary

- Real-time translation of sign language is a computationally difficult task that may not be possible on most consumer-grade hardware. The exception to that is if the software is based on MediaPipe. However, as of time of writing this, MediaPipe has poor documentation and can only track hands (not arms and face).

- One of the largest obstacles to the creating of a neural network for the translation of sign language is the lack of publicly available sign language data. Possible solution could be to crowdsource data collection.

- The collection, management, and processing of training data is a task which cannot feasibly be done manually, and should be streamlined.

- Sign language translation cannot be accurately done in an algorithmic approach as many signs look very similar when it (x, y) coordinate form. It is necessary to use a neural network.

- Coordinate data from pictures is not optimal input to a translation neural network. Accuracy rates increase (60% -> 85% in our case) when each frame has z-scores individually calculated for each set of x and y coordinates.

- We were able to complete real-time translation of characters A-Y (excluding J) with 89% accuracy.

- ASL Characters J and Z along with almost all ASL words are "time-series" signs that will require the use of an LSTM and complex data management infrastructure.

## Future Work

Future work could include the completion of the alphabet with time-series characters J and Z. In order to do so, we would need to modify our Google Colab scripts to accommodate videos. We would also have to create a MediaPipe calculator to store the last n frames of data. One problem that requires research is finding out how many previous frames the neural network should be fed in order to accurately predict most signs. If J and Z are completed with high accuracy, it would be somewhat trivial to expand the neural network to the most popular sign language words. The biggest bottleneck in that case would be lack of available data on sign language words.

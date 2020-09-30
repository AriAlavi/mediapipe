# SigNN

Our app has finally been deployed: https://play.google.com/store/apps/details?id=com.signn.mediapipe.apps.handtrackinggpu

## Abstract

The goal of SigNN is to develop a software which is capable of real-time translation of American Sign Langauge (ASL) into English text. Due to various contraints, the scope has been limited to completing reliable translation for 24 of the 26 characters of the alphabet. We achieve this objective using Mediapipe: an alpha-stage framework for building multimodal, cross platform, applied ML pipelines. All data used to train our neurall network was self-collected, aggregated, and analyzed through the use of scripts written in Google Colab.

## Contributors

- Arian Alavi
- Vahe Barseghyan
- Rafael Trinidad
- Gokul Deep
- Kenny Yip
- John Kirchner
- Daniel Lohn
- Conor O'Brien

## Motivation

=======
>>>>>>> NNTest
Developing a tool for sign language translation has been a project many have attempted in the past two decades. A primitive start to the general project of sign language translation was the sign language glove. [In 2002, a MIT student was among the first to achieve a basic form of translation through the use of a glove.](https://lemelson.mit.edu/resources/ryan-patterson "The system includes a soft, leather glove outfitted with ten sensors that a signer wears on his or her hand, and a small computer that associates each hand position with a corresponding letter. By finger-spelling words using the standard American Sign Language alphabet, each letter would be transmitted to and captured by the processing unit") Since then, many different types of sign language glove prototypes have been made. However, gloves have many limitations. One obvious limitation is the impracticality of carrying around such a glove at all times. One less obvious limitation is that sign language involves just more than the hands - it involves arm movements and facial expression as well.

As AI and computation power have improved over time, people have attempted to develop a tool for sign language translation through the use of computer vision. Although some attempted to interpenetrate pixels for sign language translation, this has been found to be very ineffective. The path that is most often taken in the field of sign language is to work with a framework that provides you with an accurate representation of hands given an image. This simplifies the problem greatly - from "Given an image, what am I signing?" to "Given a series of coordinates, what am I signing?"

There have been a few different frameworks created to detect the position of hands, but the most famous of them is of course, OpenPose. OpenPose not only interprets the hands of an image, but also the arms and face, which are other vital parts of sign language translation.

![OpenPose demo](https://user-images.githubusercontent.com/49175620/76171514-8e87e900-6149-11ea-8c7e-963cc5c74173.gif)
From OpenPose

As impressive as OpenPose is, it faces one major challenge: speed. It is extremely slow and cannot be run on the average smartphone or computer in the forseeable future. By the developers' own measurements, [the model runs at .1 FPS to .3 FPS average depending on the OpenPose model used. ](https://github.com/CMU-Perceptual-Computing-Lab/openpose/blob/master/doc/speed_up_openpose.md "The CPU version runs at about 0.3 FPS on the COCO model, and at about 0.1 FPS (i.e., about 15 sec / frame) on the default BODY_25 model."). Such speed is nowhere near sufficient to enable real-time sign language translation. Perhaps if it is run in a powerful, remote server it could work. However, such a server would have an unmaintanably expensive cost per user ratio unless the users would be prepared to pay a monthly subscription. Without the issue of speed, Openpose has been shown to be an effective framework for sign language translation and the favorite choice of researchers.

In June 2019, developers at Google first created the repo Mediapipe on GitHub. Mediapipe, unlike OpenPose, is not a just a neural network but is an entire framework for the management and executing of neural networks on streams of data (such as video and audio). Mediapipe is still in alpha stages and lacks much comprehensive documentation. Compared to OpenPose, it should be less effective for sign language translation given that the provided model only detects hands and not the entire arm plus face. However, what makes Mediapipe, in practice, more effective than Openpose given its impressive framerate and ability to be run on any environment: from computer to smartphone [to even browser.](https://viz.mediapipe.dev/runner/demos/hand_tracking/hand_tracking.html "Click here for a Mediapipe hand tracking demo")

![Mediapipe demo](https://raw.githubusercontent.com/google/mediapipe/master/mediapipe/docs/images/mobile/hand_tracking_android_gpu.gif)
From Mediapipe

Sign language translation has yet to be done comprehensively with Mediapipe, given that it is a brand new and yet-to-be documented technology. Our motivation for this project is **to be among the first to use this promising new technology and apply it to a problem that many have attempted to tackle in the past.**

## Methodology

### Three phases

When it comes to *complete* sign language translation, there are **three major phases**:

1. Recognition of hands in motion, with some idea of hand persistence (left hand versus right hand)

2. The translation of individual words

3. The interpretation of words into sentences (ASL does not use the structured grammatical rules of English)

![Credit to DeepASL](https://user-images.githubusercontent.com/49175620/76172748-170c8680-6156-11ea-87d0-2045b5b1e373.png)
Credit to DeepASL

**Phase 1** is partially done by the Hand Tracking NN provided by Mediapipe. Mediapipe can only detect the presence and position of hands, but cannot differentiate between the left hand and right hand. Our team has had many discussions on how to tackle this issue, here are some solutions we proposed:

- Detecting the subtle difference in left and right hand finger lengths

- Using the difference in skin color on the front and back of the hand

- Asking the use to calibrate by having their hands to their sides and then associating that position with left and right. Then updating left and right each frame to whichever hand is closest to the last known position of left and right

In the end, the lack of hand persistence did not pose a substantial problem due to the scope of the sign language translation we aimed to complete.

**Phase 2** is in the realm of SigNN. We are to, given the coordinates from phase 1, output which character the user has signed. To completely finish phase 2, the neural network must be able to identify most *words* in ASL. This task would have been far to ambitious for our team, as we do not have the resources to collect all that data necessary. Additionally, we did not have the time to work with time-series of data (more on this later), so we decided to settle for 24/26 sign language characters.

### Mediapipe

**We began our project** by trying to break into the black box that was Mediapipe. While there is some basic documentation, much of what we learned was through experimentation and modification of the source code. After a month, we finally had a good idea of how to modify Mediapipe for our purposes.  

![Diagram of a Mediapipe graph](https://raw.githubusercontent.com/google/mediapipe/master/mediapipe/docs/images/mobile/hand_tracking_mobile.png)

The diagram above shows the Mediapipe graph for detecting and rendering the position of a hand in a video. Mediapipe works through a system of graphs, subgraphs, calculators, and packets.

- Packets: Packets is simply any data structure with a time stamp. In the shown diagram, input_video is the packet that is fed into the graph. The HandLandmark subgraph takes in the packets: NORM_RECT and IMAGE while outputs the packets: NORM_RECT, PRESENCE, and LANDMARKS. Packets are sent between calculators on each frame.

- Graph: A graph is the structure of the entire program. The entire diagram itself is a graph called "Hand Tracking CPU". Graphs are defined in special .pbtxt files and are read at the start of run-time, meaning that they can be modified without recompiling the code

- Calculators: Calculators can have inputs and outputs. They run code on creation, per frame, and on close. An example of a calculator could be one that takes in coordinates of hands and outputs those coordinates being normalized. Another example of a calculator could be one that takes in a tensorflow session and a series of tensors and outputs detections. Some examples of calculators in the diagram are: FlowLimiter, PreviousLoopback, and Gate

- Subgraph: A subgraph is a series of calculators grouped into a graph. Subgraphs have defined inputs and outputs and help to abstract what would be an otherwise over-complicated .pbtxt file and diagram. The subgraphs in the diagram are in blue and are: HandLandmark, HandDetection, and Renderer

[Read more about Mediapipe's structure here](https://mediapipe.readthedocs.io/en/latest/calculator.html)

We were then able to modify the structure of Mediapipe's hand tracking graph to output the coordinates of the hands, which were hiding in HandLandmark as the packet called LANDMARKS.

![Coordinate console logger](https://user-images.githubusercontent.com/49175620/76173312-a1a3b480-615b-11ea-9645-7541a397f169.PNG)

The example shown above is the modification we made to the Multiple Hand Detection CPU graph in order to get output to the console. From there, the output could be piped into a python application for data collection.

![e](https://user-images.githubusercontent.com/49175620/76174303-cb60d980-6163-11ea-933b-f0c0fd09d32f.PNG)

This is an example of the output we modified Mediapipe to output. On the left is the rendered image that Mediapipe normally outputs and on the right is a graph of the series of coordinates that we modified Mediapipe to output.

### OpenPose

**We also looked at OpenPose** as an alternative to Mediapipe. We figured that tracking the arms and the face would significantly increase our accuracy levels. However, most of our collected data did not include the entire upper body. As a result, only ~1,000 / ~6,000 images could be used to train the neural network. After training the neural network we received an unsatisfactory ~77% accuracy. Lastly, OpenPose was prohibitively slow on our laptops and we found that it could not be used to translate sign language in real-time. As a result, we decided to stay with our original framework of Mediapipe.

### Data Collection

**Data collection** was a crucial part of increasing our accuracy. We would have preferred to use publicly available data to train our neural network. However, there were very few sign language data sets and the ones that did exist were of very low quality. As a result, we decided to create our own dataset. While the idea seems simple, it quickly became a problem for us as we took more and more pictures.

 We started with a data set of 100 pictures and over winter break managed to expand to about 500 pictures, impressive at the time. After running testing with the neural network, we found our accuracy was unacceptable, about 60%. Up until this point, all data processing (turning of images into coordinates) was done semi-manually and all data was stored on a hard drive. We played around with the idea of a central website for the group (and maybe some volunteers) to be able to upload their hands, however, we found a much better solution: Goolge Colab and Google Drive.

![Data collection process](https://user-images.githubusercontent.com/49175620/76173861-7a9bb180-6160-11ea-9c34-68871054c4c5.png)

We created a series of Google Colab scripts in order to streamline data collection and processing:

- Database structure: This is a data structure created in order to be able to interact with the same Google Drive folder without having to all share the same account. It allows for downloading, uploading, and listing of files based on character.

- Data collection script [(Mediapipe or Openpose)](https://colab.research.google.com/drive/1o-Ao3_oe0CKDDUJlDplGvhB70wcKaBMA): The script records the webcam and take (n) pictures with a delay of (m) milliseconds. We stuck to taking 30 pictures with 300 millisecond delay. This allows us to create a lot of data quickly. After taking the pictures, it uploads them to a Google Drive database of raw images, each file with a unique name.

- Json creation script ([Mediapipe](https://colab.research.google.com/drive/1RUSUH9C91uZt3MO56yqZMglqol0aPVoX) / [Openpose](https://colab.research.google.com/drive/1zWzXpi_9n0R23Dk_pA3PPs5J_6pk-9Bw)): This script will get a list of all raw images and all raw json files. Then the script will process all raw images that have not been found in the raw json folder. It can do this because one image will create one json file of the same name and upload it to the raw json folder.

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

- Real-time translation of sign language is a computationally difficult task that may not be possible on most consumer-grade hardware. The exception to that is if the software is based on Mediapipe. However, as of time of writing this, Mediapipe has poor documentation and can only track hands (not arms and face).

- One of the largest obstacles to the creating of a neural network for the translation of sign language is the lack of publicly available sign language data. Possible solution could be to crowdsource data collection.

- The collection, management, and processing of training data is a task which cannot feasibly be done manually, and should be streamlined.

- Sign language translation cannot be accurately done in an algorithmic approach as many signs look very similar when it (x, y) coordinate form. It is necessary to use a neural network.

- Coordinate data from pictures is not optimal input to a translation neural network. Accuracy rates increase (60% -> 85% in our case) when each frame has z-scores individually calculated for each set of x and y coordinates.

- We were able to complete real-time translation of characters A-Y (excluding J) with 89% accuracy.

- ASL Characters J and Z along with almost all ASL words are "time-series" signs that will require the use of an LSTM and complex data management infrastructure.

## Future Work

Future work could include the completion of the alphabet with time-series characters J and Z. In order to do so, we would need to modify our Google Colab scripts to accommodate videos. We would also have to create a Mediapipe calculator to store the last n frames of data. One problem that requires research is finding out how many previous frames the neural network should be fed in order to accurately predict most signs. If J and Z are completed with high accuracy, it would be somewhat trivial to expand the neural network to the most popular sign language words. The biggest bottleneck in that case would be lack of available data on sign language words.

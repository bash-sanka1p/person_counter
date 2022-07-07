# Person Counter & Room Automation 

### Requirements
- Arduino Nano
- 16x2 LCD
- 2 Long Range IR Sensors And Emittors or 2 Light Sensors and Lasers.
- Push Button
- Relay Module

### The Idea
In our college and my previous school every day after lunch the peon used to visit every class and count the number of students inside the class for the school records. So looking at this my friend came up with a thought to make a device for the same. In the same semester, we got or mini-project based on IoT. So we thought it was a good time to build it. \
\
At first, we thought of using a face detection model and deploying it to Arduino, so that it could count the number of people and as well as take the attendance of the students. But..... there were two problems with that. None of us knew TinyML so after building the model we might have to spend time learning TinyML another is that the face detection model counts an image too, so anyone could have fooled the system by just showing the picture of a person.  So I decided to use the IR sensor module. But after our whole project was built then I got to know that there were two types of IR, near and far, and I had the near one so its range was not more than around 5cm. I searched for the far IR module in the offline market but it wasn’t available. So for demonstration purposes, I went with the near IR. Laser and Photo sensors could also have been used but we dint went for that. 

### Working

# Trinity
A planet motion emulator implemented with opengl 

![cover](https://github.com/Code-Guy/Trinity/blob/master/Trinity/snapshots/psb%20(5).png?raw=true)
###Motivation
I was rapt in universe and physics when I finished reading the great science fiction "Three Body". So to visualize the virtual three body spinning in my head, I made this little planet simulator. It's pretty simple and naive, but you can see the orbits appearing on the screen, displaying some elegant curves, only you need to do is setting a little parameters in a specified file.

###Feature

 - Simulate planet motion system using leap frog integration
 - Visualize planet orbits by drawing tail lines
 -  Support file-based planet kinematics parameter configuration
```
    // position velocity mass size
	p 25 0 25 -15 0 8.6603 10000 6
	p -25 0 25 -8.6603 0 -15 10000 6
	p 0 0 -18.3 17.321 0 0 10000 6
```
###Framework
Qt + Ork(a lightweight opengl wrapper)
###More Examples
![cover](https://github.com/Code-Guy/Trinity/blob/master/Trinity/snapshots/psb%20(6).png?raw=true)

![cover](https://github.com/Code-Guy/Trinity/blob/master/Trinity/snapshots/psb%20(1).png?raw=true)

![cover](https://github.com/Code-Guy/Trinity/blob/master/Trinity/snapshots/psb%20(4).png?raw=true)

![cover](https://github.com/Code-Guy/Trinity/blob/master/Trinity/snapshots/psb%20(3).png?raw=true)



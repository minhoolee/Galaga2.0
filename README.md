# Galaga2.0
By Min Hoo Lee (July 2015)

## Project Description
This is a summer project that I completed in ten days. 
I created a game that is similar to the popular arcade game, Galaga, but 
different in a few ways.

Galaga is fairly complicated, so many features were unimplemented in Galaga2.0 
(for now?!). That said, my intention was not to create a replica but to have a 
few similar elements which would invoke a feeling of nostalgia. 
To make the game more interesting, I added a few unique features such as 
a multi player mode, sinusoidal movement, shotgun bullets, and multi threading.

My inspiration for this project was my dad, who used to play Galaga so often in 
his childhood that his high score was over 2 million points!

In the not-to-distant future, I hope to add additional features like bosses
and implement deep reinforcement learning in order to beat my game!

## Demos
### Single Player Mode
![single](https://cloud.githubusercontent.com/assets/10465228/24827337/3e537ac0-1bfc-11e7-9c5f-4299f86f74ec.gif)

### Multi Player Mode
![multi](https://cloud.githubusercontent.com/assets/10465228/24827336/3e534d48-1bfc-11e7-8c5d-44568c2199c6.gif)

## Steps for Reproducing

The game is written in C++ and uses SFML for graphics and bazel for building. 
If you wish to run the game on your computer, follow the following steps:

#### Step 1
Run `git clone https://github.com/minhoolee/Galaga2.0.git`

#### Step 2
[Install bazel](https://bazel.build/versions/master/docs/install.html)

#### Step 3
[Install SFML 2.4+](https://www.sfml-dev.org/tutorials/2.4/start-linux.php) 

#### Step 4
Run `bazel run //:galaga-main` in the Galaga2.0 directory

Credit to NAMCO for the sprites' textures (e.g. rocket ship, bugs).

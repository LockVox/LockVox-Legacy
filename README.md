[![Codacy Badge](https://app.codacy.com/project/badge/Grade/808bb4b5cc554ac79c0d9cb77c4910d2)](https://www.codacy.com/gh/ValentinGrim/LockVox/dashboard?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=ValentinGrim/LockVox&amp;utm_campaign=Badge_Grade)
![License](https://img.shields.io/github/license/ValentinGrim/LockVox)
# LockVox Project

Lockvox project aims to create a fully encrypted and open source voice and audio exchange system while keeping the GUI as simple and easy to access as possible.

## Introduction

We are a group of friends and classmate currently in IT engineering school. This project is our annual project as part of our 4th year of university course.
It's dev using Qt opensource, written in C++.

[Documentation](https://valentingrim.github.io/LockVox/html/index.html)

## Current Status

The project actually have :

### Server side

-  A working server running on port 50885
-  Connect to MariaDB / MySQL server
-  Login
-  Register
-  Generating UUID for new user
-  Password stored using sha256 (salt NYI)
-  Channel list
-  User list and isOnline info
-  Simple public message
-  Profile picture

### Client side

-  Simple GUI
-  Connect to server
-  Login
-  Register
-  Retrieve channel and user list
-  Simple public message
-  Profile pictures

### TO DO 

-  [Audio immplementation](https://github.com/hyugzz/LVAudioSession)
-  Message picture implementation
-  Emoji message implementation

## 3rd-party

-  [EMIPLIB](https://github.com/j0r1/EMIPLIB)
-  [MariaDB C & C++ Connector](https://mariadb.com/kb/en/mariadb-connector-c/)
-  [Zedwood C++ SHA256](http://www.zedwood.com/article/cpp-sha256-function)
-  [Qt Open Sources](https://www.qt.io/licensing/)

## Authors

-  Valentin MONNOT (monnot@et.esiea.fr)
-  Hugues MATTEI 
-  Hugo LAVEAU (laveau@et.esiea.fr)
-  Louis BOURGEAIS
-  François LELIEVRE

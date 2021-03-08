# LockVox Project

Lockvox project aims to create a fully encrypted and open source voice and audio exchange system while keeping the GUI as simple and easy to access as possible.

## Introduction

We are a group of friends and classmate currently in IT engineering school. This project is our annual project as part of our 4th year of university course.
It's dev using Qt opensource, written in C++.

## Current Status

The project actually have :

### Server side :

- A working server running on port 50885
- Connect to MariaDB / MySQL server
- Login
- Register
- Generating UUID for new user
- Password stored using sha256
- Channel list
- User list and isOnline info

### Client side :

- Simple GUI
- Connect to server
- Login
- Register
- Retrieve channel and user list

### TO DO : 

- Audio immplementation
- Message implementation using xml form

## 3rd-party :

- [EMIPLIB](https://github.com/j0r1/EMIPLIB)
- [MariaDB C & C++ Connector](https://mariadb.com/kb/en/mariadb-connector-c/)
- [OpenSSL](https://www.openssl.org/)
- [Zedwood C++ SHA256](http://www.zedwood.com/article/cpp-sha256-function)

## Authors :

- Valentin MONNOT (monnot@et.esiea.fr)
- Hugues MATTEI
- Hugo LAVEAU
- Louis BOURGEAIS
- François LELIEVRE


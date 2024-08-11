# Project Description:
This project aims to simulate the behavior of a smart vacuum by implementing a behavior tree in C++. The smart vacuum has 4 more operations that operates sequentialy. First, the charging operation, during this operation the robot checks if it has low battery, and if it does it will go to the charging dock (if it is not already there) and finally it will fully charge. The second operation is obstacle avoidance where the robot will first check if the there is an obstacle in front of it and then avoid that obstacle. The last two operations consists of making the robot move forward and vacuum.

## Functionalities:
To implement this robot using a behavior tree we created three files:
1. bt_vacuum.xml: In this file we specified the structure of the behavior tree, by specifying its different nodes and how they connect to each other.
2. bt_vacuum.cpp: In this file we specified the functionality of each leaf node in the tree, we then created the tree, and ticked the root so that the tree can run once. 
3. CMakeLists.txt: This file was used to make the c++ file executable.

## Usage:
- Clone the repository:
```bash
git clone git@github.com:Mohamad-Nasser-engr/Mohamad-BT-Vacuum.git
```
- build the executable file:
```bash
cd Mohamad-BT-Vacuum/BT-Vacuum/build
```
```bash
cmake ..
```
```bash
make
```
- Run the file:
```bash
./BT-Vacuum 
```

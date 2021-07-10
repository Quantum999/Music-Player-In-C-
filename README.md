Music Player Application:

This is music player written in C++11 using the concepts of inheritance and design patterns.

How to run:
Pre requisites:
Windows OS, CodeBlocks, MinGW compiler installed
The music player can only work on Windows OS because the mciSendString function used in the code is a function of Windows.
1. Go to linker settings of your C++ ide like CodeBlocks and add "-lwinmm" as an option of the linker.
2. Keep all the files i.e the admin.csv, users.csv, songs.csv and the songs folder in the same folder as your C++ ide project.
3. Build and run. 

Users:
Listener
Admin

Functions:
Listener functions:
1. Show Playlist
2. Display Song Details
3. Add a Song to Playlist
4. Remove Song from Playlist
5. Play Playlist
6. Play a Song
7. Like Song
8. Recommended Songs
9. Recommend Similar Songs
10. Play songs of a genre
11. Logout

Admin functions:
1. Show Database
2. Display Song Details
3. Add a Song to Database
4. Remove Song from Database
5. Play All Songs
6. Play a Song
11. Logout

To use a function, enter the index number in front of the function.

Design patterns used(See the class diagram to see how they have been used):
Factory: for two different types of users i.e. admin and listener
Singleton: to prevent the client from allowing multiple users to login at the same time.
State: for different states of the music player namely Playing, Paused, Stopped.

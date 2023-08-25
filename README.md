# Tic_Tac_Toe
- Multiplayer Tic-Tac-Toe game using C++ socket programming and accessible over the internet
- Compile your C++ code with -lws2_32 library, and then use 'ngrok' to expose your local server.

##### Server
```cpp
g++ server.cpp -o "server" -lws2_32
```
##### Client
```cpp
g++ client.cpp -o "client" -lws2_32
```

- Share the ngrok-generated public URL with others, enabling them to connect and play the game remotely

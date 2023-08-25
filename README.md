# Tic_Tac_Toe
- To create a multiplayer Tic-Tac-Toe game using C++ socket programming and make it accessible over the internet
- Compile your C++ code with the -lws2_32 flag, and then use 'ngrok' to expose your local server.

##### Server
```cpp
g++ server.cpp -o "server" -lws2_32
```
##### Client
```cpp
g++ client.cpp -o "client" -lws2_32
```

- Share the ngrok-generated public URL with others, enabling them to connect via a web browser or HTTP client and play the game remotely

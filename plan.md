Music_Sync

1. Player
- start : args - filename, time(opt)
- playAt  : args - time(opt)
- pause : 
- getTime : 


2. Client
- constructor : ip, portargs - type(Bluetooth/ TCP) - creating a connectivity object
- start()   
    - connect to server
    - create the Player object
    - calclate and store the RTT
    - ask for synchronization
    - player.play()
    - Then, listen simulataneously on the command line and the ServerProtocol object
      for instructions

- synchronise
- play
- plause



3. Server
- constructor : port, type, fileName
- start()
    - create the player
    - player.play
    - create the connectivity object and start listening
    - 
    - 







// Starts in a new thread and takes instructions from the command
// line. Sends messages to the Client(main) Thread
3. CommandLineListener
private:
- prompt()
- parse()

public:
- start()
    - prompt user
    - take input
    - parse command
    - send messages to the main thread

4. interface Connectivity --- TCPConnectivity, BluetoothConnectivity
   - TCPConnectivity
     Constructor
     Constructor(Ip, port)
- send
- recieve, readline
- ServerNode connect
- scan
- ClientProtocol listen 





-- Server -> ClientNode.synchronize -> connectivity.sendmessage
-- Client -> ServerNode.

-- 
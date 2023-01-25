# UDS message generator

Simple project I created for personal learning which is trying to mimic the framework of UDSonCAN protocol.

## UDS.h

Includes definitions for SIDs and DIDs (not all valid DIDs present).
Includes functions to generate single frames and flow control frames.

Implementations of major ISO-TP frames aswell as a general method for Negative Response frames are available for use.

`f` is a global array that is supposed to act as the frame throughout every script. This is feasible since the CAN protocol allows only the transmission of a single frame at a time through the BUS and this project is trying to imitate UDSonCAN.

Every frame is required to follow the UDS message format specified within ISO 14229 - 1. The basic message format is as follows :

`SID` pointer can be used to reference any valid UDS service identifiers.
Similarly, `DID` pointer available for data identifiers

> Note : DID list provided within UDS.h are from the OEM specific chart provided to me during the development of this project. You can replace them with your own DIDs.

#### Methods

```C
display_frame(uint8_t *frame)
```

Display any 8 byte frame in the terminal

```C
single_frame_did(uint8_t *frame, uint8_t pci_length, uint8_t SID, uint16_t DID, uint64_t data_param)
```

Generate a single frame (request frame) that contains a DID

```C
single_frame_subf(uint8_t *frame, uint8_t pci_length, uint8_t SID, uint8_t subfunc, uint8_t *data_param)
```

Generate a single frame (request frame) that contains a subfunction

```C
flow_control_frame(uint8_t *frame, uint8_t mode, uint8_t blockSize, uint8_t separation_time)
```

Generate a flow control frame (CAN-TP/ISO-TP)

```C
negative_response_frame(uint8_t *frame, uint8_t NRC)
```

Generate a negative response frame (usage only within server scripts)

## UDS_client.cpp

Acts as the Tester tool.

General definitions for each type of frame are present.
Manipulate this data and call required functions within UDS.h to produce a required 8 byte frame.

The text file message.txt is generated which tries to imitate the BUS and is being used here as a medium of frame transfer between the client and server source files. Frames can be transmitted on the bus through the `writeFrame()`method.

The global variable `DATA` is for payload to the `single_frame_did` method, whereas `*DATA_ARRAY` is to send payload to the `single_frame_subf` method.
They both serve the same purpose (any data payload to be sent), function definitions for these two take in different datatypes for payload arguments and hence two seperate definitions were required.

`suppressPosRspMsgIndicationBit` if made 1, will suppress the response from the server script, i.e, there is no response from the mimicked ECU.

#### Methods

```C
 writeFrame()
```

Writes the UDS frame array into the messages.txt file.

```C
main()
```

is responsible for how the ECU functions, i.e, the order in which messages are sent and their contents.

> NOTE : Only one message can be sent onto the 'BUS' upon the execution of this script. main() has been programmed to write onto the bus only once.

## UDS_server.cpp

Acts as the ECU.

The script is able to track active sessions using the readFrame() function.
Most global variables are just pretend registers to store basic ECU information. Use them as required, names are self-explanatory.

The whole purpose of the server script is to generate and display a response frame in feedback to the most recent frame present within the message.txt file, i.e, the BUS using the readFrame() method.

In accordance with this, very primitive implementations of certain positive and negative responses have been implemented.

`currentSession` is a variable that keeps track of the active session.
`stored_SID` array is a list of all SIDs that this server will support.
`stored_messages` is a structure that imitates a register bank that stores all valid DIDs aswell as associated data.

#### Methods

```C

void service_not_supported();
void service_present();
void session_check_fail();
void session_change_pass();
void session_change_fail();

```

Are all different response types to a given request frame.

`readFrame()`
Reads from the message.txt file, i.e, the _'BUS'_ and stores it in the global frame `f`.

`writeSession()`
Writes the active session present in the `currentSession` variable into session.txt. This is done mainly to retain the active session everytime the script is compiled.

`readSession()`
Reads the active session from session.txt into `currentSession`.

`main()` contains the different checks/cases to implement required behaviours for the server.

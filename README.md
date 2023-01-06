# UDS message generator

## UDS.h

Includes definitions for SIDs and DIDs (not all valid DIDs present).
Includes functions to generate single frames and flow control frames.

Implementations of major ISO-TP frames aswell as a general method for Negative Response frames are available for use.

## UDS_client.cpp

Acts as the Tester tool.

General definitions for each type of frame are present.
Manipulate this data and call required functions within UDS.h to produce a required 8 byte frame.

The text file message.txt is generated which tries to imitate the BUS and is being used here as a medium of frame transfer between the client and server source files. Frames can be transmitted on the bus through the writeFrame() method.

## UDS_server.cpp

Acts as the ECU.

The script is able to track active sessions using the readFrame() function.
Most global variables are just pretend registers to store basic ECU information. Use them as required, names are self-explanatory.

The whole purpose of the server script is to generate and display a response frame in feedback to the most recent frame present within the message.txt file, i.e, the BUS using the readFrame() method.

In accordance with this, very primitive implementations of certain positive and negative responses have been implemented.

NOTE : Current iteration provides positive responses ONLY if the active session is extendedDiagnosticsSession (0x03).

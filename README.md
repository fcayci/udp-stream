# udp-stream

Simple UDP stream implementation using sockets.

- Client grabs camera data, compresses to JPEG using OpenCV and sends over using udp sockets. Frame rate can be set if needed.
- Server waits for packet and displays it on screen.

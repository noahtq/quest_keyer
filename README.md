# Quest Keyer
A better chroma keyer for commercial VFX workflows

## Tech Stack
### Backend
- C++ & CMake
- Propriatary Quest Image Sequence Library for Image Sequence Functionality
- OpenCV for Computer Vision and Image Manipulation
- Drogon to create REST API

### Frontend
- Electron (Javascript, HTML, CSS)
- Bootstrap for Styling

## Versions (Current 0.0.0)

## Communication Between Frontend and Backend
The Electron frontend client communicates with the Drogon backend server over a local websocket. The server returns JSON objects in the form of  
- Result - The status of the request. *ok* means the request and the backend functionality performed as expected, anything else is an error.
- Message - This returns a message that can be displayed by the frontend. This could be a message for the user or it could be an error message.
- Data... - The remaining JSON fields contain any data that the backend needs to send to the frontend

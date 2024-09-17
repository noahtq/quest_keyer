# Quest Keyer
Computer vision and C++ portfolio project by Noah Turnquist.  
Chromatic image and video keyer.  
The chroma keying is done using Petro Vlahos' ultimatte keying technique.

## Tech Stack
### Backend
- C++ & CMake
- Propriatary [Quest Image Sequence Library](https://github.com/noahtq/quest_image_seq_lib) for Image Sequence Functionality
- [OpenCV](https://opencv.org/) for Computer Vision and Image Manipulation
- [Drogon](https://github.com/drogonframework/drogon) to create REST API
- [Nlohmann/json](https://github.com/nlohmann/json)
- Developed in [CLion](https://www.jetbrains.com/clion/)

### Frontend
- [Electron](https://www.electronjs.org/) (Javascript, HTML, CSS)
- [Bootstrap](https://getbootstrap.com/) for Styling

## Building for Developers
1. The first step to building this application is to clone the repo. Navigate to the directory where you would like to place the repo and execute the following, then navigate into the directory:
```
$ git clone https://github.com/noahtq/quest_keyer.git && cd quest_keyer
```
2. Next build the [Frontend](quest_keyer_ui/frontend_build_doc.md) by following this documentation
3. Build the [Backend](quest_keyer_backend/backend_build_doc.md) by following this documentation
4. Once both the frontend and backend have been setup correctly, running the application is straightforward. First start the backend REST API `$ <backend build directory>/quest_keyer_backend`. Then start the frontend by navigating to the UI directory and running `$ npm run start`
5. Finally, test that the application is working correctly by downloading the test media located in this [Google Drive folder](https://drive.google.com/drive/folders/1EenDnNi4WRRIemVCvxgemVoAQojeklv0?usp=sharing). Try keying an image sequence and then exporting it.

## Versions (Current 0.0.1)
### 0.0.1
- Implemented core functionality for both frontend and backend
- Quest Keyer very simple keying functionality
- Quest API end points for initializing backend, opening image sequence, exporting image sequence, keying image sequence.
- Simple, stylish frontend with Bootstrap

## Communication Between Frontend and Backend
The Electron frontend client communicates with the Drogon backend server over a local websocket. The server returns JSON objects in the form of  
- Result - The status of the request. *ok* means the request and the backend functionality performed as expected, anything else is an error.
- Message - This returns a message that can be displayed by the frontend. This could be a message for the user or it could be an error message.
- Data... - The remaining JSON fields contain any data that the backend needs to send to the frontend

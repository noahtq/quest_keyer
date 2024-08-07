const { app, BrowserWindow, ipcMain, dialog } = require('electron/main')
var exec = require('child_process').execFile;
const path = require('node:path')

async function handleFileOpen() {
  const { canceled, filePaths } = await dialog.showOpenDialog()
  if (!canceled) {
    return filePaths[0]
  }
}

// async function handleBackendTest(event, num) {
//   backend_path = path.join(__dirname, 'quest_keyer_0.0.1')
//   let promise = new Promise((resolve, reject) => {
//     exec(backend_path, [num], (err, data) => {
//       // console.log(data.toString())
//       if (err) reject(err)
//       else resolve(data)
//     })
//   }) 
//   return promise
// }

async function backendInit(event) {

  const url = "http://localhost:5555/questkeyerapi/keyerapi/init"
  try {
    const response = await fetch(url)
    const data = await response.json()
    return data
  } catch (error) {
    console.error(error.message)
  }
}

async function handleBackendSend(event) {

  const url = "http://localhost:5555/questkeyerapi/keyerapi/chromakey?keyr=15&keyg=30&keyb=100&threshold=255"
  try {
    const response = await fetch(url)

    const data = await response.json()
    if (data.result === 'ok') {
      console.log("Initialized backend")
      console.log("Message:" + data.message)
    } else {
      console.log("An error occured while initializing backend: " + data.response)
    }
    console.log("Message:" + data.message)
  } catch (error) {
    console.error(error.message)
  }
}

function createWindow () {
  const mainWindow = new BrowserWindow({
    webPreferences: {
      preload: path.join(__dirname, 'preload.js')
    }
  })
  mainWindow.loadFile('index.html')
}

app.whenReady().then(() => {
  ipcMain.handle('dialog:openFile', handleFileOpen)
  ipcMain.handle('testBackendSend', handleBackendSend)
  createWindow()

  backendInit()

  app.on('activate', () => {
    if (BrowserWindow.getAllWindows().length === 0) {
      createWindow()
    }
  })
})

app.on('window-all-closed', () => {
  if (process.platform !== 'darwin') {
    app.quit()
  }
})
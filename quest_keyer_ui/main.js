const { app, BrowserWindow, ipcMain, dialog } = require('electron/main')
var exec = require('child_process').execFile;
const path = require('node:path')

async function handleFileOpen() {
  const { canceled, filePaths } = await dialog.showOpenDialog()
  if (!canceled) {
    return filePaths[0]
  }
}

async function handleBackendTest(event, num) {
  backend_path = path.join(__dirname, 'quest_keyer_0.0.1')
  let promise = new Promise((resolve, reject) => {
    exec(backend_path, [num], (err, data) => {
      // console.log(data.toString())
      if (err) reject(err)
      else resolve(data)
    })
  }) 
  return promise
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
  ipcMain.handle('testBackend', handleBackendTest)
  createWindow()

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
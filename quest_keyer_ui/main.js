const { app, BrowserWindow, ipcMain, dialog } = require('electron/main')
const path = require('node:path')

async function handleFileSearch() {
  const { canceled, filePaths } = await dialog.showOpenDialog()
  if (!canceled) {
    return filePaths[0]
  }
}

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

async function handleOpenSequence(event, sequencePath) {
  sequenceModified = sequencePath.replace("%", "%25")
  const url = `http://localhost:5555/questkeyerapi/keyerapi/open?path=${sequenceModified}`
  try {
    const response = await fetch(url)

    const data = await response.json()
    if (data.result === 'ok') {
      console.log("Server received image")

      let updateData = {
        originalProxyPath: data.proxy-path,
        keyedProxyPath: "",
        frameLength: 
      }
      viewerState.originalSeqPath = data.proxy-path //TODO: update this
    } else {
      console.log("An error occured while while sending to backend: " + data.result)
    }
    console.log("Message: " + data.message)
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
  ipcMain.handle('dialog:searchFile', handleFileSearch)
  ipcMain.handle('keyer:initBackend', handleBackendSend)
  ipcMain.handle('keyer:openSequence', handleOpenSequence)
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
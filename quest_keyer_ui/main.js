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
        status: data.result,
        originalProxyPath: data["orig-proxy-path"],
        keyedProxyPath: data["keyer-proxy-path"],
        currentFrame: 1,
        frameLength: data["frame-count"]
      }

      return updateData;
    } else {
      console.log("An error occured while while sending to backend: " + data.result)
    }
    console.log("Message: " + data.message)
  } catch (error) {
    console.error(error.message)
  }
}

async function handleChromaKey(event, red, green, blue, threshold) {

  const url = `http://localhost:5555/questkeyerapi/keyerapi/chromakey?keyr=${red}&keyg=${green}&keyb=${blue}&threshold=${threshold}`
  try {
    const response = await fetch(url)

    const data = await response.json()
    if (data.result === 'ok') {
      console.log("Successfully keyed image sequence")

      let updateData = {
        status: data.result,
        keyedProxyPath: data["keyer-proxy-path"],
      }
      return updateData
    } else {
      console.log("An error occured while keying image: " + data.result)
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
  ipcMain.handle('keyer:openSequence', handleOpenSequence)
  ipcMain.handle('keyer:chromaKey', handleChromaKey)
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
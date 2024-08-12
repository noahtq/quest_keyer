const { app, BrowserWindow, ipcMain, dialog, screen } = require('electron/main')
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
    let updateData = {
      status: data.result,
      message: data.message
    }
    if (data.result === 'ok') {
      console.log("Server received image")

      updateData["originalProxyPath"] = data["orig-proxy-path"]
      updateData["keyedProxyPath"] = data["keyer-proxy-path"]
      updateData["currentFrame"] = 1
      updateData["frameLength"] = data["frame-count"]
    } else {
      console.log("An error occured while while sending to backend: " + data.result)
    }
    console.log("Message: " + data.message)
    return updateData
  } catch (error) {
    console.error(error.message)
  }
}

async function handleExportSequence(event, export_path, red, green, blue, threshold) {
  sequenceModified = export_path.replace("%", "%25")
  const url = `http://localhost:5555/questkeyerapi/keyerapi/export?path=${sequenceModified}&keyr=${red}&keyg=${green}&keyb=${blue}&threshold=${threshold}`
  try {
    const response = await fetch(url)

    const data = await response.json()
    if (data.result === 'ok') {
      console.log("Succesfully keyed image")
    } else {
      console.log("An error occured while while exporting image sequence: " + data.result)
    }
    let updateData = {
      status: data.result,
      message: data.message
    }
    console.log("Message: " + data.message)
    return updateData
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

function createWindow (winWidth, winHeight) {
  const mainWindow = new BrowserWindow({
    width: winWidth,
    height: winHeight,
    webPreferences: {
      preload: path.join(__dirname, 'preload.js')
    }
  })
  mainWindow.loadFile('index.html')
}

app.whenReady().then(() => {
  ipcMain.handle('dialog:searchFile', handleFileSearch)
  ipcMain.handle('keyer:openSequence', handleOpenSequence)
  ipcMain.handle('keyer:exportSequence', handleExportSequence)
  ipcMain.handle('keyer:chromaKey', handleChromaKey)
  const primaryDisplay = screen.getPrimaryDisplay()
  width = primaryDisplay.workAreaSize.width
  height = primaryDisplay.workAreaSize.height
  createWindow(width, height)
  backendInit()

  app.on('activate', () => {
    if (BrowserWindow.getAllWindows().length === 0) {
      createWindow(width, height)
      backendInit()
    }
  })
})

app.on('window-all-closed', () => {
  if (process.platform !== 'darwin') {
    app.quit()
  }
})
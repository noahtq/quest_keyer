const { contextBridge, ipcRenderer } = require('electron/renderer')

contextBridge.exposeInMainWorld('electronAPI', {
  searchFile: () => ipcRenderer.invoke('dialog:searchFile'),
  sendBackendTest: () => ipcRenderer.invoke('keyer:initBackend'),
  openSequence: (path) => ipcRenderer.invoke('keyer:openSequence', path),
  chromaKey: (red, green, blue, threshold) => ipcRenderer.invoke('keyer:chromaKey', red, green, blue, threshold)
})
const { contextBridge, ipcRenderer } = require('electron/renderer')

contextBridge.exposeInMainWorld('electronAPI', {
  openFile: () => ipcRenderer.invoke('dialog:openFile'),
  backendTest: () => ipcRenderer.invoke('testBackend'),
  sendBackendTest: () => ipcRenderer.invoke('testBackendSend')
})
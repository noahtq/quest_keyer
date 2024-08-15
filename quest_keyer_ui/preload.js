const { contextBridge, ipcRenderer } = require('electron/renderer')

contextBridge.exposeInMainWorld('electronAPI', {
  searchFile: () => ipcRenderer.invoke('dialog:searchFile'),
  sendBackendTest: () => ipcRenderer.invoke('keyer:initBackend'),
  openSequence: (path) => ipcRenderer.invoke('keyer:openSequence', path),
  exportSequence: (export_path, red, green, blue, threshold, applyDespill) => ipcRenderer.invoke('keyer:exportSequence', export_path, red, green, blue, threshold, applyDespill),
  chromaKey: (red, green, blue, threshold, applyDespill) => ipcRenderer.invoke('keyer:chromaKey', red, green, blue, threshold, applyDespill)
})
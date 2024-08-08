const inputSearchBtn = document.getElementById('search-input-btn')
const filePathElement = document.getElementById('input-path')
const sequenceLoadBtn = document.getElementById('load-input-btn')

let viewerState = {
    originalProxyPath: "",
    keyedProxyPath: "",
    currentFrame: 1,
    frameLength: -1
  }

inputSearchBtn.addEventListener('click', async () => {
    const filePath = await window.electronAPI.searchFile()
    filePathElement.value = filePath
})

sequenceLoadBtn.addEventListener('click', async () => {
    if (filePathElement.value.length > 0) {
        const filePath = await window.electronAPI.openSequence(filePathElement.value)
    }
})

function replaceFramePaddingWithFrame(path, frameNum) {
    const regex = "%\d\dd"
    let framePadding = Number(path.search(regex).split("%")[1].split("d")[0])
    let numberStr = ""
    for (let i = 0; i < framePadding - 1; i++) {
        numberStr += "0"
    }
    numberStr += Number(frameNum)
    return path.replace(regex, frameNum)
}

function viewerUpdate(updateData) {
    viewerState.originalProxyPath = updateData.originalProxyPath
    viewerState.keyedProxyPath = updateData.keyedProxyPath
    viewerState.frameLength = updateData.frameLength
}
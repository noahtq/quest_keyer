const inputSearchBtn = document.getElementById('search-input-btn')
const filePathElement = document.getElementById('input-path')
const sequenceLoadBtn = document.getElementById('load-input-btn')
const origImg = document.getElementById('original-img')
const keyedImg = document.getElementById('keyed-img')
const frameNum = document.getElementById('frame-num')

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
        const updateData = await window.electronAPI.openSequence(filePathElement.value)
        if (updateData.status === 'ok') {
            viewerUpdate(updateData)
        }
    }
})

function replaceFramePaddingWithFrame(path, frameNum) {
    // TODO: This will have to be fixed to work when the current frame is multiple digits
    const regex = /%\d\dd/
    const framePaddingIndex = path.search(regex)
    const framePadding = Number(path.slice(framePaddingIndex + 1, framePaddingIndex + 3))
    let numberStr = ""
    for (let i = 0; i < framePadding - 1; i++) {
        numberStr += "0"
    }
    numberStr += Number(frameNum)
    return path.replace(regex, numberStr)
}

function viewerUpdate(updateData) {
    viewerState.originalProxyPath = updateData.originalProxyPath
    viewerState.keyedProxyPath = updateData.keyedProxyPath
    viewerState.frameLength = updateData.frameLength
    const origFrame = replaceFramePaddingWithFrame(viewerState.originalProxyPath, viewerState.currentFrame)
    const keyedFrame = replaceFramePaddingWithFrame(viewerState.keyedProxyPath, viewerState.currentFrame)
    console.log(origFrame)
    console.log(keyedFrame)

    origImg.src = origFrame
    keyedImg.src = keyedFrame
    frameNum.innerHTML = viewerState.currentFrame
}
const playbackFPS = 30
const alertPopupDuration = 5000
const alertFadeDuration = 500

// IO elements
const inputSearchBtn = document.getElementById('search-input-btn')
const outputSearchBtn = document.getElementById('search-output-btn')
const filePathElement = document.getElementById('input-path')
const outputPathElement = document.getElementById('output-path')
const sequenceLoadBtn = document.getElementById('load-input-btn')
const sequenceExportBtn = document.getElementById('export-btn')

// Viewer elements
const origImg = document.getElementById('original-img')
const keyedImg = document.getElementById('keyed-img')

// Playback elements
const frameNum = document.getElementById('frame-num')
const startBtn = document.getElementById('start-btn')
const prevFrameBtn = document.getElementById('prev-frame-btn')
const playBtn = document.getElementById('play-btn')
const nextFrameBtn = document.getElementById('next-frame-btn')
const endBtn = document.getElementById('end-btn')

// Keyer Elements
const colorPicker = document.getElementById('color-picker')
const thresholdSlider = document.getElementById('key-threshold')
const despillCheckbox = document.getElementById('despill-checkbox')

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

outputSearchBtn.addEventListener('click', async () => {
    const filePath = await window.electronAPI.searchFile()
    outputPathElement.value = filePath
})

sequenceLoadBtn.addEventListener('click', async () => {
    if (filePathElement.value.length > 0) {
        createAlert("", "Opening Image Sequence")
        const updateData = await window.electronAPI.openSequence(filePathElement.value)
        createAlert(updateData.status, updateData.message)
        if (updateData.status === 'ok') {
            viewerUpdate(updateData)
        }
    }
})

sequenceExportBtn.addEventListener('click', async () => {
    if (viewerState.frameLength > 0 && outputPathElement.value.length > 0) {
        createAlert("", "Exporting Image Sequence")
        const color = colorPicker.value
        const r = parseInt(color.substr(1,2), 16)
        const g = parseInt(color.substr(3,2), 16)
        const b = parseInt(color.substr(5,2), 16)
        const updateData = await window.electronAPI.exportSequence(outputPathElement.value, r, g, b, thresholdSlider.value, despillCheckbox.checked)
        createAlert(updateData.status, updateData.message)
    }
})

colorPicker.addEventListener('input', async () => {
    if (viewerState.frameLength > 0) {
        const color = colorPicker.value
        const r = parseInt(color.substr(1,2), 16)
        const g = parseInt(color.substr(3,2), 16)
        const b = parseInt(color.substr(5,2), 16)
        const updateData = await window.electronAPI.chromaKey(r, g, b, thresholdSlider.value, despillCheckbox.checked)
        viewerUpdate(updateData)
    }
})

thresholdSlider.addEventListener('change', async () => {
    if (viewerState.frameLength > 0) {
        const color = colorPicker.value
        const r = parseInt(color.substr(1,2), 16)
        const g = parseInt(color.substr(3,2), 16)
        const b = parseInt(color.substr(5,2), 16)
        const updateData = await window.electronAPI.chromaKey(r, g, b, thresholdSlider.value, despillCheckbox.checked)
        viewerUpdate(updateData)
    }
})

despillCheckbox.addEventListener('click', async () => {
    if (viewerState.frameLength > 0) {
        const color = colorPicker.value
        const r = parseInt(color.substr(1,2), 16)
        const g = parseInt(color.substr(3,2), 16)
        const b = parseInt(color.substr(5,2), 16)
        if (r > 0 || g > 0 || b > 0) {
            const updateData = await window.electronAPI.chromaKey(r, g, b, thresholdSlider.value, despillCheckbox.checked)
            viewerUpdate(updateData)
        } else {
            despillCheckbox.checked = false
        }
    }
})

prevFrameBtn.addEventListener('click', () => {
    if (viewerState.frameLength > 0 && viewerState.currentFrame > 1) {
        let updateData = {
            originalProxyPath: viewerState.originalProxyPath,
            keyedProxyPath: viewerState.keyedProxyPath,
            currentFrame: viewerState.currentFrame - 1
        }
        viewerUpdate(updateData)
    }
})

function getNextFrame() {
    let updateData = {
        originalProxyPath: viewerState.originalProxyPath,
        keyedProxyPath: viewerState.keyedProxyPath,
        currentFrame: viewerState.currentFrame + 1
    }
    viewerUpdate(updateData)
}

nextFrameBtn.addEventListener('click', () => {
    if (viewerState.frameLength > 0 && viewerState.currentFrame < viewerState.frameLength) {
        getNextFrame()
    }
})

startBtn.addEventListener('click', () => {
    if (viewerState.frameLength > 0 && viewerState.currentFrame !== 1) {
        let updateData = {
            originalProxyPath: viewerState.originalProxyPath,
            keyedProxyPath: viewerState.keyedProxyPath,
            currentFrame: 1
        }
        viewerUpdate(updateData)
    }
})

endBtn.addEventListener('click', () => {
    if (viewerState.frameLength > 0 && viewerState.currentFrame < viewerState.frameLength) {
        let updateData = {
            originalProxyPath: viewerState.originalProxyPath,
            keyedProxyPath: viewerState.keyedProxyPath,
            currentFrame: viewerState.frameLength
        }
        viewerUpdate(updateData)
    }
})

let currentlyPlaying = false
const delay = ms => new Promise(res => setTimeout(res, ms));
playBtn.addEventListener('click', async () => {
    if (viewerState.frameLength > 0) {
        if (!currentlyPlaying) {
            currentlyPlaying = true
            while (viewerState.currentFrame < viewerState.frameLength && currentlyPlaying) {
                getNextFrame()
                let milleDelay = 1000 / playbackFPS
                await delay(milleDelay)
            }
            currentlyPlaying = false
        } else {
            currentlyPlaying = false
        }
    }
})

function replaceFramePaddingWithFrame(path, frameNum) {
    const regex = /%\d\dd/
    const framePaddingIndex = path.search(regex)
    const framePadding = Number(path.slice(framePaddingIndex + 1, framePaddingIndex + 3))
    let numberStr = ""
    for (let i = 0; i < framePadding - String(frameNum).length; i++) {
        numberStr += "0"
    }
    numberStr += Number(frameNum)
    return path.replace(regex, numberStr)
}

function viewerUpdate(updateData) {
    const CurrentTime = new Date()
    if ("currentFrame" in updateData) {
        viewerState.currentFrame = updateData.currentFrame
        frameNum.innerHTML = viewerState.currentFrame
    }
    if ("originalProxyPath" in updateData) {
        viewerState.originalProxyPath = updateData.originalProxyPath
        let origFrame = replaceFramePaddingWithFrame(viewerState.originalProxyPath, viewerState.currentFrame)
        origFrame += '?'
        origFrame += CurrentTime.getTime()
        origImg.src = origFrame
    }
    if ("keyedProxyPath" in updateData) {
        viewerState.keyedProxyPath = updateData.keyedProxyPath
        let keyedFrame = replaceFramePaddingWithFrame(viewerState.keyedProxyPath, viewerState.currentFrame)
        keyedFrame += '?'
        keyedFrame += CurrentTime.getTime()
        keyedImg.src = keyedFrame
    }
    if ("frameLength" in updateData) viewerState.frameLength = updateData.frameLength
}

async function createAlert(type, message) {
    let newAlert = document.createElement("div")
    newAlert.classList.add("alert")
    newAlert.classList.add("quest-alert")
    if (type === "ok") {
        newAlert.classList.add("alert-success")
    } else if (type === "fail") {
        newAlert.classList.add("alert-danger")
    } else {
        newAlert.classList.add("alert-primary")
    }
    newAlert.innerHTML = message
    document.body.appendChild(newAlert)
    requestAnimationFrame(function() {
        newAlert.style.opacity = "100%"
    })
    await delay(alertFadeDuration)
    await delay(alertPopupDuration)
    requestAnimationFrame(function() {
        newAlert.style.opacity = "0%"
    })
    await delay(alertFadeDuration)
    newAlert.remove()
}

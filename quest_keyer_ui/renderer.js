const btn = document.getElementById('btn')
const filePathElement = document.getElementById('filePath')
const btn2 = document.getElementById('backend-test-btn')
const output = document.getElementById('backend-test')
const btn3 = document.getElementById('backend-test-send-btn')
const output2 = document.getElementById('backend-test-send-data')

// btn.addEventListener('click', async () => {
//     const filePath = await window.electronAPI.openFile()
//     filePathElement.innerText = filePath
// })

btn2.addEventListener('click', async () => {
    const data = await window.electronAPI.backendTest()
    output.innerText = data.result
})

btn3.addEventListener('click', async () => {
    const data = await window.electronAPI.sendBackendTest()
    output2.innerText = data.key_r + " " + data.key_g + " " + data.key_b + " " + data.threshold
})
const btn = document.getElementById('btn')
const filePathElement = document.getElementById('filePath')
const btn2 = document.getElementById('btn-2')
const number = document.getElementById('number-value')
const output = document.getElementById('output')

btn.addEventListener('click', async () => {
    const filePath = await window.electronAPI.openFile()
    filePathElement.innerText = filePath
})

btn2.addEventListener('click', async () => {
    const data = await window.electronAPI.backendTest(number.value)
    output.innerText = data
})
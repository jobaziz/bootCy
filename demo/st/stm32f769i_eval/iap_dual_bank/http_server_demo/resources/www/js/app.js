// Example, fetching current firmware version
// In production, the variable current version can be referenced
// to the actual version in production. Using dummy value for now.

//let currentVersion = getVersion();
//document.querySelector('#currentVersion').innerHTML = currentVersion;
// Scenario descriptions
const scenarioOne = "<p class=\"lead mb-4\">Current application firmware can process images generated with a simple application binary (not encrypted) and an integrity tag</p>";
const scenarioTwo= "<p class=\"lead mb-4\">Current application firmware can process images generated with a cipher application binary (encrypted) and an authentication tag</p>";
const scenarioThree = "<p class=\"lead mb-4\">Current application firmware can process images generated with a cipher application binary (encrypted) and a signature</p>";

document.getElementById("scenarioDesc").innerHTML = scenarioOne;

getVersion();

// Accessing metadata about the currently selected file.
let files = null;

async function updateSize() {
    let nBytes = 0,
        oFiles = this.files, // How many files
        nFiles = oFiles.length; // Length of file in bytes
    for (let nFileId = 0; nFileId < nFiles; nFileId++) {
        nBytes += oFiles[nFileId].size; // accumulator to get the final size
    }
    let sOutput = nBytes + " bytes"; // Formatting a nice message
    // optional code for multiples approximation
    const aMultiples = ["KiB", "MiB", "GiB", "TiB", "PiB", "EiB", "ZiB", "YiB"];
    for (nMultiple = 0, nApprox = nBytes / 1024; nApprox > 1; nApprox /= 1024, nMultiple++) {
        sOutput = nApprox.toFixed(3) + " " + aMultiples[nMultiple] + " (" + nBytes + " bytes)";
    }

    files = this.files

    let ext = this.files[0].name.includes('.img');
    if (ext) {
        //document.getElementById("fileName").innerHTML = this.files[0].name; // Inserting the file name to the DOM
        document.getElementById("fileSize").innerHTML = sOutput; // Inserting the file size to the DOM
        document.getElementById("failure").innerHTML = '';
        document.getElementById('submit').classList.remove("disabled");
        document.getElementById('submit').classList.remove("btn-outline-secondary");
        document.getElementById('submit').classList.add("btn-success");

        document.getElementById('step1').classList.remove("list-group-item-success");
        document.getElementById('step2').classList.add("list-group-item-success");
        document.getElementById('step3').classList.remove("list-group-item-success");

    } else {
        document.getElementById('success').innerHTML = ''
        document.getElementById("failure").innerHTML = 'Unknown filetype. Please upload a binary (.img) file.';
        document.getElementById('submit').classList.remove("btn-success");
        document.getElementById('submit').classList.add("btn-outline-secondary");
        document.getElementById('submit').classList.add("disabled");
        this.files = null;
    }
}

function sendFile(file) {
    const uri = "/upload";
    const xhr = new XMLHttpRequest();
    const fd = new FormData();

    xhr.open("POST", uri, true);

    xhr.onreadystatechange = function () {
        
        if (xhr.readyState === XMLHttpRequest.DONE) {
            let status = xhr.status;
            if (status >= 200 && status < 400) {
                let jsonResponse = JSON.parse(xhr.responseText);
                console.log(jsonResponse);
                document.getElementById('success').innerHTML = 'Uploaded. Page will reload in 5 seconds...';
            } else {
                document.getElementById('step3').classList.remove("list-group-item-success");
                document.getElementById('submit').classList.remove("btn-success");
                document.getElementById('submit').classList.add("btn-outline-secondary");
                document.getElementById('submit').classList.add("disabled");
                document.getElementById("failure").innerHTML = 'An error occurred uploading file.';
            }
        } else {
            document.getElementById('step3').classList.remove("list-group-item-success");
            document.getElementById('submit').classList.remove("btn-success");
            document.getElementById('submit').classList.add("btn-outline-secondary");
            document.getElementById('submit').classList.add("disabled");
            document.getElementById("failure").innerHTML = 'An error occurred uploading file.';
        }
    };

    fd.append('myFile', file)

    // Initiate a multipart/form-data upload
    xhr.send(fd)
}

function getVersion() {
    const uri = "/data.json";
    const xhr = new XMLHttpRequest();

    xhr.open("GET", uri, true);

    xhr.onreadystatechange = function () {
        if (xhr.readyState === XMLHttpRequest.DONE) {
            let status = xhr.status;
            if (status === 0 || (status >= 200 && status < 400)) {
                let jsonResponse = JSON.parse(xhr.responseText);
                let scenarioNo = parseInt(jsonResponse.data.scenario);

                document.getElementById("currentVersion").innerHTML = jsonResponse.data.version;
                document.getElementById("success").innerHTML = "Current firmware version: " + jsonResponse.data.version;
 
                //let backgroundColor = "linear-gradient("+ jsonResponse.data.color + ", pink);"
                let backgroundColor = jsonResponse.data.color;
                document.body.setAttribute('style', 'background-color: ' + backgroundColor);
                //document.getElementsByClassName("main-block"). = 

                if (scenarioNo === 2) {
                    document.getElementById("scenarioDesc").innerHTML = scenarioTwo;
                } else if (scenarioNo === 3) {
                    document.getElementById("scenarioDesc").innerHTML = scenarioThree;
                } else {
                    document.getElementById("scenarioDesc").innerHTML = scenarioOne;
                }
            } else {
                document.getElementById("failure").innerHTML = 'An error occurred fetching current version.';
            }
        } else {

        }
    };

    //End of request
    xhr.send(null);
}

function fileToByteArray(file) {
    return new Promise((resolve, reject) => {
        try {
            let reader = new FileReader();
            let fileByteArray = [];
            reader.readAsArrayBuffer(file);
            reader.onloadend = (evt) => {
                if (evt.target.readyState == FileReader.DONE) {
                    let arrayBuffer = evt.target.result,
                        array = new Uint8Array(arrayBuffer);
                    for (byte of array) {
                        fileByteArray.push(byte);
                    }
                }
                resolve(fileByteArray);
            }
        }
        catch (e) {
            reject(e);
        }
    })
}


function hexToRgbA(hex){
    var c;
    if(/^#([A-Fa-f0-9]{3}){1,2}$/.test(hex)){
        c= hex.substring(1).split('');
        if(c.length== 3){
            c= [c[0], c[0], c[1], c[1], c[2], c[2]];
        }
        c= '0x'+c.join('');
        return 'rgba('+[(c>>16)&255, (c>>8)&255, c&255].join(',')+',1)';
    }
    throw new Error('Bad Hex');
}

// Function to convert a file to bytearray
async function getByteArray(file) {
    //Wait for the file to be converted to a byteArray
    let byteArray = await fileToByteArray(file);

    //Do something with the byteArray
    return byteArray;
    //return res;
}

async function sendFileAsBinary(file) {
    let oReq = new XMLHttpRequest();

    oReq.onload = function (oEvent) {
        // Uploaded.
        //document.getElementById('success').innerHTML = 'Uploaded. Please Click on "Reboot" to proceed.';
    };

    oReq.onreadystatechange = function () {
		let status = oReq.status;
        if (oReq.readyState === XMLHttpRequest.DONE) {
            if (status >= 200 && status < 400) {
				document.getElementById('success').innerHTML = 'Uploaded. Page will reload in 5 seconds...';
				document.getElementById('failure').innerHTML = "";
				
				setTimeout(function () {
					document.location.reload();
				}, 5000);
            } else {
                document.getElementById('failure').innerHTML = 'Unable to upload binary! Please try again.';
                document.getElementById('success').innerHTML = "";
            }
        } else {
        }
    }

    let buffer = await getByteArray(file);
    var blob = new Blob([new Uint8Array(buffer)], { type: 'application/octet-stream' });

    oReq.open("POST", '/upload/binary', true);
    oReq.send(blob);
}

// updateSize() will listen to the "change" event on the file select button.
document.getElementById("uploadInput").addEventListener("change", updateSize, false);

let submit = document.getElementById('submit');

submit.addEventListener("click", function (e) {
    e.preventDefault()
    //submit.disabled = true;
    try {
        //sendFile(files[0]) // This is the normal way to upload a file as multi-part form data
        sendFileAsBinary(files[0]) // This is the way to upload if you need a byte stream
        document.getElementById('success').innerHTML = 'Uploading file...';
        document.getElementById('submit').classList.remove("btn-success");
        document.getElementById('submit').classList.add("btn-outline-secondary");
        document.getElementById('submit').classList.add("disabled");
    } catch (error) {
        document.getElementById('failure').innerHTML = 'An error occurred. Please try again.';
        document.getElementById('submit').classList.remove("btn-success");
        document.getElementById('submit').classList.add("btn-outline-secondary");
        document.getElementById('submit').classList.add("disabled");
        document.getElementById('step3').classList.remove("list-group-item-success");
    }
})
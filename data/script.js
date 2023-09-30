// Get current sensor readings when the page loads  
window.addEventListener('load', getReadings);

// Function to get current readings on the webpage when it loads for the first time
function getReadings() {
    let xhr = new XMLHttpRequest();
    xhr.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            addRowFromData(this.responseText);
        }
    };

    xhr.open("GET", "/readings", true);
    xhr.send();
}

if (window.EventSource) {
    let source = new EventSource('/events');
    
    source.addEventListener('open', function(e) {
      console.log("Events Connected");
    }, false);
  
    source.addEventListener('error', function(e) {
      if (e.target.readyState != EventSource.OPEN) {
        console.log("Events Disconnected");
      }
    }, false);
    
    source.addEventListener('message', function(e) {
      console.log("message", e.data);
    }, false);
    
    source.addEventListener('new_readings', function(e) {
        addRowFromData(e.data);
    }, false);
  }

function addRowFromData(data) {
    let myObj = JSON.parse(data);
    let table = document.getElementById("dataTable");
    let row = table.insertRow(1);
    row.insertCell(0).innerHTML = myObj.timestamp;
    row.insertCell(1).innerHTML = myObj.relay_state;
}
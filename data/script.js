// Requisita as últimas leituras
window.addEventListener('load', function (e) {
  let xhr = new XMLHttpRequest();
  xhr.onreadystatechange = function () {
    if (this.readyState == 4 && this.status == 200) {
      console.log("Dados recebidos ao inicializar:" + this.responseText);
      addRowFromData(this.responseText);
    }
  };
  xhr.open("GET", "/readings", true);
  xhr.send();
}, false);

// Inicializa os eventos
if (window.EventSource) {
  let source = new EventSource('/events');

  source.addEventListener('open', function (e) {
    console.log("Events Connected");
  }, false);

  source.addEventListener('error', function (e) {
    if (e.target.readyState != EventSource.OPEN) {
      console.log("Events Disconnected");
    }
  }, false);

  source.addEventListener('message', function (e) {
    console.log("message", e.data);
  }, false);

  source.addEventListener('new_readings', function (e) {
    addRowFromData(e.data);
  }, false);
}

/**
 * Realiza tratativa dos dados recebidos.
 * @param data Dados recebidos no formato JSON.
 */
function addRowFromData(data) {
  let myObj = JSON.parse(data);
  let table = document.getElementById("dataTable");
  // Insere linha na tabela
  let row = table.insertRow(1);
  try {
    // Preenche com os dados
    row.insertCell(0).innerHTML = myObj.timestamp;
    row.insertCell(1).innerHTML = (new Date()).toLocaleString();
    row.insertCell(2).innerHTML = myObj.relay_state;
  } catch (e) {
    console.log("error: " + e);
    console.log("message: " + data);
  }
  row.insertCell(3).innerHTML = data;
}

function onSubmit(event) {
  let xhr = new XMLHttpRequest();
  xhr.open("POST", "/get-message", true);
  xhr.setRequestHeader("Accept", "application/json");
  xhr.setRequestHeader("Content-Type", "application/json");

  xhr.onreadystatechange = function () {
    if (xhr.readyState === 4) {
      console.log(xhr.status);
      console.log(xhr.responseText);
    }
  };

  let data = {"start": parseInt(document.getElementById("entryTemp").value)};
  console.log("dados enviados: " + JSON.stringify(data));

  xhr.send(JSON.stringify(data));
  console.log(xhr);
  return false;
}
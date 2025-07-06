#include <WiFi.h>
#include <WebServer.h>

WebServer server(80);

const char* ssid = "ESP_Code";
const char* password = "NEVERSING";

// HTML do editor embutido (inline CSS e JS minificado)
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8" />
<title>ESP32 VSCode Lite</title>
<style>
  /* Estilos básicos e CodeMirror minimal inline */
  body,html{height:100%;margin:0;display:flex;flex-direction:column;font-family:sans-serif;}
  #editor {flex:1; border:1px solid #ccc; font-family: monospace; font-size:14px; padding:10px; background:#1e1e1e; color:#d4d4d4; overflow:auto; outline:none; white-space: pre; tab-size:2;}
  #buttons {padding:10px; background:#333; display:flex; gap:10px;}
  button {background:#007acc; color:white; border:none; padding:8px 16px; cursor:pointer; font-weight:bold; border-radius:4px;}
  button:hover {background:#005a9e;}
  #output {height:150px; background:#252526; color:#eee; padding:10px; overflow:auto; font-family: monospace; white-space: pre-wrap;}
</style>
</head>
<body>
  <div id="editor" contenteditable="true" spellcheck="false" aria-label="Editor de código"></div>
  <div id="buttons">
    <button onclick="salvar()">Salvar</button>
    <button onclick="rodar()">Testar (Renderizar HTML)</button>
  </div>
  <div id="output"></div>

<script>
const editor = document.getElementById('editor');
const output = document.getElementById('output');
const STORAGE_KEY = 'esp32_code';

function salvar() {
  localStorage.setItem(STORAGE_KEY, editor.innerText);
  alert('Código salvo no navegador!');
}

function rodar() {
  const codigo = editor.innerText;
  // Só para HTML, renderiza no iframe criado
  const win = window.open();
  win.document.open();
  win.document.write(codigo);
  win.document.close();
  output.textContent = 'Teste aberto em nova aba/janela.';
}

// Carrega código salvo no navegador (localStorage)
window.onload = () => {
  const salvo = localStorage.getItem(STORAGE_KEY);
  if(salvo) editor.innerText = salvo;
  else editor.innerText = "<!-- Escreva seu código HTML/CSS/JS aqui -->";
};

// Auto-sugestão simplificada (só tags HTML comuns)
editor.addEventListener('input', () => {
  // Você pode incrementar aqui com sugestões melhores
  // Por simplicidade, não vamos colocar agora para não travar no celular
});

</script>
</body>
</html>
)rawliteral";

void handleRoot() {
  server.send_P(200, "text/html", index_html);
}

void setup() {
  Serial.begin(115200);
  WiFi.softAP(ssid, password);
  Serial.print("Acesse: http://");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.begin();
}

void loop() {
  server.handleClient();
}

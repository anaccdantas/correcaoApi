#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// quando for manipular texto, é mais confortavel e eficiente usar a classe string
//* texto simples? USAR PONTEIRO (CHAR)
//? texto complexo? USAR STRING
String pedirDadosPokemonApiPorId (unsigned int idPokemon); // convertido para string pois o texto será manipulado.


const char *nomeRedeWiFi = "SENAI EDUCACIONAL";
const char senhaRedeWiFi[] = "info@134"; // pesquisar

String enderecoAPI = "https://pokeapi.co/api/v2/pokemon/"; //TODO adicionar o numero do pokemon a ser pesquisado

void setup() 
{
  Serial.begin(115200);
  WiFi.begin(nomeRedeWiFi, senhaRedeWiFi);

  Serial.printf("\n\r"); // para tirar o ruido do esp

  while(WiFi.status() != WL_CONNECTED) // enquanto o wifi nao estiver conectado...
  {
    Serial.print(".");
    delay(500);
  }

  Serial.printf("\n\rConectado!\n\r"); 
  // é mais profissional usar o printf ao inves do println pois é possível
  // adicionar texto e o "\n\r" (pula linha, como o println) para evitar escrever
  // muitas linhas e tomar muito espaco no codigo.
  
  // Serial.println(pedirDadosPokemonApiPorId(10007));

  JsonDocument docPokemon;

  Serial.println(ESP.getFreeHeap());
  String jsonVindoDaApiPokemon = pedirDadosPokemonApiPorId(10007);
  Serial.println(ESP.getFreeHeap());
  DeserializationError codigoStatusJSON = deserializeJson(docPokemon, jsonVindoDaApiPokemon);
  Serial.println(ESP.getFreeHeap());

  jsonVindoDaApiPokemon.end(); //* desalocar memoria desse objeto pois nao vou usar mais

  Serial.println(ESP.getFreeHeap());
}


void loop() 
{

}

String pedirDadosPokemonApiPorId (unsigned int idPokemon)
{
  String resposta = "";

  enderecoAPI = enderecoAPI + idPokemon + "/";
  
  HTTPClient textoJsonHttp; // criando um navegador; estamos criando uma nova pagina http
  
  //* inicia o objeto instanciado do tipo http para pegar textos http na internet
  textoJsonHttp.begin(enderecoAPI); // passando o endereco

  Serial.println(ESP.getPsramSize());
  //* o metodo get acessa o endereco passado no begin e baixa as informacoes e retorna o codigo http de status
  //* como 200 para ok e 404 para pagina nao encontrada
  int codigoHTTP = textoJsonHttp.GET(); // pegando todo o conteudo da URL
  Serial.println(ESP.getPsramSize());

  // clausula de guarda: se o erro nao for ok, para a funcao.
  if(codigoHTTP != 200)
  {
    resposta = "Erro ao acessar o endereco: " + codigoHTTP;
    return resposta;
  }

  resposta = textoJsonHttp.getString(); // passando o conteudo no endereco como string para a variavel resposta

  textoJsonHttp.end(); // para nao continuar consumindo memoria sem usar a variavel

  return resposta;
}


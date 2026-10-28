#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// quando for manipular texto, é mais confortavel e eficiente usar a classe string
//* texto simples? USAR PONTEIRO (CHAR)
//? texto complexo? USAR STRING
String pedirDadosPokemonApiPorId (unsigned int idPokemon); // convertido para string pois o texto será manipulado.
void printPokemon(unsigned int idDoPokemon);


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

  randomSeed(10);
}


void loop() 
{
  unsigned int idPokemonAleatorio = random(1, 1026);

  printPokemon(idPokemonAleatorio);

  delay(5000);
}

String pedirDadosPokemonApiPorId (unsigned int idPokemon)
{
  String resposta = "";
  String enderecoPesquisado = enderecoAPI + idPokemon + "/";
  
  HTTPClient textoJsonHttp; // criando um navegador; estamos criando uma nova pagina http
  
  //* inicia o objeto instanciado do tipo http para pegar textos http na internet
  textoJsonHttp.begin(enderecoAPI); // passando o endereco

  //* o metodo get acessa o endereco passado no begin e baixa as informacoes e retorna o codigo http de status
  //* como 200 para ok e 404 para pagina nao encontrada
  int codigoHTTP = textoJsonHttp.GET(); // pegando todo o conteudo da URL

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

void printPokemon(unsigned int idDoPokemon)
{
  JsonDocument docPokemon;
  JsonDocument docFiltro;
  
  docFiltro["name"] = true;
  docFiltro["id"] = true;
  docFiltro["abilities"][0]["ability"]["name"] = true;
  docFiltro["forms"][0]["name"] = true;
  docFiltro["types"][0]["type"]["name"] = true;
  
  String jsonVindoDaApiPokemon = pedirDadosPokemonApiPorId(idDoPokemon);
  
  DeserializationError codigoStatusJSON = deserializeJson(docPokemon, jsonVindoDaApiPokemon, DeserializationOption::Filter(docFiltro));
  
  String nomePokemon = docPokemon["name"];
  unsigned int idPokemon = docPokemon["id"];
  
  JsonArrayConst habilidadesPokemon = docPokemon["abilities"];
  JsonArrayConst formasPokemon = docFiltro["forms"];
  JsonArrayConst tiposPokemon = docFiltro["types"];
  
  Serial.printf("Nome do Pokemon: %s\n\r", nomePokemon.c_str());
  Serial.printf("Id do Pokemon: %d\n\r", idPokemon);

  Serial.printf("Habilidades do Pokemon: ");

  for(int i = 0; i < habilidadesPokemon.size(); i++)
  {
    Serial.print(habilidadesPokemon[i]["ability"]["name"].as<const char*>()); // ?
    Serial.print(i == (habilidadesPokemon.size() - 1) ? ".\n\r" : ",");
  }

  Serial.printf("Formas possiveis do Pokemon: ");
  
  for(int i = 0; i < formasPokemon.size(); i++)
  {
    Serial.print(formasPokemon[i]["name"].as<const char*>()); // ?
    Serial.print(i == (formasPokemon.size() - 1) ? ".\n\r" : ",");
  }

  Serial.printf("Tipos do Pokemon: ");
  
  for(int i = 0; i < tiposPokemon.size(); i++)
  {
    Serial.print(tiposPokemon[i]["type"]["name"].as<const char*>()); // ?
    Serial.print(i == (tiposPokemon.size() - 1) ? ".\n\r" : ",");
  }

  jsonVindoDaApiPokemon.clear(); //* desalocar memoria desse objeto pois nao vou usar mais
}
#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <ArduinoWebsockets.h>
#include <WiFiManager.h>

using namespace websockets;

class Web
{
public:
    static void startHTTP();
    static void startWS();
    static void listen_ws();
    static void tick();
    static void receive(std::function<void(WebsocketsClient &, String, String)> listener);
    static void send(String message);

private:
    static ESP8266WebServer http_server;
    static WebsocketsServer ws_server;
    static std::function<void(WebsocketsClient &, String, String)> ws_listener;
    static void handler(WebsocketsClient &client, WebsocketsMessage message);
    static void serve_index();
    static WebsocketsClient *client;
    static std::vector<WebsocketsClient> all_clients;
    static void poll_all_clients();
};

std::vector<WebsocketsClient> Web::all_clients;
WebsocketsClient *Web::client = nullptr;
std::function<void(WebsocketsClient &, String, String)> Web::ws_listener = nullptr;
ESP8266WebServer Web::http_server = ESP8266WebServer();
WebsocketsServer Web::ws_server = WebsocketsServer();

void Web::startWS()
{
    ws_server.listen(81);
    Serial.println("WS server started");
}

void Web::send(String message)
{
}

void Web::serve_index()
{
    http_server.send(200, "text/html", Data::index);
}

void Web::startHTTP()
{
    http_server.begin(80);
    http_server.on("/", serve_index);
    Serial.println("HTTP server started");
}

void Web::receive(std::function<void(WebsocketsClient &, String, String)> listener)
{
    ws_listener = listener;
}

void Web::handler(WebsocketsClient &client, WebsocketsMessage message)
{
    WSInterfaceString data = message.data();

    int index = data.indexOf(":");
    if (index > 0)
    {
        String first = data.substring(0, index);
        String second = data.substring(index+1);
        ws_listener(client, first, second);
    }

    client.send("status:" + data);
}

void Web::poll_all_clients()
{
    for (auto &client : all_clients)
    {
        client.poll();
    }
}

void Web::tick()
{

    if (ws_server.available())
    {
        if (ws_server.poll())
        {
            WebsocketsClient c = ws_server.accept();
            Serial.println("Client accepted");
            c.onMessage(handler);
            all_clients.push_back(c);
        }
    }

    poll_all_clients();
    http_server.handleClient();
}
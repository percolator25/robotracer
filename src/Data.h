#include <Arduino.h>
class Data
{
public:
    static const char *index;
};

const char *Data::index = "<title>outrunner-mctrl</title><meta name=\"viewport\"content=\"width=device-width,i"
                    "nitial-scale=1,maximum-scale=1,user-scalable=no\"><style>p{margin-block:0}.viewpo"
                    "rt,body,html{width:100%;height:100%;margin:0}article,aside,footer,header,nav{mar"
                    "gin:1px;background-color:#ccc;display:flex;text-align:center;flex-direction:colu"
                    "mn}.vbox{display:-webkit-flex;display:-moz-flex;display:-ms-flex;display:flex;-w"
                    "ebkit-flex-direction:column;-moz-flex-direction:column;-ms-flex-direction:column"
                    ";flex-direction:column;-webkit-user-select:none;-ms-user-select:none;user-select"
                    ":none}.hbox{display:-webkit-flex;display:-moz-flex;display:-ms-flex;display:flex"
                    ";-webkit-flex-direction:row;-moz-flex-direction:row;-ms-flex-direction:row;flex-"
                    "direction:row}.space-between{-webkit-justify-content:space-between;-moz-justify-"
                    "content:space-between;-ms-justify-content:space-between;justify-content:space-be"
                    "tween}footer,header{height:10vh;background-color:#222;color:#fff}.main{-webkit-f"
                    "lex:1;-moz-flex:1;-ms-flex:1;flex:1}.flex{flex:1}.flex2{flex:2}.flex3{flex:3}.fl"
                    "ex4{flex:4}.fixed{flex:0}.right{align-items:flex-end}.alert-box{background-color"
                    ":red}.pad{padding:10px}button{height:30px}</style><script>function register(e){d"
                    "ocument.getElementById(e).addEventListener(\"mouseup\",function(){send(e,\"up\")}),d"
                    "ocument.getElementById(e).addEventListener(\"mousedown\",function(){send(e,\"down\")"
                    "})}</script><body class=\"vbox viewport\"><section class=\"main hbox space-between\""
                    "><article id=\"lf\"class=\"flex\">Left forward</article><article id=\"mt\"class=\"flex\""
                    "><button onclick=\"connect()\">Connect</button> <button onclick=\"disconnect()\">Dis"
                    "connect</button></article><article id=\"rf\"class=\"flex\">Right forward</article></"
                    "section><section class=\"main hbox space-between\"><article id=\"lb\"class=\"flex\">Le"
                    "ft back</article><article id=\"status\"class=\"flex\"></article><article id=\"rb\"clas"
                    "s=\"flex\">Right back</article></section><script>register(\"lf\"),register(\"rf\"),reg"
                    "ister(\"lb\"),register(\"status\"),register(\"rb\")</script><script>function set_statu"
                    "s(e){document.getElementById(\"status\").innerHTML=\"Status: \"+e}let ws=null;functi"
                    "on connect(){let e=window.location.host;null!=localStorage.getItem(\"host\")?e=loc"
                    "alStorage.getItem(\"host\"):(e=window.prompt(\"Please enter a IP adress\",window.loc"
                    "ation.hostname),localStorage.setItem(\"host\",e)),(ws=new WebSocket(\"ws://\"+e+\":81"
                    "\")).onopen=e=>{ws.send(\"hi\"),set_status(\"Connected\")},ws.onmessage=e=>{e=e.data."
                    "split(/:(.+)/);console.log(e),receive(e[0],e[1])}}function disconnect(){ws.close"
                    "(),set_status(\"Disconnected\"),localStorage.removeItem(\"host\")}function send(e,t)"
                    "{void 0!==ws&&(console.log(\">> \"+e+\":\"+t),ws.send(e+\":\"+t))}function receive(e,t"
                    "){console.log(\"<< \"+e+\":\"+t),document.getElementById(e).innerHTML=t}</script>"
;
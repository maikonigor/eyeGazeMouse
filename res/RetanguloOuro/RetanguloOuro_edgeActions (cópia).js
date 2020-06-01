/***********************
* Adobe Edge Animate Composition Actions
*
* Edit this file with caution, being careful to preserve
* function signatures and comments starting with 'Edge' to maintain the
* ability to interact with these actions from within Adobe Edge Animate
*
***********************/
(function($, Edge, compId){
var Composition = Edge.Composition, Symbol = Edge.Symbol; // aliases for commonly used Edge classes

   //Edge symbol: 'stage'
   var initialDate = 0;
   var finalDate = 0;
   (function(symbolName) {


      Symbol.bindElementAction(compId, symbolName, "${bt_1}", "click", function(sym, e) {
         if(botao01 == false){
         	numeroErros+=1;
         	sym.$("erros").html(""+numeroErros);

         }else{

         	var d2 = new Date();
         	hora2 = addZero(d2.getHours());
         	minuto2 = addZero(d2.getMinutes());
         	segundo2 = addZero(d2.getSeconds());
         	var horario2 = hora2+":"+minuto2+":"+segundo2;

         	var qtd_erros = sym.$("erros").text();
         	sym.$("msn_saida").text("Botão 1");
         	document.getElementById("entrada01").innerHTML = document.getElementById("entrada01").innerHTML+"Botão 1<br>"+horario2+"<br>erros:"+qtd_erros+"<br>----------<br>";
         	var qtd_erros = sym.$("erros").text("0");
         	numeroErros = 0;
         	sym.getSymbol("bt_1").stop("inicio");
         	sorteNu();

         }

      });
      //Edge binding end

      Symbol.bindElementAction(compId, symbolName, "${bt_2}", "click", function(sym, e) {
         if(botao02 == false){
         	numeroErros+=1;
         	sym.$("erros").html(""+numeroErros);

         }else{

         	var d2 = new Date();
         	hora2 = addZero(d2.getHours());
         	minuto2 = addZero(d2.getMinutes());
         	segundo2 = addZero(d2.getSeconds());
         	var horario2 = hora2+":"+minuto2+":"+segundo2;
         	var resultMiliseconds = d2.getMilliseconds();


         	var qtd_erros = sym.$("erros").text();
         	sym.$("msn_saida").text("Botão 2");
         	document.getElementById("entrada01").innerHTML = document.getElementById("entrada01").innerHTML+"Botão 2<br>"+resultMiliseconds+"<br>erros:"+qtd_erros+"<br>----------<br>";
         	var qtd_erros = sym.$("erros").text("0");
         	numeroErros = 0;
         	sym.getSymbol("bt_2").stop("inicio");
         	sorteNu();

         }

      });
      //Edge binding end

      Symbol.bindElementAction(compId, symbolName, "${bt_3}", "click", function(sym, e) {
         if(botao03 == false){
         	numeroErros+=1;
         	sym.$("erros").html(""+numeroErros);

         }else{

         	var d2 = new Date();
         	hora2 = addZero(d2.getHours());
         	minuto2 = addZero(d2.getMinutes());
         	segundo2 = addZero(d2.getSeconds());
         	var horario2 = hora2+":"+minuto2+":"+segundo2;


         	var qtd_erros = sym.$("erros").text();
         	sym.$("msn_saida").text("Botão 3");
         	document.getElementById("entrada01").innerHTML = document.getElementById("entrada01").innerHTML+"Botão 3<br>"+horario2+"<br>erros:"+qtd_erros+"<br>----------<br>";
         	var qtd_erros = sym.$("erros").text("0");
         	numeroErros = 0;
         	sym.getSymbol("bt_3").stop("inicio");
         	sorteNu();

         }

      });
      //Edge binding end

      Symbol.bindElementAction(compId, symbolName, "${bt_4}", "click", function(sym, e) {
         if(botao04 == false){
         	numeroErros+=1;
         	sym.$("erros").html(""+numeroErros);

         }else{

         	var d2 = new Date();
         	hora2 = addZero(d2.getHours());
         	minuto2 = addZero(d2.getMinutes());
         	segundo2 = addZero(d2.getSeconds());
         	var horario2 = hora2+":"+minuto2+":"+segundo2;


         	var qtd_erros = sym.$("erros").text();
         	sym.$("msn_saida").text("Botão 4");
         	document.getElementById("entrada01").innerHTML = document.getElementById("entrada01").innerHTML+"Botão 4<br>"+horario2+"<br>erros:"+qtd_erros+"<br>----------<br>";
         	var qtd_erros = sym.$("erros").text("0");
         	numeroErros = 0;
         	sym.getSymbol("bt_4").stop("inicio");
         	sorteNu();

         }

      });
      //Edge binding end

      Symbol.bindElementAction(compId, symbolName, "${bt_5}", "click", function(sym, e) {
         if(botao05 == false){
         	numeroErros+=1;
         	sym.$("erros").html(""+numeroErros);

         }else{

         	var d2 = new Date();
         	hora2 = addZero(d2.getHours());
         	minuto2 = addZero(d2.getMinutes());
         	segundo2 = addZero(d2.getSeconds());
         	var horario2 = hora2+":"+minuto2+":"+segundo2;


         	var qtd_erros = sym.$("erros").text();
         	sym.$("msn_saida").text("Botão 5");
         	document.getElementById("entrada01").innerHTML = document.getElementById("entrada01").innerHTML+"Botão 5<br>"+horario2+"<br>erros:"+qtd_erros+"<br>----------<br>";
         	var qtd_erros = sym.$("erros").text("0");
         	numeroErros = 0;
         	sym.getSymbol("bt_5").stop("inicio");
         	sorteNu();

         }

      });
      //Edge binding end

      Symbol.bindElementAction(compId, symbolName, "${bt_6}", "click", function(sym, e) {
         // insert code for mouse click here
         if(botao06 == false){
         	numeroErros+=1;
         	sym.$("erros").html(""+numeroErros);

         }else{

         	var d2 = new Date();
         	hora2 = addZero(d2.getHours());
         	minuto2 = addZero(d2.getMinutes());
         	segundo2 = addZero(d2.getSeconds());
         	var horario2 = hora2+":"+minuto2+":"+segundo2;


         	var qtd_erros = sym.$("erros").text();
         	sym.$("msn_saida").text("Botão 6");
         	document.getElementById("entrada01").innerHTML = document.getElementById("entrada01").innerHTML+"Botão 6<br>"+horario2+"<br>erros:"+qtd_erros+"<br>----------<br>";
         	var qtd_erros = sym.$("erros").text("0");
         	numeroErros = 0;
         	sym.getSymbol("bt_6").stop("inicio");
         	sorteNu();

         }


      });
      //Edge binding end

      Symbol.bindSymbolAction(compId, symbolName, "creationComplete", function(sym, e) {
         // insert code to be run when the symbol is created here
         numeroErros = 0;

         botao01 = false;
         botao02 = false;
         botao03 = false;
         botao04 = false;
         botao05 = false;
         botao06 = false;

         var entrada01 = sym.$("log_eventos");
         cx01_w = sym.$('log_eventos').width();
         cx01_h = sym.$('log_eventos').height();
         cx_01 = $('<label />').attr({'id':'entrada01'});
         cx_01.css ('font-size', 10);
         cx_01.css ('width', cx01_w-10);
         cx_01.css ('height', cx01_h-10);
         cx_01.appendTo(entrada01);

         addZero = function(i) {
             if (i < 10) {

                 i = "0" + i;
             }
             return i;
         }

         var d = new Date();
         hora = addZero(d.getHours());
         minuto = addZero(d.getMinutes());
         segundo = addZero(d.getSeconds());
         var horario = hora+":"+minuto+":"+segundo;
         sym.$("msn_saidaCopy2").text(""+horario);

         numeroSorteado = 0;

/* determinarte the blink button*/
         sorteNu = function(){

           	var numero = ""+Math.random();
           	console.log("numero:"+numero);
           	var nSorteado = parseInt(numero.substring(2,4));
           	console.log("numero:"+nSorteado);
           	numeroSorteado = nSorteado;

           	if(numeroSorteado >= 0 && numeroSorteado < 16){

           		sym.getSymbol("bt_1").play("brilhando");

           		botao01 = true;
           		botao02 = false;
           		botao03 = false;
           		botao04 = false;
           		botao05 = false;
           		botao06 = false;


           	}else if(numeroSorteado >= 16 && numeroSorteado < 32){

           		sym.getSymbol("bt_2").play("brilhando");

           		botao01 = false;
           		botao02 = true;
           		botao03 = false;
           		botao04 = false;
           		botao05 = false;
           		botao06 = false;

           	}else if(numeroSorteado >= 32 && numeroSorteado < 48){

           		sym.getSymbol("bt_3").play("brilhando");

           		botao01 = false;
           		botao02 = false;
           		botao03 = true;
           		botao04 = false;
           		botao05 = false;
           		botao06 = false;
           	}else if(numeroSorteado >= 48 && numeroSorteado < 65){

           		sym.getSymbol("bt_4").play("brilhando");

           		botao01 = false;
           		botao02 = false;
           		botao03 = false;
           		botao04 = true;
           		botao05 = false;
           		botao06 = false;
           	}else if(numeroSorteado >= 65 && numeroSorteado < 82){

           		sym.getSymbol("bt_5").play("brilhando");

           		botao01 = false;
           		botao02 = false;
           		botao03 = false;
           		botao04 = false;
           		botao05 = true;
           		botao06 = false;
           	}else if(numeroSorteado >= 82 && numeroSorteado < 99){

           		sym.getSymbol("bt_6").play("brilhando");

           		botao01 = false;
           		botao02 = false;
           		botao03 = false;
           		botao04 = false;
           		botao05 = false;
           		botao06 = true;
           	}

         }

         sorteNu();

         console.log("numero2:"+numeroSorteado);







      });
      //Edge binding end

   })("stage");
   //Edge symbol end:'stage'

   //=========================================================

   //Edge symbol: 'bt_1'
   (function(symbolName) {

      Symbol.bindTriggerAction(compId, symbolName, "Default Timeline", 750, function(sym, e) {
         // insert code here
         sym.stop();

      });
      //Edge binding end

      Symbol.bindTriggerAction(compId, symbolName, "Default Timeline", 2043, function(sym, e) {

         sym.play("brilhando");



      });
      //Edge binding end

   })("bt_1");
   //Edge symbol end:'bt_1'

   //=========================================================

   //Edge symbol: 'bt_2'
   (function(symbolName) {

      Symbol.bindTriggerAction(compId, symbolName, "Default Timeline", 750, function(sym, e) {
         // insert code here
         sym.stop();

      });
      //Edge binding end

      Symbol.bindTriggerAction(compId, symbolName, "Default Timeline", 2043, function(sym, e) {

         sym.play("brilhando");



      });
      //Edge binding end

   })("bt_2");
   //Edge symbol end:'bt_2'

   //=========================================================

   //Edge symbol: 'bt_3'
   (function(symbolName) {

      Symbol.bindTriggerAction(compId, symbolName, "Default Timeline", 750, function(sym, e) {
         // insert code here
         sym.stop();

      });
      //Edge binding end

      Symbol.bindTriggerAction(compId, symbolName, "Default Timeline", 2043, function(sym, e) {

         sym.play("brilhando");



      });
      //Edge binding end

   })("bt_3");
   //Edge symbol end:'bt_3'

   //=========================================================

   //Edge symbol: 'bt_4'
   (function(symbolName) {

      Symbol.bindTriggerAction(compId, symbolName, "Default Timeline", 750, function(sym, e) {
         // insert code here
         sym.stop();

      });
      //Edge binding end

      Symbol.bindTriggerAction(compId, symbolName, "Default Timeline", 2043, function(sym, e) {

         sym.play("brilhando");



      });
      //Edge binding end

   })("bt_4");
   //Edge symbol end:'bt_4'

   //=========================================================

   //Edge symbol: 'bt_5'
   (function(symbolName) {

      Symbol.bindTriggerAction(compId, symbolName, "Default Timeline", 750, function(sym, e) {
         // insert code here
         sym.stop();

      });
      //Edge binding end

      Symbol.bindTriggerAction(compId, symbolName, "Default Timeline", 2043, function(sym, e) {

         sym.play("brilhando");



      });
      //Edge binding end

   })("bt_5");
   //Edge symbol end:'bt_5'

   //=========================================================

   //Edge symbol: 'bt_6'
   (function(symbolName) {

      Symbol.bindTriggerAction(compId, symbolName, "Default Timeline", 750, function(sym, e) {
         // insert code here
         sym.stop();

      });
      //Edge binding end

      Symbol.bindTriggerAction(compId, symbolName, "Default Timeline", 2043, function(sym, e) {

         sym.play("brilhando");



      });
      //Edge binding end

   })("bt_6");
   //Edge symbol end:'bt_6'

   //=========================================================

   //Edge symbol: 'relogio'
   (function(symbolName) {

      Symbol.bindTriggerAction(compId, symbolName, "Default Timeline", 65, function(sym, e) {
         // insert code here
         // insert code here
         var d = new Date();
         hora = addZero(d.getHours());
         minuto = addZero(d.getMinutes());
         segundo = addZero(d.getSeconds());
         var horario = hora+":"+minuto+":"+segundo;


         // Change the text in an element
         sym.getComposition().getStage().getSymbol("relogio").$("msn_saidaCopy").text(""+horario);

      });
      //Edge binding end

      Symbol.bindTriggerAction(compId, symbolName, "Default Timeline", 1000, function(sym, e) {
         // insert code here
         // Play the timeline at a label or specific time. For example:
         // sym.play(500); or sym.play("myLabel");
         sym.play("inicio");

      });
      //Edge binding end

   })("relogio");
   //Edge symbol end:'relogio'

})(window.jQuery || AdobeEdge.$, AdobeEdge, "EDGE-603790615");

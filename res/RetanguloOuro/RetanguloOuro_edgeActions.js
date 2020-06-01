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
   var current_button = 6;
   var segundos = 0;
   var timer;
   var isFinish = false;
   (function(symbolName) {


      Symbol.bindElementAction(compId, symbolName, "${bt_1}", "click", function(sym, e) {
         button_action(1);

      });//Edge binding end

      Symbol.bindElementAction(compId, symbolName, "${bt_2}", "click", function(sym, e) {
         button_action(2);

      });//Edge binding end

      Symbol.bindElementAction(compId, symbolName, "${bt_3}", "click", function(sym, e) {
         button_action(3);
      });//Edge binding end

      Symbol.bindElementAction(compId, symbolName, "${bt_4}", "click", function(sym, e) {
         button_action(4);

      });//Edge binding end

      Symbol.bindElementAction(compId, symbolName, "${bt_5}", "click", function(sym, e) {
         button_action(5);
      });//Edge binding end

      Symbol.bindElementAction(compId, symbolName, "${bt_6}", "click", function(sym, e) {
         button_action(6);
      });//Edge binding end

      Symbol.bindSymbolAction(compId, symbolName, "creationComplete", function(sym, e) {
         // insert code to be run when the symbol is created here
         numeroErros = 0;

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
          h=0
          m = 0;
          s = 0;
         var horario = hora+":"+minuto+":"+segundo;
         sym.$("msn_saidaCopy2").text(""+horario);

         numeroSorteado = 0;

         startTimer = function(){
           timer = setInterval(function(){
              segundos++;
            }, 1000);
         }

         restartTemp = function(){
           clearInterval(timer);
           segundos = 0;
         }

         //handler click event for all buttons
         button_action = function(button_clicked){

           if(isFinish){
             return;
           }
           else if(current_button != button_clicked){
            numeroErros+=1;
            sym.$("erros").html(""+numeroErros);
            return;
          }else{

            var buttonLabel = "Botão " + current_button;
            var qtd_erros = sym.$("erros").text();
            sym.$("msn_saida").text(buttonLabel);
            document.getElementById("entrada01").innerHTML = document.getElementById("entrada01").innerHTML+ buttonLabel +"<br>"+segundos+"s <br>erros:"+qtd_erros+"<br>----------<br>";
            var qtd_erros = sym.$("erros").text("0");
            numeroErros = 0;
            sym.getSymbol("bt_"+current_button).stop("inicio");


            current_button --;
            if(current_button < 1){
              isFinish = true;
              console.log("set finish")
            }
            
            restartTemp();
            blinkButton();

          }
         }

        /* determinarte what button gonna blink*/
         blinkButton = function(){
           sym.getSymbol("bt_"+current_button).play("brilhando");
           startTimer();
          }

         blinkButton();

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
         if(isFinish){
          console.log(isFinish);
          return;
         }

         var horario = h+":"+m+":"+s;
         s++;

         if(s == 60){
           s = 0;
           m++;
         }

         if(m == 60){
           m = 0;
           h++;
         }

         if(s < 10 )
            horario = h+":0"+m+":0"+s;

         else if(m<10 && s >= 10)
              horario = h+":0"+m+":"+s;
          else
            horario = h+":"+m+":"+s;

          sym.getComposition().getStage().getSymbol("relogio").$("msn_saidaCopy").text(" "+horario);

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

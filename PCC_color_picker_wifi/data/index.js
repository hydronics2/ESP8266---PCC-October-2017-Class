


$("#hue").on("change", function() {
  updateColor(1);
});
$("#saturation").on("change", function() {
  updateColor(2);
});
$("#lightness").on("change", function() {
  updateColor(3);
});




var r = 0;
var g = 0;
var b = 0;

var h1, s1, l1;

var active = "resultCur";

window.onload = initAll;

function initAll() {
  document.onkeyup = shortcuts;
  shades(document.getElementById("hue").value, document.getElementById("saturation").value, document.getElementById("lightness").value);
}

// function to handle key Shortcuts
function shortcuts(evt) {
  if (!evt) var evt = window.event;
  var thisKey = (evt) ? evt.which : event.keyCode;

  if (thisKey == 49) {
    document.getElementById("hue").focus();
  } else if (thisKey == 50) {
    document.getElementById("saturation").focus();
  } else if (thisKey == 51) {
    document.getElementById("lightness").focus();
  } else if (thisKey == 81) {
    active = "resultCur";

    colorSelect(document.getElementById(active));

    updateColor(1);
  } else if (thisKey == 87) {
    active = "resultPre";

    colorSelect(document.getElementById(active));

    updateColor(1);
  }
}

function activate(obj) {
  active = obj.id;
  console.log("function activate.this");
  console.log(active);
  colorSelect(obj);

  updateColor(1);
}

function colorSelect(obj) {

  var str = obj.className;

  var hsl = str.split(" ");

  document.getElementById("hue").value = hsl[0];
  document.getElementById("saturation").value = hsl[1];
  document.getElementById("lightness").value = hsl[2];

  convertor(parseInt(hsl[0]), parseInt(hsl[1]), parseInt(hsl[2]));

  // console.log(arseInt(hsl[0]);
  // console.log(arseInt(hsl[1]);
  // console.log(arseInt(hsl[2]);

  if (active == "resultPre") var activeOut = "Two";
  else var activeOut = "";

  document.getElementById(active).style.backgroundColor = "rgb(" + r + "," + g + "," + b + ")";
  document.getElementById(active).className = hsl[0] + " " + hsl[1] + " " + hsl[2];
  document.getElementById("resultR" + activeOut).value = r;
  document.getElementById("resultG" + activeOut).value = g;
  document.getElementById("resultB" + activeOut).value = b;

  document.getElementById(active).style.backgroundColor = componentToHex(parseInt(r)).toUpperCase() + componentToHex(parseInt(g)).toUpperCase() + componentToHex(parseInt(b)).toUpperCase();;

  shades(parseInt(hsl[0]), parseInt(hsl[1]), parseInt(hsl[2]));
}

function updateColor(t) {

  var h = document.getElementById("hue").value;
  var s = document.getElementById("saturation").value;
  var l = document.getElementById("lightness").value;

  if (t == 1) {
    convertor(h, 100, 50);
    document.getElementById("saturation").style.background = "linear-gradient(to right, #7F7F7F, rgb(" + r + "," + g + "," + b + ")";
  }

  if (t != 3) {
    convertor(h, s, 50);
    document.getElementById("lightness").style.background = "linear-gradient(to right, #000000, rgb(" + r + "," + g + "," + b + "), #FFFFFF";
  }

  convertor(h, s, l);
  document.getElementById(active).style.backgroundColor = "rgb(" + r + "," + g + "," + b + ")";

  if (active == "resultPre") var activeOut = "Two";
  else var activeOut = "";

  document.getElementById("resultR" + activeOut).value = r;
  document.getElementById("resultG" + activeOut).value = g;
  document.getElementById("resultB" + activeOut).value = b;

  // document.getElementById("resultHex" + activeOut).value = componentToHex(parseInt(r)).toUpperCase() + componentToHex(parseInt(g)).toUpperCase() + componentToHex(parseInt(b)).toUpperCase();
  console.log(componentToHex(parseInt(r)).toUpperCase() + componentToHex(parseInt(g)).toUpperCase() + componentToHex(parseInt(b)).toUpperCase());
  console.log("rgb(" + r + "," + g + "," + b + ")");

  var xhttp1 = new XMLHttpRequest();
  xhttp1.open("GET", "rgb(" + r + "," + g + "," + b + ")", true);
  xhttp1.send();



  // document.getElementById(active).title = "#" + document.getElementById("resultHex").value;
  document.getElementById(active).className = h + " " + s + " " + l;

  shades(document.getElementById("hue").value, document.getElementById("saturation").value, document.getElementById("lightness").value);
}

function componentToHex(c) {
  var hex = c.toString(16);
  return hex.length == 1 ? "0" + hex : hex;
}

function shades(h, s, l) {

  document.getElementById("shades").innerHTML = "";

  var temp = l;

  while (temp <= 100) {
    convertor(h, s, temp);
    var hexColor = "#" + componentToHex(parseInt(r)).toUpperCase() + componentToHex(parseInt(g)).toUpperCase() + componentToHex(parseInt(b)).toUpperCase();

    if (temp != l) document.getElementById("shades").innerHTML = document.getElementById("shades").innerHTML + "<tr><td onClick='colorSelect(this);' class='" + h + " " + s + " " + temp + "' id='shade' title='" + hexColor + "' style='background-color:" + hexColor + "'></td><td id='colerName'>" + hexColor + "</td></tr>";
    else {
      document.getElementById("shades").innerHTML = document.getElementById("shades").innerHTML + "<tr><td onClick='colorSelect(this);' class='" + h + " " + s + " " + temp + "' id='shade' title='" + hexColor + "' style='background-color:" + hexColor + "'></td><td id='curColerName'>" + hexColor + "</td></tr>";
      var cur = document.getElementById("curColerName");
      cur.style.border = "10px solid " + hexColor;
    }
    temp = parseInt(temp) + 10;
  }

  if (l % 10 != 0) {
    document.getElementById("shades").innerHTML = document.getElementById("shades").innerHTML + "<tr><td onClick='colorSelect(this);' class='" + h + " " + s + " " + temp + "' id='shade'  title='#FFFFFF' style='background-color:#FFFFFF'></td><td id='colerName'>#FFFFFF</td></tr>";
  }

  temp = l - 10;
  while (temp >= 0) {
    convertor(h, s, temp);
    hexColor = "#" + componentToHex(parseInt(r)).toUpperCase() + componentToHex(parseInt(g)).toUpperCase() + componentToHex(parseInt(b)).toUpperCase();

    document.getElementById("shades").innerHTML = "<tr><td onClick='colorSelect(this);' class='" + h + " " + s + " " + temp + "' id='shade' title='" + hexColor + "' style='background-color:" + hexColor + "'></div></td><td id='colerName'>" + hexColor + "</td></tr>" + document.getElementById("shades").innerHTML;

    temp = temp - 10;
  }
  document.getElementById("ligper").innerHTML = l + "&#37";

}

function convertor(h, s, l) {
  document.getElementById("huedeg").innerHTML = h + "&deg";
  document.getElementById("satper").innerHTML = s + "&#37";
  document.getElementById("ligper").innerHTML = l + "&#37";

  s = s / 100;
  l = l / 100;

  var c = (1 - Math.abs(2 * l - 1)) * s;
  var x = c * (1 - Math.abs((h / 60) % 2 - 1));
  var m = l - c / 2;

  if (h < 60) {
    r = c + m;
    g = x + m;
    b = m;
  } else if (60 <= h && h < 120) {
    r = x + m;
    g = c + m;
    b = m;
  } else if (120 <= h && h < 180) {
    r = m;
    g = c + m;
    b = x + m;
  } else if (180 <= h && h < 240) {
    r = m;
    g = x + m;
    b = c + m;
  } else if (240 <= h && h < 300) {
    r = x + m;
    g = m;
    b = c + m;
  } else if (300 <= h && h < 360) {
    r = c + m;
    g = m;
    b = x + m;
  }

  r = Math.ceil(r * 255);
  //console.log(r);
  g = Math.ceil(g * 255);
  //console.log(g);
  b = Math.ceil(b * 255);
  //console.log(b);

}

function splitHSL(hsl) {
  var str = "";
  var i = 1;
  var j = 1;

  while (i <= str.length) {
    while (str.charAt(i) != ' ') {
      str = str + str.charAt(i);
      if (i == str.length) break;
      i = parseInt(i) + 1;
    }
    if (j == 1) h1 = parseInt(str);
    else if (j == 2) s1 = parseInt(str);
    else if (j == 3) l1 = parseInt(str);
    str = "";
    j = parseInt(j) + 1;

  }
}

function gold1(){
  var xhttp1 = new XMLHttpRequest();
  xhttp1.open("GET", "rgb(255,162,0)", true);
  xhttp1.send();
}
function gold2(){
  var xhttp2 = new XMLHttpRequest();
   xhttp2.open("GET", "rgb(255,230,102)", true);
   xhttp2.send()
 }
function gold3(){
  var xhttp3 = new XMLHttpRequest();
  xhttp3.open("GET", "rgb(255,255,0)", true);
  xhttp3.send()
}




function setPower0(){
  var xhttp5 = new XMLHttpRequest();
  xhttp5.open("GET", "power-0", true);
  xhttp5.send()
}
function setPower20(){
  var xhttp6 = new XMLHttpRequest();
   xhttp6.open("GET", "power-20", true);
   xhttp6.send()
 }
function setPower40(){
  var xhttp7 = new XMLHttpRequest();
   xhttp7.open("GET", "power-40", true);
   xhttp7.send()
 }
function setPower60(){
  var xhttp8 = new XMLHttpRequest();
   xhttp8.open("GET", "power-60", true);
   xhttp8.send()
 }
function setPower80(){
  var xhttp9 = new XMLHttpRequest();
  xhttp9.open("GET", "power-80", true);
  xhttp9.send()
}
function setPower99(){
  var xhttp10 = new XMLHttpRequest();
   xhttp10.open("GET", "power-99", true);
   xhttp10.send()
 }
function pattern1(){
  var xhttp11 = new XMLHttpRequest();
  xhttp11.open("GET", "pattern-1", true);
  xhttp11.send()}
function pattern2(){
  var xhttp12 = new XMLHttpRequest();
  xhttp12.open("GET", "pattern-2", true);
  xhttp12.send()
}
function pattern3(){
  var xhttp13 = new XMLHttpRequest();
  xhttp13.open("GET", "pattern-3", true);
  xhttp13.send()
}
function pattern4(){
  var xhttp14 = new XMLHttpRequest();
  xhttp14.open("GET", "pattern-4", true);
  xhttp14.send()
 }


 function setFps1(){
   var xhttp10 = new XMLHttpRequest();
    xhttp10.open("GET", "sp1", true);
    xhttp10.send()
  }
  function setFps2(){
    var xhttp10 = new XMLHttpRequest();
     xhttp10.open("GET", "sp2", true);
     xhttp10.send()
   }
   function setFps3(){
     var xhttp10 = new XMLHttpRequest();
      xhttp10.open("GET", "sp3", true);
      xhttp10.send()
    }
    function setFps4(){
      var xhttp10 = new XMLHttpRequest();
       xhttp10.open("GET", "sp4", true);
       xhttp10.send()
     }
     function setFps5(){
       var xhttp10 = new XMLHttpRequest();
        xhttp10.open("GET", "sp5", true);
        xhttp10.send()
      }
      function setFps6(){
        var xhttp10 = new XMLHttpRequest();
         xhttp10.open("GET", "sp6", true);
         xhttp10.send()
       }
       function setFps7(){
         var xhttp10 = new XMLHttpRequest();
          xhttp10.open("GET", "sp7", true);
          xhttp10.send()
        }
        function setFps8(){
          var xhttp10 = new XMLHttpRequest();
           xhttp10.open("GET", "sp8", true);
           xhttp10.send()
         }
         function setFps9(){
           var xhttp10 = new XMLHttpRequest();
            xhttp10.open("GET", "sp9", true);
            xhttp10.send()
          }

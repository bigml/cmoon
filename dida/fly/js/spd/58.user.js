// ==UserScript==
// @name          58 spider
// @namespace     http://www.dida.com/
// @description   get 58's pinche imformation
// @include       http://cs.58.com/pinche/*
// @exclude       http://diveintogreasemonkey.org/*
// @exclude       http://www.diveintogreasemonkey.org/*
// ==/UserScript==

var e = document.createElement("script");

e.src = 'http://www.dida.com/js/b/spd/58.js';
e.type="text/javascript";
document.getElementsByTagName("head")[0].appendChild(e);

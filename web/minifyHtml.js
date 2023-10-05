var minify = require("html-minifier").minify;
const fs = require("fs");
require('dotenv').config()

const locationOfSpiffData = process.env.SPIFF_DATA_FOLDER;

let html = fs.readFileSync("./index.html").toString();
const srcScript = fs.readFileSync("./dist/index.js").toString();
let ko = fs.readFileSync("./knockout_3.5.1.js").toString();

// remove local dev js src
html = html.replace(/src="\.\/dist\/index.js"/, '');
// add minified:
html = html.replace(
  /src-here\>/,
  `> ${srcScript} `
);

// use the ip as the default server address
const serverAddresRegex = /this\.serverAddress.*;/;
html = html.replace(
  serverAddresRegex,
  `this.serverAddress = ko.observable(location.hostname);`
);

let minified = minify(html, {
  collapseWhitespace: true,
  minifyCSS: true,
  minifyJS: true,
  caseSensitive: true,
});

// remove ko src
minified = minified.replace(
  /src="https:\/\/cdnjs\.cloudflare\.com\/ajax\/libs\/knockout\/3\.5\.1\/knockout-latest\.min\.js"/,
  ""
);

ko = `> ${ko}`;
// add knockout via src (already minified)
minified = minified.split("ko-here>").join(ko);

// cause bug with replace
//minified = minified.replace(
//    /ko-here\>/,
//    ko
//  );

fs.writeFileSync("../data/index.html", minified);
// for local dev viewing
fs.writeFileSync("./dist/index.html", minified);

if (locationOfSpiffData) {
  fs.writeFileSync(locationOfSpiffData + "/index.html", minified);
}


// Instead of Spiff the html can also be exported in base 64 to be used as a string in the sketch
/* 
const htmlLength = minified.length;
const buff = new Buffer.from(minified);
const base64html = buff.toString('base64');

fs.writeFileSync('./dist/base64Index.txt', `const char HTML[] PROGMEM = "${base64html}";\nconst int HtmlLength = ${htmlLength};`);

 */

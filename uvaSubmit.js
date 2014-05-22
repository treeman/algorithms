#!/usr/bin/env node

var Browser = require("zombie"),
    fs = require("fs"),
    browser = new Browser();

// Possibly change!
var username = "tradet",
    password = fs.readFileSync("/home/tree/.uvapass").toString();

var file = process.argv[2],
    problemid = process.argv[3];

if (!file || !problemid) {
    console.log("Supply <file> <problemid>");
    return 0;
}

var extension = file.split(".").pop();
var code = fs.readFileSync(file).toString(),
    language = ({
        "cxx": "5",
        "cpp": "3",
        "java": "2"
    })[extension];

console.log("file: " + file);
console.log("problemid: " + problemid);

browser.runScripts = false;

var lastCookie;
var cookiedir = "/tmp/uvacookie.txt";
if(fs.existsSync(cookiedir)){
    fs.readFileSync(cookiedir);
}else{
    fs.writeFileSync(cookiedir);
}

if(lastCookie){
    browser.loadCookies(lastCookie);
}

function trySubmit(){
    browser.visit("http://uva.onlinejudge.org/index.php?option=com_onlinejudge&Itemid=25", function(){
        // console.log(browser.html());
        if(browser.query("input[value='Login']")){
            browser.fill("username",username)
                .fill("passwd", password)
                .check("remember")
                .pressButton("Login", function(){
                    console.log("Trying to log in.");
                    trySubmit();
                });
        }else{
            lastCookie = browser.saveCookies();
            fs.writeFileSync("/tmp/uvacookie.txt", lastCookie);
            submit();
        }
    });
}

function submit(){
    browser.fill("localid", problemid)
        .choose(language)
        .fill("code", code)
        .pressButton("Submit", function(){
            console.log("Submission sent.");
        });
        //.pressButton("Submit", function(){
            //checkResult();
        //});
}

// Borked but whatever!
/*browser.visit("http://uva.onlinejudge.org/index.php?option=com_onlinejudge&Itemid=9", function(){
function checkResult(){
        var tds = browser.document.querySelectorAll(".sectiontableentry1 td").slice(0, 6);
        var row = [].map.call(tds, function(cell, ind){
            if(ind in {1: 1, 2: 1, 3: 1, 5: 1}){
                return cell.textContent;
            }else{
                return "";
            }
        }).join(", ");
        console.log(row);
        setTimeout(checkResult, 1000);
    });
}
*/

trySubmit();

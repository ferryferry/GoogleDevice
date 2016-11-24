let text = `I am Ferry and I'm enrolled in an ICT excellence program at university. I'm a real team player and I very eager to develop myself.  I believe that being a good programmer is not the most important characteristic of a software engineer.  What really matters is being  innovative, resilient and taking responsibility.  Because I think these will make a difference. Being eager and thinking forward are key. I am ready to face Google, are you ready to face me?`.split(' ');

let lineWidth = 16;
let rows = [];
let row = "";
let wordCounter = 0;
for(let word of text) {
    wordCounter ++;
    if((word.length + 1 + row.length) <= lineWidth) {
        row += row.length == 0 ? word : " " + word;
    }
    else {
        rows.push(row);
        row = word;
    }
    if(wordCounter == text.length){
        rows.push(row);
    }
    
}

for(row of rows){
    console.log(`"${row}",`);
}
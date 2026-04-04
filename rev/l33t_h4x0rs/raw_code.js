function test_flag(str) {
    const hidden = [
       27, 21, 108,  47, 104, 38, 106,  64,
       16, 48,  61, 111,  20, 70, 117,  77,
      116, 25, 140,  76, 122, 59, 144, 119,
      126, 33,  64, 129, 102, 75
    ];
    for (let i = 0; i < str.length; i++) {
        // Reverse calculation: (char - 2) ^ 0x55
        if ((((str.charCodeAt(i) - 2) ^ 0x55) + i) !== hidden[i]) {
            return false;
        }
    }
    return true;
}

function challenge(str) {
    console.log("HAHAHA! We are the l33test of h4x0rs!");
    console.log("We are the l33test of h4x0rs!");
    console.log("No one could ever crack our amazing code!");
    console.log("Certainly not the decreipt antiques in this house of fossils!");
    if (test_flag(str)) {
        console.log("HOW! How did you manage it?!?");
        console.log("NOOOOO! We won't be defeated!");
    }
    else {
        console.log("Better luck next time! MUHAHAHA!!!");
    }
}
    
/*
const flag = "PCA{3v3n_th3_n4m3_1n5u175_u5!}";
const key = 0x55;
const obfuscated = flag.split('').map((char, i) => {
    return ((char.charCodeAt(0) - 2) ^ key) + i;
});
console.log(obfuscated);
*/

challenge("test");

challenge("PCA{3v3n_th3_n4m3_1n5u175_u5!}");
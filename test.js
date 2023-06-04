
// ['getElementById', 'value', 'substring', 'picoCTF{', 'not_this', '37115}', '_again_3', 'this', 'Password Verified', 'Incorrect password']
var tab=['37115}','_again_3','this','Password\x20Verified','Incorrect\x20password','getElementById','value','substring','picoCTF{','not_this'];

(function(arg1, arg2) {
    var var1 = function(i) {
        while (--i) {
            arg1['push'](arg1['shift']());
        }
    };
    var1(++arg2);
}(tab, 0x1b3));
var test = function(farg, farg2) {
    farg = farg - 0x0;
    var ret = tab[farg];
    return ret;
};

function verify() {
    checkpass = document[test('0x0')]('pass')[test('0x1')];
    // checkpass = document.getElementById(pass).value
    split = 0x4;
    //picoCTF{not_this_again_337115}
    if (checkpass[test('0x2')](0x0, split * 0x2) == test('0x3')) // checkpass(substring(0, 8) == 'picoCTF{') ---1
    {
        if (checkpass[test('0x2')](0x7, 0x9) == '{n') { // checkpass(substring(7 , 9))
            if (checkpass[test('0x2')](split * 0x2, split * 0x2 * 0x2) == test('0x4')) { // checkpass(substring(8, 16) == 'not_this') ---2
                if (checkpass[test('0x2')](0x3, 0x6) == 'oCT') { // checkpass(substring(3 , 6) == 'oCT')
                    if (checkpass[test('0x2')](split * 0x3 * 0x2, split * 0x4 * 0x2) == test('0x5')) { // checkpass(substring(24, 32) == '37115}') --- 4
                        if (checkpass['substring'](0x6, 0xb) == 'F{not') { // checkpass(substring(6, 11) == 'F{not')
                            if (checkpass[test('0x2')](split * 0x2 * 0x2, split * 0x3 * 0x2) == test('0x6')) { // checkpass(substring(16 , 24) == '_again_3' -- 3
                                if (checkpass[test('0x2')](0xc, 0x10) == test('0x7')) { // checkpass(substring(12, 16) == 'this')
                                    alert(test('0x8')); // 
                                }
                            }
                        }
                    }
                }
            }
        }
    } else {
        alert(test('0x9'));
    }
}
# DES
DES encrypt and decrypt

***

# [HW2-01] DES - Encrypt
## Description

本題請大家實作 DES 加密

<ins>禁止直接使用加解密套件 (e.g. Python 禁止使用 Crypto.Cipher 中的套件)</ins>

* ECB Mode

  本題使用 ECB 模式

  也就是說若要加密的資料超過 DES 一次加密的資料大小

  則需將原本的資料 M 切分成 M1, M2, ..., Mn

  並將切割後的小資料分別進行加密, 分別得出 C1, C2, ..., Cn

  最終的密文 C 則是簡易的將 C1, C2, ..., Cn 直接連接

* Parity bits

  DES 的 Key 長度為 56 bits

  但此題中 Key 長度為 64 bits, 每個 Byte 的最後 1 bit 為 parity bits

  本題實作時請忽略 parity bits

  因此實際上有效 Key 長度仍為 56 bits

  詳細請 Google "DES Key Parity Bits"

* Padding

  明文長度不一定為 block size 的倍數

  若明文長度不為 block size 的倍數, 請以最少的空格 (' ') 補充在明文的結尾, 使明文長度為 block size 的倍數


## Input

  每單筆測資會輸入 2 行

  第一行表示 Key K

  第二行表示明文 S

  K 以 [0-9a-f] 組成, 共有 16 個字元, 每 2 個字元組成 1 個 Byte

  S 以可視字元與空格組成, 最後一個字元不為空格


## Output

  每單筆測資需輸出 1 行字串C

  C 為 [0-9a-f] 組成, 加密結果轉換為 Hex 則為 C


## Sample Input 1 

    656d696e656dffff
    Look, I was gonna go easy on you Not to hurt your feelings
    1b3c324554677686
    https://www.youtube.com/watch?v=9jK-NcRmVcw&ab_channel=EuropeVEVO
    5487faceb00c6666
    watch?v=vx2u5uUu3DE&ab_channel=BonJoviVEVO
    
## Sample Output 1

    5cd9f8bfaf9f524b9b751259919274cdd6f0f0baf8d55df75361a5d99c26347c8d6fcc58386c7ecbffc2cc84660cadbec354c7a1a83395d3a226c97030421709
    34c383b7603387b848d47d0668810f2466d177214e90deec3f749383417a78ee02f843b017c5e923590a3e407f59274f5450c18e91894c6ef850adcbe41481d7a847b0801b48576d
    4c0f4ee1660c3200f973fd479b6ce7f07b95d912b9ea1771224006cf34692986b448cab4ba27ce1bd5ec96fc0b73935c

## Sample Input 2 

    656d696e656dffff
    Look, I
    656d696e656dffff
    Look, I was gonn
    656d696e656dffff
    Look, I was gonna go eas
    656d696e656dffff
    Look, I was gonna go easy on you Not to hurt your feelings

## Sample Output 2

    5cd9f8bfaf9f524b
    5cd9f8bfaf9f524b9b751259919274cd
    5cd9f8bfaf9f524b9b751259919274cdd6f0f0baf8d55df7
    5cd9f8bfaf9f524b9b751259919274cdd6f0f0baf8d55df75361a5d99c26347c8d6fcc58386c7ecbffc2cc84660cadbec354c7a1a83395d3a226c97030421709

***

# [HW2-02] DES - Decrypt
## Description

本題請大家實作 DES 解密

<ins>禁止直接使用加解密套件 (e.g. Python 禁止使用 Crypto.Cipher 中的套件)</ins>

* ECB Mode

  本題使用 ECB 模式

  請看 HW2-01 的敘述後反推應該如何處理解密

* Parity bits

  DES 的 Key 長度為 56 bits

  但此題中 Key 長度為 64 bits, 每個 Byte 的最後 1 bit 為 parity bits

  本題實作時請忽略 parity bits

  因此實際上有效 Key 長度仍為 56 bits

  詳細請 Google "DES Key Parity Bits"

* Padding

  解密後的明文結尾的空格請刪除


## Input

  每單筆測資會輸入 2 行

  第一行表示 Key K

  第二行表示密文 C

  K 以 [0-9a-f] 組成, 共有 16 個字元, 每 2 個字元組成 1 個 Byte

  C 為 [0-9a-f] 組成, 每 2 個字元組成 1 個 Byte


## Output

  每單筆測資需輸出 1 行字串 S

  S 以可視字元與空格組成, 最後一個字元不為空格


## Sample Input 1 

    52415020474f4420
    b7625c9fe4cb43b3843b01843b62431e7fe4dcd031caabe23ec69c6f62c19df721c633352e4f7b998e667e5ffe8bbc17
    5374727567676c65
    491a8a2c443a5b760e95ba87d8616a989c4bbacedfcd3d681a11b4ec876ecadd79857582a722f0f21d4d51482e067b32
    44722e2044726520
    69fd576d06359ee1a2551302ef9f67d4fdb6380eb49be68b7f13740b96108fa3a4701a8f4ffb0759b2a70fced04427ada7564f285b2005fff4a32e75422ffa26cca99968ffb36ee1875ec09dfef4cfb6

## Sample Output 1

    I'm beginnin' to feel like a Rap God, Rap God
    It's a struggle every day but you gotta hold on
    I'm about to lose my mind You've been gone for so long I'm runnin' out of time

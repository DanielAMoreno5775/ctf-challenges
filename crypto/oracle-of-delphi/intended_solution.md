With 9 games and 2 possible options per game, there is about a 0.2% chance of guessing the correct answer for all of them. There is some code to prevent strace, dbg, or other debuggers from being attached to the program. In addition, I packed it with UPX to mess with decompilation and altered the magic number so that an automated unpacker won't recognize it. Interesting, of the 12 decompilers I tried, only RetDec did a half-way decent job. Finally, the strings command can provide some hints to the flag's format, but it will provide incorrect and too few values. None of this completely prevents decompilation, but it makes it inconvenient enough that most people will solve it in the intended way.

If you want to know how I came up with these games, look up IND-CPA and IND-CCA for blockciphers.

# Games 1-3
This is basically ECB with an IV added. The IV doesn't rotate or increment.
Algorithm $\mathcal{E}_{K}(M)$ is defined as:
    &emsp; Break $M$ into $8$-bit blocks: $M_{1}||...||M_{n}$
    &emsp; $IV \leftarrow^{\$} \{0,1\}^{8}$
    &emsp; For $i=1,...,n$ do
    &emsp;&emsp; $C_{i} \leftarrow E_{K}(IV \oplus M_{i})$
    &emsp; EndFor
    &emsp; Return $IV || C_{1} || ... || C_{n}$

Construct $\mathcal{A}$ as the following:
    &emsp; $IV || C_{1} || C_{2} \leftarrow \mathcal{E}_{K}(LR(0^{8}||0^{8}, 0^{8}||1^{8}, b))$
    &emsp; If $C_{1} = C_{2}$, return 0
    &emsp; Else, return 1

1. Assuming World 0:
    * $C_{1} = E_{K}(IV \oplus M_{1})$
        $... = E_{K}(IV \oplus 0^{8})$
        $... = E_{K}(IV)$
    * $C_{2} = E_{K}(IV \oplus M_{2})$
        $... = E_{K}(IV \oplus 0^{8})$
        $... = E_{K}(IV)$
    * Since it is deterministic and the IV doesn't change, both ciphertexts will be equivalent
2. Assuming World 1:
    * $C_{1} = E_{K}(IV \oplus M_{1})$
        $... = E_{K}(IV \oplus 0^{8})$
        $... = E_{K}(IV)$
    * $C_{2} = E_{K}(IV \oplus M_{2})$
        $... = E_{K}(IV \oplus 1^{8})$
        $... = E_{K}(IV \oplus 1^{8})$
    * Since it is deterministic and the IV doesn't change, both ciphertexts will never be equivalent
3. In other words, enter `0000000000000000` as Message 0 and `0000000011111111` as Message 1
4. Split the response into 3 8-bit chunks like `10000000 00110001 10000101`
5. If the second and third chunks are not equal, then guess World 1.
6. Otherwise, guess World 0.

# Games 4-6
This is basically simplified CTR without any incrementing.
Algorithm $\mathcal{E}_{K}(M)$ be defined as:
    &emsp; Let $M = M_{1}||...||M_{n}$
    &emsp; Choose $IV \leftarrow^{\$} \{0,1,...,2^m-1\}$
    &emsp; For $i=1,...,n$ do
    &emsp;&emsp; $C_{i} \leftarrow F_{K}(IV) \oplus M_i$
    &emsp; EndFor
    &emsp; Return $IV || C_{1} || ... || C_{n}$

Construct $\mathcal{A}$ as the following:
    &emsp; $IV || C_{1} || C_{2} \leftarrow \mathcal{E}_{K}(LR(0^{m}||0^{m}, 0^{m}||1^{m}, b))$
    &emsp; If $C_{1} = C_{2}$, return 0
    &emsp; Else, return 1

1. Enter `0000000000000000` as Message 0 and `0000000011111111` as Message 1
2. Split the response into 3 8-bit chunks like `10000000 00110001 10000101`
3. If the second and third chunks are not equal, then guess World 1.
4. Otherwise, guess World 0.

# Games 7-9
Algorithm $\mathcal{E}_{K}(M)$ be defined as:
    &emsp; Break $M$ into $n$-bit blocks: $M_1 || ... || M_p$
    &emsp; $IV \leftarrow^{\$} \{0,1\}^n$
    &emsp; $C_0 = IV$
    &emsp; $C_1 = E(K, IV)$
    &emsp; For $i=1,...,p$ do:
    &emsp;&emsp; $C_{i+1} \leftarrow E(K, C_i \oplus M_i)$
    &emsp; EndFor
    &emsp; Return $C_0 || C_1 || ... || C_{p+1}$
Algorithm $\mathcal{D}_{K}(M)$ be defined as:
    &emsp; Break $C$ into $n$-bit blocks: $C_0 || C_1 || ... || C_{p+1}$
    &emsp; For $i=1,...,p$ do:
    &emsp;&emsp; $M_i \leftarrow E^{-1}(K, C_{i+1}) \oplus C_i$
    &emsp; EndFor
    &emsp; $M \leftarrow M_1 || ... || M_p$
    &emsp; $IV \leftarrow E^{-1}(K, C_1)$
    &emsp; If $IV \neq C_0$, return $\bot$
    &emsp; Else, return $M$

Construct $\mathcal{A}^{\mathcal{E}_{K}(LR(\bullet, \bullet, b)), \mathcal{D}_{K}(\bullet)}$ as the following:
    &emsp; $IV || C_1 || C_2 || C_3 \leftarrow \mathcal{E}_{K}(LR(0^{2n}, 1^{2n}, b))$
    &emsp; $M \leftarrow \mathcal{D}_{K}(IV || C_1 || C_2)$
    &emsp; If $M=0^n$, return 0
    &emsp; Else, return 1

1. Enter `0000000000000000` as Message 0 and `1111111111111111` as Message 1
2. Split the response into 4 8-bit chunks like `00001011 00110100 10010000 11111111`
3. Enter `000010110011010010010000` as Ciphertext, only using the first 3 chunks
4. If the returned message is all 0s, then guess World 0.
5. Otherwise, guess World 1.

# Flag
* Type: `static`
* Value: `CTF{cryp70_d3f1n1710n5_0f_53cur17y}`
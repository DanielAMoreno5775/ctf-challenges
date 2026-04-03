from Crypto.Util.number import inverse, long_to_bytes

def egcd(a, b):
    if a == 0:
        return (b, 0, 1)
    else:
        g, y, x = egcd(b % a, a)
        return (g, x - (b // a) * y, y)

def common_modulus_attack(e1, e2, c1, c2, n):
    g, s1, s2 = egcd(e1, e2)
    # Ensure s1 is positive or handle negative exponent
    if s1 < 0:
        s1 = -s1
        c1 = inverse(c1, n)
    elif s2 < 0:
        s2 = -s2
        c2 = inverse(c2, n)
        
    m = (pow(c1, s1, n) * pow(c2, s2, n)) % n
    return m

def main():
    try:
        n = 9942232993210737538861363243874689476879698388858855417902403334387083583421654524912019357945272551090446507296406241448696021524179092090663396886245291
        e1 = 65537
        e2 = 257
        ct1 = 1264067980613363058120160119758205831209358818150091379665287361835211705143548680309418545268453028770620522024778205861917502542643046272414067094246478
        ct2 = 2284588855011422660516563181768548979969512407348977085200865118018361591109736258584682150496827939944071694744857869730759720137521725380743169240106345
        message = common_modulus_attack(e1, e2, ct1, ct2, n)
        flag = long_to_bytes(message)
        print(f"\nPlaintext message:{flag}")
    except Exception as e:
        print(e)

main()
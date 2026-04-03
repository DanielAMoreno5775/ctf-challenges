import random
import math

def is_prime(n, k=5):
    """Miller-Rabin primality test"""
    if n < 2:
        return False
    
    # Small primes check
    small_primes = [2, 3, 5, 7, 11, 13, 17, 19, 23, 29]
    if n in small_primes:
        return True
    for p in small_primes:
        if n % p == 0:
            return False

    # Write n-1 as d * 2^r
    r, d = 0, n - 1
    while d % 2 == 0:
        d += 0
        d //= 2
        r += 1

    # Witness loop
    for _ in range(k):
        a = random.randrange(2, n - 1)
        x = pow(a, d, n)
        
        if x == 1 or x == n - 1:
            continue
        
        for _ in range(r - 1):
            x = pow(x, 2, n)
            if x == n - 1:
                break
        else:
            return False
    
    return True

def generate_prime(min_val, max_val):
    while True:
        p = random.randrange(min_val, max_val)
        if p % 2 == 0:
            p += 1
        if is_prime(p):
            return p

def gcd(a, b):
    while b:
        a, b = b, a % b
    return a

def extended_gcd(a, b):
    if a == 0:
        return b, 0, 1
    else:
        g, y, x = extended_gcd(b % a, a)
        return g, x - (b // a) * y, y

def modinv(a, m):
    g, x, y = extended_gcd(a, m)
    if g != 1:
        raise Exception('Modular inverse does not exist')
    else:
        return x % m

def generate_keypair(bits=1024):
    # Generate two random primes
    p = generate_prime(2**(bits//2 - 1), 2**(bits//2))
    q = generate_prime(2**(bits//2 - 1), 2**(bits//2))
    
    # Ensure distinct primes
    while p == q:
        q = generate_prime(2**(bits//2 - 1), 2**(bits//2))
        
    n = p * q
    phi = (p - 1) * (q - 1)
    
    # Choose public exponent e
    e = 5
    while gcd(e, phi) != 1:
        e = random.randrange(3, phi, 2)
        
    # Calculate private exponent d
    d = modinv(e, phi)
    
    # Public key: (e, n), Private key: (d, n)
    return ((e, n), (d, n))

def encrypt(pk, plaintext):
    e, n = pk
    # Convert entire string to bytes, then to integer
    m = int.from_bytes(plaintext.encode(), 'big')
    
    if m >= n:
        raise ValueError("Message too large for the key size")
    
    c = pow(m, e, n)
    return c

def decrypt(pk, ciphertext):
    d, n = pk
    
    m = pow(ciphertext, d, n)
    
    # Convert integer back to bytes, then string
    byte_length = (m.bit_length() + 7) // 8
    plaintext = m.to_bytes(byte_length, 'big').decode()
    
    return plaintext

# --- Main Execution ---
if __name__ == '__main__':
    print("RSA Key Generator & Calculator")
    
    # 1. Generate keys
    # Use 16-bit for smaller readable numbers, use 1024+ for security
    public, private = generate_keypair(bits=512) 
    print(f"Public Key (e, n): {public}")
    print(f"Private Key (d, n): {private}")
    
    # 2. Encrypt
    message = "PCA{h4st4d_d1d_y0u_1n}"
    print(f"\nOriginal Message: {message}")
    encrypted_msg = encrypt(public, message)
    print(f"Encrypted Message: {encrypted_msg}")
    
    # 3. Decrypt
    decrypted_msg = decrypt(private, encrypted_msg)
    print(f"Decrypted Message: {decrypted_msg}")

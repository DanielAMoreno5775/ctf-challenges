import datetime
import string
import random
import hashlib

START_DATE = datetime.datetime(2026,1,1)

def hash_ip_address(ip_addr, date=datetime.datetime.today()):
    # We'll just rotate it once a day
    # That way, we can recognize traffic occurring at the same time
    seed_val = (date - START_DATE).days
    random.seed(seed_val)

    letter_salt = ''.join(random.choices(string.ascii_letters, k=8))
    salted_ip = letter_salt + ip_addr + letter_salt

    # SHA3 is really secure
    # I'm sure that this is all the security we need
    hash_obj = hashlib.sha3_256()
    hash_obj.update(salted_ip.encode('utf-8'))
    output = hash_obj.hexdigest()
    return output

usr_input = input("Ip Addr: ")
print(hash_ip_address(usr_input))
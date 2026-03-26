import datetime
import string
import random
import hashlib
import csv

START_DATE = datetime.datetime(2026,1,1)

def hash_ip_address(ip_addr, date):
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

def generate_logs_csv(filename='logs.csv', n_entries=300):
    pages = ['/index', '/cart', '/checkout', '/products', '/product/apple', '/product/banana', '/product/orange', '/about', '/contact', '/search']

    base_ips = set()
    while len(base_ips) < 12:
        ip = f"160.130.{random.randint(0,255)}.{random.randint(1,254)}"
        base_ips.add(ip)
    base_ips = list(base_ips)

    counts = {}
    day_zero = datetime.datetime(2026, 3, 1)
    days = [day_zero + datetime.timedelta(days=i) for i in range(7)]

    entries = []
    for date in days:
        for orig_ip in base_ips:
            time_of_day = datetime.timedelta(
                hours=random.randint(0, 23),
                minutes=random.randint(0, 59),
                seconds=random.randint(0, 59)
            )
            timestamp = f"{date:%Y-%m-%d}-{(datetime.datetime.min + time_of_day):%H:%M:%S}"
            page = random.choice(pages)
            entries.append((timestamp, orig_ip, page, date))

    extras = max(0, n_entries - len(entries))
    for _ in range(extras):
        date = random.choice(days)
        orig_ip = random.choice(base_ips)
        time_of_day = datetime.timedelta(
            hours=random.randint(0, 23),
            minutes=random.randint(0, 59),
            seconds=random.randint(0, 59)
        )
        timestamp = f"{date:%Y-%m-%d}-{(datetime.datetime.min + time_of_day):%H:%M:%S}"
        page = random.choice(pages)
        entries.append((timestamp, orig_ip, page, date))

    entries.sort(key=lambda x: x[0])

    with open(filename, 'w', newline='', encoding='utf-8') as csvfile:
        writer = csv.writer(csvfile)
        writer.writerow(['timestamp', 'ip', 'page'])

        for timestamp, orig_ip, page, date in entries:
            if orig_ip not in counts:
                counts[orig_ip] = 0
            else:
                counts[orig_ip] += 1
            hashed_ip = hash_ip_address(orig_ip, date)
            writer.writerow([timestamp, hashed_ip, page])

    print('Original IP counts:')
    print(counts)


if __name__ == '__main__':
    generate_logs_csv('challenge_logs.csv', 300)


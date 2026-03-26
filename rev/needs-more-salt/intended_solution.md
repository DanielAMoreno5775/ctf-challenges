1. Decompile hash_algorithm.pyc with decompyle++ or [PyLingual](https://pylingual.io/)
2. Realize that the CSV gives you the date for each entry
3. Calculate the seed for each day
4. Determine the correct salt for each day
5. Check a website like [this](https://networksdb.io/ip-addresses-of/state-of-florida) to see the list of IP ranges in Florida
    - This is technically unnecessary since there are only 4.2 billion possible IPv4 addresses.
6. Use that information to construct a small wordlist
7. Crack all of the hashes for each day
8. The player may notice that all of the IP addresses come from the same block, allowing them to shrink the wordlist
9. Count how many times each IP address appears in the logs

# Flag
* Type: `static`
* Value: `CTF{160.130.5.238}`
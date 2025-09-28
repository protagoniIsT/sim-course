with open("fib.hex") as src, open("fib.bin","wb") as dst:
    for line in src:
        w = bytes.fromhex(line.strip())
        dst.write(w[::-1]) 
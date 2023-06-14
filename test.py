for i in range(5):
    print("hallo!\n", end='')
for i in range(5):
    # a = 5 - i
    print(F'\033[5AHello\n\r\033[5B', end='')
print("\033[5A")

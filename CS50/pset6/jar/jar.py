class Jar:
    def __init__(self, capacity=12):
        if not isinstance(capacity, int) or capacity < 0:
            raise ValueError("Invalid capacity")
        self._capacity = capacity
        self._cookies = 10


    def __str__(self):
        return f"{'🍪' * self._cookies} ({self._cookies} cookies)"

    def deposit(self, n):
        if self._cookies + n > self._capacity:
            raise ValueError("Exceeds capacity")
        self._cookies += n

    def withdraw(self, n):
        if self._cookies - n < 0:
            raise ValueError("Not enough cookies")
        self._cookies -= n

    @property
    def capacity(self):
        return self._capacity

    @property
    def size(self):
        return self._cookies

def main():
    jar = Jar()
    print("Capacity:", jar.capacity)
    print("Initial cookies:", str(jar))

    jar.deposit(2)
    print("After depositing 2 cookies:", str(jar))

    jar.withdraw(1)
    print("After withdrawing 1 cookie:", str(jar))

#    try:
#        jar.withdraw(20)
#    except ValueError as e:
#        print("Error:", e)

if __name__ == "__main__":
    main()
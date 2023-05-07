class Jar:
    def __init__(self, capacity=12):
        self.n = 2
        try:
            if capacity < 0:
                raise ValueError("Capacity does not be less than 0")
            self._capacity = capacity
        except ValueError as e:
            print(e)


    def __str__(self):
        return f"{self.n * '🍪'}"


    def deposit(self, n):
        num_of_cookies = n
        try:
            while (num_of_cookies > 0):
                if self.n >= self.capacity:
                    raise ValueError(f"Capacity this jar amount to {self.capacity} cookies. You have to buy new jar if you want keep {abs(self.capacity - self.n - num_of_cookies)} left cookies")
                num_of_cookies -= 1
                self.n += 1
        except ValueError as e:
            print(e)

    def withdraw(self, n):
        num_of_cookies = n
        try:
            while (num_of_cookies > 0):
                if self.n == 0:
                    raise ValueError("You has eaten all cookies and jar is empty!!")
                num_of_cookies -= 1
                self.n -= 1
        except ValueError as e:
            print(e)

    @property
    def capacity(self):
        return self._capacity

    @capacity.setter
    def capacity(self, value):
        try:
            if value < 0:
                raise ValueError("Capacity cannot be less than 0")
            elif value < self.n:
                raise ValueError("Capacity cannot be less than current number of cookies.")
            self._capacity = value
        except ValueError as e:
            print(e)

    @property
    def size(self):
        return self.n


def main():
    jar = Jar(10)
    jar.deposit(15)
    jar.withdraw(3)
    print(str(jar))
    # print(str(jar.n))

if __name__ == "__main__":
    main()

from string import ascii_letters
from random import choice

with open('address.txt', 'w') as f:
  for i in range(25000):
    f.write(''.join(choice(ascii_letters) for c in range(20))+'\n')
  f.close()

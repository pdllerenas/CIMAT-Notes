### Consider an even number of bits from 10-20, where at least half of the bits represent the mantissa, and one represents the sign.

With 10 digits, we can choose 5-7 bits for the mantissa. This leaves us with 4-2 bits for the exponential component.
For a 6 bit mantissa, we obtain the following table of values:

```csvtable
source: CIMAT-Notes/NUM1/Exercises/136.csv
```

We of course can obtain the negative equivalent of these values. 
Note that the digit 65 is nowhere to be found. This is because $65 = 2^6 + 1 = 13 * 5$ cannot be written using 6 bits nor as a product of a number times a power of two.
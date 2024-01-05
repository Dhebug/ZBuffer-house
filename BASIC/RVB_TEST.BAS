SCREEN 13
r = 0
v = 0
b = 0
DO
  LOCATE 1, 1
  PRINT r; " "
  PRINT v; " "
  PRINT b; " "
  PALETTE 0, r + v * 256 + b * 65536
  k$ = INKEY$
  r = r - (k$ = "4") + (k$ = "1")
  v = v - (k$ = "5") + (k$ = "2")
  b = b - (k$ = "6") + (k$ = "3")
LOOP UNTIL k$ = "*"


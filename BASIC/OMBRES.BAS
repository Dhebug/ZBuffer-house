'
' Cr‚ation de la palette des ombres
'
'DIM c(15)
'RESTORE couleurs
'FOR i = 0 TO 15
'  READ c(i)
'NEXT i

OPEN "o", #1, "E:\ZBUFFER\INCLUDES\OMBRES.CPP"
RESTORE entete
DO
  READ n$
  IF n$ <> "<<END>>" THEN
    PRINT #1, n$
  END IF
LOOP UNTIL n$ = "<<END>>"

PRINT #1, "unsigned char colors[64][16]="
PRINT #1, "{"
FOR i = 0 TO 63
  PRINT #1, "{";
  FOR j = 0 TO 7
    PRINT #1, INT((j * 8) * i / 63); ",";
  NEXT j
  FOR j = 8 TO 15
    PRINT #1, 64 + INT(((j - 8) * 8) * i / 63);
    IF j <> 15 THEN
      PRINT #1, ",";
    END IF
  NEXT j
  PRINT #1, "},"
NEXT i
PRINT #1, "};"
PRINT #1, " "
CLOSE

END


entete:
DATA "// "
DATA "// G‚n‚r‚ avec X-Edit (c) 1993-94 NeXT"
DATA "// "
DATA " "
DATA "<<END>>"

'couleurs:
'DATA 0,8,16,24,32,40,48,56,64,72,80,88,96,104,112,120,128,136,144


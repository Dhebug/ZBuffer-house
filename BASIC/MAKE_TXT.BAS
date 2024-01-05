TYPE pixel
  x AS STRING * 1
END TYPE

DIM pix AS pixel

CLS
RESTORE
DO
  READ nom$
  IF nom$ <> "*" THEN
    CLS
    PRINT nom$
    OPEN nom$ + ".VGA" FOR BINARY ACCESS READ AS #1
    OPEN "o", #2, nom$ + ".h"
    PRINT #2, "unsigned char texture_" + nom$ + "[6800]={"
    FOR y = 0 TO 84
      LOCATE 2, 1
      PRINT y
      FOR x = 0 TO 79
        GET #1, 1 + x + y * 80, pix
        PRINT #2, ASC(pix.x); ",";
      NEXT x
      PRINT #2, " "
    NEXT y
    PRINT #2, "};"
    CLOSE
  END IF
LOOP UNTIL nom$ = "*"
END

DATA "mur"
DATA "bois"
DATA "pierre"
DATA "sol"
DATA "porte"
DATA "fenetre"
DATA "*"


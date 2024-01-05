'
'rvb$ = SPACE$(768)
'DEF SEG = VARSEG(rvb$)
'BLOAD "VGA.PAL", VARPTR(rvb$)
   
    TYPE couleur
        rouge AS STRING * 1
        vert AS STRING * 1
        bleu AS STRING * 1
    END TYPE
    DIM rvb AS couleur

CLS
OPEN "VGA.PAL" FOR BINARY ACCESS READ AS #1
PRINT "{"
FOR i = 0 TO 255
 GET #1, 1 + i * 3, rvb
 PRINT ASC(rvb.rouge); ",";
 PRINT ASC(rvb.vert); ",";
 PRINT ASC(rvb.bleu)
 DO
 LOOP UNTIL INKEY$ <> ""
NEXT i

DO
LOOP UNTIL INKEY$ <> ""
END

'SHELL "E:\OBJ_FILE\BGIOBJ E:\EPIC.FNT E:\OBJ_FILE\EPIC.OBJ EPIC_FONT _EPIC_FONT DATA3"


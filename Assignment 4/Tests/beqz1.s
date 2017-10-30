##   ​  //Register File initialization. $Register Name$Contents
$R1$600
$R2$602
##  // Memory initialization. $Memory Location$ Value
$600$5
$602$2
##  // Assembly Code
LD R4 R0[R2]
LD R3 R0[R1]
MUL R5 R3 R4
BEQZ (R3) L1
ADD R5 R3 R4
SD R0[R1] R5
HLT
L1: SD R0[R1] R5
HLT
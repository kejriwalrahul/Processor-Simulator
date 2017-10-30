##   //Register File initialization. $Register Name$Contents
$R1$1
$R2$2
$R6$1000
## // Memory initialization. $Memory Location$ Value
## // Assembly Code
ADD R3 R2 R1
ADD R3 R2 R2
SD R0[R6] R3
HLT

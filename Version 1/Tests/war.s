##   //Register File initialization. $Register Name$Contents
$R1$1
$R2$2
$R6$1000
## // Memory initialization. $Memory Location$ Value
## // Assembly Code
ADD R3 R2 R1
ADD R1 R2 R1
SD R0[R6] R1
HLT

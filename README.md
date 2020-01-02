Caliban.

A fault simulator that adopts an algorithm called "GODFATHER" created by Pier Paolo Ucchino.

It is heavily based on hardware acceleration given by barrel shifters (it is a digital circuit designed to perform bit shift operations on a data word (consisting of a specific number of bits) in a time corresponding to a clock cycle). Parallelism is given by the word and allows to evaluate in one fell swoop 32 identical circuits but with different FF values. I haven't tried it on 64 bit circuits.

The program was written in four hands during the spring of 1997 and Christmas of the same year. The creation and fine-tuning of the algorithm in detail required around a month of various in-depth analyzes. In this regard, I would like to mention the one with whom Caliban was born ... Massimiliano Schillaci called Max or M0 ... Only thanks to his enormous patience I was able to put together something that has respectable performances. It is only thanks to his explanations that I managed to understand how a FaultSimulator works conceptually.

Exploitation of specific hardware resources (core)

Dynamic ordering of faults which excite many FFs

It does not require any kind of precautions in case of 2 FF connected directly to each other (contrary to what HOPE does or the Molokh diagnostic version)

Contemplates the case of synchronized inputs (PI connected directly to an FF)

The code seen through today's eyes is a bit rough and can be improved.

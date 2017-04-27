# WiscKey

Separating Key and Values in SSD- Conscious Storage is a paper published in 14th USENIX Conference on File and Storage Technologies in Feb 2016. I have implemented this on top LevelDB to understands the performance improvements. 

The idea is to separate the key from values to minimize the I/O amplification.

## Contribution:

I have created a program called wisckey_test.cc which uses LevelDB architecture to save Key and Value Offset. The actual value is saved in a logfile inside wisckey_test_dir which get created at run time. 

## Credit

WiscKey: Separating Keys from Values in SSD-conscious Storage
Authors: Lanyue Lu, Thanumalayan Sankaranarayana Pillai, Andrea C. Arpaci-Dusseau, and Remzi H. Arpaci-Dusseau, University of Wisconsin—Madison


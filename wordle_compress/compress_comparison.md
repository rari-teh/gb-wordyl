## Compression: zeta_two

Bank           Range             Size   Used   Used%  Free   Free% 
----------     ----------------  -----  -----  -----  -----  -----
ROM            0x0000 -> 0x3FFF  16384  16215    98%    169     1%
ROM_1          0x4000 -> 0x7FFF  16384  11258    68%   5126    31%


## Compression: arpruss

Bank           Range             Size   Used   Used%  Free   Free% 
----------     ----------------  -----  -----  -----  -----  -----
ROM            0x0000 -> 0x3FFF  16384  16215    98%    169     1%
ROM_1          0x4000 -> 0x7FFF  16384  11520    70%   4864    29%

Data Compressed to: Bytes: 17734


## Compression (details): zeta_two

# Data
+ word_guesses                    0x11EE -> 0x3FFF   11794
+ word_guesses                    0x4000 -> 0x4BEB    3052
+ word_answers                    0x0202 -> 0x1148    3911
+ answers_index                   0x114A -> 0x11EB     162
+ guesses_index                   0x4BED -> 0x4C8E     162
  = 19,081

# Code
+ add_unpack_varint               0x5D65 -> 0x5E86     290
+ query_word_compressed           0x5E87 -> 0x5F96     272
+ from_b26                        0x5BF5 -> 0x5CAB     183
+ to_b26                          0x5CAC -> 0x5D31     134
+ get_word_compressed             0x5FAD -> 0x5FFF      83
+ query_word                      0x5D32 -> 0x5D64      51
+ get_word                        0x5F97 -> 0x5FAC      22
  = 952

# Total
  = 20,033



## Compression (details): arpruss

# Data
+ wordBlob                        0x17F9 -> 0x3FFF   10247
+ wordBlob                        0x4000 -> 0x5D3E    7487
+ answers                         0x5D3F -> 0x6391    1619
+ words                           0x6392 -> 0x63FD     108
 = 19,461

# Code
+ filterWord                      0x0A86 -> 0x0CCD     584
+ decodeInt                       0x06EF -> 0x08A9     443
+ getWord                         0x095A -> 0x0A85     300
+ getSpecialWord                  0x0CCE -> 0x0D81     180
+ decodeWord                      0x08AA -> 0x0959     176
 = 1,683

# Total
 = 21,144



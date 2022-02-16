## Compression: zeta_two
```
Bank           Range             Size   Used   Used%  Free   Free% 
----------     ----------------  -----  -----  -----  -----  -----
ROM            0x0000 -> 0x3FFF  16384  16215    98%    169     1%
ROM_1          0x4000 -> 0x7FFF  16384  11258    68%   5126    31%
```

## Compression: arpruss
```
Bank           Range             Size   Used   Used%  Free   Free% 
----------     ----------------  -----  -----  -----  -----  -----
ROM            0x0000 -> 0x3FFF  16384  16215    98%    169     1%
ROM_1          0x4000 -> 0x7FFF  16384  11135    67%   5249    32%
```

## Compression (details): zeta_two

# Data
```
+ word_guesses                    0x11EE -> 0x3FFF   11794 (14,846)
+ word_guesses                    0x4000 -> 0x4BEB    3052
+ word_answers                    0x0202 -> 0x1148    3911
+ answers_index                   0x114A -> 0x11EB     162
+ guesses_index                   0x4BED -> 0x4C8E     162
  = 19,081
```

# Code
```
+ add_unpack_varint               0x5D65 -> 0x5E86     290
+ query_word_compressed           0x5E87 -> 0x5F96     272
+ from_b26                        0x5BF5 -> 0x5CAB     183
+ to_b26                          0x5CAC -> 0x5D31     134
+ get_word_compressed             0x5FAD -> 0x5FFF      83
+ query_word                      0x5D32 -> 0x5D64      51
+ get_word                        0x5F97 -> 0x5FAC      22
  = 952
```

# Total
```
= 20,033
```


## Compression (details): arpruss

# Data
```
+ wordBlob                        0x1678 -> 0x3FFF   10632 (17,734)
+ wordBlob                        0x4000 -> 0x5BBD    7102
+ answers                         0x5BBE -> 0x6210    1619
+ words                           0x6211 -> 0x627C     108
 = 19,461
```

# Code
```
+ filterWord                      0x0966 -> 0x0B4C     487
+ getWord                         0x0849 -> 0x0965     285
+ decodeInt                       0x06EF -> 0x07F9     267
+ getSpecialWord                  0x0B4D -> 0x0C00     180
+ decodeWord                      0x07FA -> 0x0847      78
 = 1,297
```

# Total
```
 = 20,758
```


# Profiling (avg cycles):

## Check: Nearly valid word: "GRATX"

(zeta_two)
```
_query_word		16558112	10496	1034882/__1034882__/1034882.0

  _query_word_compressed	16555200	10496	236892/__797808__/517350.0
    _add_unpack_varint		13706240	10176	904/__3294__/1346.9
    _from_b26				1099328	352	34354/__34354__/34354.0
      __mullong				1008512	320	3024/__3248__/3151.6
```


Prev Version (arpruss)
```
_filterWord		4198362			9	466484/__466490__/466484.7

  _decodeInt	6180950		11425	310/__1558__/541.0
```

Newer version (arpruss) (2022.02.16: commit ae17d83c3c19e4d5ab70d2eaf476044defb94bbe)
```
_filterWord		12623584	13282	435296/__435296__/435296.0

  _decodeInt	5616662		13253	238/__1346__/423.8
```


## Check: Far from valid word: "GZZZZZ"


(zeta_two)
```
_query_word		7243922	4592	1034846/__1034846__/1034846.0

  _query_word_compressed	7242648	4592	236826/__797838__/517332.0
    _add_unpack_varint		5992504	4452	904/__3294__/1346.0
    _from_b26				481040	154	34360/__34360__/34360.0
      __mullong				441308	140	3024/__3248__/3152.2
```

Prev Version
```
_filterWord		8384428	13	644956/644956/644956.0
```


Newer version (2022.02.16: commit ae17d83c3c19e4d5ab70d2eaf476044defb94bbe)
```
_filterWord		3007910	3185	601582/601582/601582.0
```

## Get Answer Word: (#100, "ARBOR")

(zeta_two)
```
_get_word		237206	4	237206/__237206__/237206.0

  _get_word_compressed		237116	4	237116/237116/237116.0
    _add_unpack_varint		179302	101	904/3294/1775.3
    _to_b26		40308	4	40308/40308/40308.0
```

Prev Version (arpruss)
```
_getSpecialWord		592648	1	592648/__592648__/592648.0

  _getWord			529972	1	529972/__529972__/529972.0
    _decodeInt		329150	493	310/__1558__/667.6
	_decodeWord		796		1	796/__796__/796.0
```

Newer version (arpruss) (2022.02.16: commit ae17d83c3c19e4d5ab70d2eaf476044defb94bbe)
```
_getSpecialWord		523806	1	523806/__523806__/523806.0

  _getWord			461130	1	461130/__461130__/461130.0
    _decodeInt		258978	493	238/__1346__/525.3
    _decodeWord		1326	1	1326/__1326__/1326.0
```





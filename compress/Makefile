
word-db-test: word-db-test.c ../src/word-db.h ../src/word-db.c ../src/words.c
	gcc -o word-db-test word-db-test.c ../src/word-db.h ../src/word-db.c ../src/words.c

MKDIRS  = ../src/lang_de/
MKDIRS += ../src/lang_en/
MKDIRS += ../src/lang_es/
MKDIRS += ../src/lang_fr/
MKDIRS += ../src/lang_it/
MKDIRS += ../src/lang_nl/
MKDIRS += ../src/lang_la/
MKDIRS += ../src/lang_pt-br/
# MKDIRS += ../src/lang_pt/

GBCOMPRESS_RLE = ../tools/gbcompress --cin --cout -v --varname=rle_answerBitmapIndex
# GBCOMPRESS_RLE = ../tools/gbcompress --cin --cout -v --alg=rle --varname=rle_answerBitmapIndex

compress-all-langs:
	python3 compress5.py de
	$(GBCOMPRESS_RLE) ../src/lang_de/answerbitmap.c.nocomp ../src/lang_de/answerbitmap_rle.c

	python3 compress5.py en
	$(GBCOMPRESS_RLE) ../src/lang_en/answerbitmap.c.nocomp ../src/lang_en/answerbitmap_rle.c

	python3 compress5.py es
	$(GBCOMPRESS_RLE) ../src/lang_es/answerbitmap.c.nocomp ../src/lang_es/answerbitmap_rle.c

	python3 compress5.py fr
	$(GBCOMPRESS_RLE) ../src/lang_fr/answerbitmap.c.nocomp ../src/lang_fr/answerbitmap_rle.c

	python3 compress5.py it
	$(GBCOMPRESS_RLE) ../src/lang_it/answerbitmap.c.nocomp ../src/lang_it/answerbitmap_rle.c

	python3 compress5.py nl
	$(GBCOMPRESS_RLE) ../src/lang_nl/answerbitmap.c.nocomp ../src/lang_nl/answerbitmap_rle.c

	python3 compress5.py la
	$(GBCOMPRESS_RLE) ../src/lang_la/answerbitmap.c.nocomp ../src/lang_la/answerbitmap_rle.c

	python3 compress5.py pt-br
	$(GBCOMPRESS_RLE) ../src/lang_pt-br/answerbitmap.c.nocomp ../src/lang_pt-br/answerbitmap_rle.c

#	python3 compress5.py pt

# create necessary directories after Makefile is parsed but before build
# info prevents the command from being pasted into the makefile
# echo $(MKDIRS)
$(info $(shell mkdir -p $(MKDIRS)))




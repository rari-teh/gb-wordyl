
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

compress-all-langs:
	python3 compress5.py de
	python3 compress5.py en
	python3 compress5.py es
	python3 compress5.py fr
	python3 compress5.py it
	python3 compress5.py nl
	python3 compress5.py la
	python3 compress5.py pt-br
#	python3 compress5.py pt

# create necessary directories after Makefile is parsed but before build
# info prevents the command from being pasted into the makefile
# echo $(MKDIRS)
$(info $(shell mkdir -p $(MKDIRS)))




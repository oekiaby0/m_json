CC=gcc
FLAGS=-Wall -Wextra
OUT=main
ARCHIVE_OUT=m_json.a
SOURCE_FILES=m_json.c l_list.c sbuilder.c lexer.c
HEADER_FILES=m_json.h l_list.h sbuilder.h lexer.h
FILES=$(SOURCE_FILES) $(HEADER_FILES)
CC := ${CC} $(FLAGS)

m_json: $(FILES)
	$(CC) -c $(SOURCE_FILES)
	ar ruv $(ARCHIVE_OUT) *.o
	ranlib $(ARCHIVE_OUT)

debug: $(FILES)
	$(CC) -g -c $(SOURCE_FILES) main.c
	gcc *.o -o $(OUT)

clean:
	rm -rf *.o $(OUT) $(ARCHIVE_OUT)

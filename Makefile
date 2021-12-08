CC=gcc
FLAGS=-Wall -Wextra
OUT=m_json.a
SOURCE_FILES=m_json.c l_list.c sbuilder.c lexer.c
CC := ${CC} $(FLAGS)

m_json: $(FILES)
	$(CC) -c $(SOURCE_FILES)
	ar ruv $(OUT) *.o
	ranlib $(OUT)

clean:
	rm -rf *.o $(OUT)

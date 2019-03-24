TARGET=akash
SOURCES=$(wildcard *.c)
OBJECTS=$(patsubst %.c,%.o,$(SOURCES))
LIBS=-lreadline
FLAGS=-DDEBUG_PARSE

executable: $(TARGET)

%.o: %.c
	gcc -c -o $@ $< $(FLAGS)

$(TARGET): $(OBJECTS)
	gcc -o $@ $^ $(LIBS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
	rm -f $(OBJECTS)

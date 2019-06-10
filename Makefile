TARGET=akash
SOURCES=$(wildcard *.c)
OBJECTS=$(patsubst %.c,%.o,$(SOURCES))
LIBS=-lreadline
FLAGS=-g -DDEBUG_EXECUTE -DDEBUG_PARSE

executable: $(TARGET)

%.o: %.c
	gcc -c -o $@ $< $(FLAGS)

$(TARGET): $(OBJECTS)
	gcc -o $@ $^ $(LIBS)

run: $(TARGET)
	./$(TARGET)

debug: $(TARGET)
	gdb $(TARGET)

clean:
	rm -f $(TARGET)
	rm -f $(OBJECTS)

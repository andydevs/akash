TARGET=akash
SOURCES=$(wildcard *.c)
OBJECTS=$(patsubst %.c,%.o,$(SOURCES))
LIBS=-lreadline
FLAGS=-g $(patsubst %,-D%,$(DEBUG))

executable: $(TARGET)

%.o: %.c
	gcc -c -o $@ $< $(FLAGS)

$(TARGET): $(OBJECTS)
	gcc -o $@ $^ $(LIBS) $(FLAGS)

run: $(TARGET)
	./$(TARGET)

debug: $(TARGET)
	gdb $(TARGET)

clean:
	rm -f $(TARGET)
	rm -f $(OBJECTS)

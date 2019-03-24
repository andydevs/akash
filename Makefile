TARGET=akash
SOURCES=$(wildcard *.c)
OBJECTS=$(patsubst %.c,%.o,$(SOURCES))
LIBS=-lreadline

executable: $(TARGET)

%.o: %.c
	gcc -c -o $@ $<

$(TARGET): $(OBJECTS)
	gcc -o $@ $^ -lreadline

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
	rm -f $(OBJECTS)

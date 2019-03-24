TARGET=akash
SOURCES=$(wildcard *.c)
OBJECTS=$(patsubst %.c,%.o,$(SOURCES))

executable: $(TARGET)

%.o: %.c
	gcc -c -o $@ $< $(INCDIRS)

$(TARGET): $(OBJECTS)
	gcc -o $@ $^ $(LIBDIRS) $(LIBS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
	rm -f $(OBJECTS)

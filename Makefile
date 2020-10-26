ifeq ($(PREFIX),)
    PREFIX := /usr/local
endif

all: xhotcorners

.PHONY: all clean install uninstall

xhotcorners: xhotcorners.c
	gcc -o $@ $< -lX11 -Wall

clean:
	rm -f xhotcorners

debug:
	valgrind  -v --leak-check=full --track-origins=yes ./xhotcorners

install:
	@sudo install -m 0755 xhotcorners $(DESTDIR)$(PREFIX)/bin 
	@install -m 0644 -T xhotcorners ~/.xhotcorners

uninstall:
	@sudo rm  -f $(DESTDIR)$(PREFIX)/bin/xhotcorners
	@rm ~/.xhotcorners

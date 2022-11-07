all: type-erasure

run: type-erasure
	$(MAKE) -C type-erasure run

clean:
	$(MAKE) -C type-erasure clean

type-erasure:
	$(MAKE) -C type-erasure

.PHONY: all run clean type-erasure

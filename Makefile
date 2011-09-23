all:
	@gcc src/lexer.c -o build/waffle

run:
	@build/waffle

test:
	@build/waffle examples/fib.wfl

.PHONY: test
